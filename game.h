#pragma once

#include "raylib.h"
#include "logic_gates.h"
#include "circle.h"
#include "vecs.h"
#include "vec.h"

void handleJustSpawned(drawn_gate_vec_t* world, DrawnGate* gate, int i, bool left_mouse_pressed);
void handleDragging(DrawnGate* gate);
void handleDeletion(drawn_gate_ptr_vec_t* to_delete, DrawnGate* gate);
void handleSelection(bool left_mouse_pressed, bool unselect_all, DrawnGate** dont_unselect, DrawnGate** val, DrawnGate** clicked_gate);
void handleUnselect(DrawnGate** clicked_gate, bool* unselect_all, DrawnGate** dont_unselect, bool left_mouse_pressed);
