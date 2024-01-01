/*******************************************************************************************
*
*   LogicLayout v0.0.1 - Layout For a Logic Gate sim I am building.
*
*   MODULE USAGE:
*       #define GUI_LOGIC_LAYOUT_IMPLEMENTATION
*       #include "gui_logic_layout.h"
*
*       INIT: GuiLogicLayoutState state = InitGuiLogicLayout();
*       DRAW: GuiLogicLayout(&state);
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 Jonathan Goffing. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_LOGIC_LAYOUT_H
#define GUI_LOGIC_LAYOUT_H

typedef struct {
    Vector2 AnchorGateList;
    Vector2 AnchorNewGate;
    
    int GateListScrollIndex;
    int GateListActive;
    bool AddGatePressed;
    bool EditGatePressed;
    bool RemoveGatePressed;
    bool NewGateActive;
    bool NameEditMode;
    char NameText[128];
    char GateListString[1024];
    Color ColorValue;
    bool CalculateTableChecked;
    bool DonePressed;
    bool CancelPressed;

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiLogicLayoutState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiLogicLayoutState InitGuiLogicLayout(void);
void GuiLogicLayout(GuiLogicLayoutState *state);

#ifdef __cplusplus
}
#endif

#endif // GUI_LOGIC_LAYOUT_H

/***********************************************************************************
*
*   GUI_LOGIC_LAYOUT IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_LOGIC_LAYOUT_IMPLEMENTATION)

#include "raygui/src/raygui.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
void AddLogicGateToList(GuiLogicLayoutState *state, const char* gate) {
    strcat(state->GateListString, gate);
}

GuiLogicLayoutState InitGuiLogicLayout(void)
{
    GuiLogicLayoutState state = { 0 };

    state.AnchorGateList = (Vector2){ 16, 16 };
    state.AnchorNewGate = (Vector2){ 1240, 96 };
    
    state.GateListScrollIndex = 0;
    state.GateListActive = 0;
    state.AddGatePressed = false;
    state.EditGatePressed = false;
    state.RemoveGatePressed = false;
    state.NewGateActive = true;
    state.NameEditMode = false;
    strcpy(state.NameText, "Name");
    strcpy(state.GateListString, "Button;And;Or;Not");
    state.ColorValue = (Color){ 0, 0, 0, 0 };
    state.CalculateTableChecked = false;
    state.DonePressed = false;
    state.CancelPressed = false;

    // Custom variables initialization

    return state;
}

void GuiLogicLayout(GuiLogicLayoutState *state)
{
    if (state->NewGateActive)
    {
        state->NewGateActive = !GuiWindowBox((Rectangle){ state->AnchorNewGate.x + 0, state->AnchorNewGate.y + 0, 288, 200 }, "New Gate");
        if (GuiTextBox((Rectangle){ state->AnchorNewGate.x + 16, state->AnchorNewGate.y + 56, 120, 24 }, state->NameText, 128, state->NameEditMode)) state->NameEditMode = !state->NameEditMode;
        state->ColorValue = GuiColorPicker((Rectangle){ state->AnchorNewGate.x + 152, state->AnchorNewGate.y + 48, 96, 96 }, NULL, state->ColorValue);
        state->CalculateTableChecked = GuiCheckBox((Rectangle){ state->AnchorNewGate.x + 16, state->AnchorNewGate.y + 112, 24, 24 }, "Calculate Table", state->CalculateTableChecked);
        state->DonePressed = GuiButton((Rectangle){ state->AnchorNewGate.x + 16, state->AnchorNewGate.y + 160, 80, 24 }, "Done"); 
        state->CancelPressed = GuiButton((Rectangle){ state->AnchorNewGate.x + 176, state->AnchorNewGate.y + 160, 80, 24 }, "Cancel"); 
    }
    
    state->GateListActive = GuiListView((Rectangle){ state->AnchorGateList.x + 0, state->AnchorGateList.y + 0, 216, 800 }, state->GateListString, &state->GateListScrollIndex, state->GateListActive);
    state->AddGatePressed = GuiButton((Rectangle){ state->AnchorGateList.x + 0, state->AnchorGateList.y + 800, 72, 24 }, "ADD"); 
    state->EditGatePressed = GuiButton((Rectangle){ state->AnchorGateList.x + 72, state->AnchorGateList.y + 800, 72, 24 }, "EDIT"); 
    state->RemoveGatePressed = GuiButton((Rectangle){ state->AnchorGateList.x + 144, state->AnchorGateList.y + 800, 72, 24 }, "DELETE"); 
}

#endif // GUI_LOGIC_LAYOUT_IMPLEMENTATION
