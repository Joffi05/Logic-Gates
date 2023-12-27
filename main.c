#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

#include "logic_gates.h"
#include "draw.h"
#include "vecs.h"
#include "vec.h"
#include "drawn_gate.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

#define GUI_LOGIC_LAYOUT_IMPLEMENTATION
#include "logic_layout.h"

typedef struct SelectedPin {
    Circle* pin;
    DrawnGate* gate;
} SelectedPin;


void ToggleFullScreenWindow(int windowHeight, int windowWidth) {
    if (!IsWindowFullscreen()) {
        int monitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));

        printf("Monitor width: %d\n", GetMonitorWidth(monitor));

        ToggleFullscreen();
    }
    else {
        ToggleFullscreen();
        SetWindowSize(windowWidth, windowHeight);
    }
}

void InitResizable(int width, int height, const char *title) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, title);
    SetWindowMinSize(width, height);
}

void and(Gate* gate) {
    gate->outputs[0] = gate->inputs[0] && gate->inputs[1];
}

void or(Gate* gate) {
    gate->outputs[0] = gate->inputs[0] || gate->inputs[1];
}

void truthy(Gate* gate) {
    gate->outputs[0] = true;
}

void falsy(Gate* gate) {
    gate->outputs[0] = false;
}

void not(Gate* gate) {
    gate->outputs[0] = !gate->inputs[0];
}

int main() {
    //init window
    int startWidth = 1000;
    int startHeight = 800;

    InitResizable(startWidth, startHeight, "Logic Gate Sim");
    MaximizeWindow();
    SetTargetFPS(60);

    //init gui
    GuiLogicLayoutState state = InitGuiLogicLayout();
    state.NewGateActive = false;

    //init world
    drawn_gate_vec_t world;
    vec_init(&world);
    
    drawn_gate_ptr_vec_t selectables;
    vec_init(&selectables);

    //and is index 0
    Gate* g_and = NewGate(2, 1);
    SetFunction(g_and, *and);
    g_and->out(g_and);
    vec_push(&selectables, NewDrawnGate(g_and, GREEN, "And", 0, 0));


    //or is index 1
    Gate* g_or = NewGate(2, 1);
    SetFunction(g_or, *or);
    g_or->out(g_or);
    vec_push(&selectables, NewDrawnGate(g_or, BLUE, "Or", 0, 0));


    //not is index 2
    Gate* g_not = NewGate(1, 1);
    SetFunction(g_not, *not);
    g_not->out(g_not);
    vec_push(&selectables, NewDrawnGate(g_not, RED, "Not", 0, 0));


    //printf("gates: %d\n", g_and->num_of_outs);


    //loop vars that dont need to be reset every frame
    int i;
    DrawnGate* val;
    DrawnGate* dont_unselect = NULL;
    bool unselect_all = NULL;
    bool left_mouse_pressed = false;
    SelectedPin* selectedPin = NULL;

    while (!WindowShouldClose()) {
        left_mouse_pressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

 		if (IsKeyPressed(KEY_F11) && IsWindowMaximized())
 		{
            ToggleFullScreenWindow(startHeight, startWidth);
 		}

        state.GateListActive = -1;


        // clicked gate has to be reupdated every frame
        DrawnGate* clicked_gate = NULL; 


        //iterates all gates in the world
        vec_foreach_ptr(&world, val, i) {
            //printf("x: %d, y: %d\n", val->x, val->y);

            //move the gate behind the mouse if it just spawned
            if (val->just_spawned) {
                val->x = GetMouseX();
                val->y = GetMouseY();

                if (left_mouse_pressed)
                    val->just_spawned = false;
                
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    vec_splice(&world, i, 1);
                }

                continue;
            }

            //Drag the gate if its selected and the mouse is pressed
            if (val->selected && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                UpdateDrawnGate(val, GetMouseDelta());
            }

            //delete if selected and back is pressed
            if (val->selected && IsKeyPressed(KEY_BACKSPACE)) {
                // TODO alle selecteted dleleten noch nicht implementiert
                vec_splice(&world, i, 1);
            }
            
            //select logic
            if (left_mouse_pressed) {
                if (CheckCollisionPointRec(GetMousePosition(), *GateBoundingBox(val))) {
                    printf("clicked gate\n");
                    clicked_gate = val;
                }
            }

            if (unselect_all) {
                printf("unselect all\n");
                val->selected = false;
                if (dont_unselect) {
                    dont_unselect->selected = true;
                }
            }

            //select pin logic
            /* int inputs_clicked = InputsClicked(val);
            if (inputs_clicked != -1) {
                selectedPin->gate = val;
                circle_ptr_vec_t* inputs = &val->inputs_c;
                Circle *c = &inputs->data[inputs_clicked];
                selectedPin->pin = c;
            }  */
        }
        
        //select logic
        unselect_all = false;
        dont_unselect = NULL;
        if (clicked_gate && !clicked_gate->selected && IsKeyUp(KEY_LEFT_SHIFT)) {
            clicked_gate->selected = true;
            dont_unselect = clicked_gate;
            unselect_all = true;
            printf("first select\n");
        }
        else if ((clicked_gate && IsKeyDown(KEY_LEFT_SHIFT))) {
            clicked_gate->selected = true;
            printf("second select\n");
        }
        else if (IsKeyUp(KEY_LEFT_SHIFT) && !clicked_gate && left_mouse_pressed) {
            unselect_all = true;
            printf("third select\n");
        } 

        //drawing
        BeginDrawing();

        vec_foreach_ptr(&world, val, i) {
            DrawGateFromDrawn(val);
        }

        GuiLogicLayout(&state);

        //spawn new gate if list is clicked
        if (state.GateListActive >= 0) {
            //DrawnGate* drawn_gate = NewDrawnFromDrawn(selectables.data[state.GateListActive]);
            DrawnGate* drawn_gate = NewDrawnFromDrawnWithCoords(selectables.data[state.GateListActive], GetMouseX(), GetMouseY());
            drawn_gate->just_spawned = true;
            vec_push(&world, *drawn_gate);
        }

        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    vec_deinit(&world);
    vec_deinit(&selectables);    

    CloseWindow();
    return 0;
}