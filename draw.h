#pragma once

#include "raylib.h"
#include "logic_gates.h"
#include "circle.h"
#include "vecs.h"
#include "drawn_gate.h"

#define MY_GATE_WIDTH 100
#define MY_BORDER_WIDTH 10

typedef struct DrawnGate DrawnGate;

typedef struct Gate Gate;

void DrawGateFromDrawn(DrawnGate* gate);
int CalculateHeight(Gate *gate);
