def check_clues(attempt):
    # Define the clues and their positions
    clues = {
        "EAST": (21, 25),
        "NORTHEAST": (25, 34),
        "BERLIN": (63, 69),
        "CLOCK": (69, 74)
    }

    valid = True

    # Check each clue in the attempt
    for clue, (start, end) in clues.items():
        if attempt[start:end] != clue:
            print(f"Clue '{clue}' is not in the correct position.")
            valid = False
        else:
            print(f"Clue '{clue}' is correctly positioned.")

    if valid:
        print("All clues are correctly positioned.")
    else:
        print("Some clues are not correctly positioned.")

# Ask for user input
user_attempt = input("Please enter your attempt: ")
check_clues(user_attempt)
