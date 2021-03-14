# Gets a text from the user
text = input("Text: ")

# Gets number of sentences
sentences = 0
sentences += text.count(".")
sentences += text.count("?")
sentences += text.count("!")

# Replaces every character that isn't a letter or a space
for l in text:
    if l.lower() not in "abcdefghijklmnopqrstuvwxyz ":
        text = text.replace(l, "", text.count(l))

# Gets number of words
text = text.split(" ")
words = len(text)

# Gets number of letters
text = "".join(text)
letters = len(text)

# Applying the algorithm
L = letters / words * 100
S = sentences / words * 100
grade = round(0.0588 * L - 0.296 * S - 15.8)

# Printing out the grade
if grade > 16:
    print("Grade 16+")
elif grade < 1:
    print("Before Grade 1")
else:
    print("Grade " + str(grade))

