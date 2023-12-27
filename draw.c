#include <stddef.h>
#include "draw.h"
#include "circle.h"

#define SHINE_GRAY CLITERAL(Color){ 130, 130, 130, 200 }

const int GATE_WIDTH = 100;
const int BORDER_WIDTH = 10;


int CalculateHeight(Gate* gate) {
    int in_or_out_larger = gate->num_of_ins > gate->num_of_outs ? gate->num_of_ins : gate->num_of_outs;
    int gateHeight = in_or_out_larger * 24;
    if (in_or_out_larger == 1) {
        gateHeight = 48;
    }
    return gateHeight;
}

//Refactor
void DrawGate(Gate* gate, int x, int y, Color color, const char* name) {
    int gateHeight = CalculateHeight(gate);

    Rectangle outer = (Rectangle) {.x = x, .y = y, .width = GATE_WIDTH, .height = gateHeight};
    DrawRectangleRounded(outer, 0.2, 1, color);

    DrawRectangle(x, y, GATE_WIDTH, gateHeight, color);

    //muss fertig gemacht werden, schrift soll zentriert
    int font_size = 30;
    DrawText(name, x + BORDER_WIDTH + (GATE_WIDTH - BORDER_WIDTH) / 2 - font_size, y + BORDER_WIDTH, font_size, BLACK);
}

void DrawGateFromDrawn(DrawnGate* gate) {
    Color color;
    if (gate->just_spawned) {
        color = (Color) {.r=gate->color.r, .g=gate->color.g, .b=gate->color.b, .a=60};
    }
    else {
        color = gate->color;
    }

    //draw gate outline if selected
    if (gate->selected) {
        DrawRectangleLines(gate->x - 10, gate->y - 10, GATE_WIDTH + 20, CalculateHeight(gate->gate) + 20, BLACK);
    }

    for (int i = 1; i <= gate->gate->num_of_ins; i++) {
        Circle* c = &gate->pins[i - 1];
        DrawCircle(c->center.x, c->center.y, c->radius, BLACK);
    }

/*     for (int i = 1 + gate->gate->num_of_ins; i <= gate->gate->num_of_outs + gate->gate->num_of_ins; i++) {
        Circle* c = &gate->pins[i - 1];
        DrawCircle(c->center.x, c->center.y, c->radius, BLACK);
    } */

    DrawGate(gate->gate, gate->x, gate->y, color, gate->name);
}