#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define INPUT_NODES 2 // Position and displacement
#define HIDDEN_NODES 100 // Hidden state size
#define OUTPUT_NODES 1
#define LEARNING_RATE 0.01
#define EPOCHS 10000

typedef struct {
    int input_size;
    int hidden_size;
    int output_size;
    double **input_weights;
    double **hidden_weights;
    double *output_weights;
    double *hidden_state;
    double *output;
    double *biases;
} RNN;

RNN rnn;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

void initialize_rnn(int input_size, int hidden_size, int output_size) {
    rnn.input_size = input_size;
    rnn.hidden_size = hidden_size;
    rnn.output_size = output_size;

    rnn.input_weights = (double **)malloc(input_size * sizeof(double *));
    rnn.hidden_weights = (double **)malloc(hidden_size * sizeof(double *));
    for (int i = 0; i < input_size; i++) {
        rnn.input_weights[i] = (double *)malloc(hidden_size * sizeof(double));
        for (int j = 0; j < hidden_size; j++) {
            rnn.input_weights[i][j] = ((double)rand() / RAND_MAX) - 0.5;
        }
    }

    for (int i = 0; i < hidden_size; i++) {
        rnn.hidden_weights[i] = (double *)malloc(hidden_size * sizeof(double));
        for (int j = 0; j < hidden_size; j++) {
            rnn.hidden_weights[i][j] = ((double)rand() / RAND_MAX) - 0.5;
        }
    }

    rnn.output_weights = (double *)malloc(hidden_size * sizeof(double));
    for (int i = 0; i < hidden_size; i++) {
        rnn.output_weights[i] = ((double)rand() / RAND_MAX) - 0.5;
    }

    rnn.biases = (double *)malloc(hidden_size * sizeof(double));
    for (int i = 0; i < hidden_size; i++) {
        rnn.biases[i] = ((double)rand() / RAND_MAX) - 0.5;
    }

    rnn.hidden_state = (double *)calloc(hidden_size, sizeof(double));
    rnn.output = (double *)malloc(output_size * sizeof(double));
}

void forward_pass(double *input) {
    double *new_hidden_state = (double *)malloc(rnn.hidden_size * sizeof(double));

    for (int j = 0; j < rnn.hidden_size; j++) {
        new_hidden_state[j] = rnn.biases[j];
        for (int i = 0; i < rnn.input_size; i++) {
            new_hidden_state[j] += input[i] * rnn.input_weights[i][j];
        }
        for (int i = 0; i < rnn.hidden_size; i++) {
            new_hidden_state[j] += rnn.hidden_state[i] * rnn.hidden_weights[i][j];
        }
        new_hidden_state[j] = sigmoid(new_hidden_state[j]);
    }

    for (int i = 0; i < rnn.hidden_size; i++) {
        rnn.hidden_state[i] = new_hidden_state[i];
    }

    free(new_hidden_state);

    for (int i = 0; i < rnn.output_size; i++) {
        rnn.output[i] = 0.0;
        for (int j = 0; j < rnn.hidden_size; j++) {
            rnn.output[i] += rnn.hidden_state[j] * rnn.output_weights[j];
        }
        rnn.output[i] = sigmoid(rnn.output[i]);
    }
}

void train(double **inputs, double *targets, int sequence_length) {
    double **deltas = (double **)malloc(2 * sizeof(double *));
    deltas[0] = (double *)malloc(rnn.hidden_size * sizeof(double));
    deltas[1] = (double *)malloc(rnn.output_size * sizeof(double));

    for (int t = 0; t < sequence_length; t++) {
        forward_pass(inputs[t]);

        for (int i = 0; i < rnn.output_size; i++) {
            deltas[1][i] = (targets[t] - rnn.output[i]) * sigmoid_derivative(rnn.output[i]);
        }

        for (int i = 0; i < rnn.hidden_size; i++) {
            deltas[0][i] = 0.0;
            for (int j = 0; j < rnn.output_size; j++) {
                deltas[0][i] += deltas[1][j] * rnn.output_weights[i];
            }
            deltas[0][i] *= sigmoid_derivative(rnn.hidden_state[i]);
        }

        for (int i = 0; i < rnn.hidden_size; i++) {
            for (int j = 0; j < rnn.output_size; j++) {
                rnn.output_weights[i] += LEARNING_RATE * deltas[1][j] * rnn.hidden_state[i];
            }
        }

        for (int i = 0; i < rnn.hidden_size; i++) {
            for (int j = 0; j < rnn.hidden_size; j++) {
                rnn.hidden_weights[i][j] += LEARNING_RATE * deltas[0][i] * rnn.hidden_state[j];
            }
        }

        for (int i = 0; i < rnn.input_size; i++) {
            for (int j = 0; j < rnn.hidden_size; j++) {
                rnn.input_weights[i][j] += LEARNING_RATE * deltas[0][j] * inputs[t][i];
            }
        }

        for (int i = 0; i < rnn.hidden_size; i++) {
            rnn.biases[i] += LEARNING_RATE * deltas[0][i];
        }
    }

    free(deltas[0]);
    free(deltas[1]);
    free(deltas);
}

