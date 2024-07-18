#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define INPUT_NODES 2 // Position and displacement
#define OUTPUT_NODES 1
#define LEARNING_RATE 0.01
#define EPOCHS 10

typedef struct {
    int num_layers;
    int *layer_sizes;
    double **weights;
    double **biases;
    double **outputs;
} RNN;

RNN rnn;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

void initialize_rnn(int num_layers, int *layer_sizes) {
    rnn.num_layers = num_layers;
    rnn.layer_sizes = (int *)malloc(num_layers * sizeof(int));

    for (int i = 0; i < num_layers; i++) {
        rnn.layer_sizes[i] = layer_sizes[i];
    }

    rnn.weights = (double **)malloc((num_layers - 1) * sizeof(double *));
    rnn.biases = (double **)malloc((num_layers - 1) * sizeof(double *));
    rnn.outputs = (double **)malloc(num_layers * sizeof(double *));

    for (int i = 0; i < num_layers; i++) {
        rnn.outputs[i] = (double *)malloc(layer_sizes[i] * sizeof(double));
    }

    for (int i = 0; i < num_layers - 1; i++) {
        int rows = layer_sizes[i];
        int cols = layer_sizes[i + 1];
        rnn.weights[i] = (double *)malloc(rows * cols * sizeof(double));
        rnn.biases[i] = (double *)malloc(cols * sizeof(double));
        for (int j = 0; j < rows * cols; j++) {
            rnn.weights[i][j] = ((double)rand() / RAND_MAX) - 0.5;
        }
        for (int j = 0; j < cols; j++) {
            rnn.biases[i][j] = ((double)rand() / RAND_MAX) - 0.5;
        }
    }
}

void forward_pass(double *input) {
    for (int i = 0; i < rnn.layer_sizes[0]; i++) {
        rnn.outputs[0][i] = input[i];
    }

    for (int l = 0; l < rnn.num_layers - 1; l++) {
        int rows = rnn.layer_sizes[l];
        int cols = rnn.layer_sizes[l + 1];

        for (int j = 0; j < cols; j++) {
            rnn.outputs[l + 1][j] = rnn.biases[l][j];
            for (int i = 0; i < rows; i++) {
                rnn.outputs[l + 1][j] += rnn.outputs[l][i] * rnn.weights[l][i * cols + j];
            }
            rnn.outputs[l + 1][j] = sigmoid(rnn.outputs[l + 1][j]);
        }
    }
}

void train(double **inputs, double *targets, int sequence_length) {
    double **deltas = (double **)malloc(rnn.num_layers * sizeof(double *));
    for (int i = 0; i < rnn.num_layers; i++) {
        deltas[i] = (double *)malloc(rnn.layer_sizes[i] * sizeof(double));
    }

    for (int t = 0; t < sequence_length; t++) {
        forward_pass(inputs[t]);

        int output_layer = rnn.num_layers - 1;
        for (int i = 0; i < rnn.layer_sizes[output_layer]; i++) {
            deltas[output_layer][i] = (targets[t] - rnn.outputs[output_layer][i]) * sigmoid_derivative(rnn.outputs[output_layer][i]);
        }

        for (int l = rnn.num_layers - 2; l >= 0; l--) {
            int rows = rnn.layer_sizes[l];
            int cols = rnn.layer_sizes[l + 1];

            for (int i = 0; i < rows; i++) {
                deltas[l][i] = 0.0;
                for (int j = 0; j < cols; j++) {
                    deltas[l][i] += deltas[l + 1][j] * rnn.weights[l][i * cols + j];
                }
                deltas[l][i] *= sigmoid_derivative(rnn.outputs[l][i]);
            }

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    rnn.weights[l][i * cols + j] += LEARNING_RATE * deltas[l + 1][j] * rnn.outputs[l][i];
                }
            }
            for (int j = 0; j < cols; j++) {
                rnn.biases[l][j] += LEARNING_RATE * deltas[l + 1][j];
            }
        }
    }

    for (int i = 0; i < rnn.num_layers; i++) {
        free(deltas[i]);
    }
    free(deltas);
}

void free_rnn() {
    for (int i = 0; i < rnn.num_layers; i++) {
        free(rnn.outputs[i]);
    }
    free(rnn.outputs);

    for (int i = 0; i < rnn.num_layers - 1; i++) {
        free(rnn.weights[i]);
        free(rnn.biases[i]);
    }
    free(rnn.weights);
    free(rnn.biases);
    free(rnn.layer_sizes);
}

int main() {
    srand(time(NULL));

    int layer_sizes[] = { INPUT_NODES, 100, 100, OUTPUT_NODES }; // Modify this to change architecture
    int num_layers = sizeof(layer_sizes) / sizeof(layer_sizes[0]);

    initialize_rnn(num_layers, layer_sizes);

    // Training data
    int positions[] = {
        22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
        64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96
    };
    double displacements[] = {
        -12, 10, 3, 2, 0, 8, 7, 1, -11, -10, 7, -4, 0,
        8, 11, 5, 12, 3, -2, -8, -11, 8, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    int num_displacements = sizeof(displacements) / sizeof(displacements[0]);

    // Normalize positions and displacements for training
    double **normalized_inputs = (double **)malloc(num_displacements * sizeof(double *));
    double normalized_displacements[num_displacements];
    for (int i = 0; i < num_displacements; i++) {
        normalized_inputs[i] = (double *)malloc(INPUT_NODES * sizeof(double));
        normalized_inputs[i][0] = (positions[i] - 22) / 52.0; // Normalized position
        normalized_inputs[i][1] = (displacements[i] + 26) / 52.0; // Normalized displacement
        normalized_displacements[i] = (displacements[i] + 26) / 52.0;
    }

    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double total_error = 0.0;

        for (int i = 0; i < num_displacements; i++) {
            forward_pass(normalized_inputs[i]);
            int output_layer = rnn.num_layers - 1;
            total_error += fabs(normalized_displacements[i] - rnn.outputs[output_layer][0]);
        }

        if (epoch % 100 == 0) {
            printf("Epoch %d, Total Error: %f\n", epoch, total_error);
        }

        train(normalized_inputs, normalized_displacements, num_displacements);
    }

    // Validate the model on training data to check learning
    for (int i = 0; i < num_displacements; i++) {
        forward_pass(normalized_inputs[i]);
        int output_layer = rnn.num_layers - 1;
        printf("\nPosition: %d, True displacement: %f, Predicted displacement: %f\n", positions[i], displacements[i], rnn.outputs[output_layer][0] * 52.0 - 26);
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
        int output_layer = rnn.num_layers - 1;
        double shift = rnn.outputs[output_layer][0] * 52.0 - 26;
        predicted_displacements[i] = shift;
        printf("%d: %f\n", i, shift);
    }

    printf("\nDetailed Decryption Report:\n");
    printf("Index\tOriginal\tPredicted Shift\tRounded Shift\tDecrypted\n");

    for (int i = 0; i < ciphertext_len; i++) {
        double shift = predicted_displacements[i];
        int rounded_shift = (int)round(shift);
        char decrypted_char = 'A' + (ciphertext[i] - 'A' + rounded_shift + 26) % 26;
        decrypted_text[i] = decrypted_char;
        printf("%d\t%c\t\t%f\t%d\t\t%c\n", i, ciphertext[i], shift, rounded_shift, decrypted_char);
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
