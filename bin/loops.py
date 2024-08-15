import itertools

def apply_shift(char, shift):
    return chr((ord(char) - 65 + shift) % 26 + 65)

def check_displacement(ciphertext, displacement, shifts):
    for position, shift in displacement.items():
        if apply_shift(ciphertext[position], shifts[position]) != apply_shift(ciphertext[position], shift):
            return False
    return True

def decrypt_with_shifts(ciphertext, displacement):
    positions = sorted(displacement.keys())
    for num_displacements in range(1, 27):  # Trying 1 to 26 displacements
        print(f"Attempting {num_displacements} displacement(s)...")
        all_shifts = list(itertools.product(range(26), repeat=len(positions)))

        for shifts in all_shifts:
            shifts_dict = dict(zip(positions, shifts))
            if check_displacement(ciphertext, displacement, shifts_dict):
                decrypted_text = ''.join(apply_shift(char, shifts_dict.get(i, 0)) for i, char in enumerate(ciphertext))
                return decrypted_text, shifts_dict
    return None, None

ciphertext = "UOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR"

displacement = {
    22: 12, 23: 15, 24: 25, 25: 20,
    26: 19, 27: 22, 28: 1, 29: 25,
    30: 7, 31: 15, 32: 22, 33: 1,
    34: 23, 64: 18, 65: 8, 66: 8,
    67: 20, 68: 7, 69: 23, 70: 20,
    71: 21, 72: 15, 73: 8, 74: 15
}

decrypted_text, shifts_dict = decrypt_with_shifts(ciphertext, displacement)

if decrypted_text:
    print(f"Decrypted text: {decrypted_text}")
    print(f"Shifts used: {shifts_dict}")
else:
    print("No valid decryption found.")