void free_rnn() {
    for (int i = 0; i < rnn.input_size; i++) {
        free(rnn.input_weights[i]);
    }
    for (int i = 0; i < rnn.hidden_size; i++) {
        free(rnn.hidden_weights[i]);
    }
    free(rnn.input_weights);
    free(rnn.hidden_weights);
    free(rnn.output_weights);
    free(rnn.biases);
    free(rnn.hidden_state);
    free(rnn.output);
}

int main() {
    srand(time(NULL));

    int input_size = INPUT_NODES;
    int hidden_size = HIDDEN_NODES;
    int output_size = OUTPUT_NODES;

    initialize_rnn(input_size, hidden_size, output_size);

    // Training data with corrected displacements
    int positions[] = {
        22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
        64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74
    };
    double displacements[] = {
        -12, 10, 3, 2, 0, 8, 7, 1, -11, -10, 7, -4, 0,
        8, 11, 5, 12, 3, -2, -8, -11, 8, -1, 0
    };
    int num_displacements = sizeof(displacements) / sizeof(displacements[0]);

    // Normalize positions and displacements for training
    double **normalized_inputs = (double **)malloc(num_displacements * sizeof(double *));
    double normalized_displacements[num_displacements];
    for (int i = 0; i < num_displacements; i++) {
        normalized_inputs[i] = (double *)malloc(INPUT_NODES * sizeof(double));
        normalized_inputs[i][0] = (positions[i] - 22) / 52.0; // Normalized position
        normalized_inputs[i][1] = (displacements[i] + 12) / 24.0; // Normalized displacement
        normalized_displacements[i] = (displacements[i] + 12) / 24.0;
    }

    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double total_error = 0.0;

        for (int i = 0; i < num_displacements; i++) {
            forward_pass(normalized_inputs[i]);
            total_error += fabs(normalized_displacements[i] - rnn.output[0]);
        }

        if (epoch % 100 == 0) {
            printf("Epoch %d, Total Error: %f\n", epoch, total_error);
        }

        train(normalized_inputs, normalized_displacements, num_displacements);
    }

    // Validate the model on training data to check learning
    for (int i = 0; i < num_displacements; i++) {
        forward_pass(normalized_inputs[i]);
        printf("\nPosition: %d, True displacement: %f, Predicted displacement: %f\n", positions[i], displacements[i], rnn.output[0] * 24.0 - 12);
    }

    // Decrypt the full ciphertext
    char ciphertext[] = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
    int ciphertext_len = strlen(ciphertext);
    char decrypted_text[ciphertext_len + 1];

    printf("\nPredicted Displacements:\n");
    double predicted_displacements[97];
    for (int i = 0; i < 97; i++) {
        double input[] = {(i - 22) / 52.0, 0}; // Normalized position with dummy displacement
        forward_pass(input);
        double shift = rnn.output[0] * 24.0 - 12;
        predicted_displacements[i] = shift;
        printf("%d: %f\n", i, shift);
    }

    printf("\nDetailed Decryption Report:\n");
    printf("Index\tOriginal\tPredicted Shift\tRounded Shift\tDecrypted\n");

    for (int i = 0; i < ciphertext_len; i++) {
        int position_index = i; // Adjust to match the position normalization used in training
        if (position_index >= 0 && position_index < 97) {
            double shift = predicted_displacements[position_index];
            int rounded_shift = (int)round(shift);
            char decrypted_char = 'A' + (ciphertext[i] - 'A' + rounded_shift + 26) % 26;
            decrypted_text[i] = decrypted_char;
            printf("%d\t%c\t\t%f\t%d\t\t%c\n", i, ciphertext[i], shift, rounded_shift, decrypted_char);
        } else {
            decrypted_text[i] = ciphertext[i]; // Handle non-alphabet characters
        }
    }

    decrypted_text[ciphertext_len] = '\0';

    printf("\nDecrypted Text: %s\n", decrypted_text);

    for (int i = 0; i < num_displacements; i++) {
        free(normalized_inputs[i]);
    }
    free(normalized_inputs);
    free_rnn();
    return 0;
}
