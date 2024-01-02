#include <stddef.h>
#include "draw.h"
#include "circle.h"

#define SHINE_GRAY CLITERAL(Color){ 130, 130, 130, 200 }


// SECTION FOR SPECIAL GATES

void DrawButton(DrawnGate* gate) {
    Rectangle inner = (Rectangle) {.x = gate->bounding_box->x + MY_BORDER_WIDTH, .y = gate->bounding_box->y + MY_BORDER_WIDTH, .width = gate->bounding_box->width - 2 * MY_BORDER_WIDTH, .height = gate->bounding_box->height - 2 * MY_BORDER_WIDTH};
    
    if (gate->gate->outputs[0]) {
        DrawRectangleRounded(inner, 0.1, 1, RED);
    }
    else {
        DrawRectangleRounded(inner, 0.1, 1, GREEN);
    }
}

void DrawLamp(DrawnGate* gate) {
    Rectangle inner = (Rectangle) {.x = gate->bounding_box->x + MY_BORDER_WIDTH, .y = gate->bounding_box->y + MY_BORDER_WIDTH, .width = gate->bounding_box->width - 2 * MY_BORDER_WIDTH, .height = gate->bounding_box->height - 2 * MY_BORDER_WIDTH};
    
    if (gate->gate->inputs[0]) {
        DrawRectangleRounded(inner, 0.1, 1, RED);
    }
    else {
        DrawRectangleRounded(inner, 0.1, 1, GREEN);
    }
}


// END SECTION FOR SPECIAL GATES


int CalculateHeight(Gate* gate) {
    int in_or_out_larger = gate->num_of_ins > gate->num_of_outs ? gate->num_of_ins : gate->num_of_outs;
    int gateHeight = in_or_out_larger * 30;
    if (in_or_out_larger == 1) {
        gateHeight = 48;
    }
    return gateHeight;
}

void DrawBox(DrawnGate* gate, Color color) {
    Rectangle outer = (Rectangle) {.x = gate->bounding_box->x, .y = gate->bounding_box->y, .width = gate->bounding_box->width, .height = gate->bounding_box->height};
    DrawRectangleRounded(outer, 0.2, 1, color);
    DrawRectangle(gate->bounding_box->x, gate->bounding_box->y, gate->bounding_box->width, gate->bounding_box->height, color);
}

void DrawGateName(DrawnGate *gate)
{
    int font_size = 30;
    int text_height = MeasureText(gate->name, font_size);
    int y = gate->bounding_box->y + (gate->bounding_box->height - (text_height / 2)) / 2;
    DrawText(gate->name, gate->bounding_box->x + MY_BORDER_WIDTH + (gate->bounding_box->width - MY_BORDER_WIDTH) / 2 - font_size, y, font_size, BLACK);
}

void DrawInOutputs(DrawnGate *gate) {
    //Print number of inputs and outputs
    for (int i = 0; i < gate->gate->num_of_outs + gate->gate->num_of_ins; i++) {
        Circle *c = &gate->pins[i];
        DrawCircle(c->center.x, c->center.y, c->radius, BLACK);
    }
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
        DrawRectangleLines(gate->bounding_box->x - 10, gate->bounding_box->y - 10, gate->bounding_box->width + 20, gate->bounding_box->height + 20, BLACK);
    }

    //draw body
    DrawBox(gate, color);

    //If gate is Button, dont draw name but red or green button
    if (strcmp(gate->name, "Button") == 0) {
        DrawButton(gate);
    }
    else if (strcmp(gate->name, "Lamp") == 0) {
        DrawLamp(gate);
    }
    
    else {
        DrawGateName(gate);
    }

    //draw in and outputs
    DrawInOutputs(gate);
}