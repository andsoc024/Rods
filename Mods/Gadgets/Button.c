// ============================================================================
// RODS
// Button
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions and definitions of structures and constants for the button 
    gadget.

    The button emits the EVENT_BUTTON_PRESSED and EVENT_BUTTON_RELEASED events, 
    when pressed or released.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "../Graph/Graph.h"
#include "../GUI/GUI.h"
#include "Gadgets.h"


// ============================================================================ PRIVATE MACROS

// **************************************************************************** BDATA

// Button Data. Pointer to the button's data object
#define BDATA \
    ((ButtonData*) button->data)


// **************************************************************************** CONTENT

// Pointed to the contained text or icon label
#define CONTENT \
    (button->subGadgets[0])


// ============================================================================ PRIVATE CONSTANTS

#define BTN_DEF_ROUNDNESS                   RRECT_DEF_ROUNDNESS
#define BTN_DEF_THICKNESS                   3.0f
#define BTN_DEF_MARGIN_RATIO                ((1.0f - MATH_PHI_INVERSE) * 0.5f)

#define BTN_TURBO_COUNT                     TURBO_COUNT

#define COL_BTN_BG                          COL_UI_BG_PRIMARY
#define COL_BTN_FG                          COL_UI_FG_PRIMARY
#define COL_BTN_EMPH                        COL_UI_FG_EMPH


// ============================================================================ PRIVATE STRUCTURES

typedef struct ButtonData{
    float roundness;
    float thickness;
    float marginRatio;
    int turboCount;
    Color colBG;
    Color colFG;
    Color colEmph;
}ButtonData;


// ============================================================================ PRIVATE FUNC DECL

static void     Button_Resize(Gadget* button);
static void     Button_ReactToEvent(Gadget* button, Event event, EventQueue* queue);
static void     Button_Update(Gadget* button, EventQueue* queue);
static void     Button_Draw(const Gadget* button, Vector2 shift);
#ifdef DEBUG_MODE
    static void Button_PrintData(const Gadget* button);
#endif






// ============================================================================ FUNC DEF

// **************************************************************************** Button_MakeAsText

// Make a text button. The gadget needs to be resized before its first use
Gadget* Button_MakeAsText(E_GadgetID id, const char* txt){
    Gadget* button = Gadget_Make(id, GT_BUTTON, IS_SELECTABLE, 1);

    Gadget* content = Label_Make(id, txt, COL_BTN_FG, RP_CENTER);
    content->isSelectable = true;
    button->subGadgets[0] = content;
    

    button->Resize        = Button_Resize;
    button->ReactToEvent  = Button_ReactToEvent;
    button->Update        = Button_Update;
    button->Draw          = Button_Draw;
    #ifdef DEBUG_MODE
        button->PrintData = Button_PrintData;
    #endif

    ButtonData* data = Memory_Allocate(NULL, sizeof(ButtonData), ZEROVAL_ALL);

    data->roundness   = BTN_DEF_ROUNDNESS;
    data->thickness   = BTN_DEF_THICKNESS;
    data->marginRatio = BTN_DEF_MARGIN_RATIO;

    data->colBG   = COL_BTN_BG;
    data->colFG   = COL_BTN_FG;
    data->colEmph = COL_BTN_EMPH;

    button->data = data;

    return button;
}


// **************************************************************************** Button_MakeAsIcon

// Make a button icon. The gadget needs to be resized before its first use
Gadget* Button_MakeAsIcon(E_GadgetID id, E_IconID icon){
    Gadget* button = Button_MakeAsText(id, " ");

    CONTENT = Gadget_Free(CONTENT);
    CONTENT = IconLabel_Make(id, icon, COL_BTN_FG, RP_CENTER);
    CONTENT->isSelectable = true;

    return button;
}


// **************************************************************************** Button_SetText

// Set the text of the button
void Button_SetText(Gadget* button, const char* txt){
    if (Button_HasIcon(button)){
        CONTENT = Gadget_Free(CONTENT);
        CONTENT = Label_Make(button->id, txt, BDATA->colFG, RP_CENTER);
        CONTENT->isSelectable = true;
        Gadget_Resize(button);
    }else{
        Label_SetText(CONTENT, txt);
    }
}


// **************************************************************************** Button_SetIcon

// Set the icon of the button
void Button_SetIcon(Gadget* button, E_IconID icon){
    if (Button_HasText(button)){
        CONTENT = Gadget_Free(CONTENT);
        CONTENT = IconLabel_Make(button->id, icon, BDATA->colFG, RP_CENTER);
        CONTENT->isSelectable = true;
        Gadget_Resize(button);
    }else{
        IconLabel_SetIcon(CONTENT, icon);
    }
}


// **************************************************************************** Button_SetColors

// Set the color scheme of the button
void Button_SetColors(Gadget* button, Color colBG, Color colFG, Color colEmph){
    BDATA->colFG = colFG;
    BDATA->colBG = colBG;
    BDATA->colEmph = colEmph;

    if (Button_HasText(button)){
        Label_SetColor(CONTENT, colFG);
    }else{
        IconLabel_SetColor(CONTENT, colFG);
    }
}


