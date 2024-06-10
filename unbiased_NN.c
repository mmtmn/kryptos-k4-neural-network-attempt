#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define INPUT_NODES 54 // 26 for one-hot encoding of character + 26 for position encoding + 2 for shifts
#define HIDDEN_NODES 10
#define OUTPUT_NODES 1
#define LEARNING_RATE 0.01
#define EPOCHS 100

typedef struct {
    double **weights;
    double *biases;
} HiddenLayer;

typedef struct {
    double **weights;
    double *biases;
} OutputLayer;

HiddenLayer hidden_layer;
OutputLayer output_layer;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

void initialize_network() {
    srand(time(NULL));

    hidden_layer.weights = (double **)malloc(INPUT_NODES * sizeof(double *));
    for (int i = 0; i < INPUT_NODES; i++) {
        hidden_layer.weights[i] = (double *)malloc(HIDDEN_NODES * sizeof(double));
    }

    hidden_layer.biases = (double *)malloc(HIDDEN_NODES * sizeof(double));

    output_layer.weights = (double **)malloc(HIDDEN_NODES * sizeof(double *));
    for (int i = 0; i < HIDDEN_NODES; i++) {
        output_layer.weights[i] = (double *)malloc(OUTPUT_NODES * sizeof(double));
    }

    output_layer.biases = (double *)malloc(OUTPUT_NODES * sizeof(double));

    for (int i = 0; i < INPUT_NODES; i++) {
        for (int j = 0; j < HIDDEN_NODES; j++) {
            hidden_layer.weights[i][j] = ((double)rand() / RAND_MAX) - 0.5;
        }
    }
    for (int i = 0; i < HIDDEN_NODES; i++) {
        hidden_layer.biases[i] = ((double)rand() / RAND_MAX) - 0.5;
    }
    for (int i = 0; i < HIDDEN_NODES; i++) {
        for (int j = 0; j < OUTPUT_NODES; j++) {
            output_layer.weights[i][j] = ((double)rand() / RAND_MAX) - 0.5;
        }
    }
    for (int i = 0; i < OUTPUT_NODES; i++) {
        output_layer.biases[i] = ((double)rand() / RAND_MAX) - 0.5;
    }
}

void forward_pass(double input[], double hidden_output[], double *output) {
    for (int i = 0; i < HIDDEN_NODES; i++) {
        hidden_output[i] = hidden_layer.biases[i];
        for (int j = 0; j < INPUT_NODES; j++) {
            hidden_output[i] += input[j] * hidden_layer.weights[j][i];
        }
        hidden_output[i] = sigmoid(hidden_output[i]);
    }
    *output = output_layer.biases[0];
    for (int i = 0; i < HIDDEN_NODES; i++) {
        *output += hidden_output[i] * output_layer.weights[i][0];
    }
    *output = sigmoid(*output) * 52 - 26; // scale output to range -26 to 26
}

void train(double input[], double target, double *total_error) {
    double hidden_output[HIDDEN_NODES];
    double output;
    forward_pass(input, hidden_output, &output);

    double output_error = (target - output) * sigmoid_derivative((output + 26) / 52.0);

    double hidden_error[HIDDEN_NODES];

    for (int i = 0; i < HIDDEN_NODES; i++) {
        hidden_error[i] = output_error * output_layer.weights[i][0];
        hidden_error[i] *= sigmoid_derivative(hidden_output[i]);
    }

    for (int i = 0; i < HIDDEN_NODES; i++) {
        output_layer.weights[i][0] += LEARNING_RATE * output_error * hidden_output[i];
    }

    output_layer.biases[0] += LEARNING_RATE * output_error;

    for (int i = 0; i < INPUT_NODES; i++) {
        for (int j = 0; j < HIDDEN_NODES; j++) {
            hidden_layer.weights[i][j] += LEARNING_RATE * hidden_error[j] * input[i];
        }
    }

    for (int i = 0; i < HIDDEN_NODES; i++) {
        hidden_layer.biases[i] += LEARNING_RATE * hidden_error[i];
    }

    *total_error += fabs(target - output);
}

