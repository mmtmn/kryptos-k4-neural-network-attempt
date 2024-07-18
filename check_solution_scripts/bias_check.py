def check_string(input_string):
    allowed_letters = set("BERLINCLOCKNORTHEASTEAST")
    input_string = input_string.upper()  # Convert to uppercase to match the allowed letters
    invalid_letters = [char for char in input_string if char not in allowed_letters]
    
    if invalid_letters:
        print(f"Unbiased letters found: {invalid_letters}")
        print(f"Count of unbiased letters: {len(invalid_letters)}")
    else:
        print("Biased attempt! All letters of your attempt are in the clues!")

# Ask for user input
user_input = input("Please enter a string: ")
check_string(user_input)
