#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define INPUT_NODES 54
#define HIDDEN_NODES 10
#define OUTPUT_NODES 1
#define LEARNING_RATE 0.001
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
    *output = sigmoid(*output) * 52 - 26;
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

void encode_number_with_position_and_displacements(int num, int position, double displacement, double *output) {
    for (int i = 0; i < INPUT_NODES; i++) {
        output[i] = 0.0;
    }
    if (num >= 0 && num < 26) {
        output[num] = 1.0;
    }
    if (position >= 0 && position < 26) {
        output[26 + position % 26] = 1.0;
    }
    output[52] = displacement / 26.0;
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

    int positions_22_25[] = {22, 23, 24, 25};
    double displacements_22_25[] = {-12, 10, 3, 2};
    int positions_26_34[] = {26, 27, 28, 29, 30, 31, 32, 33, 34};
    double displacements_26_34[] = {0, 8, 7, 1, -11, -10, 7, -4, 0};
    int positions_64_69[] = {64, 65, 66, 67, 68, 69};
    double displacements_64_69[] = {8, 11, 5, 12, 3, -2};
    int positions_70_74[] = {70, 71, 72, 73, 74};
    double displacements_70_74[] = {-8, -11, 8, -1, 0};

    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double total_error = 0.0;

        for (int i = 0; i < 4; i++) {
            double input[INPUT_NODES];
            encode_number_with_position_and_displacements(i, positions_22_25[i], displacements_22_25[i], input);
            double target = displacements_22_25[i];
            train(input, target, &total_error);
        }

        for (int i = 0; i < 9; i++) {
            double input[INPUT_NODES];
            encode_number_with_position_and_displacements(i, positions_26_34[i], displacements_26_34[i], input);
            double target = displacements_26_34[i];
            train(input, target, &total_error);
        }

        for (int i = 0; i < 6; i++) {
            double input[INPUT_NODES];
            encode_number_with_position_and_displacements(i, positions_64_69[i], displacements_64_69[i], input);
            double target = displacements_64_69[i];
            train(input, target, &total_error);
        }

        for (int i = 0; i < 5; i++) {
            double input[INPUT_NODES];
            encode_number_with_position_and_displacements(i, positions_70_74[i], displacements_70_74[i], input);
            double target = displacements_70_74[i];
            train(input, target, &total_error);
        }

        printf("Epoch %d, Total Error: %f\n", epoch, total_error);
    }

    char ciphertext[] = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
    int ciphertext_len = strlen(ciphertext);
    char decrypted_text[ciphertext_len + 1];
    for (int i = 0; i < ciphertext_len; i++) {
        double input[INPUT_NODES];
        double hidden_output[HIDDEN_NODES];
        double output;
        encode_number_with_position_and_displacements(i, i, 0, input);
        forward_pass(input, hidden_output, &output);
        int shift = (int)round(output);
        decrypted_text[i] = ((ciphertext[i] - 'A' - shift + 26) % 26) + 'A';
    }
    decrypted_text[ciphertext_len] = '\0';
    printf("Decrypted Text: %s\n", decrypted_text);

    free_network();
    return 0;
}
