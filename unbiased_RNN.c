#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define INPUT_NODES 1 // Only 1 input representing the current character
#define HIDDEN_NODES 10
#define OUTPUT_NODES 1 // Only one output representing the shift
#define LEARNING_RATE 0.01
#define EPOCHS 1000000

typedef struct {
    double *weights_input_hidden;
    double *weights_hidden_hidden;
    double *weights_hidden_output;
    double *hidden_bias;
    double *output_bias;
    double *hidden_state;
} RNN;

RNN rnn;

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

void initialize_rnn() {
    srand(time(NULL));

    rnn.weights_input_hidden = (double *)malloc(INPUT_NODES * HIDDEN_NODES * sizeof(double));
    rnn.weights_hidden_hidden = (double *)malloc(HIDDEN_NODES * HIDDEN_NODES * sizeof(double));
    rnn.weights_hidden_output = (double *)malloc(HIDDEN_NODES * OUTPUT_NODES * sizeof(double));
    rnn.hidden_bias = (double *)malloc(HIDDEN_NODES * sizeof(double));
    rnn.output_bias = (double *)malloc(OUTPUT_NODES * sizeof(double));
    rnn.hidden_state = (double *)malloc(HIDDEN_NODES * sizeof(double));

    for (int i = 0; i < INPUT_NODES * HIDDEN_NODES; i++) {
        rnn.weights_input_hidden[i] = ((double)rand() / RAND_MAX) - 0.5;
    }
    for (int i = 0; i < HIDDEN_NODES * HIDDEN_NODES; i++) {
        rnn.weights_hidden_hidden[i] = ((double)rand() / RAND_MAX) - 0.5;
    }
    for (int i = 0; i < HIDDEN_NODES * OUTPUT_NODES; i++) {
        rnn.weights_hidden_output[i] = ((double)rand() / RAND_MAX) - 0.5;
    }
    for (int i = 0; i < HIDDEN_NODES; i++) {
        rnn.hidden_bias[i] = ((double)rand() / RAND_MAX) - 0.5;
        rnn.hidden_state[i] = 0.0; // Initialize hidden state to zero
    }
    for (int i = 0; i < OUTPUT_NODES; i++) {
        rnn.output_bias[i] = ((double)rand() / RAND_MAX) - 0.5;
    }
}

void forward_pass_rnn(double input, double *hidden_state, double *output) {
    double new_hidden_state[HIDDEN_NODES];

    for (int i = 0; i < HIDDEN_NODES; i++) {
        new_hidden_state[i] = rnn.hidden_bias[i];
        new_hidden_state[i] += input * rnn.weights_input_hidden[i];
        for (int j = 0; j < HIDDEN_NODES; j++) {
            new_hidden_state[i] += hidden_state[j] * rnn.weights_hidden_hidden[i * HIDDEN_NODES + j];
        }
        new_hidden_state[i] = sigmoid(new_hidden_state[i]);
    }

    for (int i = 0; i < OUTPUT_NODES; i++) {
        output[i] = rnn.output_bias[i];
        for (int j = 0; j < HIDDEN_NODES; j++) {
            output[i] += new_hidden_state[j] * rnn.weights_hidden_output[j * OUTPUT_NODES + i];
        }
        output[i] = sigmoid(output[i]);
    }

    // Update hidden state
    for (int i = 0; i < HIDDEN_NODES; i++) {
        hidden_state[i] = new_hidden_state[i];
    }
}

