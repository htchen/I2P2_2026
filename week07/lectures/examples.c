#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    const char *cursor;
    bool ok;
} Parser;

static void skip_spaces(Parser *parser) {
    while (isspace((unsigned char)*parser->cursor)) {
        ++parser->cursor;
    }
}

static long parse_expression(Parser *parser);

static long parse_factor(Parser *parser) {
    skip_spaces(parser);
    if (*parser->cursor == '(') {
        ++parser->cursor;
        const long value = parse_expression(parser);
        skip_spaces(parser);
        if (*parser->cursor != ')') {
            parser->ok = false;
            return 0;
        }
        ++parser->cursor;
        return value;
    }
    if (!isdigit((unsigned char)*parser->cursor)) {
        parser->ok = false;
        return 0;
    }
    long value = 0;
    while (isdigit((unsigned char)*parser->cursor)) {
        value = value * 10 + (*parser->cursor - '0');
        ++parser->cursor;
    }
    return value;
}

static long parse_term(Parser *parser) {
    long value = parse_factor(parser);
    skip_spaces(parser);
    while (parser->ok && *parser->cursor == '*') {
        ++parser->cursor;
        value *= parse_factor(parser);
        skip_spaces(parser);
    }
    return value;
}

static long parse_expression(Parser *parser) {
    long value = parse_term(parser);
    skip_spaces(parser);
    while (parser->ok && *parser->cursor == '+') {
        ++parser->cursor;
        value += parse_term(parser);
        skip_spaces(parser);
    }
    return value;
}

int main(void) {
    Parser parser = {"12 + 3 * (4 + 1)", true};
    const long result = parse_expression(&parser);
    skip_spaces(&parser);
    if (!parser.ok || *parser.cursor != '\0') {
        fputs("parse error\n", stderr);
        return 1;
    }
    printf("%ld\n", result);
    return 0;
}
