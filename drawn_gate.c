#include <stddef.h>
#include "draw.h"
#include "circle.h"
#include <stdio.h>

#define SHINE_GRAY CLITERAL(Color){ 130, 130, 130, 200 }

const int DG_GATE_WIDTH = 100;
const int DG_BORDER_WIDTH = 10;


void UpdateDrawnGate(DrawnGate* gate, Vector2 mouse_delta) {
    gate->x += mouse_delta.x;
    gate->y += mouse_delta.y;

    for (int i = 1; i <= gate->gate->num_of_ins; i++) {
        Circle* c = &gate->pins[i - 1];
        c->center.x += mouse_delta.x;
        c->center.y += mouse_delta.y;
        printf("updated\n");
    }

/*     for (int i = 1 + gate->gate->num_of_ins; i <= gate->gate->num_of_outs + gate->gate->num_of_ins; i++) {
        Circle* c = &gate->pins[i - 1];
        DrawCircle(c->center.x, c->center.y, c->radius, BLACK);
    } */
}

DrawnGate* NewDrawnGate(Gate* gate, Color color, const char* name, int x, int y) {
    //alloc size + size for input and output circles
    DrawnGate* ptr = malloc(sizeof(DrawnGate) + (gate->num_of_ins + gate->num_of_outs) * sizeof(Circle));
    ptr->color = color;
    ptr->gate = gate;
    ptr->name = name;
    ptr->just_spawned = false;
    ptr->selected = false;
    ptr->x = x;
    ptr->y = y;
    ptr->bounding_box = NULL;
    memset(ptr + sizeof(DrawnGate), 0, (gate->num_of_ins + gate->num_of_outs) * sizeof(Circle));

    for (int i = 1; i <= gate->num_of_ins; i++) {
        Circle* c = &ptr->pins[i - 1];
        c->center.x = (float)x;
        c->center.y = (float)(y + CalculateHeight(gate) / (gate->num_of_ins + 1) * i);
        c->radius = 8.0;
    }

    for (int i = 1; i <= gate->num_of_outs; i++) {
        Circle* c = &ptr->pins[i - 1 + gate->num_of_outs];
        c->center.x = (float)x + DG_GATE_WIDTH;
        c->center.y = (float)(y + CalculateHeight(gate) / (gate->num_of_outs + 1) * i);
        c->radius = 8.0;
    }

    return ptr;
}

/* int InputsClicked(DrawnGate* gate) {
    Vector2 mouse_pos = GetMousePosition();

    int i;
    Circle* c;
    vec_foreach_ptr(gate->inputs_c, c, i) {
        //printf("X cord: %d, Y cord: %d\n", c->center.x, c->center.y);
        if (CheckCollisionPointCircle(mouse_pos, c->center, c->radius)) {
            return i;
        }
    }

    return -1;
}

int OutputsClicked(DrawnGate* gate) {
    Vector2 mouse_pos = GetMousePosition();

    int i;
    Circle* c;
    vec_foreach_ptr(gate->outputs_c, c, i) {
        if (CheckCollisionPointCircle(mouse_pos, c->center, c->radius)) {
            return i;
        }
    }

    return -1;
} */

DrawnGate* NewDrawnFromDrawn(DrawnGate* input) {
    DrawnGate* gate = NewDrawnGate(NewGateFromGate(input->gate), input->color, input->name, input->x, input->y);
    return gate;
}

DrawnGate* NewDrawnFromDrawnWithCoords(DrawnGate* input, int x, int y) {
    DrawnGate* gate = NewDrawnGate(NewGateFromGate(input->gate), input->color, input->name, x, y);
    return gate;
}

Rectangle* GateBoundingBox(DrawnGate* gate) {
    if (gate->bounding_box == NULL) {
        gate->bounding_box = MemAlloc(sizeof(Rectangle));
    }
    gate->bounding_box->x = gate->x;
    gate->bounding_box->y = gate->y;
    gate->bounding_box->height = CalculateHeight(gate->gate);
    gate->bounding_box->width = DG_GATE_WIDTH;
    return gate->bounding_box;
}