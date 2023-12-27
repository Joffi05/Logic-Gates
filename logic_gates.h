#pragma once

#include <stdbool.h>
#include "vecs.h"
 
typedef struct Gate {
    gate_vec_t* to_update;
    // inputs points to the outputs of other gates, so it's a 2d array
    bool** inputs;
    // outputs is stored in the gate to which other gates refer to as inputs so its a 1d array
    bool* outputs;
    int num_of_ins;
    int num_of_outs;
    void (*out)(Gate *self);
} Gate;

Gate* NewGate(int i_ins, int i_outs);
Gate* NewGateFromGate(Gate* input);
void PropaGate(Gate* gate);
void SetFunction(Gate* gate, void (*out)(Gate *self));
void ConnectGates(Gate* gate_o, Gate* gate_i, int output_i, int input_i);
void FreeGate(Gate* gate);