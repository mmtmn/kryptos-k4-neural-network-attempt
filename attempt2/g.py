def decrypt(ciphertext, key):
    plaintext = []
    for char in ciphertext:
        if char in key:
            plaintext.append(key[char])
        else:
            plaintext.append(char)
    return ''.join(plaintext)

# Given ciphertext
ciphertext = "UOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"

# Initialize the key with known mappings from the clues
key = {
    'F': 'E', 'L': 'A', 'R': 'S', 'V': 'T',  # FLRV -> EAST
    'Q': 'N', 'P': 'O', 'R': 'R', 'N': 'T', 'G': 'H', 'K': 'E', 'S': 'A',  # QQPRNGKSS -> NORTHEAST
    'B': 'B', 'E': 'E', 'R': 'R', 'L': 'L', 'I': 'I', 'N': 'N',  # FBNUYP -> BERLIN
    'M': 'C', 'Z': 'L', 'F': 'O', 'P': 'C', 'K': 'K'  # MZFPK -> CLOCK
}

# Decrypt the ciphertext
plaintext = decrypt(ciphertext, key)
print(plaintext)
