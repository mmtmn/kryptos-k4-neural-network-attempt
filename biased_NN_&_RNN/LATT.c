#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define INPUT_NODES 54 // 26 for one-hot encoding of character + 26 for position encoding + 2 for shifts
#define HIDDEN_NODES 100
#define OUTPUT_NODES 26
#define LEARNING_RATE 0.1
#define META_LEARNING_RATE 0.01
#define EPOCHS 100
#define META_EPOCHS 10

typedef struct {
    double weights[INPUT_NODES][HIDDEN_NODES];
    double recurrent_weights[HIDDEN_NODES][HIDDEN_NODES];
    double biases[HIDDEN_NODES];
} HiddenLayer;

typedef struct {
    double weights[HIDDEN_NODES][OUTPUT_NODES];
    double biases[OUTPUT_NODES];
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
    for (int i = 0; i < INPUT_NODES; i++) {
        for (int j = 0; j < HIDDEN_NODES; j++) {
            hidden_layer.weights[i][j] = ((double)rand() / RAND_MAX) - 0.5;
        }
    }
    for (int i = 0; i < HIDDEN_NODES; i++) {
        for (int j = 0; j < HIDDEN_NODES; j++) {
            hidden_layer.recurrent_weights[i][j] = ((double)rand() / RAND_MAX) - 0.5;
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

void forward_pass(double input[], double prev_hidden_output[], double hidden_output[], double output[]) {
    for (int i = 0; i < HIDDEN_NODES; i++) {
        hidden_output[i] = hidden_layer.biases[i];
        for (int j = 0; j < INPUT_NODES; j++) {
            hidden_output[i] += input[j] * hidden_layer.weights[j][i];
        }
        for (int j = 0; j < HIDDEN_NODES; j++) {
            hidden_output[i] += prev_hidden_output[j] * hidden_layer.recurrent_weights[j][i];
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

void train(double input[], double target[], double prev_hidden_output[], double hidden_output[], double *total_error, HiddenLayer *hidden_layer_ptr, OutputLayer *output_layer_ptr) {
    double output[OUTPUT_NODES];
    forward_pass(input, prev_hidden_output, hidden_output, output);

    double output_error[OUTPUT_NODES];
    double hidden_error[HIDDEN_NODES];

    for (int i = 0; i < OUTPUT_NODES; i++) {
        output_error[i] = (target[i] - output[i]) * sigmoid_derivative(output[i]);
        *total_error += fabs(target[i] - output[i]);
    }

    for (int i = 0; i < HIDDEN_NODES; i++) {
        hidden_error[i] = 0.0;
        for (int j = 0; j < OUTPUT_NODES; j++) {
            hidden_error[i] += output_error[j] * output_layer_ptr->weights[i][j];
        }
        hidden_error[i] *= sigmoid_derivative(hidden_output[i]);
    }

    for (int i = 0; i < HIDDEN_NODES; i++) {
        for (int j = 0; j < OUTPUT_NODES; j++) {
            output_layer_ptr->weights[i][j] += LEARNING_RATE * output_error[j] * hidden_output[i];
        }
    }

    for (int i = 0; i < OUTPUT_NODES; i++) {
        output_layer_ptr->biases[i] += LEARNING_RATE * output_error[i];
    }

    for (int i = 0; i < INPUT_NODES; i++) {
        for (int j = 0; j < HIDDEN_NODES; j++) {
            hidden_layer_ptr->weights[i][j] += LEARNING_RATE * hidden_error[j] * input[i];
        }
    }

    for (int i = 0; i < HIDDEN_NODES; i++) {
        for (int j = 0; j < HIDDEN_NODES; j++) {
            hidden_layer_ptr->recurrent_weights[i][j] += LEARNING_RATE * hidden_error[j] * prev_hidden_output[i];
        }
    }

    for (int i = 0; i < HIDDEN_NODES; i++) {
        hidden_layer_ptr->biases[i] += LEARNING_RATE * hidden_error[i];
    }
}

void encode_char_with_position_and_shifts(char c, int position, int pos_shift, int neg_shift, double *output) {
    for (int i = 0; i < 54; i++) {
        output[i] = 0.0;
    }
    output[c - 'A'] = 1.0;
    output[26 + position % 26] = 1.0;
    output[52] = pos_shift / 26.0; // Normalize the shift values
    output[53] = neg_shift / 26.0; // Normalize the shift values
}

void encode_char(char c, double *output) {
    for (int i = 0; i < 26; i++) {
        output[i] = 0.0;
    }
    output[c - 'A'] = 1.0;
}

char decode_char(double *input) {
    int max_index = 0;
    for (int i = 1; i < 26; i++) {
        if (input[i] > input[max_index]) {
            max_index = i;
        }
    }
    return 'A' + max_index;
}

void copy_hidden_layer(HiddenLayer *dest, HiddenLayer *src) {
    memcpy(dest->weights, src->weights, sizeof(src->weights));
    memcpy(dest->recurrent_weights, src->recurrent_weights, sizeof(src->recurrent_weights));
    memcpy(dest->biases, src->biases, sizeof(src->biases));
}

void copy_output_layer(OutputLayer *dest, OutputLayer *src) {
    memcpy(dest->weights, src->weights, sizeof(src->weights));
    memcpy(dest->biases, src->biases, sizeof(src->biases));
}

void meta_train() {
    HiddenLayer meta_hidden_layer;
    OutputLayer meta_output_layer;

    copy_hidden_layer(&meta_hidden_layer, &hidden_layer);
    copy_output_layer(&meta_output_layer, &output_layer);

    // Training data based on the clues
    char *ciphertext_clues[] = {"NYPVTT", "MZFPK", "QQPRNGKSS", "FLRV"};
    char *plaintext_clues[] = {"BERLIN", "CLOCK", "NORTHEAST", "EAST"};
    int clue_positions[] = {63, 69, 25, 21}; // Starting positions of the clues in the ciphertext
    int num_clues = 4;

    // Additional training data
    int additional_positions[] = {64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 26, 27, 28, 29, 30, 31, 32, 33, 34, 22, 23, 24, 25};
    char additional_ciphertext[] = "NYPVTTMZFPKQQPRNGKSSFLRV";
    char additional_plaintext[] = "BERLINCLOCKNORTHEASTEAST";
    int positive_shifts[] = {14, 4, 11, 8, 18, 7, 13, 12, 10, 11, 0, 23, 24, 2, 3, 13, 14, 18, 0, 1, 25, 15, 1, 24};
    int negative_shifts[] = {-12, -22, -15, -18, -8, -13, -11, -14, -16, -11, -26, -3, -2, -24, -23, -13, -12, -8, -26, -25, -1, -11, -18, -5};
    int num_additional_clues = 24;

    for (int meta_epoch = 0; meta_epoch < META_EPOCHS; meta_epoch++) {
        for (int epoch = 0; epoch < EPOCHS; epoch++) {
            double total_error = 0.0;
            for (int clue = 0; clue < num_clues; clue++) {
                int len = strlen(ciphertext_clues[clue]);
                double prev_hidden_output[HIDDEN_NODES] = {0};
                for (int i = 0; i < len; i++) {
                    double input[54];
                    double target[26];
                    double hidden_output[HIDDEN_NODES];
                    encode_char_with_position_and_shifts(ciphertext_clues[clue][i], clue_positions[clue] + i, 0, 0, input);
                    encode_char(plaintext_clues[clue][i], target);
                    train(input, target, prev_hidden_output, hidden_output, &total_error, &meta_hidden_layer, &meta_output_layer);
                    memcpy(prev_hidden_output, hidden_output, sizeof(hidden_output));
                }
            }
            for (int i = 0; i < num_additional_clues; i++) {
                double input[54];
                double target[26];
                double hidden_output[HIDDEN_NODES];
                double prev_hidden_output[HIDDEN_NODES] = {0};
                encode_char_with_position_and_shifts(additional_ciphertext[i], additional_positions[i], positive_shifts[i], negative_shifts[i], input);
                encode_char(additional_plaintext[i], target);
                train(input, target, prev_hidden_output, hidden_output, &total_error, &meta_hidden_layer, &meta_output_layer);
                memcpy(prev_hidden_output, hidden_output, sizeof(hidden_output));
            }
            if (epoch % 10 == 0) {
                printf("Meta Epoch %d, Epoch %d, Total Error: %f\n", meta_epoch, epoch, total_error);
            }
        }

        // Update main network weights using meta-learning rate
        for (int i = 0; i < INPUT_NODES; i++) {
            for (int j = 0; j < HIDDEN_NODES; j++) {
                hidden_layer.weights[i][j] += META_LEARNING_RATE * (meta_hidden_layer.weights[i][j] - hidden_layer.weights[i][j]);
            }
        }
        for (int i = 0; i < HIDDEN_NODES; i++) {
            for (int j = 0; j < HIDDEN_NODES; j++) {
                hidden_layer.recurrent_weights[i][j] += META_LEARNING_RATE * (meta_hidden_layer.recurrent_weights[i][j] - hidden_layer.recurrent_weights[i][j]);
            }
        }
        for (int i = 0; i < HIDDEN_NODES; i++) {
            hidden_layer.biases[i] += META_LEARNING_RATE * (meta_hidden_layer.biases[i] - hidden_layer.biases[i]);
        }
        for (int i = 0; i < HIDDEN_NODES; i++) {
            for (int j = 0; j < OUTPUT_NODES; j++) {
                output_layer.weights[i][j] += META_LEARNING_RATE * (meta_output_layer.weights[i][j] - output_layer.weights[i][j]);
            }
        }
        for (int i = 0; i < OUTPUT_NODES; i++) {
            output_layer.biases[i] += META_LEARNING_RATE * (meta_output_layer.biases[i] - output_layer.biases[i]);
        }
    }
}

int main() {
    initialize_network();
    meta_train();

    // Decrypt the full ciphertext
    char ciphertext[] = "OBKRUOXOGHULBSOLIFBBWFLRVQQPRNGKSSOTWTQSJQSSEKZZWATJKLUDIAWINFBNYPVTTMZFPKWGDKZXTJCDIGKUHUAUEKCAR";
    int ciphertext_len = strlen(ciphertext);
    char decrypted_text[ciphertext_len + 1];
    double prev_hidden_output[HIDDEN_NODES] = {0};
    for (int i = 0; i < ciphertext_len; i++) {
        double input[54];
        double hidden_output[HIDDEN_NODES];
        double output[26];
        encode_char_with_position_and_shifts(ciphertext[i], i, 0, 0, input);
        forward_pass(input, prev_hidden_output, hidden_output, output);
        decrypted_text[i] = decode_char(output);
        memcpy(prev_hidden_output, hidden_output, sizeof(hidden_output));
    }
    decrypted_text[ciphertext_len] = '\0';
    printf("Decrypted Text: %s\n", decrypted_text);

    return 0;
}
