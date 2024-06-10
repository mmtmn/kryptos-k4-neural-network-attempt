#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define INPUT_NODES 1
#define OUTPUT_NODES 1
#define LEARNING_RATE 0.01
#define EPOCHS 100

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

void forward_pass(double input) {
    rnn.outputs[0][0] = input;

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

void train(double *inputs, double *targets, int sequence_length) {
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

    int layer_sizes[] = { INPUT_NODES, 100, 100, 100, OUTPUT_NODES }; // Modify this to change architecture
    int num_layers = sizeof(layer_sizes) / sizeof(layer_sizes[0]);

    initialize_rnn(num_layers, layer_sizes);

    // Training data
    double displacements[] = {
        -12, 10, 3, 2, 0, 8, 7, 1, -11, -10, 7, -4, 0,
        8, 11, 5, 12, 3, -2, -8, -11, 8, -1, 0
    };
    int num_displacements = sizeof(displacements) / sizeof(displacements[0]);

    // Normalize displacements for training
    double normalized_displacements[num_displacements];
    for (int i = 0; i < num_displacements; i++) {
        normalized_displacements[i] = (displacements[i] + 26) / 52.0;
    }

    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double total_error = 0.0;

        for (int i = 0; i < num_displacements; i++) {
            forward_pass(normalized_displacements[i]);
            int output_layer = rnn.num_layers - 1;
            total_error += fabs(normalized_displacements[i] - rnn.outputs[output_layer][0]);
        }

        if (epoch % 100 == 0) {
            printf("Epoch %d, Total Error: %f\n", epoch, total_error);
        }

        train(normalized_displacements, normalized_displacements, num_displacements);
    }

    // Validate the model on training data to check learning
    for (int i = 0; i < num_displacements; i++) {
        forward_pass(normalized_displacements[i]);
        int output_layer = rnn.num_layers - 1;
        printf("True displacement: %f, Predicted displacement: %f\n", displacements[i], rnn.outputs[output_layer][0] * 52.0 - 26);
    }

    // Decrypt the full ciphertext
    char ciphertext[] = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
    int ciphertext_len = strlen(ciphertext);
    char decrypted_text[ciphertext_len + 1];

    // Initialize hidden state to zero for decryption
    for (int i = 0; i < rnn.layer_sizes[1]; i++) {
        rnn.outputs[1][i] = 0.0;
    }

    for (int i = 0; i < ciphertext_len; i++) {
        double input = (ciphertext[i] - 'A') / 25.0;
        forward_pass(input);
        int output_layer = rnn.num_layers - 1;
        double shift = rnn.outputs[output_layer][0] * 52.0 - 26;
        decrypted_text[i] = 'A' + (ciphertext[i] - 'A' + (int)shift + 26) % 26;
    }
    decrypted_text[ciphertext_len] = '\0';
    printf("Decrypted Text: %s\n", decrypted_text);

    free_rnn();
    return 0;
}
