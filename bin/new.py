
def vigenere_decrypt(ciphertext, key):
    decrypted_text = []
    key_length = len(key)
    key_as_int = [ord(i) for i in key]
    ciphertext_int = [ord(i) for i in ciphertext]
    for i in range(len(ciphertext_int)):
        value = (ciphertext_int[i] - key_as_int[i % key_length]) % 26
        decrypted_text.append(chr(value + 65))
    return ''.join(decrypted_text)

ciphertext = "UOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"
key1 = "kryptos".upper()
key2 = "opensesame".upper()

# Decrypt using the first key
decrypted_text1 = vigenere_decrypt(ciphertext, key1)
print("Decrypted with key 'kryptos':", decrypted_text1)

# Decrypt using the second key
decrypted_text2 = vigenere_decrypt(ciphertext, key2)
print("Decrypted with key 'opensesame':", decrypted_text2)

# Check if the decrypted text matches any of the clues
clues = [
    (22, "EAST"),
    (26, "NORTHEAST"),
    (64, "BERLIN"),
    (70, "CLOCK")
]

def check_clues(decrypted_text, clues):
    for position, word in clues:
        if decrypted_text[position:position+len(word)] == word:
            print(f"Clue matched at position {position}: {word}")
        else:
            print(f"Clue not matched at position {position}: {word}")

print("\nChecking clues for key 'kryptos':")
check_clues(decrypted_text1, clues)

print("\nChecking clues for key 'opensesame':")
check_clues(decrypted_text2, clues)
