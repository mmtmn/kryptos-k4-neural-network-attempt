def vigenere_decrypt(ciphertext, key):
    plaintext = []
    key_length = len(key)
    
    for i, char in enumerate(ciphertext):
        if char.isalpha():
            shift = ord(key[i % key_length]) - ord('A')
            decrypted_char = chr((ord(char) - shift - ord('A')) % 26 + ord('A'))
            plaintext.append(decrypted_char)
        else:
            plaintext.append(char)
    
    return ''.join(plaintext)

# Given ciphertext
ciphertext = "OBKR UOXOGHULBSOLIFBBWFLRVQQPRNGKSSO TWTQSJQSSEKZZWATJKLUDIAWINFBNYP VTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"

# Partial key from clues
partial_key = "CLOCK"  # This is just a starting point, we need to expand it

# Decrypt the message using the partial key
decrypted_message = vigenere_decrypt(ciphertext.replace(" ", ""), partial_key)
print(decrypted_message)
def expand_key(partial_key, length):
    expanded_key = (partial_key * (length // len(partial_key))) + partial_key[:length % len(partial_key)]
    return expanded_key

# Length of the ciphertext without spaces
ciphertext_length = len(ciphertext.replace(" ", ""))

# Expand the partial key to match the length of the ciphertext
expanded_key = expand_key(partial_key, ciphertext_length)

# Decrypt the message using the expanded key
decrypted_message = vigenere_decrypt(ciphertext.replace(" ", ""), expanded_key)
print(decrypted_message)
