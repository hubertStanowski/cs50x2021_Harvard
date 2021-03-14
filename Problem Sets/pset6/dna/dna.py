import sys
import csv


def main():
    # Check if used correctly
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py csv txt")

    # Open and read from the csv file into the database
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        database = []

        for row in reader:
            person = row
            for key in person.keys():
                if key != "name":
                    person[key] = int(person[key])
            database.append(person)

    # Open and read the file with the DNA sequence
    with open(sys.argv[2]) as file:
        sequence = file.readline()

    # Get longes sequences of individual STRs
    strs = {}
    for key in database[0].keys():
        if key != "name":
            c = sequence.count(key)
            test = key * c
            while test not in sequence:
                c -= 1
                test = key * c
            strs[key] = c

    # Compare the sequence's STRs with each person's STRs
    for person in database:
        check = True
        for key in person.keys():
            if key != "name":
                if person[key] != strs[key]:
                    check = False

        # If match print person's name and exit
        if check == True:
            print(person["name"])
            sys.exit(0)

    # If not match print No match and exit
    print("No match")
    sys.exit(1)


main()