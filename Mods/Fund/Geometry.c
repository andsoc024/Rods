// ============================================================================
// RODS
// Geometry
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for working with the geometry structures: Point, Vector2, Size 
    and Rect.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "Fund.h"


// ============================================================================ FUNC DEF

// **************************************************************************** Geo_SetRect

// Set a rectangle with (x,y) as the given point type and size equal to w x h
Rect Geo_SetRect(float x, float y, float w, float h, E_RectPointType pointType){
    x -= ((pointType % 3) * (w * 0.5f));
    y -= ((pointType / 3) * (h * 0.5f));

    return RECT(x, y, w, h);
}


// **************************************************************************** Geo_SetRectPS

// Set a rectangle with p as the given point type and the given size
Rect Geo_SetRectPS(Point p, Size size, E_RectPointType pointType){
    return Geo_SetRect(p.x, p.y, size.width, size.height, pointType);
}


// **************************************************************************** Geo_SetRect

// The top left corner of the rectangle
Point Geo_RectOrigin(Rect rect){
    return POINT(rect.x, rect.y);
}


// **************************************************************************** Geo_RectSize

// The size of the rectangle
Size Geo_RectSize(Rect rect){
    return SIZE(rect.width, rect.height);
}


// **************************************************************************** Geo_SizeToRect

// Turn the size to a rectangle, with top left corner at (0, 0)
Rect Geo_SizeToRect(Size size){
    return RECT(0, 0, size.width, size.height);
}


// **************************************************************************** Geo_RectPoint

// Return the requested point of the rectangle
Point Geo_RectPoint(Rect rect, E_RectPointType pointType){
    Point p;
    p.x = rect.x + (pointType % 3) * (rect.width * 0.5f);
    p.y = rect.y + (pointType / 3) * (rect.height * 0.5f);

    return p;
}


// **************************************************************************** Geo_RectLimit

// The limit of the rectangle on the x or y axis towards the given direction. 
// If the direction is DIR_NONE or invalid, return INVALID
float Geo_RectLimit(Rect rect, E_Direction dir){
    switch (dir){
        case DIR_RIGHT: {return rect.x + rect.width;}
        case DIR_DOWN:  {return rect.y + rect.height;}
        case DIR_LEFT:  {return rect.x;}
        case DIR_UP:    {return rect.y;}
        default: {break;}
    }

    return INVALID;
}


// **************************************************************************** Geo_OppositePoint

// The reflection of the point in (0, 0)
Point Geo_OppositePoint(Point p){
    return POINT(-p.x, -p.y);
}


// **************************************************************************** Geo_TransposePoint

// Swap the x and y coords of the point
Point Geo_TransposePoint(Point p){
    return POINT(p.y, p.x);
}


// **************************************************************************** Geo_TransposeSize

// Swap the width and height of the size
Size Geo_TransposeSize(Size size){
    return SIZE(size.height, size.width);
}


// **************************************************************************** Geo_TransposeRect

// Swap the x and y coords as well as the width and height of the rectangle
Rect Geo_TransposeRect(Rect rect){
    return RECT(rect.y, rect.x, rect.height, rect.width);
}


// **************************************************************************** Geo_RandomPoint

// return a random point in the rectangle
Point Geo_RandomPoint(Rect rect){
    Point p;
    p.x = Math_RandomFloat(rect.x, rect.x + rect.width);
    p.y = Math_RandomFloat(rect.y, rect.y + rect.height);

    return p;
}


// **************************************************************************** Geo_PointsAreEqual

// Return true if the two points are the same, using the default float 
// precision
bool Geo_PointsAreEqual(Point p1, Point p2){
    return FEQ(p1.x, p2.x) && 
           FEQ(p1.y, p2.y);
}


// **************************************************************************** Geo_PointIsInRect

// Return true if the point is inside the rectangle
bool Geo_PointIsInRect(Point p, Rect rect){
    return IS_IN_RANGE(p.x, rect.x, rect.x + rect.width) &&
           IS_IN_RANGE(p.y, rect.y, rect.y + rect.height);
}


