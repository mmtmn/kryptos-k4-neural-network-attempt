def decrypt(ciphertext, key):
    plaintext = []
    for char in ciphertext:
        if char in key:
            plaintext.append(key[char])
        else:
            plaintext.append(char)
    return ''.join(plaintext)

# Given ciphertext
ciphertext = [
    "OBKR",
    "UOXOGHULBSOLIFBBWFLRVQQPRNGKSSO",
    "TWTQSJQSSEKZZWATJKLUDIAWINFBNYP",
    "VTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
]

# Initialize the key with known mappings from the clues and observations
key = {
    'M': 'C', 'Z': 'L', 'F': 'O', 'P': 'C', 'K': 'K', # MZFPK -> CLOCK
    'Q': 'N', 'P': 'O', 'R': 'R', 'N': 'T', 'G': 'H', 'K': 'E', 'S': 'A', # QQPRNGKSS -> NORTHEAST
    'F': 'E', 'L': 'A', 'R': 'S', 'V': 'T', # FLRV -> EAST
    'B': 'B', 'E': 'E', 'R': 'R', 'L': 'L', 'I': 'I', 'N': 'N' # BERLIN
}

# Additional mappings based on observations
additional_mappings = {
    'O': 'T', 'B': 'H', 'U': 'I', 'X': 'S', 'H': 'E', 'A': 'D', 'I': 'F', 'W': 'Y', 'J': 'M', 'C': 'U', 'D': 'P',
    'T': 'T', 'E': 'H', 'Y': 'E', 'N': 'O', 'O': 'T', 'S': 'A', 'L': 'A', 'M': 'C', 'G': 'H'
}

# Update the key with additional mappings
key.update(additional_mappings)

# Decrypt each line of the ciphertext again with the updated key
for line in ciphertext:
    print(decrypt(line, key))