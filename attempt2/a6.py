def decrypt(ciphertext, key):
    plaintext = []
    for char in ciphertext:
        if char in key:
            plaintext.append(key[char])
        else:
            plaintext.append(char)
    return ''.join(plaintext)

# Given ciphertext
ciphertext = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"

# Initialize the key with known mappings from the clues
key = {
    'M': 'C', 'Z': 'L', 'F': 'O', 'P': 'C', 'K': 'K', # MZFPK -> CLOCK
    'Q': 'N', 'P': 'O', 'R': 'R', 'N': 'T', 'G': 'H', 'K': 'E', 'S': 'A', # QQPRNGKSS -> NORTHEAST
    'F': 'E', 'L': 'A', 'R': 'S', 'V': 'T', # FLRV -> EAST
    'O': 'T', 'B': 'H', 'U': 'I', 'X': 'S', 'H': 'E', 'A': 'D', 'I': 'F', 'W': 'Y', 'J': 'M', 'C': 'U', 'D': 'P'
}

# Decrypt the ciphertext
plaintext = decrypt(ciphertext, key)
print("Partially decrypted text:", plaintext)

# Refine the key based on the partially decrypted text
key.update({
    'I': 'F', 'A': 'D', 'H': 'E', 'T': 'H', 'Y': 'O', 'N': 'T', 'M': 'C', 'E': 'L', 'D': 'P', 'C': 'U', 'L': 'A', 'S': 'A', 'U': 'I', 'P': 'O', 'F': 'E', 'E': 'L', 'O': 'T', 'B': 'H', 'K': 'E', 'R': 'S', 'G': 'H', 'J': 'M', 'W': 'Y', 'X': 'S', 'Z': 'L'
})

# Decrypt again with the updated key
plaintext = decrypt(ciphertext, key)
print("Fully decrypted text:", plaintext)