"""Runnable Python contrast for the Week 7 expression pipeline."""

from __future__ import annotations

from dataclasses import dataclass
from enum import Enum, auto


class TokenKind(Enum):
    INTEGER = auto()
    PLUS = auto()
    MINUS = auto()
    STAR = auto()
    SLASH = auto()
    LEFT_PAREN = auto()
    RIGHT_PAREN = auto()
    END = auto()


@dataclass(frozen=True)
class Token:
    kind: TokenKind
    value: int = 0
    position: int = 0


class ParseError(ValueError):
    pass


class Lexer:
    def __init__(self, source: str, maximum: int = (1 << 63) - 1) -> None:
        self.source = source
        self.position = 0
        self.maximum = maximum

    def peek(self) -> str:
        if self.position == len(self.source):
            return ""
        return self.source[self.position]

    def take(self) -> str:
        current = self.peek()
        if current:
            self.position += 1
        return current

    def next_token(self) -> Token:
        while self.peek().isspace():
            self.take()
        start = self.position
        current = self.peek()
        if not current:
            return Token(TokenKind.END, position=start)
        if current.isdigit():
            value = 0
            while self.peek().isdigit():
                digit = int(self.take())
                if value > (self.maximum - digit) // 10:
                    raise ParseError("integer literal is out of range")
                value = value * 10 + digit
            return Token(TokenKind.INTEGER, value, start)
        self.take()
        punctuation = {
            "+": TokenKind.PLUS,
            "-": TokenKind.MINUS,
            "*": TokenKind.STAR,
            "/": TokenKind.SLASH,
            "(": TokenKind.LEFT_PAREN,
            ")": TokenKind.RIGHT_PAREN,
        }
        if current not in punctuation:
            raise ParseError(f"invalid character at position {start}")
        return Token(punctuation[current], position=start)


class AstKind(Enum):
    INTEGER = auto()
    ADD = auto()
    SUBTRACT = auto()
    MULTIPLY = auto()
    DIVIDE = auto()
    NEGATE = auto()


@dataclass(frozen=True)
class Ast:
    kind: AstKind
    value: int = 0
    left: Ast | None = None
    right: Ast | None = None

    def __post_init__(self) -> None:
        is_number = self.kind is AstKind.INTEGER and self.left is None
        is_unary = (
            self.kind is AstKind.NEGATE
            and self.left is not None
            and self.right is None
        )
        is_binary = (
            self.kind not in (AstKind.INTEGER, AstKind.NEGATE)
            and self.left is not None
            and self.right is not None
        )
        if not (is_number or is_unary or is_binary):
            raise ValueError("invalid AST shape")


class Parser:
    def __init__(self, source: str) -> None:
        self.lexer = Lexer(source)
        self.current = self.lexer.next_token()

    def advance(self) -> None:
        self.current = self.lexer.next_token()

    def parse(self) -> Ast:
        result = self.parse_expression()
        if self.current.kind is not TokenKind.END:
            raise ParseError("unexpected trailing token")
        return result

    def parse_expression(self) -> Ast:
        left = self.parse_term()
        while self.current.kind in (TokenKind.PLUS, TokenKind.MINUS):
            operation = self.current.kind
            self.advance()
            right = self.parse_term()
            kind = (
                AstKind.ADD
                if operation is TokenKind.PLUS
                else AstKind.SUBTRACT
            )
            left = Ast(kind=kind, left=left, right=right)
        return left

    def parse_term(self) -> Ast:
        left = self.parse_unary()
        while self.current.kind in (TokenKind.STAR, TokenKind.SLASH):
            operation = self.current.kind
            self.advance()
            right = self.parse_unary()
            kind = (
                AstKind.MULTIPLY
                if operation is TokenKind.STAR
                else AstKind.DIVIDE
            )
            left = Ast(kind=kind, left=left, right=right)
        return left

    def parse_unary(self) -> Ast:
        if self.current.kind is TokenKind.PLUS:
            self.advance()
            return self.parse_unary()
        if self.current.kind is TokenKind.MINUS:
            self.advance()
            return Ast(kind=AstKind.NEGATE, left=self.parse_unary())
        return self.parse_primary()

    def parse_primary(self) -> Ast:
        if self.current.kind is TokenKind.INTEGER:
            value = self.current.value
            self.advance()
            return Ast(kind=AstKind.INTEGER, value=value)
        if self.current.kind is TokenKind.LEFT_PAREN:
            self.advance()
            inside = self.parse_expression()
            if self.current.kind is not TokenKind.RIGHT_PAREN:
                raise ParseError("expected ')'")
            self.advance()
            return inside
        raise ParseError("expected an integer or '('")


def truncate_divide(left: int, right: int) -> int:
    if right == 0:
        raise ZeroDivisionError("division by zero")
    magnitude = abs(left) // abs(right)
    return -magnitude if (left < 0) != (right < 0) else magnitude


def evaluate(node: Ast) -> int:
    if node.kind is AstKind.INTEGER:
        return node.value
    if node.left is None:
        raise ValueError("missing left operand")
    left = evaluate(node.left)
    if node.kind is AstKind.NEGATE:
        return -left
    if node.right is None:
        raise ValueError("missing right operand")
    right = evaluate(node.right)
    if node.kind is AstKind.ADD:
        return left + right
    if node.kind is AstKind.SUBTRACT:
        return left - right
    if node.kind is AstKind.MULTIPLY:
        return left * right
    if node.kind is AstKind.DIVIDE:
        return truncate_divide(left, right)
    raise ValueError("unknown AST kind")


def generate(node: Ast) -> list[str]:
    if node.kind is AstKind.INTEGER:
        return [f"PUSH {node.value}"]
    if node.left is None:
        raise ValueError("missing operand")
    result = generate(node.left)
    if node.kind is AstKind.NEGATE:
        return result + ["NEG"]
    if node.right is None:
        raise ValueError("missing right operand")
    operation = {
        AstKind.ADD: "ADD",
        AstKind.SUBTRACT: "SUB",
        AstKind.MULTIPLY: "MUL",
        AstKind.DIVIDE: "DIV",
    }[node.kind]
    return result + generate(node.right) + [operation]


def main() -> None:
    tree = Parser("1 + 2 * 3").parse()
    assert evaluate(tree) == 7
    assert generate(tree) == ["PUSH 1", "PUSH 2", "PUSH 3", "MUL", "ADD"]
    assert evaluate(Parser("-(8 / 3)").parse()) == -2
    try:
        Parser("1 + )").parse()
    except ParseError:
        pass
    else:
        raise AssertionError("invalid expression was accepted")
    print("Week 7 Python contrasts passed.")


if __name__ == "__main__":
    main()
