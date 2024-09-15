#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define CIPHERTEXT_LENGTH 97
#define POPULATION_SIZE 1000
#define GENERATIONS 10000
#define ALPHABET_LENGTH 26
#define MUTATION_RATE 0.02

// The K4 ciphertext
char ciphertext[CIPHERTEXT_LENGTH] = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOLGUFBBF";

// Expected letter frequencies in English text (percent)
double english_freq[ALPHABET_LENGTH] = {
    12.70, 9.06, 8.17, 7.51, 6.97, 6.75, 6.33, 6.09, 5.99, 4.25, 
    4.03, 2.78, 2.76, 2.41, 2.36, 2.23, 2.02, 1.97, 1.93, 1.49, 
    0.98, 0.77, 0.15, 0.15, 0.10, 0.07
};

// Function to shuffle the alphabet and generate a random key
void generate_key(char *key) {
    char alphabet[ALPHABET_LENGTH] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < ALPHABET_LENGTH; i++) {
        int j = rand() % ALPHABET_LENGTH;
        char temp = alphabet[i];
        alphabet[i] = alphabet[j];
        alphabet[j] = temp;
    }
    memcpy(key, alphabet, ALPHABET_LENGTH);
}

// Function to calculate the chi-squared fitness score
double fitness(const char *plaintext) {
    int freq[ALPHABET_LENGTH] = {0};
    int total = 0;
    for (int i = 0; i < CIPHERTEXT_LENGTH; i++) {
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z') {
            freq[plaintext[i] - 'A']++;
            total++;
        }
    }

    double chi_squared = 0;
    for (int i = 0; i < ALPHABET_LENGTH; i++) {
        double expected = english_freq[i] * total / 100;
        if (expected > 0) {
            double observed = freq[i];
            chi_squared += (observed - expected) * (observed - expected) / expected;
        }
    }

    return chi_squared;
}

// Decrypts the ciphertext using the key
void decrypt(const char *ciphertext, char *plaintext, const char *key) {
    char reverse_key[ALPHABET_LENGTH];
    for (int i = 0; i < ALPHABET_LENGTH; i++) {
        reverse_key[key[i] - 'A'] = 'A' + i;
    }
    for (int i = 0; i < CIPHERTEXT_LENGTH; i++) {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            plaintext[i] = reverse_key[ciphertext[i] - 'A'];
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[CIPHERTEXT_LENGTH] = '\0';
}

// Mutates the key with a given mutation rate
void mutate(char *key) {
    for (int i = 0; i < ALPHABET_LENGTH; i++) {
        if ((double) rand() / RAND_MAX < MUTATION_RATE) {
            int j = rand() % ALPHABET_LENGTH;
            char temp = key[i];
            key[i] = key[j];
            key[j] = temp;
        }
    }
}

// Crossover between two parent keys
void crossover(const char *parent1, const char *parent2, char *child) {
    int start = rand() % ALPHABET_LENGTH;
    int end = rand() % ALPHABET_LENGTH;

    if (start > end) {
        int temp = start;
        start = end;
        end = temp;
    }

    int used[ALPHABET_LENGTH] = {0};
    for (int i = start; i < end; i++) {
        child[i] = parent1[i];
        used[parent1[i] - 'A'] = 1;
    }

    int child_idx = 0;
    for (int i = 0; i < ALPHABET_LENGTH; i++) {
        if (!used[parent2[i] - 'A']) {
            while (child_idx >= start && child_idx < end) child_idx++;
            child[child_idx++] = parent2[i];
        }
    }
}

// Selects the best candidate from the population using tournament selection
void select_best(char population[][ALPHABET_LENGTH], double scores[], char *best_key) {
    int best_index = 0;
    double best_score = scores[0];
    for (int i = 1; i < POPULATION_SIZE; i++) {
        if (scores[i] < best_score) {
            best_score = scores[i];
            best_index = i;
        }
    }
    memcpy(best_key, population[best_index], ALPHABET_LENGTH);
}

// The main genetic algorithm loop
void genetic_algorithm() {
    char population[POPULATION_SIZE][ALPHABET_LENGTH];
    char best_key[ALPHABET_LENGTH];
    double best_score = INFINITY;

    // Generate the initial population
    for (int i = 0; i < POPULATION_SIZE; i++) {
        generate_key(population[i]);
    }

    for (int gen = 0; gen < GENERATIONS; gen++) {
        char plaintext[CIPHERTEXT_LENGTH + 1];
        double scores[POPULATION_SIZE];

        // Evaluate each individual in the population
        for (int i = 0; i < POPULATION_SIZE; i++) {
            decrypt(ciphertext, plaintext, population[i]);
            scores[i] = fitness(plaintext);

            if (scores[i] < best_score) {
                best_score = scores[i];
                memcpy(best_key, population[i], ALPHABET_LENGTH);
                printf("Generation %d, New best score: %f\nPlaintext: %s\n", gen, best_score, plaintext);
            }
        }

        // Create the new population
        for (int i = 0; i < POPULATION_SIZE; i++) {
            char parent1[ALPHABET_LENGTH], parent2[ALPHABET_LENGTH], child[ALPHABET_LENGTH];
            select_best(population, scores, parent1);
            select_best(population, scores, parent2);
            crossover(parent1, parent2, child);
            mutate(child);
            memcpy(population[i], child, ALPHABET_LENGTH);
        }
    }

    // Final best result
    char final_plaintext[CIPHERTEXT_LENGTH + 1];
    decrypt(ciphertext, final_plaintext, best_key);
    printf("Best final plaintext: %s\n", final_plaintext);
}

int main() {
    srand(time(NULL));
    genetic_algorithm();
    return 0;
}