// **************************************************************************** Geo_RectsOverlap

// Return true if the two rectangles overlap
bool Geo_RectsOverlap(Rect rect1, Rect rect2){
    return IS_IN_RANGE(rect1.x, rect2.x - rect1.width,  rect2.x + rect2.width)   &&
           IS_IN_RANGE(rect1.y, rect2.y - rect1.height, rect2.y + rect2.height);
}


// **************************************************************************** Geo_PointsDistance

// The distance between the two points
float Geo_PointsDistance(Point p1, Point p2){
    return Math_Hypotenuse(p2.x - p1.x, p2.y - p1.y);
}


// **************************************************************************** Geo_PointsDelta

// Return a vector from p1 to p2
Vector2 Geo_PointsDelta(Point p1, Point p2){
    return VECTOR(p2.x - p1.x, p2.y - p1.y);
}


// **************************************************************************** Geo_MovePoint

// Move the point by dx and dy
Point Geo_MovePoint(Point p, float dx, float dy){
    p.x += dx;
    p.y += dy;

    return p;
}


// **************************************************************************** Geo_MovePointToDir

// Move the point towards the given direction, by the given distance
Point Geo_MovePointToDir(Point p, E_Direction dir, float distance){
    switch (dir){
        case DIR_RIGHT: {p.x += distance; break;}
        case DIR_DOWN:  {p.y += distance; break;}
        case DIR_LEFT:  {p.x -= distance; break;}
        case DIR_UP:    {p.y -= distance; break;}
        default: {break;}
    }

    return p;
}


// **************************************************************************** Geo_TranslatePoint

// Move the point by the given vector
Point Geo_TranslatePoint(Point p, Vector2 vector){
    return Geo_MovePoint(p, vector.x, vector.y);
}


// **************************************************************************** Geo_MoveRect

// Move the rectangle by dx and dy
Rect Geo_MoveRect(Rect rect, float dx, float dy){
    rect.x += dx;
    rect.y += dy;

    return rect;
}


// **************************************************************************** Geo_MoveRectToDir

// Move the rectangle towards the given direction, by the given distance
Rect Geo_MoveRectToDir(Rect rect, E_Direction dir, float distance){
    Point p = RORIGIN(rect);
    p = Geo_MovePointToDir(p, dir, distance);
    return RECT(p.x, p.y, rect.width, rect.height);
}


// **************************************************************************** Geo_TranslateRect

// Move the rectangle by the given vector
Rect Geo_TranslateRect(Rect rect, Vector2 vector){
    return Geo_MoveRect(rect, vector.x, vector.y);
}


// **************************************************************************** Geo_PlaceRect

// Place the rectangle so that pos is the rectangle's given point
Rect Geo_PlaceRect(Rect rect, Point pos, E_RectPointType pointType){
    return Geo_SetRectPS(pos, RSIZE(rect), pointType);
}


// **************************************************************************** Geo_ResizeRect

// Change the size of the rectangle, retaining its position at the given point
Rect Geo_ResizeRect(Rect rect, Size newSize, E_RectPointType pointType){
    Point p = Geo_RectPoint(rect, pointType);
    return Geo_SetRectPS(p, newSize, pointType);
}


// **************************************************************************** Geo_AlignRect

// Align rect1 to the given point of rect2
Rect Geo_AlignRect(Rect rect1, Rect rect2, E_RectPointType pointType){
    Point p = Geo_RectPoint(rect2, pointType);
    return Geo_PlaceRect(rect1, p, pointType);
}


// **************************************************************************** Geo_ScalePoint

// Scale the point by the given scale factor
Point Geo_ScalePoint(Point p, float scaleF){
    p.x *= scaleF;
    p.y *= scaleF;

    return p;
}


// **************************************************************************** Geo_ScaleSize

