# Get card number
card = input("Number: ")
l = len(card)
start = card[0:2]
card = int(card)
multi = 0
non = 0

# Check whether card is of correct length
if l not in [13, 15, 16]:
    print("INVALID")

while card > 0:

    # Add to non multi
    non += card % 10
    card //= 10

    # Add to multi
    product = (card % 10) * 2
    while product > 0:
        multi += product % 10
        product //= 10

    card //= 10

if (multi + non) % 10 == 0:
    if start in ["34", "37"]:
        print("AMEX")
    elif start in ["51", "52", "53", "54", "55"]:
        print("MASTERCARD")
    elif start[0] == "4":
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")