void train_rnn(double *inputs, double *targets, int sequence_length) {
    double hidden_states[sequence_length][HIDDEN_NODES];
    double outputs[sequence_length][OUTPUT_NODES];
    double output_errors[sequence_length][OUTPUT_NODES];
    double hidden_errors[sequence_length][HIDDEN_NODES];

    // Forward pass through time
    for (int t = 0; t < sequence_length; t++) {
        forward_pass_rnn(inputs[t], rnn.hidden_state, outputs[t]);
        for (int i = 0; i < HIDDEN_NODES; i++) {
            hidden_states[t][i] = rnn.hidden_state[i];
        }
    }

    // Calculate output errors
    for (int t = 0; t < sequence_length; t++) {
        for (int i = 0; i < OUTPUT_NODES; i++) {
            output_errors[t][i] = (targets[t] - outputs[t][i]) * sigmoid_derivative(outputs[t][i]);
        }
    }

    // Backpropagate errors through time
    for (int t = sequence_length - 1; t >= 0; t--) {
        for (int i = 0; i < HIDDEN_NODES; i++) {
            hidden_errors[t][i] = 0.0;
            for (int j = 0; j < OUTPUT_NODES; j++) {
                hidden_errors[t][i] += output_errors[t][j] * rnn.weights_hidden_output[i * OUTPUT_NODES + j];
            }
            hidden_errors[t][i] *= sigmoid_derivative(hidden_states[t][i]);
        }
    }

    // Update weights and biases
    for (int t = 0; t < sequence_length; t++) {
        for (int i = 0; i < HIDDEN_NODES; i++) {
            for (int j = 0; j < OUTPUT_NODES; j++) {
                rnn.weights_hidden_output[i * OUTPUT_NODES + j] += LEARNING_RATE * output_errors[t][j] * hidden_states[t][i];
            }
        }
        for (int i = 0; i < OUTPUT_NODES; i++) {
            rnn.output_bias[i] += LEARNING_RATE * output_errors[t][i];
        }
        for (int i = 0; i < INPUT_NODES; i++) {
            for (int j = 0; j < HIDDEN_NODES; j++) {
                rnn.weights_input_hidden[i * HIDDEN_NODES + j] += LEARNING_RATE * hidden_errors[t][j] * inputs[t];
            }
        }
        for (int i = 0; i < HIDDEN_NODES; i++) {
            for (int j = 0; j < HIDDEN_NODES; j++) {
                rnn.weights_hidden_hidden[i * HIDDEN_NODES + j] += LEARNING_RATE * hidden_errors[t][i] * hidden_states[t - 1][j];
            }
        }
        for (int i = 0; i < HIDDEN_NODES; i++) {
            rnn.hidden_bias[i] += LEARNING_RATE * hidden_errors[t][i];
        }
    }
}

void free_rnn() {
    free(rnn.weights_input_hidden);
    free(rnn.weights_hidden_hidden);
    free(rnn.weights_hidden_output);
    free(rnn.hidden_bias);
    free(rnn.output_bias);
    free(rnn.hidden_state);
}

int main() {
    initialize_rnn();

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

        // Train RNN with each sequence
        train_rnn(normalized_displacements, normalized_displacements, num_displacements);

        if (epoch % 100 == 0) {
            printf("Epoch %d, Training...\n", epoch);
        }
    }

    // Validate the model on training data to check learning
    double predicted_displacements[num_displacements];
    for (int t = 0; t < num_displacements; t++) {
        forward_pass_rnn(normalized_displacements[t], rnn.hidden_state, predicted_displacements + t);
    }

    for (int i = 0; i < num_displacements; i++) {
        printf("True displacement: %f, Predicted displacement: %f\n", displacements[i], predicted_displacements[i] * 52.0 - 26);
    }

    // Decrypt the full ciphertext
    char ciphertext[] = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
    int ciphertext_len = strlen(ciphertext);
    char decrypted_text[ciphertext_len + 1];

    // Reset hidden state for decryption
    for (int i = 0; i < HIDDEN_NODES; i++) {
        rnn.hidden_state[i] = 0.0;
    }

    for (int i = 0; i < ciphertext_len; i++) {
        double input = (ciphertext[i] - 'A') / 25.0;
        double output[OUTPUT_NODES];
        forward_pass_rnn(input, rnn.hidden_state, output);
        double shift = output[0] * 52.0 - 26;
        decrypted_text[i] = 'A' + (ciphertext[i] - 'A' + (int)shift + 26) % 26;
    }
    decrypted_text[ciphertext_len] = '\0';
    printf("Decrypted Text: %s\n", decrypted_text);

    free_rnn();
    return 0;
}