// Scale the size by the given scale factor
Size Geo_ScaleSize(Size size, float scaleF){
    size.width  *= scaleF;
    size.height *= scaleF;

    return size;
}


// **************************************************************************** Geo_ScaleRect

// Scale the rectangle by the given scale factor
Rect Geo_ScaleRect(Rect rect, float scaleF){
    rect.x      *= scaleF;
    rect.y      *= scaleF;
    rect.width  *= scaleF;
    rect.height *= scaleF;

    return rect;
}


// **************************************************************************** Geo_ProjectPoint

// Transform the point by scaling and translating
Point Geo_ProjectPoint(Point p, float scaleF, Point origin){
    p = Geo_ScalePoint(p, scaleF);
    p = Geo_TranslatePoint(p, origin);

    return p;
}


// **************************************************************************** Geo_UnprojectPoint

// Transform the point by translating and scaling. The opposite of project
Point Geo_UnprojectPoint(Point p, float scaleF, Point origin){
    p = Geo_TranslatePoint(p, Geo_OppositePoint(origin));
    p = Geo_ScalePoint(p, 1.0f / scaleF);

    return p;
}


// **************************************************************************** Geo_ProjectRect

// Transform the rectangle by scaling and translating
Rect Geo_ProjectRect(Rect rect, float scaleF, Point origin){
    rect = Geo_ScaleRect(rect, scaleF);
    rect = Geo_TranslateRect(rect, origin);

    return rect;
}


// **************************************************************************** Geo_UnprojectRect

// Transform the rectangle by translating and scaling. The opposite of project
Rect Geo_UnprojectRect(Rect rect, float scaleF, Point origin){
    rect = Geo_TranslateRect(rect, Geo_OppositePoint(origin));
    rect = Geo_ScaleRect(rect, 1.0f / scaleF);

    return rect;
}


// **************************************************************************** Geo_SizeIsInRange

// Return true if the size is within and including the limits. If the limit is 
// NULL, ignore it
bool Geo_SizeIsInRange(Size size, const Size* minSize, const Size* maxSize){
    return ((minSize == NULL) || (size.width >= minSize->width && size.height >= minSize->height)) &&
           ((maxSize == NULL) || (size.width <= maxSize->width && size.height <= maxSize->height));
}


// **************************************************************************** Geo_PutSizeInRange

// Ensure that the size is within the given limits. If a limit is NULL, it is 
// ignored
Size Geo_PutSizeInRange(Size size, const Size* minSize, const Size* maxSize){
    
    if (minSize != NULL){
        size.width  = MAX(size.width, minSize->width);
        size.height = MAX(size.height, minSize->height);
    }

    if (maxSize != NULL){
        size.width = MIN(size.width, maxSize->width);
        size.height = MIN(size.height, maxSize->height);
    }

    return size;
}


// **************************************************************************** Geo_PutMarginsInLimits

// Ensure that the margin is less than half of the minimum dimension of the 
// containing size. No limit on the negative side
float Geo_PutMarginInLimits(float margin, Size cSize){
    float maxMargin = MIN_DIM(cSize) * 0.5f;
    return MIN(margin, maxMargin);
}


// **************************************************************************** Geo_ApplySizeMargins

// Apply the margin on all 4 sides of the size
Size Geo_ApplySizeMargins(Size size, float margin){
    margin = Geo_PutMarginInLimits(margin, size);

    size.width  -= (2.0f * margin);
    size.height -= (2.0f * margin);

    return size;
}


// **************************************************************************** Geo_ApplyRectMargins

// Apply the margin on all 4 sides of the rectangle
Rect Geo_ApplyRectMargins(Rect rect, float margin){
    margin = Geo_PutMarginInLimits(margin, RSIZE(rect));

    rect.x += margin;
    rect.y += margin;

    rect.width  -= (2.0f * margin);
    rect.height -= (2.0f * margin);

    return rect;
}


// **************************************************************************** Geo_FitSizeInSize

