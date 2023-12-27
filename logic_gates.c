#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "raylib/src/raylib.h"
#include "logic_gates.h"
#include "vecs.h"

Gate* NewGate(int i_ins, int i_outs) {
    Gate* gate = malloc(sizeof(Gate));
    gate->num_of_ins = i_ins;
    gate->num_of_outs = i_outs;
    
    gate->inputs = malloc(sizeof(bool*) * gate->num_of_ins);
    gate->outputs = malloc(sizeof(bool) * gate->num_of_outs);

    gate_vec_t* vec = malloc(sizeof(gate_vec_t*));
    vec_init(&vec);
    gate->to_update = vec;

    return gate;
}

Gate* NewGateFromGate(Gate* input) {
    Gate* gate = malloc(sizeof(Gate));
    memcpy(gate, input, sizeof(Gate));
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
    vec_push(gate_o->to_update, *gate_i);
}

void FreeGate(Gate* gate) {
    vec_deinit(gate->to_update);
    free(gate->inputs);
    free(gate->outputs);
    free(gate);
}