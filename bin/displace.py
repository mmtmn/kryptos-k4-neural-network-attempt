def calculate_displacement(ciphertext, clues):
    displacement = {}
    for position, plaintext in clues:
        for i, char in enumerate(plaintext):
            cipher_char = ciphertext[position + i]
            shift = (ord(cipher_char) - ord(char)) % 26
            displacement[position + i] = shift
            print(f"Position {position + i}: Cipher '{cipher_char}' -> Plain '{char}' | Shift: {shift}")
    return displacement

ciphertext = "UOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"

clues = [
    (22, "EAST"),
    (26, "NORTHEAST"),
    (64, "BERLIN"),
    (70, "CLOCK")
]

displacement = calculate_displacement(ciphertext, clues)

print("\nDisplacement values:")
for position, shift in displacement.items():
    print(f"Position {position}: Shift {shift}")
