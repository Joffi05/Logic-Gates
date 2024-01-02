#include <stddef.h>
#include "draw.h"
#include "circle.h"
#include <stdio.h>
#include "uuid/uuid.h"
#include "drawn_gate.h"

#define SHINE_GRAY CLITERAL(Color){ 130, 130, 130, 200 }

Rectangle* GenerateBoundingBox(DrawnGate* gate, int x, int y, int width, int height) {
    Rectangle* rect = malloc(sizeof(Rectangle));
    rect->x = x;
    rect->y = y;
    rect->height = height;
    rect->width = width;
    return rect;
}

void UpdateDrawnGate(DrawnGate* gate, Vector2 mouse_delta) {
    gate->bounding_box->x += mouse_delta.x;
    gate->bounding_box->y += mouse_delta.y;

    for (int i = 0; i < gate->gate->num_of_ins + gate->gate->num_of_outs; i++) {
        Circle* c = &gate->pins[i];
        c->center.x += mouse_delta.x;
        c->center.y += mouse_delta.y;
    }
}

//TODO FIXEN
DrawnGate* NewDrawnGate(Gate* gate, Color color, const char* name, int x, int y) {
    DrawnGate* ptr = malloc(sizeof(DrawnGate));
    uuid_generate_random(ptr->uuid);    
    ptr->color = color;
    ptr->gate = gate;
    //Allocate string again
    ptr->name = strdup(name);
    ptr->just_spawned = false;
    ptr->selected = false;

    //If gate is button generate quadratic dimensions
    if(strcmp(ptr->name, "Button") == 0) {
        ptr->bounding_box = GenerateBoundingBox(ptr, x, y, 48, 48);
    }
    else {
        ptr->bounding_box = GenerateBoundingBox(ptr, x, y, MY_GATE_WIDTH, CalculateHeight(gate));
    }

    //alloc input and output circles
    ptr->pins = malloc((gate->num_of_ins + gate->num_of_outs) * sizeof(Circle));

    for (int i = 0; i < gate->num_of_ins; i++) {
        Circle* c = &ptr->pins[i];
        c->center.x = (float)x;
        c->center.y = (float)(y + ptr->bounding_box->height / (gate->num_of_ins + 1) * (i + 1));
        c->radius = 8.0;
    }

    for (int i = 0; i < gate->num_of_outs; i++) {
        Circle* c = &ptr->pins[i + gate->num_of_ins];
        c->center.x = (float)(x + ptr->bounding_box->width);
        c->center.y = (float)(y + ptr->bounding_box->height / (gate->num_of_outs + 1) * (i + 1));
        c->radius = 8.0;
    }

    return ptr;
}


void FreeDrawnGate(DrawnGate* gate) {
    FreeGate(gate->gate);
    free(gate->name);
    free(gate->bounding_box);
    free(gate->pins);
    free(gate);
    gate = NULL;
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
    DrawnGate* gate = NewDrawnGate(NewGateFromGate(input->gate), input->color, input->name, input->bounding_box->x, input->bounding_box->y);
    return gate;
}

DrawnGate* NewDrawnFromDrawnWithCoords(DrawnGate* input, int x, int y) {
    DrawnGate* gate = NewDrawnGate(NewGateFromGate(input->gate), input->color, input->name, x, y);
    return gate;
}