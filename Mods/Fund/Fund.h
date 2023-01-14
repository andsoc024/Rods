// ============================================================================
// RODS
// Fundamentals Header
// by Andreas Socratous
// Jan 2023
// ============================================================================


#ifndef FUND_GUARD
#define FUND_GUARD


// ============================================================================ INFO
/*
    Functions for error handling and memory management, math functions, 
    functions for working with the Bytes structure and directions, for string 
    manipulation, geometry and working with grid structures, time and for 
    managing the program's window. 
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"


// ============================================================================ FUNC DECL

// ---------------------------------------------------------------------------- Error Functions

void            Err_FatalError(const char* errorMsg);
void            Err_Assert(int condition, const char* errorMsg);

// ---------------------------------------------------------------------------- Memory Functions

void*           Memory_Allocate(void* ptr, int size, E_ZeroValMode zeroValMode);
void*           Memory_Copy(void* dst, const void* src, int size);
void            Memory_Write(void* dst, const void* src, int size);
void            Memory_Set(void* ptr, int size, unsigned char value);
void*           Memory_Free(void* ptr);
void            Memory_FreeAll(int nObjects, ...);

// ---------------------------------------------------------------------------- Math Functions

int             Math_RandomInt(int minV, int maxV);
float           Math_RandomFloat(float minV, float maxV);
bool            Math_FloatsAreEqual(float num1, float num2, float precision);
float           Math_Sqrt(float num);
float           Math_RightTriangleSide(float side2, float hypotenuse);
float           Math_Hypotenuse(float side1, float side2);
int             Math_PowI(int base, int exp);
float           Math_PowF(float base, int exp);
int             Math_Factorial(int num);
int             Math_Round(float num);
float           Math_RadToDeg(float radians);
float           Math_DegToRad(float degrees);
float           Math_PrincipalAngle(float angle);
int             Math_AngleQuarter(float angle);
float           Math_Sin(float angle);
float           Math_Cos(float angle);
float           Math_Tan(float angle);

// ---------------------------------------------------------------------------- Bytes Functions

bool            Bytes_IsValid(Bytes bytes);
Bytes           Bytes_FromInt(int num, int nBytes);
int             Bytes_ToInt(Bytes bytes);
Bytes           Bytes_FromFloat(float num);
float           Bytes_ToFloat(Bytes bytes);
#ifdef DEBUG_MODE
    void        Bytes_Print(Bytes bytes, bool withNewLine);
#endif

// ---------------------------------------------------------------------------- Direction Functions

E_Direction     Direction_Random(bool includeNone);
E_Direction     Direction_Rotate(E_Direction dir, int times);
E_Direction     Direction_Opposite(E_Direction dir);
E_Direction     Direction_FromKey(KeyboardKey key);
E_Direction     Direction_FromLegDir(int legDir);
int             Direction_ToLegDir(E_Direction dir);
bool            Direction_IsValidLegsValue(int legs);
int             Direction_RotateLegs(int legs, int times);
#ifdef DEBUG_MODE
    void        Direction_PrintLegs(int legs, bool withNewLine);
#endif

// ---------------------------------------------------------------------------- String Functions

int             String_Length(const char* str);
bool            String_IsEqual(const char* str1, const char* str2);
char*           String_Copy(char* dst, const char* src);
char*           String_Concat(char* dst, int nStrings, ...);
char*           String_FromInt(char* dst, int num);
char*           String_Pad(char* str, int minLength, char padChar);

// ---------------------------------------------------------------------------- Geometry Functions

Rect            Geo_SetRect(float x, float y, float w, float h, E_RectPointType pointType);
Rect            Geo_SetRectPS(Point p, Size size, E_RectPointType pointType);
Point           Geo_RectOrigin(Rect rect);
Size            Geo_RectSize(Rect rect);
Rect            Geo_SizeToRect(Size size);
Point           Geo_RectPoint(Rect rect, E_RectPointType pointType);
float           Geo_RectLimit(Rect rect, E_Direction dir);
Point           Geo_OppositePoint(Point p);
Point           Geo_TransposePoint(Point p);
Size            Geo_TransposeSize(Size size);
Rect            Geo_TransposeRect(Rect rect);
Point           Geo_RandomPoint(Rect rect);
bool            Geo_PointsAreEqual(Point p1, Point p2);
bool            Geo_PointIsInRect(Point p, Rect rect);
bool            Geo_RectsOverlap(Rect rect1, Rect rect2);
float           Geo_PointsDistance(Point p1, Point p2);
Vector2         Geo_PointsDelta(Point p1, Point p2);
Point           Geo_MovePoint(Point p, float dx, float dy);
Point           Geo_MovePointToDir(Point p, E_Direction dir, float distance);
Point           Geo_TranslatePoint(Point p, Vector2 vector);
Rect            Geo_MoveRect(Rect rect, float dx, float dy);
Rect            Geo_MoveRectToDir(Rect rect, E_Direction dir, float distance);
Rect            Geo_TranslateRect(Rect rect, Vector2 vector);
Rect            Geo_PlaceRect(Rect rect, Point pos, E_RectPointType pointType);
Rect            Geo_ResizeRect(Rect rect, Size newSize, E_RectPointType pointType);
Rect            Geo_AlignRect(Rect rect1, Rect rect2, E_RectPointType pointType);
Point           Geo_ScalePoint(Point p, float scaleF);
Size            Geo_ScaleSize(Size size, float scaleF);
Rect            Geo_ScaleRect(Rect rect, float scaleF);
Point           Geo_ProjectPoint(Point p, float scaleF, Point origin);
Point           Geo_UnprojectPoint(Point p, float scaleF, Point origin);
Rect            Geo_ProjectRect(Rect rect, float scaleF, Point origin);
Rect            Geo_UnprojectRect(Rect rect, float scaleF, Point origin);
bool            Geo_SizeIsInRange(Size size, const Size* minSize, const Size* maxSize);
Size            Geo_PutSizeInRange(Size size, const Size* minSize, const Size* maxSize);
float           Geo_PutMarginInLimits(float margin, Size cSize);
Size            Geo_ApplySizeMargins(Size size, float margin);
Rect            Geo_ApplyRectMargins(Rect rect, float margin);
Size            Geo_FitSizeInSize(Size size, Size cSize, float margin);
Size            Geo_FitSizeOnSize(Size size, Size cSize);
Rect            Geo_ProjectSizeInSize(Size size, Size cSize, float margin);
Rect            Geo_ProjectSizeInRect(Size size, Rect cRect, float margin);
Rect            Geo_PutRectInRect(Rect rect, Rect cRect, float margin);
#ifdef DEBUG_MODE
    void        Geo_PrintPoint(Point p, bool withNewLine);
    void        Geo_PrintVector(Vector2 vector, bool withNewLine);
    void        Geo_PrintSize(Size size, bool withNewLine);
    void        Geo_PrintRect(Rect rect, bool withNewLine);
#endif

// ---------------------------------------------------------------------------- Grid Functions

Grid            Grid_SetBetweenNodes(GNode node1, GNode node2);
bool            Grid_IsValid(Grid grid);
int             Grid_N(Grid grid);
GNode           Grid_RandomNode(Grid grid);
GNode           Grid_LastNode(Grid grid);
GNode           Grid_NextNode(GNode node, Grid grid);
int             Grid_Limit(Grid grid, E_Direction dir);
GNode           Grid_OppositeNode(GNode node);
bool            Grid_NodesAreEqual(GNode node1, GNode node2);
bool            Grid_NodeIsInRange(GNode node, int minX, int maxX, int minY, int maxY);
bool            Grid_NodeIsInGrid(GNode node, Grid grid);
GNode           Grid_PutNodeInRange(GNode node, int minX, int maxX, int minY, int maxY);
GNode           Grid_PutNodeInGrid(GNode node, Grid grid);
bool            Grid_Overlap(Grid grid1, Grid grid2);
Grid            Grid_Section(Grid grid, Grid grid2);
GNode           Grid_MoveNode(GNode node, int dx, int dy);
GNode           Grid_MoveNodeToDir(GNode node, E_Direction dir, int distance);
GNode           Grid_TranslateNode(GNode node, GNode vector);
Size            Grid_CalcCellSize(Grid grid, Size gridSize);
float           Grid_CalcSqrCellSize(Grid grid, Size gridSize);
Point           Grid_NodeToPoint(GNode node, float cellSize);
GNode           Grid_PointToNode(Point p, float cellSize);
Size            Grid_ToSize(Grid grid, float cellSize);
Grid            Grid_FromSize(Size size, float cellSize);
Rect            Grid_ToRect(Grid grid, float cellSize);
Grid            Grid_FromRect(Rect rect, float cellSize);
Grid            Grid_SectionFromRect(Rect rect, float cellSize, Grid totalGrid);
Size            Grid_FitSizeToGrid(Size size, Grid grid);
#ifdef DEBUG_MODE
    void        Grid_PrintNode(GNode node, bool withNewLine);
    void        Grid_Print(Grid grid, bool withNewLine);
#endif


#endif // FUND_GUARD


