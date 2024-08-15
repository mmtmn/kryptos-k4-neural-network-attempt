def decrypt(ciphertext, key):
    plaintext = []
    for char in ciphertext:
        if char in key:
            plaintext.append(key[char])
        else:
            plaintext.append(char)
    return ''.join(plaintext)

def is_valid_decryption(decrypted_text):
    # Check if the decrypted text contains the expected words
    return "THE" in decrypted_text and "NORTHEAST" in decrypted_text and "EAST" in decrypted_text and "BERLIN" in decrypted_text and "CLOCK" in decrypted_text

def recursive_decrypt(ciphertext, key, index=0):
    if index == len(ciphertext):
        decrypted_text = [decrypt(line, key) for line in ciphertext]
        if all(is_valid_decryption(line) for line in decrypted_text):
            return decrypted_text
        return None

    current_line = ciphertext[index]
    for char in current_line:
        if char not in key:
            for possible_char in "ABCDEFGHIJKLMNOPQRSTUVWXYZ":
                if possible_char not in key.values():
                    key[char] = possible_char
                    result = recursive_decrypt(ciphertext, key, index + 1)
                    if result:
                        return result
                    del key[char]
            return None
    return recursive_decrypt(ciphertext, key, index + 1)

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
    'O': 'T', 'U': 'I', 'X': 'S', 'H': 'E', 'A': 'D', 'I': 'F', 'W': 'Y', 'J': 'M', 'C': 'U', 'D': 'P',
    'T': 'T', 'E': 'H', 'Y': 'E', 'N': 'O', 'S': 'A', 'G': 'H'
}

# Update the key with additional mappings
key.update(additional_mappings)

# Decrypt the ciphertext using the recursive function
decrypted_text = recursive_decrypt(ciphertext, key)
if decrypted_text:
    for line in decrypted_text:
        print(line)
else:
    print("No valid decryption found.")
