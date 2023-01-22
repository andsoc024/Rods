// ============================================================================
// RODS
// Switch
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions and definitions of structures and constants for the switch 
    gadget.

    The switch gadget can be in one or two states: on or off. When it changes, 
    it emits the EVENT_SWITCH_CHANGED event. The EVENT_UPDATE_SWITCH causes the 
    switch to change its state.
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

// **************************************************************************** SWDATA

// Pointer to the data object of the switch gadget
#define SWDATA \
    ((SwitchData*) sw->data)


// ============================================================================ PRIVATE CONSTANTS

#define SW_ASPECT_RATIO                     ((MATH_PHI + 2.0f) * 0.5f)              
#define SW_ROUNDNESS                        1.0f
#define SW_THICKNESS                        2.0f
// Ratio to the groove radius
#define SW_HANDLE_RATIO                     MATH_PHI
// Ratio to the handle radius
#define SW_LENGTH_RATIO                     MATH_PHI
#define SW_ANIM_FRAMES_N                    ((FPS * 1) / 5)

#define COL_SW_BG                           COL_UI_BG_PRIMARY
#define COL_SW_FG                           COL_UI_FG_PRIMARY
#define COL_SW_EMPH                         COL_UI_FG_EMPH
#define COL_SW_ACTIVE                       COL_UI_BG_ACTIVE


// ============================================================================ PRIVATE STRUCTURES

// **************************************************************************** SwitchData

// The data object of the switch gadget
typedef struct SwitchData{
    bool value;
    Point handleTargetPos[2];
    Point handlePos;
    float handleRadius;
    float handleIncr;
    float bgRadius;
    Rect bgRect;
    Rect reactRect;
    E_RectPointType alignment;
}SwitchData;



// ============================================================================ PRIVATE FUNC DECL

void            Switch_Resize(Gadget* sw);
void            Switch_ReactToEvent(Gadget* sw, Event event, EventQueue* queue);
void            Switch_Update(Gadget* sw, EventQueue* queue);
void            Switch_Draw(const Gadget* sw, Vector2 shift);
#ifdef DEBUG_MODE
    void        Switch_PrintData(const Gadget* sw);
#endif






// ============================================================================ FUNC DEF

// **************************************************************************** Switch_Make

// Make a switch gadget. It needs to be resized before its first use
Gadget* Switch_Make(E_GadgetID id, bool value){
    Gadget* sw = Gadget_Make(id, GT_SWITCH, IS_SELECTABLE, 0);

    sw->Resize        = Switch_Resize;
    sw->ReactToEvent  = Switch_ReactToEvent;
    sw->Update        = Switch_Update;
    sw->Draw          = Switch_Draw;
    #ifdef DEBUG_MODE
        sw->PrintData = Switch_PrintData;
    #endif

    SwitchData* data = Memory_Allocate(NULL, sizeof(SwitchData), ZEROVAL_ALL);
    data->value = value;
    data->alignment = RP_CENTER;

    sw->data = data;

    return sw;
}


// **************************************************************************** Switch_SetValue

// Set the value of the switch, optionally with animation
void Switch_SetValue(Gadget* sw, bool value, bool withAnim){
    if (value == SWDATA->value) {return;}

    SWDATA->value = value;

    float incr = (SWDATA->handleTargetPos[1].x - SWDATA->handleTargetPos[0].x) / (float) SW_ANIM_FRAMES_N;
    SWDATA->handleIncr = (value ? incr : -incr);

    if (!withAnim){
        Switch_FinishAnim(sw);
    }
}


// **************************************************************************** Switch_ToggleValue

// Change the state of the switch
void Switch_ToggleValue(Gadget* sw, bool withAnim){
    bool value = !(SWDATA->value);
    Switch_SetValue(sw, value, withAnim);
}


// **************************************************************************** Switch_FinishAnim

// Finish the switch animation, if any
void Switch_FinishAnim(Gadget* sw){
    SWDATA->handlePos = SWDATA->handleTargetPos[SWDATA->value ? 1 : 0];
    SWDATA->handleIncr = 0.0f;
}


// **************************************************************************** Switch_SetAlignment

// Change the alignment of the switch within its containing rectangle
void Switch_SetAlignment(Gadget* sw, E_RectPointType alignment){
    SWDATA->alignment = alignment;
    Gadget_Resize(sw);
}


// **************************************************************************** Switch_GetValue

// Return the boolean state of the switch
bool Switch_GetValue(const Gadget* sw){
    return SWDATA->value;
}


// **************************************************************************** Switch_IsAnimating

// Return true if the switch is currently animating
bool Switch_IsAnimating(const Gadget* sw){
    return SWDATA->handleIncr != 0.0f;
}


// **************************************************************************** Switch_GetAlignment

// Return the alignment of the switch, within its containing rectangle
int Switch_GetAlignment(const Gadget* sw){
    return SWDATA->alignment;
}






// ============================================================================ PRIVATE FUNC DEF

// **************************************************************************** Switch_Resize

