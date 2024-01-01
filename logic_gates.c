#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "raylib/src/raylib.h"
#include "logic_gates.h"
#include "vecs.h"

// Erstellt ein neues Gate mit i_ins Eingängen und i_outs Ausgängen
Gate* NewGate(int i_ins, int i_outs) {
    Gate* gate = malloc(sizeof(Gate));
    gate->num_of_ins = i_ins;
    gate->num_of_outs = i_outs;

    // Reserviert Speicher für die Zeiger
    gate->inputs = malloc(sizeof(bool*) * gate->num_of_ins);
    // Reserviert Speicher für die bool-Werte und initialisiert sie mit 0
    for (int i = 0; i < gate->num_of_ins; i++) {
        gate->inputs[i] = malloc(sizeof(bool));
        *(gate->inputs[i]) = false;
    }

    gate->outputs = malloc(sizeof(bool) * gate->num_of_outs);
    memset(gate->outputs, 0, sizeof(bool) * gate->num_of_outs); // Initialisiert die Ausgänge mit Nullen

    gate_ptr_vec_t* vec = malloc(sizeof(gate_ptr_vec_t));
    vec_init(vec);
    gate->to_update = vec;

    return gate;
}

Gate* NewGateFromGate(Gate* input) {
    if (input == NULL) {
        return NULL; // Handle null input
    }

    Gate* gate = malloc(sizeof(Gate));
    if (gate == NULL) {
        return NULL; // Handle failed allocation
    }

    gate->num_of_ins = input->num_of_ins;
    gate->num_of_outs = input->num_of_outs;

    // Allocate memory for the inputs array
    gate->inputs = malloc(input->num_of_ins * sizeof(bool*));
    if (gate->inputs == NULL) {
        free(gate); // Free allocated memory in case of failure
        return NULL;
    }
    // Allocate memory for each bool* in the inputs array and initialize them
    for (int i = 0; i < gate->num_of_ins; i++) {
        gate->inputs[i] = malloc(sizeof(bool));
        if (gate->inputs[i] == NULL) {
            // Handle failed allocation: free all previously allocated memory
            for (int j = 0; j < i; j++) {
                free(gate->inputs[j]);
            }
            free(gate->inputs);
            free(gate);
            return NULL;
        }
        // Initialize each bool
        *(gate->inputs[i]) = false; // Or use the value from input if needed
    }

    // Allocate and initialize outputs array
    gate->outputs = malloc(input->num_of_outs * sizeof(bool));
    if (gate->outputs == NULL) {
        // Free all allocated memory in case of failure
        for (int i = 0; i < gate->num_of_ins; i++) {
            free(gate->inputs[i]);
        }
        free(gate->inputs);
        free(gate);
        return NULL;
    }
    memset(gate->outputs, 0, sizeof(bool) * gate->num_of_outs);

    // Allocate and initialize to_update vector
    gate_ptr_vec_t* vec = malloc(sizeof(gate_ptr_vec_t));
    if (vec == NULL) {
        // Free all allocated memory in case of failure
        free(gate->outputs);
        for (int i = 0; i < gate->num_of_ins; i++) {
            free(gate->inputs[i]);
        }
        free(gate->inputs);
        free(gate);
        return NULL;
    }
    vec_init(vec);
    gate->to_update = vec;

    // Copy values from input gate's to_update vector
    int i;
    Gate* val;
    vec_foreach_ptr(input->to_update, val, i) {
        vec_push(gate->to_update, val);
    }

    return gate;
}

void PropaGate(Gate* gate) {
    //cant update self if realiant on self
    // TODO
    // BUG
    gate->out(gate);

    int i;
    Gate* val;
    vec_foreach_ptr(gate->to_update, val, i) {
        PropaGate(val);
    }
}

void SetFunction(Gate* gate, void (*out)(Gate *self)) {
    gate->out = out;
}

void ConnectGates(Gate* gate_o, Gate* gate_i, int output_i, int input_i) {
    if (gate_o->num_of_outs <= output_i || gate_i->num_of_ins <= input_i) {
        printf("Cant connect gates with invalid in or output");
        exit(LOG_ERROR);
    }

    gate_i->inputs[input_i] = &(gate_o->outputs[output_i]);
    vec_push(gate_o->to_update, gate_i);
}

void FreeGate(Gate* gate) {
    if (gate == NULL) {
        return; // If the gate pointer is NULL, there's nothing to free
    }
    // Free each input pointer
    for (int i = 0; i < gate->num_of_ins; i++) {
        free(gate->inputs[i]);
    }
    // Free the array of input pointers
    free(gate->inputs);
    // Free the outputs array
    free(gate->outputs);
    // Deinitialize and free the to_update vector
    vec_deinit(gate->to_update); // If vec_deinit frees the data, this is needed
    free(gate->to_update);
    // Finally, free the gate itself
    free(gate);
}
