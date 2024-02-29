import re

# Split the string at every white-space character:

txt = "let Sum(A) = Psum (A,Order A )"
x = re.split("\s", txt)
print(x)
