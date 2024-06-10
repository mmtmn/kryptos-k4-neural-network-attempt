def calculate_displacement(ciphertext, plaintext, start):
    displacement = []
    for i, plain_char in enumerate(plaintext):
        cipher_char = ciphertext[start + i]
        displacement_value = (ord(plain_char) - ord(cipher_char)) % 26
        if displacement_value > 13:  # Convert to negative displacement for values > 13
            displacement_value -= 26
        displacement.append((start + i + 1, displacement_value))  # 1-based index
    return displacement

# Ciphertext given
ciphertext = "UOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"

# Clues
clues = [
    (22, "EAST"),
    (26, "NORTHEAST"),
    (64, "BERLIN"),
    (70, "CLOCK")
]

# Calculate displacements for each clue
for start, plaintext in clues:
    displacements = calculate_displacement(ciphertext, plaintext, start - 1)
    print(f"Displacements for plaintext starting at position {start}:")
    for position, displacement in displacements:
        print(f"Position {position}: {displacement}")


"""
                           OBKR
UOXOGHULBSOLIFBBWFLRVQQPRNGKSSO
TWTQSJQSSEKZZWATJKLUDIAWINFBNYP
VTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR

Jim Sandborn, the creator of the sculpture and puzzle has given us clues, revealing that characters:

    22-25 FLRV decrypt to EAST.
    26-34 QQPRNGKSS decrypt to NORTHEAST.
    64-69 NYPVTT decrypt to BERLIN.
    70-74 MZFPK decrypt to CLOCK.

That means we know the plaintext looks like this, where . represents an unknown character:

                           ....
.................EASTNORTHEAST.
............................BER
LINCLOCK.......................

Position 22: -12
Position 23: 10
Position 24: 3
Position 25: 2
Position 26: 0
Position 27: 8
Position 28: 7
Position 29: 1
Position 30: -11
Position 31: -10
Position 32: 7
Position 33: -4
Position 34: 0
Position 64: 8
Position 65: 11
Position 66: 5
Position 67: 12
Position 68: 3
Position 69: -2
Position 70: -8
Position 71: -11
Position 72: 8
Position 73: -1
Position 74: 0


"""