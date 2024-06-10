def check_string_length():
    user_input = input("Please enter a string: ")
    if len(user_input) == 97:
        print("The string is 97 characters long.")
    else:
        print(f"The string is {len(user_input)} characters long, which is not 97.")

# Call the function
check_string_length()