// Resize the switch gadget, within its containing rectangle
void Switch_Resize(Gadget* sw){
    Size size = SIZE(SW_ASPECT_RATIO, 1.0f);
    size = Geo_FitSizeInSize(size, RSIZE(sw->cRect), 0.0f);
    Rect cRect = Geo_AlignRect(TO_RECT(size), sw->cRect, SWDATA->alignment);

    float totalW = cRect.width;
    SWDATA->handleRadius = size.height * 0.5f;
    SWDATA->bgRadius = SWDATA->handleRadius / SW_HANDLE_RATIO;

    SWDATA->handleTargetPos[0].x = cRect.x + SWDATA->handleRadius;
    SWDATA->handleTargetPos[1].x = cRect.x + cRect.width - SWDATA->handleRadius;
    SWDATA->handleTargetPos[0].y = cRect.y + (cRect.height * 0.5f);
    SWDATA->handleTargetPos[1].y = SWDATA->handleTargetPos[0].y;

    SWDATA->handlePos = (SWDATA->value ? SWDATA->handleTargetPos[1] : SWDATA->handleTargetPos[0]);
    SWDATA->handleIncr = 0.0f;

    SWDATA->bgRect = RECT(cRect.x, cRect.y + cRect.height * 0.5f - SWDATA->bgRadius, 
                          totalW, SWDATA->bgRadius * 2.0f);

    SWDATA->reactRect = RECT(cRect.x, cRect.y + cRect.height * 0.5f - SWDATA->handleRadius, 
                          totalW, SWDATA->handleRadius * 2.0f);
}


// **************************************************************************** Switch_ReactToEvent

// The switch recats to mouse, keyboard and other GUI events
void Switch_ReactToEvent(Gadget* sw, Event event, EventQueue* queue){
    switch (event.id){
        case EVENT_MOUSE_MOVE:{
            sw->isSelected = Geo_PointIsInRect(event.data.mouse.pos, SWDATA->reactRect);
            break;
        }

        case EVENT_MOUSE_DRAG:{
            if (!Geo_PointIsInRect(event.data.mouse.pos, SWDATA->reactRect)){
                sw->isPressed = false;
            }
            break;
        }

        case EVENT_MOUSE_PRESSED:{
            if (Geo_PointIsInRect(event.data.mouse.pos, SWDATA->reactRect)){
                sw->isPressed = true;
            }
            break;
        }

        case EVENT_MOUSE_RELEASED:{
            if (sw->isPressed){
                sw->isPressed = false;
                Switch_ToggleValue(sw, WITH_ANIM);
                Queue_AddEvent(queue, Event_SetAsSwitchChanged(sw->id, SWDATA->value));
            }
            break;
        }

        case EVENT_KEY_PRESSED:{
            if (event.data.key == WKEY_ENTER){
                Switch_ToggleValue(sw, WITH_ANIM);
                Queue_AddEvent(queue, Event_SetAsSwitchChanged(sw->id, SWDATA->value));
            }
            break;
        }

        case EVENT_UPDATE_SWITCH:{
            if (event.target == (int) sw->id){
                if (event.data.switchValue != SWDATA->value){
                    Switch_SetValue(sw, event.data.switchValue, WITH_ANIM);
                }
            }
            break;
        }

        default: {break;}
    }
}


// **************************************************************************** Switch_Update

// Update eventual animation
void Switch_Update(Gadget* sw, UNUSED EventQueue* queue){
    SWDATA->handlePos.x += SWDATA->handleIncr;

    if (SWDATA->handleIncr < 0.0f && 
        SWDATA->handlePos.x <= SWDATA->handleTargetPos[0].x){
        SWDATA->handlePos.x = SWDATA->handleTargetPos[0].x;
        SWDATA->handleIncr = 0.0f;
    }

    if (SWDATA->handleIncr > 0.0f && 
        SWDATA->handlePos.x >= SWDATA->handleTargetPos[1].x){
        SWDATA->handlePos.x = SWDATA->handleTargetPos[1].x;
        SWDATA->handleIncr = 0.0f;
    }
}


// **************************************************************************** Switch_Draw

// Draw the switch
void Switch_Draw(const Gadget* sw, Vector2 shift){
    Color outlineColor = sw->isPressed  ? COL_SW_BG   : 
                         sw->isSelected ? COL_SW_EMPH : COL_SW_FG;

    Color bgColor = SWDATA->value ? COL_SW_ACTIVE : COL_SW_BG;

    Shape_DrawOutlinedRoundedRect(Geo_TranslateRect(SWDATA->bgRect, shift), 
                                  SW_ROUNDNESS, SW_THICKNESS, bgColor, COL_SW_FG);

    Shape_DrawOutlinedCircle(Geo_TranslatePoint(SWDATA->handlePos, shift),  
                             SWDATA->handleRadius, SW_THICKNESS, COL_SW_BG, outlineColor);
}


#ifdef DEBUG_MODE
// **************************************************************************** Switch_PrintData

    // Multiline print of the parameters of the data object of the switch 
    // gadget
    void Switch_PrintData(const Gadget* sw){
        CHECK_NULL(sw, WITH_NEW_LINE)
        CHECK_NULL(sw->data, WITH_NEW_LINE)

        printf("Value:             %s\n", Bool_ToString(SWDATA->value, LONG_FORM));
        printf("Handle Target Pos: "); Geo_PrintPoint(SWDATA->handleTargetPos[0], WITHOUT_NEW_LINE);
        printf(" - "); Geo_PrintPoint(SWDATA->handleTargetPos[1], WITH_NEW_LINE);
        printf("Handle Pos:        "); Geo_PrintPoint(SWDATA->handlePos, WITH_NEW_LINE);
        printf("Handle Radius:     %.3f\n", SWDATA->handleRadius);
        printf("Handle Incr:       %.3f\n", SWDATA->handleIncr);
        printf("BG Radius:         %.3f\n", SWDATA->bgRadius);
        printf("BG Rect:           "); Geo_PrintRect(SWDATA->bgRect, WITH_NEW_LINE);
        printf("React Rect:        "); Geo_PrintRect(SWDATA->reactRect, WITH_NEW_LINE);
        printf("Alignment:         %s\n", RectPointType_ToString(SWDATA->alignment));
    }
#endif




