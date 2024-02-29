import string
import re

class Token:
    def __init__(self, token_type, lexeme):
        self.token_type = token_type
        self.lexeme = lexeme

# Define token types
TOKEN_TYPES = {
    "LET": "let",
    "IDENTIFIER": r"[a-zA-Z_][a-zA-Z0-9_]*",
    "INTEGER": r"\d+",
    "OPERATOR": r"[+\-*/<>&.@/:=˜|$!#%^_[\]{}\"‘?]+",
    "STRING": r"''(?:\\t|\\n|\\\\|\\''|\\\(\\\)|[a-zA-Z0-9_+\-*/<>&.@/:=˜|$!#%^_()[\]{}\"‘?])*''",
    "DELETE": r"[ \t\n]+",
    "PUNCTUATION": r"[(),;]",
    "LETTER": r"[a-zA-Z]",
    "DIGIT": r"\d",
}

def tokenize(input_string):
    tokens = []
    while input_string:
        # Skip whitespace, tabs, and newlines
        while input_string and input_string[0] in {' ', '\t', '\n'}:
            input_string = input_string[1:]

        # Check each token type
        found_token = False
        for token_type, pattern in TOKEN_TYPES.items():
            if token_type == "DELETE":
                regex = pattern
            else:
                regex = f"^{pattern}"
            match = re.match(regex, input_string)
            if match:
                lexeme = match.group()
                tokens.append(Token(token_type, lexeme))
                input_string = input_string[len(lexeme):]
                found_token = True
                break

        # Handle unrecognized tokens
        if not found_token:
            raise ValueError(f"Unrecognized token: {input_string}")

    return tokens

# Example
input_program = """
let Sum(A) = Psum (A,Order A )
in Print ( Sum (1,2,3,4,5) )
"""
tokens = tokenize(input_program)
for token in tokens:
    print(f"Type: {token.token_type}, Lexeme: {token.lexeme}")