// **************************************************************************** Button_SetRoundness

// Set the corner roundness of the button
void Button_SetRoundness(Gadget* button, float roundness){
    BDATA->roundness = roundness;
}


// **************************************************************************** Button_SetOutlineThickness

// Set the outline thickness of the button
void Button_SetOutlineThickness(Gadget* button, float thickness){
    BDATA->thickness = thickness;
}


// **************************************************************************** Button_HasText

// Return true if the button has a text label content
bool Button_HasText(const Gadget* button){
    return (CONTENT->type == GT_LABEL);
}


// **************************************************************************** Button_HasIcon

// Return true if the button has an icon content
bool Button_HasIcon(const Gadget* button){
    return (CONTENT->type == GT_ICON);
}


// **************************************************************************** Button_GetRoundness

// Return the corner roundness of the button
float Button_GetRoundness(const Gadget* button){
    return BDATA->roundness;
}


// **************************************************************************** Button_GetOutlineThickness

// Return the outline thickness of the button
float Button_GetOutlineThickness(const Gadget* button){
    return BDATA->thickness;
}


// **************************************************************************** Button_GetContext

// Return the text or icon content of the button
Gadget* Button_GetContent(const Gadget* button){
    return CONTENT;
}






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** Button_Resize

// Resize the button to its containing rectangle
static void Button_Resize(Gadget* button){
    float margin = MIN_DIM(button->cRect) * BDATA->marginRatio;
    CONTENT->cRect = Geo_ApplyRectMargins(button->cRect, margin);
}


// **************************************************************************** Button_ReactToEvent

// React to a mouse or keyboard event
static void Button_ReactToEvent(Gadget* button, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_MOUSE_MOVE:{
            if (Geo_PointIsInRect(event.data.mouse.pos, button->cRect)){
                button->isSelected = CONTENT->isSelected = true;
            }else{
                button->isSelected = CONTENT->isSelected = false;
            }
            break;
        }

        case EVENT_MOUSE_DRAG:{
            if (!Geo_PointIsInRect(event.data.mouse.pos, button->cRect)){
                button->isPressed = false;
            }
            break;
        }

        case EVENT_MOUSE_PRESSED:{
            if (Geo_PointIsInRect(event.data.mouse.pos, button->cRect)){
                button->isPressed = true;
                Queue_AddEvent(queue, Event_SetAsButtonPressed(button->id));
            }
            break;
        }

        case EVENT_MOUSE_RELEASED:{
            if (button->isPressed){
                button->isPressed = false;
                BDATA->turboCount = 0;
                Queue_AddEvent(queue, Event_SetAsButtonReleased(button->id));
            }
            break;
        }

        case EVENT_KEY_PRESSED:{
            if (event.data.key == WKEY_ENTER){
                if (button->isSelected){
                    button->isPressed = true;
                    Queue_AddEvent(queue, Event_SetAsButtonPressed(button->id));
                }
            }
            break;
        }

        case EVENT_KEY_RELEASED:{
            if (button->isPressed && event.data.key == WKEY_ENTER){
                button->isPressed = false;
                BDATA->turboCount = 0;
                Queue_AddEvent(queue, Event_SetAsButtonReleased(button->id));
            }
            break;
        }

        default: {break;}
    }
}


// **************************************************************************** Button_Update

// For turbo button functionality
static void Button_Update(Gadget* button, EventQueue* queue){
    if (button->isPressed){
        BDATA->turboCount++;
        if (BDATA->turboCount > BTN_TURBO_COUNT){
            Queue_AddEvent(queue, Event_SetAsButtonPressed(button->id));
        }
    }
}
// **************************************************************************** Button_Draw

// Draw the background of the button
static void Button_Draw(const Gadget* button, Vector2 shift){
    Color color = button->isPressed  ? BDATA->colBG : 
                  button->isSelected ? BDATA->colEmph : BDATA->colFG;
    Shape_DrawOutlinedRoundedRect(Geo_TranslateRect(button->cRect, shift), 
                                  BDATA->roundness, BDATA->thickness, 
                                  BDATA->colBG, color);
}


#ifdef DEBUG_MODE
// **************************************************************************** Button_PrintData

    // Multiline print of the parameters of the button's data object
    static void Button_PrintData(const Gadget* button){
        CHECK_NULL(button, WITH_NEW_LINE)
        CHECK_NULL(button->data, WITH_NEW_LINE)

        printf("Roundness:    %.3f\n", BDATA->roundness);
        printf("Thickness:    %.3f\n", BDATA->thickness);
        printf("Margin Ratio: %.3f\n", BDATA->marginRatio);
        printf("FG Color:     "); Color_Print(BDATA->colFG, WITH_NEW_LINE);
        printf("BG Color:     "); Color_Print(BDATA->colBG, WITH_NEW_LINE);
        printf("Emph Color:   "); Color_Print(BDATA->colEmph, WITH_NEW_LINE);
    }
#endif