void encode_number_with_position_and_shifts(int num, int position, int pos_shift, int neg_shift, double *output) {
    for (int i = 0; i < 54; i++) {
        output[i] = 0.0;
    }
    output[num] = 1.0;
    output[26 + position % 26] = 1.0;
    output[52] = pos_shift / 26.0; // Normalize the shift values
    output[53] = neg_shift / 26.0; // Normalize the shift values
}

void encode_shift(int shift, double *output) {
    output[0] = shift / 26.0;
}

void free_network() {
    for (int i = 0; i < INPUT_NODES; i++) {
        free(hidden_layer.weights[i]);
    }
    free(hidden_layer.weights);
    free(hidden_layer.biases);

    for (int i = 0; i < HIDDEN_NODES; i++) {
        free(output_layer.weights[i]);
    }
    free(output_layer.weights);
    free(output_layer.biases);
}

int main() {
    initialize_network();

    // Training data based on the clues
    int ciphertext_clues_shifts[][8] = {
        {14, 4, 11, 8, 18, 7, -1, -1},
        {12, 25, 15, 11, 10, -1, -1, -1},
        {16, 16, 15, 17, 13, 6, 10, 18},
        {5, 11, 17, 21, -1, -1, -1, -1}
    };
    int clue_positions[] = {63, 69, 25, 21}; // Starting positions of the clues in the ciphertext
    int num_clues = 4;

    // Additional training data
    int additional_positions[] = {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 26, 27, 28, 29, 30, 31, 32, 33, 34, 22, 23, 24, 25};
    int positive_shifts[] = {14, 4, 11, 8, 18, 7, 13, 12, 10, 11, 0, 23, 24, 2, 3, 13, 14, 18, 0, 1, 25, 15, 1, 24};
    int negative_shifts[] = {-12, -22, -15, -18, -8, -13, -11, -14, -16, -11, -26, -3, -2, -24, -23, -13, -12, -8, -26, -25, -1, -11, -18, -5};
    int num_additional_clues = 24;

    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double total_error = 0.0;
        for (int clue = 0; clue < num_clues; clue++) {
            int len = sizeof(ciphertext_clues_shifts[clue]) / sizeof(ciphertext_clues_shifts[clue][0]);
            for (int i = 0; i < len; i++) {
                if (ciphertext_clues_shifts[clue][i] == -1) continue; // Skip unused elements
                double input[54];
                encode_number_with_position_and_shifts(i, clue_positions[clue] + i, 0, 0, input);
                double target = ciphertext_clues_shifts[clue][i];
                train(input, target, &total_error);
            }
        }
        for (int i = 0; i < num_additional_clues; i++) {
            double input[54];
            encode_number_with_position_and_shifts(i, additional_positions[i], positive_shifts[i], negative_shifts[i], input);
            double target = positive_shifts[i];
            train(input, target, &total_error);
        }

        printf("Epoch %d, Total Error: %f\n", epoch, total_error);
    }

    // Decrypt the full ciphertext
    char ciphertext[] = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
    int ciphertext_len = strlen(ciphertext);
    char decrypted_text[ciphertext_len + 1];
    for (int i = 0; i < ciphertext_len; i++) {
        double input[54];
        double hidden_output[HIDDEN_NODES];
        double output;
        encode_number_with_position_and_shifts(i, i, 0, 0, input);
        forward_pass(input, hidden_output, &output);
        int shift = (int)round(output);
        decrypted_text[i] = ((ciphertext[i] - 'A' - shift + 26) % 26) + 'A';
    }
    decrypted_text[ciphertext_len] = '\0';
    printf("Decrypted Text: %s\n", decrypted_text);

    free_network();
    return 0;
}
