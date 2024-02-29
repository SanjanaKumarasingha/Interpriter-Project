import re


class Token:
    def __init__(self, token_type, lexeme):
        self.token_type = token_type
        self.lexeme = lexeme


# Define token types
TOKEN_TYPES = {
    "IDENTIFIER": r"[a-zA-Z_][a-zA-Z0-9_]*",
    "INT": r"\d+",  # Integer
    "OPERATOR": r"[+|-|/|*]+",
    "STRING": r"'(?:\\[tn\\'\(\);,]|''|[\w\s\d+\-*<>&.@/:=˜|$!#%^_[\]{}\"‘?;])*'",
    "comment": r'//(?:\'\'\'\'|\(|\)|;|,|\\| |\t|ht|\w|[\d\s+\-*<>&.@/:=˜|$!#%^_[\]{}"‘?;])*',
    "punctuation": r"[\(\);,]",
    # "letter": r"[A-Za-z]*",
    # "digit": r"[0-9]*",
    "operator": r'[+\-*<>&.@/:=˜|$!#%^_[\]{}"‘?;]',
}


def tokenize(input_string):
    tokens = []

    while input_string:
        for token_type, pattern in TOKEN_TYPES.items():
            regex = re.compile(pattern)
            match = regex.match(input_string)

            while match:
                lexeme = match.group()
                tokens.append(Token(token_type, lexeme))
                input_string = input_string[len(lexeme) :].lstrip()
                match = regex.match(input_string)

    return tokens


# Example usage
input_program = "let Sum(A)  Psum (A,Order A )"
tokens = tokenize(input_program)
for token in tokens:
    print(f"{token.token_type} {token.lexeme}")
