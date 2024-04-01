import re
import sys


class Scanner:
    """The interface comprises the methods lookahead and consume.
    Other methods should not be called from outside of this class."""

    def __init__(self, input_file):
        """Reads the whole input_file to input_string, which remains constant.
        current_char_index counts how many characters of input_string have
        been consumed.
        current_token holds the most recently found token and the
        corresponding part of input_string."""

        # source code of the program to be compiled
        self.input_string = input_file.read()

        # index where the unprocessed part of input_string starts
        self.current_char_index = 0

        # a pair (most recently read token, matched substring of input_string)
        self.current_token = self.get_token()

    def skip_white_space(self):
        """Consumes all characters in input_string up to the next
        non-white-space character."""
        if self.current_char_index >= len(self.input_string) - 1:
            return

        while self.input_string[self.current_char_index].isspace():
            self.current_char_index += 1

    def get_token(self):
        """Returns the next token and the part of input_string it matched.
        The returned token is None if there is no next token.
        The characters up to the end of the token are consumed."""
        self.skip_white_space()
        # find the longest prefix of input_string that matches a token
        token, longest = None, ""
        for t, r in Token.token_regexp:
            match = re.match(r, self.input_string[self.current_char_index :])
            if match and match.end() > len(longest):
                token, longest = t, match.group()
        # consume the token by moving the index to the end of the matched part
        self.current_char_index += len(longest)
        return (token, longest)

    def lookahead(self):
        """Returns the next token without consuming it.
        Returns None if there is no next token."""
        return self.current_token[0]

    def consume(self, *tokens):
        """Returns the next token and consumes it, if it is in tokens.
        Raises an exception otherwise.
        If the token is a number or an identifier, its value is returned
        instead of the token."""
        current = self.current_token

        if len(self.input_string[self.current_char_index :]) == 0:
            self.current_token = (
                None,
                "",
            )  # catches the end-of-file errors so lookahead returns none.
        else:
            self.current_token = self.get_token()  # otherwise we consume the token

        if (
            current[0] in tokens
        ):  # tokens could be a single token, or it could be group of tokens.
            if (
                current[0] is Token.ID or current[0] is Token.NUM
            ):  # if token is ID or NUM
                return current[1]  # return the value of the ID or NUM
            else:  # otherwise
                return current[0]  # return the token
        else:  # if current_token is not in tokens
            raise Exception("non-token detected")  # raise non-token error


class Token:
    # The following enumerates all tokens.
    DO = "DO"
    ELSE = "ELSE"
    READ = "READ"
    WRITE = "WRITE"
    END = "END"
    IF = "IF"
    THEN = "THEN"
    WHILE = "WHILE"
    SEM = "SEM"
    BEC = "BEC"
    LESS = "LESS"
    EQ = "EQ"
    GRTR = "GRTR"
    LEQ = "LEQ"
    NEQ = "NEQ"
    GEQ = "GEQ"
    ADD = "ADD"
    SUB = "SUB"
    MUL = "MUL"
    DIV = "DIV"
    LPAR = "LPAR"
    RPAR = "RPAR"
    NUM = "NUM"
    ID = "ID"

    # The following list gives the regular expression to match a token.
    # The order in the list matters for mimicking Flex behaviour.
    # Longer matches are preferred over shorter ones.
    #  For same-length matches, the first in the list is preferred.
    token_regexp = [
        (DO, "do"),
        (ELSE, "else"),
        (READ, "read"),
        (WRITE, "write"),
        (END, "end"),
        (IF, "if"),
        (THEN, "then"),
        (WHILE, "while"),
        (SEM, ";"),
        (BEC, ":="),
        (LESS, "<"),
        (EQ, "="),
        (NEQ, "!="),
        (GRTR, ">"),
        (LEQ, "<="),
        (GEQ, ">="),
        (ADD, "[+]"),  # + is special in regular expressions
        (SUB, "-"),
        (MUL, "[*]"),
        (DIV, "[/]"),
        (LPAR, "[(]"),  # ( is special in regular expressions
        (RPAR, "[)]"),  # ) is special in regular expressions
        (ID, "[a-z]+"),
        (NUM, "[0-9]+"),
    ]

    def indent(s, level):
        return "    " * level + s + "\n"


# Initialise scanner.

scanner = Scanner(sys.stdin)

# Show all tokens in the input.

token = scanner.lookahead()
test = "let Sum(A) = Psum (A,Order A )"

while token != None:
    if token in [Token.NUM, Token.ID]:
        token, value = scanner.consume(token)
        print(token, value)
    else:
        print(scanner.consume(token))
    token = scanner.lookahead()
