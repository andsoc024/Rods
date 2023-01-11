// ============================================================================
// RODS
// Graphics Header
// by Andreas Socratous
// Dec 2022
// ============================================================================


#ifndef GRAPH_GUARD
#define GRAPH_GUARD


// ============================================================================ INFO
/*
    Graphics functions for manipulating colors, managing the magic color, 
    managing the flying rectangles, working with virtual and scroll graphics, 
    drawing text, drawing shapes, making or loading and drawing textures as 
    well as calculating and drawing the timer display.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"


// ============================================================================ FUNC DECL

// ---------------------------------------------------------------------------- Color Functions

Color           Color_Random(unsigned char minV, unsigned char maxV);
Color           Color_Blend(Color color1, int f1, Color color2, int f2);
Color           Color_ChangeBrightness(Color color, int brightness);
Color           Color_SetAlpha(Color color, unsigned char alpha);
#ifdef DEBUG_MODE
    void        Color_Print(Color color, bool withNewLine);
#endif

// ---------------------------------------------------------------------------- Magic Color Functions

MagicColor*     MCol_Make(Color color1, Color color2);
MagicColor*     MCol_Free(MagicColor* mcol);
void            MCol_MakeDefault(void);
void            MCol_FreeDefault(void);
Color           MCol(MagicColor* mcol);
void            MCol_Update(MagicColor* mcol);
void            MCol_Reset(MagicColor* mcol);
#ifdef DEBUG_MODE
    void        MCol_Print(MagicColor* mcol);
#endif

// ---------------------------------------------------------------------------- Flying Rectangles Functions

FRects*         FRects_Make(void);
FRects*         FRects_Free(FRects* fRects);
void            FRects_Resize(FRects* fRects);
void            FRects_Update(FRects* fRects);
void            FRects_Draw(FRects* fRects);
#ifdef DEBUG_MODE
    void        FRect_Print(FRect* fRect, bool withNewLine);
    void        FRects_Print(FRects* fRects);
#endif

// ---------------------------------------------------------------------------- Virtual Graphics Functions

VGraph*         VGraph_Make(Size vScreen, Rect cRect, float margin);
VGraph*         VGraph_Free(VGraph* vg);
void            VGraph_Resize(VGraph* vg, Rect cRect);
Point           VGraph_ProjectPoint(Point p, VGraph* vg);
Point           VGraph_UnprojectPoint(Point p, VGraph* vg);
Rect            VGraph_ProjectRect(Rect rect, VGraph* vg);
Rect            VGraph_UnprojectRect(Rect rect, VGraph* vg);
#ifdef DEBUG_MODE
    void        VGraph_Print(VGraph* vg);
#endif

// ---------------------------------------------------------------------------- Scroll Graphics Functions

SGraph*         SGraph_Make(Size vScreen, Rect view, float margin, Size minscreen, Size maxScreen);
SGraph*         SGraph_Free(SGraph* sg);
void            SGraph_Recalc(SGraph* sg);
void            SGraph_SetDefault(SGraph* sg);
void            SGraph_SetView(SGraph* sg, Rect view);
void            SGraph_SetViewport(SGraph* sg, Rect viewport);
void            SGraph_MoveViewport(SGraph* sg, float dx, float dy);
void            SGraph_MoveViewportToDir(SGraph* sg, E_Direction dir, float distance);
void            SGraph_TranslateViewport(SGraph* sg, Vector2 vector);
void            SGraph_PlaceViewport(SGraph* sg, Point pos, E_RectPointType pointType);
void            SGraph_ResizeViewport(SGraph* sg, Size newSize, E_RectPointType pointType);
void            SGraph_SetVScreen(SGraph* sg, Size vScreen);
void            SGraph_Zoom(SGraph* sg, float zoom);
void            SGraph_SetMinScreen(SGraph* sg, Size minScreen);
void            SGraph_SetMaxScreen(SGraph* sg, Size maxScreen);
float           SGraph_CalcViewportPosRatio(SGraph* sg, E_Orientation orientation);
float           SGraph_CalcViewportSizeRatio(SGraph* sg, E_Orientation orientation);
void            SGraph_SetViewportPosFromRatio(SGraph* sg, E_Orientation orientation, float posRatio);
Point           SGraph_ProjectPoint(Point p, SGraph* sg);
Point           SGraph_UnprojectPoint(Point p, SGraph* sg);
Rect            SGraph_ProjectRect(Rect rect, SGraph* sg);
Rect            SGraph_UnprojectRect(Rect rect, SGraph* sg);
#ifdef DEBUG_MODE
    void        SGraph_Print(SGraph* sg);
#endif

// ---------------------------------------------------------------------------- Font Functions

void            Font_LoadDefault(void);
void            Font_UnloadDefault(void);
Size            Font_CalcTextSize(const char* txt, float fontSize);
Rect            Font_CalcTextRect(const char* txt, float fontSize, Point pos, E_RectPointType pointType);
Point           Font_CalcTextPos(const char* txt, float fontSize, Point pos, E_RectPointType pointType);
float           Font_FitTextInSize(const char* txt, Size cSize);
void            Font_DrawText(const char* txt, Point pos, float fontSize, Color color);

// ---------------------------------------------------------------------------- Shape Functions

void            Shape_DrawOutlinedCircle(Point center, float radius, float thickness, Color fillColor, 
                                         Color outlineColor);
void            Shape_DrawOutlinedEllipse(Rect rect, float thickness, Color fillColor, Color outlineColor);
void            Shape_DrawOutlinedRect(Rect rect, float thickness, Color fillColor, Color outlineColor);
void            Shape_DrawOutlinedRoundedRect(Rect rect, float roundness, float thickness, Color fillColor, 
                                              Color outlineColor);
void            Shape_DrawRectangleLines(Rect rect, float thickness, Color color);
void            Shape_DrawSource(Point pos, float tileSize, Color color);
void            Shape_DrawCircleInImage(Image* im, Point center, float radius, Color color);
void            Shape_DrawRodInImage(Image* im, int legs, Color color);
void            Shape_DrawSelBoxInImage(Image* im, Color color);

// ---------------------------------------------------------------------------- Texture Functions

Texture2D       Texture_LoadIcon(E_IconID id);
Texture2D       Texture_MakeRod(int legs);
Texture2D       Texture_MakeSelBox(void);
void            Texture_LoadAll(void);
void            Texture_UnloadAll(void);
#ifdef DEBUG_MODE
    void        Texture_Print(Textures* textures);
#endif

// ---------------------------------------------------------------------------- Rod Model Functions

RodModel*       RodModel_Make(float tileSize);
RodModel*       RodModel_Free(RodModel* rodModel);
void            RodModel_Resize(RodModel* rodModel, float tileSize);
void            RodModel_Draw(Rod* rod, RodModel* rodModel, Point pos, Color color);
#ifdef DEBUG_MODE
    void        RodModel_Print(RodModel* rodModel);
#endif

// ---------------------------------------------------------------------------- Timer Display Functions

TimDisp*        TimDisp_Make(Size cSize, E_RectPointType alignment);
TimDisp*        TimDisp_Free(TimDisp* td);
void            TimDisp_Resize(TimDisp* td, Size cSize);
void            TimDisp_SetAlignment(TimDisp* td, E_RectPointType alignment);
void            TimDisp_Draw(ElTime t, TimDisp* td, Point pos, Color color);
#ifdef DEBUG_MODE
    void        TimDisp_Print(TimDisp* td);
#endif


#endif // GRAPH_GUARD 

