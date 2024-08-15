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

def build_partial_key():
    key = [''] * 74  # Initialize an empty key of length 74

    # Clue 1: "MZFPK" -> "CLOCK"
    key[69:74] = "CLOCK"

    # Clue 2: 74th letter is K in both plaintext and ciphertext
    key[73] = 'A'

    # Clue 3: "QQPRNGKSS" -> "NORTHEAST"
    key[25] = chr((ord('Q') - ord('N') + 26) % 26 + ord('A'))
    key[26] = chr((ord('Q') - ord('O') + 26) % 26 + ord('A'))
    key[27] = chr((ord('P') - ord('R') + 26) % 26 + ord('A'))
    key[28] = chr((ord('R') - ord('T') + 26) % 26 + ord('A'))
    key[29] = chr((ord('N') - ord('H') + 26) % 26 + ord('A'))
    key[30] = chr((ord('G') - ord('E') + 26) % 26 + ord('A'))
    key[31] = chr((ord('K') - ord('A') + 26) % 26 + ord('A'))
    key[32] = chr((ord('S') - ord('S') + 26) % 26 + ord('A'))
    key[33] = chr((ord('S') - ord('T') + 26) % 26 + ord('A'))

    # Clue 4: "FLRV" -> "EAST"
    key[21] = chr((ord('F') - ord('E') + 26) % 26 + ord('A'))
    key[22] = chr((ord('L') - ord('A') + 26) % 26 + ord('A'))
    key[23] = chr((ord('R') - ord('S') + 26) % 26 + ord('A'))
    key[24] = chr((ord('V') - ord('T') + 26) % 26 + ord('A'))

    # Clue 5: "BERLIN"
    key[63] = chr((ord('B') - ord('B') + 26) % 26 + ord('A'))
    key[64] = chr((ord('E') - ord('E') + 26) % 26 + ord('A'))
    key[65] = chr((ord('R') - ord('R') + 26) % 26 + ord('A'))
    key[66] = chr((ord('L') - ord('L') + 26) % 26 + ord('A'))
    key[67] = chr((ord('I') - ord('I') + 26) % 26 + ord('A'))
    key[68] = chr((ord('N') - ord('N') + 26) % 26 + ord('A'))

    return ''.join(key)

# Given ciphertext
ciphertext = "OBKR UOXOGHULBSOLIFBBWFLRVQQPRNGKSSO TWTQSJQSSEKZZWATJKLUDIAWINFBNYP VTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"

# Build the partial key
partial_key = build_partial_key()

# Fill in the missing parts of the key with 'A' (no shift)
partial_key = ''.join([char if char else 'A' for char in partial_key])

# Decrypt the message using the refined key
decrypted_message = vigenere_decrypt(ciphertext.replace(" ", ""), partial_key)
print(decrypted_message)