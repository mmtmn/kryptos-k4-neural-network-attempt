import string

ciphertext = [
    "OBKR",
    "UOXOGHULBSOLIFBBWFLRVQQPRNGKSSO",
    "TWTQSJQSSEKZZWATJKLUDIAWINFBNYP",
    "VTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
]

# Clues
clues = {
    "MZFPK": "CLOCK",
    "QQPRNGKSS": "NORTHEAST",
    "FLRV": "EAST",
    "BERLIN": "BERLIN"
}

# Function to apply a Vigenère cipher decryption
def vigenere_decrypt(ciphertext, key):
    decrypted_text = []
    key_length = len(key)
    key_as_int = [ord(i) for i in key]
    ciphertext_int = [ord(i) for i in ciphertext]
    for i in range(len(ciphertext_int)):
        value = (ciphertext_int[i] - key_as_int[i % key_length]) % 26
        decrypted_text.append(chr(value + 65))
    return ''.join(decrypted_text)

# Function to derive key from clues
def derive_key_from_clues(ciphertext, clues):
    key = ['A'] * len(ciphertext[1])  # Initialize key with 'A's
    for clue_cipher, clue_plain in clues.items():
        for i in range(len(ciphertext)):
            pos = ciphertext[i].find(clue_cipher)
            if pos != -1:
                for j in range(len(clue_cipher)):
                    key[pos + j] = chr((ord(ciphertext[i][pos + j]) - ord(clue_plain[j])) % 26 + 65)
    return ''.join(key)

# Derive key from clues
key = derive_key_from_clues(ciphertext, clues)
print("Derived Key from Clues:", key)

# Decrypt the ciphertext using the derived key
decrypted_text = [vigenere_decrypt(line, key) for line in ciphertext]

# Print the decrypted text
for line in decrypted_text:
    print(line)

# Adjust the key based on partially decrypted text
# Manually adjust the key based on observed patterns
adjusted_key = list(key)
# Ensure the adjusted_key list is the correct length
adjusted_key.extend(['A'] * (len(ciphertext[1]) - len(adjusted_key)))

# Adjust based on observed pattern
adjusted_key[0] = 'O'
adjusted_key[1] = 'B'
adjusted_key[2] = 'K'
adjusted_key[3] = 'R'
# Further adjustments based on observed patterns
# Adjust the key based on the length of the longest ciphertext line
for i in range(len(adjusted_key), len(ciphertext[1])):
    adjusted_key.append('A')

# Convert adjusted key back to string
adjusted_key = ''.join(adjusted_key)
print("Adjusted Key:", adjusted_key)

# Decrypt the ciphertext using the adjusted key
decrypted_text = [vigenere_decrypt(line, adjusted_key) for line in ciphertext]

# Print the decrypted text
for line in decrypted_text:
    print(line)
