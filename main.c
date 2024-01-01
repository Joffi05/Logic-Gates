#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

#include "logic_gates.h"
#include "draw.h"
#include "vecs.h"
#include "vec.h"
#include "drawn_gate.h"
#include "game.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

#define GUI_LOGIC_LAYOUT_IMPLEMENTATION
#include "logic_layout.h"


//experimental:
typedef struct SelectedPin {
    Circle* pin;
    DrawnGate* gate;
} SelectedPin;
// ----------------


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
    // Check if either input is NULL
    if (gate->inputs[0] == NULL || gate->inputs[1] == NULL) {
        printf("Null input detected in AND gate\n");
        
        // Set output to false if any input is NULL
        gate->outputs[0] = false;
        return;
    }

    // Dereference the inputs and perform the AND operation
    gate->outputs[0] = *(gate->inputs[0]) && *(gate->inputs[1]);
}

void or(Gate* gate) {
    // Check if either input is NULL
    if (gate->inputs[0] == NULL || gate->inputs[1] == NULL) {
        printf("Null input detected in OR gate\n");
        
        // Set output to false if any input is NULL
        gate->outputs[0] = false;
        return;
    }

    // Dereference the inputs and perform the OR operation
    gate->outputs[0] = *(gate->inputs[0]) || *(gate->inputs[1]);
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

void button(Gate* gate) {
    return;
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

    //init to delete vec
    drawn_gate_ptr_vec_t to_delete;
    vec_init(&to_delete);
    
    drawn_gate_ptr_vec_t selectables;
    vec_init(&selectables);

     //button is index 0
    Gate* g_button = NewGate(0, 1);
    SetFunction(g_button, button);
    g_button->out(g_button);
    vec_push(&selectables, NewDrawnGate(g_button, YELLOW, &"Button", 0, 0));
 
    //and is index 1
    Gate* g_and = NewGate(10, 1);
    SetFunction(g_and, and);
    g_and->out(g_and);
    vec_push(&selectables, NewDrawnGate(g_and, GREEN, &"And", 0, 0));

    //or is index 2
    Gate* g_or = NewGate(2, 1);
    SetFunction(g_or, or);
    g_or->out(g_or);
    vec_push(&selectables, NewDrawnGate(g_or, BLUE, &"Or", 0, 0));

    //not is index 3
    Gate* g_not = NewGate(1, 1);
    SetFunction(g_not, not);
    g_not->out(g_not);
    vec_push(&selectables, NewDrawnGate(g_not, RED, &"Not", 0, 0));

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

        // clicked gate has to be reupdated every frame
        DrawnGate* clicked_gate = NULL; 
        state.GateListActive = -1;

        //iterates all gates in the world
        vec_foreach_ptr(&world, val, i) {
            //move the gate behind the mouse if it just spawned
            if (val->just_spawned) {
                if (left_mouse_pressed)
                    val->just_spawned = false;
            }

            //Drag the gate if its selected and the mouse is pressed or if it just spawned
            handleDragging(val);

            //delete if selected and back is pressed
            handleDeletion(&to_delete, val);            
            //select logic
            handleSelection(left_mouse_pressed, unselect_all, &dont_unselect, &val, &clicked_gate);

            //select pin logic
            /* int inputs_clicked = InputsClicked(val);
            if (inputs_clicked != -1) {
                selectedPin->gate = val;
                circle_ptr_vec_t* inputs = &val->inputs_c;
                Circle *c = &inputs->data[inputs_clicked];
                selectedPin->pin = c;
            }  */
        }

        //printf("Length: %d\n", world.length);
        //printf("To delete: %d\n", to_delete.length);
        
        int index;
        //delete gates that are marked for deletion
        //Problem ist, dass wenn ein element gelöscht wird sich der vec verschiebt, und dann nur ein element gelöscht wird
        vec_foreach_rev(&to_delete, val, i) {
            printf("Delete i: %d\n", i);

            for (int i = 0; i < world.length; i++) {
                if (world.data[i].uuid == val->uuid) {
                    //FreeDrawnGate(&world.data[i]);
                    vec_splice(&world, i, 1);
                    vec_compact(&world);
                    break;
                }
            }
        }

        vec_clear(&to_delete);

/*         FreeDrawnGate(gate);
        vec_splice(world, i, 1);
        gate = NULL;
 */

        //unselect logic
        handleUnselect(&clicked_gate, &unselect_all, &dont_unselect, left_mouse_pressed);

        //drawing
        BeginDrawing();

        vec_foreach_ptr(&world, val, i) {
            DrawGateFromDrawn(val);
        }

        GuiLogicLayout(&state);

        //spawn new gate if list is clicked
        if (state.GateListActive >= 0) {
            //here is spawn gate logic
            //DrawnGate* drawn_gate = NewDrawnFromDrawn(selectables.data[state.GateListActive]);
            DrawnGate* drawn_gate = NewDrawnFromDrawnWithCoords(selectables.data[state.GateListActive], GetMouseX(), GetMouseY());
            drawn_gate->just_spawned = true;
            vec_push(&world, *drawn_gate);
        }

        ClearBackground(RAYWHITE);

        EndDrawing();
    }


    vec_foreach_ptr(&world, val, i) {
        FreeDrawnGate(val);
        printf("Freeing gate\n");
    }
    vec_foreach(&selectables, val, i) {
        FreeDrawnGate(val);
        printf("Freeing gate\n");
    }

    vec_deinit(&world);
    vec_deinit(&selectables);    

    CloseWindow();
    exit(0);
    return 0;
}