// The maximum size, with the same aspect ratio, that fits inside the 
// containing size, with margins applied
Size Geo_FitSizeInSize(Size size, Size cSize, float margin){
    cSize = Geo_ApplySizeMargins(cSize, margin);

    Size res;

    res.width = cSize.width;
    res.height = res.width * (size.height / size.width);
    if (res.height > cSize.height){
        res.height = cSize.height;
        res.width = res.height * (size.width / size.height);
    }

    return res;
}


// **************************************************************************** Geo_FitSizeOnSize

// The minimum size, with the same aspect ratio, that contains cSize
Size Geo_FitSizeOnSize(Size size, Size cSize){
    Size res;

    res.width = cSize.width;
    res.height = res.width * (size.height / size.width);
    if (res.height < cSize.height){
        res.height = cSize.height;
        res.width = res.height * (size.width / size.height);
    }

    return res;
}


// **************************************************************************** Geo_ProjectSizeInSize

// The maximum rectangle, with the same aspect ratio, that fits inside the 
// containing rectangle with the same size as cSize, with margins applied, 
// centered inside the containing rectangle
Rect Geo_ProjectSizeInSize(Size size, Size cSize, float margin){
    return Geo_ProjectSizeInRect(size, TO_RECT(cSize), margin);
}


// **************************************************************************** Geo_ProjectSize

// The maximum rectangle, with the same aspect ratio, that fits inside cRect, 
// with margins applied, centered in cRect
Rect Geo_ProjectSizeInRect(Size size, Rect cRect, float margin){
    Size resSize = Geo_FitSizeInSize(size, RSIZE(cRect), margin);
    Point center = Geo_RectPoint(cRect, RP_CENTER);
    return Geo_SetRectPS(center, resSize, RP_CENTER);
}


// **************************************************************************** Geo_PutRectInRect

// Ensure that rect is inside cRect, with margins applied. If rect is bigger 
// than cRect, with either its width or height, center it in cRect on the x or 
// y axis
Rect Geo_PutRectInRect(Rect rect, Rect cRect, float margin){
    cRect = Geo_ApplyRectMargins(cRect, margin);

    // X-axis
    if (rect.width >= cRect.width){
        rect.x = cRect.x + (cRect.width - rect.width) * 0.5f;
    }else{
        float xLimit = cRect.x + cRect.width - rect.width;
        rect.x = PUT_IN_RANGE(rect.x, cRect.x, xLimit);
    }

    // Y-axis
    if (rect.height >= cRect.height){
        rect.y = cRect.y + (cRect.height - rect.height) * 0.5f;
    }else{
        float yLimit = cRect.y + cRect.height - rect.height;
        rect.y = PUT_IN_RANGE(rect.y, cRect.y, yLimit);
    }

    return rect;
}


#ifdef DEBUG_MODE
// **************************************************************************** Geo_PrintPoint

    // Print of the point in the form (x, y). Optionally with new line
    void Geo_PrintPoint(Point p, bool withNewLine){
        printf("(%.3f, %.3f)", p.x, p.y);
        if (withNewLine) {printf("\n");}
    }


// **************************************************************************** Geo_PrintVector

    // Print of the vector in the form (x, y). Optionally with new line
    void Geo_PrintVector(Vector2 vector, bool withNewLine){
        Geo_PrintPoint(vector, withNewLine);
    }


// **************************************************************************** Geo_PrintSize

    // Print the size in the form (width, height). Optionally with new line
    void Geo_PrintSize(Size size, bool withNewLine){
        printf("(%.3f, %.3f)", size.width, size.height);
        if (withNewLine) {printf("\n");}
    }


// **************************************************************************** Geo_PrintRect

    // Print the rectangle in the form (x, y, width, height). Optionally with 
    // new line
    void Geo_PrintRect(Rect rect, bool withNewLine){
        printf("(%.3f, %.3f, ", rect.x, rect.y);
        printf("%.3f, %.3f)", rect.width, rect.height);
        if (withNewLine) {printf("\n");}
    }
#endif





