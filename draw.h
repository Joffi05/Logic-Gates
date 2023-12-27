#pragma once

#include "raylib.h"
#include "logic_gates.h"
#include "circle.h"
#include "vecs.h"
#include "drawn_gate.h"

typedef struct DrawnGate DrawnGate;

typedef struct Gate Gate;

void DrawGateFromDrawn(DrawnGate* gate);
int CalculateHeight(Gate* gate);
