from cs50 import get_int

# Gets an int between 1 and 8 inclusive
while True:
    height = get_int("Height of the priamid: ")
    if height in range(1, 9):
        break

# Prints a piramid of the given height
for i in range(1, height + 1):
    print(" " * (height - i) + "#" * i + "  " + "#" * i)
