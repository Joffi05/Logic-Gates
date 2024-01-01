#pragma once

#include "raylib.h"
#include "logic_gates.h"
#include "circle.h"
#include "vecs.h"
#include <uuid/uuid.h>

typedef struct DrawnGate DrawnGate;

typedef struct Gate Gate;

//TODO drawn gate brauch GuiState um clicks auf inputs / outputs und select zu handlen
typedef struct DrawnGate {
    uuid_t uuid;
    Gate* gate;
    Color color;
    const char* name;
    int x;
    int y;
    bool selected;
    bool just_spawned;
    Rectangle* bounding_box;
    Circle* pins;
} DrawnGate;

DrawnGate* NewSpawnedDrawnGate(Gate* gate, Color color, const char* name, int x, int y);
DrawnGate* NewDrawnGate(Gate* gate, Color color, const char* name, int x, int y);
DrawnGate* NewDrawnFromDrawn(DrawnGate* input);
DrawnGate* NewDrawnFromDrawnWithCoords(DrawnGate* input, int x, int y);
Rectangle* GateBoundingBox(DrawnGate* gate);
void FreeDrawnGate(DrawnGate* gate);


void UpdateDrawnGate(DrawnGate* gate, Vector2 mouse_delta);


int InputsClicked(DrawnGate* gate);
int OutputsClicked(DrawnGate* gate);