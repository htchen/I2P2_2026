"""Runnable Python contrasts for the Week 1 C examples."""


def twice(value: int) -> int:
    return value * 2


def add_one(value: int) -> int:
    return value + 1


def read_two_integers(line: str) -> tuple[int, int]:
    fields = line.split()
    if len(fields) != 2:
        raise ValueError("expected two integers")
    return int(fields[0]), int(fields[1])


def sum_positive_squares(lines: list[str], maximum: int = 100) -> int:
    answer = 0
    count = 0
    for line in lines:
        for token in line.split():
            if count == maximum:
                raise ValueError("too many integers")
            value = int(token)
            if value > 0:
                answer += value * value
            count += 1
    return answer


def classify_integer(value: int) -> str:
    if value < 0:
        sign = "negative"
    elif value > 0:
        sign = "positive"
    else:
        sign = "zero"
    parity = "even" if value % 2 == 0 else "odd"
    return f"{value} is {sign} and {parity}"


def main() -> None:
    assert twice(21) == 42
    assert add_one(7) == 8
    assert read_two_integers("10 32") == (10, 32)
    assert sum_positive_squares(["-2 0 3", "4"]) == 25
    assert classify_integer(-3) == "-3 is negative and odd"
    assert classify_integer(0) == "0 is zero and even"
    print("Week 1 Python contrasts passed.")


if __name__ == "__main__":
    main()
