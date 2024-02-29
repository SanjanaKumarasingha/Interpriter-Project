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
