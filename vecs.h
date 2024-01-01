#pragma once
#include "logic_gates.h"
#include "vec.h"


typedef struct Gate Gate;
typedef struct DrawnGate DrawnGate;
typedef struct Circle Circle;


typedef vec_t(Gate) gate_vec_t;
typedef vec_t(Gate *) gate_ptr_vec_t;
typedef vec_t(DrawnGate) drawn_gate_vec_t;
typedef vec_t(DrawnGate *) drawn_gate_ptr_vec_t;
