#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define INPUT_NODES 2 // Only 2 for displacements
#define HIDDEN_NODES 10
#define OUTPUT_NODES 1 // Only one output representing the shift
#define LEARNING_RATE 0.01
#define EPOCHS 1000000

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

void forward_pass(double input[], double hidden_output[], double output[]) {
    for (int i = 0; i < HIDDEN_NODES; i++) {
        hidden_output[i] = hidden_layer.biases[i];
        for (int j = 0; j < INPUT_NODES; j++) {
            hidden_output[i] += input[j] * hidden_layer.weights[j][i];
        }
        hidden_output[i] = sigmoid(hidden_output[i]);
    }
    for (int i = 0; i < OUTPUT_NODES; i++) {
        output[i] = output_layer.biases[i];
        for (int j = 0; j < HIDDEN_NODES; j++) {
            output[i] += hidden_output[j] * output_layer.weights[j][i];
        }
        output[i] = sigmoid(output[i]);
    }
}

void train(double input[], double target[], double *total_error) {
    double hidden_output[HIDDEN_NODES];
    double output[OUTPUT_NODES];
    forward_pass(input, hidden_output, output);

    double output_error[OUTPUT_NODES];
    double hidden_error[HIDDEN_NODES];

    for (int i = 0; i < OUTPUT_NODES; i++) {
        output_error[i] = (target[i] - output[i]) * sigmoid_derivative(output[i]);
        *total_error += fabs(target[i] - output[i]);
    }

    for (int i = 0; i < HIDDEN_NODES; i++) {
        hidden_error[i] = 0.0;
        for (int j = 0; j < OUTPUT_NODES; j++) {
            hidden_error[i] += output_error[j] * output_layer.weights[i][j];
        }
        hidden_error[i] *= sigmoid_derivative(hidden_output[i]);
    }

    for (int i = 0; i < HIDDEN_NODES; i++) {
        for (int j = 0; j < OUTPUT_NODES; j++) {
            output_layer.weights[i][j] += LEARNING_RATE * output_error[j] * hidden_output[i];
        }
    }

    for (int i = 0; i < OUTPUT_NODES; i++) {
        output_layer.biases[i] += LEARNING_RATE * output_error[i];
    }

    for (int i = 0; i < INPUT_NODES; i++) {
        for (int j = 0; j < HIDDEN_NODES; j++) {
            hidden_layer.weights[i][j] += LEARNING_RATE * hidden_error[j] * input[i];
        }
    }

    for (int i = 0; i < HIDDEN_NODES; i++) {
        hidden_layer.biases[i] += LEARNING_RATE * hidden_error[i];
    }
}

void encode_input_with_displacements(double displacement, double *output) {
    output[0] = displacement / 26.0;
    output[1] = (displacement + 26) / 26.0; // Adding displacement offset for better scaling
}

void encode_output(double displacement, double *output) {
    output[0] = (displacement + 26) / 52.0; // Scaling the displacement to be in range [0, 1]
}

double decode_output(double *output) {
    return output[0] * 52.0 - 26; // Scale back to the range [-26, 26]
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

    // Training data
    double displacements[] = {
        -12, 10, 3, 2, 0, 8, 7, 1, -11, -10, 7, -4, 0,
        8, 11, 5, 12, 3, -2, -8, -11, 8, -1, 0
    };

    int num_displacements = sizeof(displacements) / sizeof(displacements[0]);

    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double total_error = 0.0;

        for (int i = 0; i < num_displacements; i++) {
            double input[INPUT_NODES];
            double target[OUTPUT_NODES];
            encode_input_with_displacements(displacements[i], input);
            encode_output(displacements[i], target);
            train(input, target, &total_error);
        }

        if (epoch % 1000 == 0) {
            printf("Epoch %d, Total Error: %f\n", epoch, total_error);
        }
    }

    // Validate the model on training data to check learning
    for (int i = 0; i < num_displacements; i++) {
        double input[INPUT_NODES];
        double hidden_output[HIDDEN_NODES];
        double output[OUTPUT_NODES];
        encode_input_with_displacements(displacements[i], input);
        forward_pass(input, hidden_output, output);
        double predicted_shift = decode_output(output);
        printf("True displacement: %f, Predicted displacement: %f\n", displacements[i], predicted_shift);
    }

    // Decrypt the full ciphertext
    char ciphertext[] = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
    int ciphertext_len = strlen(ciphertext);
    char decrypted_text[ciphertext_len + 1];
    for (int i = 0; i < ciphertext_len; i++) {
        double input[INPUT_NODES];
        double hidden_output[HIDDEN_NODES];
        double output[OUTPUT_NODES];
        // Use a default displacement for decryption as the specific displacements are not known
        encode_input_with_displacements(0, input);
        forward_pass(input, hidden_output, output);
        double shift = decode_output(output);
        decrypted_text[i] = 'A' + (ciphertext[i] - 'A' + (int)shift + 26) % 26;
    }
    decrypted_text[ciphertext_len] = '\0';
    printf("Decrypted Text: %s\n", decrypted_text);

    free_network();
    return 0;
}
