#include "game.h"
#include "stdio.h"
#include "draw.h"
#include "logic_gates.h"
#include "drawn_gate.h"
#include "vec.h"

void handleDragging(DrawnGate* gate) {
    if ((gate->selected && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) || (gate->just_spawned)) {
        UpdateDrawnGate(gate, GetMouseDelta());
    }
}

void handleDeletion(drawn_gate_ptr_vec_t* to_delete, DrawnGate* gate) {
    if (gate->selected && IsKeyPressed(KEY_BACKSPACE)) {
        printf("Deleting gate: %s\n", gate->name);
        vec_push(to_delete, gate);
    }
}

void handleSelection(bool left_mouse_pressed, bool unselect_all, DrawnGate** dont_unselect, DrawnGate** val, DrawnGate** clicked_gate) {
    if (left_mouse_pressed) {
        if (CheckCollisionPointRec(GetMousePosition(), *GateBoundingBox(*val))) {
            *clicked_gate = *val;
        }
    }

    if (unselect_all) {
        (*val)->selected = false;
        if (*dont_unselect) {
            (*dont_unselect)->selected = true;
        }
    }
}

void handleUnselect(DrawnGate** clicked_gate, bool* unselect_all, DrawnGate** dont_unselect, bool left_mouse_pressed) {
    *unselect_all = false;
    *dont_unselect = NULL;
    if (*clicked_gate && !(*clicked_gate)->selected && IsKeyUp(KEY_LEFT_SHIFT)) {
        (*clicked_gate)->selected = true;
        *dont_unselect = *clicked_gate;
        *unselect_all = true;
    }
    else if ((*clicked_gate && IsKeyDown(KEY_LEFT_SHIFT))) {
        (*clicked_gate)->selected = true;
    }
    else if (IsKeyUp(KEY_LEFT_SHIFT) && !*clicked_gate && left_mouse_pressed) {
        *unselect_all = true;
    }
}