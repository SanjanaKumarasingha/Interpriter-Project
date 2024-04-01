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


# input_program = "let Sum(A)  Psum (A,Order A )"
# tokens = tokenize(input_program)
# for token in tokens:
#     print(f"{token.token_type} {token.lexeme}")


if __name__ == "__main__":
    try:
        with open("input.txt", "r") as file:
            input_program = file.read()
            tokens = tokenize(input_program)
            for token in tokens:
                print(f"{token.token_type} {token.lexeme}")
    except FileNotFoundError:
        print(f"Error: File not found at '{file.name}'")
    except Exception as e:
        print(f"An error occurred: {e}")
