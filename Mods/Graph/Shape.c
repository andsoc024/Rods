// ============================================================================
// RODS
// Shape
// by Andreas Socratous
// Jan 2023
// ============================================================================


// ============================================================================ INFO
/*
    Functions for drawing shapes on the screen or in images.
*/


// ============================================================================ DEPENDENCIES

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#include "../Public/Public.h"
#include "../Fund/Fund.h"
#include "Graph.h"


// ============================================================================ PRIVATE CONSTANTS

#define RRECT_SEGMENTS_N                    10

#define ROD_LEG_WIDTH_RATIO                 (1.0f - MATH_PHI_INVERSE)
#define ROD_SOURCE_RADIUS_RATIO             (MATH_PHI * ROD_LEG_WIDTH_RATIO * 0.5f)
#define ROD_SOURCE_OUTLINE_THICKNESS        1.0f
#define COL_SOURCE_OUTLINE                  BLACK

#define SELBOX_CORNER_SIZE_RATIO            ((1.0f - MATH_PHI_INVERSE) * 0.5f)
#define SELBOX_CORNER_RADIUS_RATIO          (SELBOX_CORNER_SIZE_RATIO * MATH_PHI_INVERSE)
#define SELBOX_EDGE_RATIO                   (SELBOX_CORNER_SIZE_RATIO - SELBOX_CORNER_RADIUS_RATIO)
#define SELBOX_THICKNESS_RATIO              0.07f
#define SELBOX_MIN_THICKNESS                3.0f
#define SELBOX_MAX_THICKNESS                10.0f






// ============================================================================ FUNC DEF

// **************************************************************************** Shape_DrawOutlinedCircle

// Draw a circle with a perimeter
void Shape_DrawOutlinedCircle(Point center, float radius, float thickness, Color fillColor, Color outlineColor){
    DrawCircleV(center, radius, outlineColor);
    DrawCircleV(center, radius - thickness, fillColor);
}


// **************************************************************************** Shape_DrawOutlinedEllipse

// Draw an ellipse with a perimeter
void Shape_DrawOutlinedEllipse(Rect cRect, float thickness, Color fillColor, Color outlineColor){
    float radiusX = cRect.width * 0.5f;
    float radiusY = cRect.height * 0.5f;
    
    float x0 = cRect.x + radiusX;
    float y0 = cRect.y + radiusY;

    DrawEllipse(x0, y0, radiusX, radiusY, outlineColor);
    DrawEllipse(x0, y0, radiusX - thickness, radiusY - thickness, fillColor);
}


// **************************************************************************** Shape_DrawOutlinedRect

// Draw a rectangle with a perimeter
void Shape_DrawOutlinedRect(Rect rect, float thickness, Color fillColor, Color outlineColor){
    DrawRectangleRec(rect, outlineColor);
    DrawRectangleRec(Geo_ApplyRectMargins(rect, thickness), fillColor);
}


// **************************************************************************** Shape_DrawOutlinedRoundedRect

// Draw a rounded rectangle with a perimeter
void Shape_DrawOutlinedRoundedRect(Rect rect, float roundness, float thickness, Color fillColor, Color outlineColor){
    DrawRectangleRounded(rect, roundness, RRECT_SEGMENTS_N, outlineColor);
    DrawRectangleRounded(Geo_ApplyRectMargins(rect, thickness), roundness, RRECT_SEGMENTS_N, fillColor);
}


// **************************************************************************** Shape_DrawRectOutline

// Draw a rectangle outline
void Shape_DrawRectOutline(Rect rect, float thickness, Color color){
    DrawRectangleRec(RECT(rect.x + rect.width - thickness, rect.y + thickness,               
                          thickness,  rect.height - 2 * thickness), color);                  // Right
    DrawRectangleRec(RECT(rect.x,                          rect.y + rect.height - thickness, 
                          rect.width, thickness),                   color);                  // Down
    DrawRectangleRec(RECT(rect.x,                          rect.y + thickness,               
                          thickness,  rect.height - 2 * thickness), color);                  // Left
    DrawRectangleRec(RECT(rect.x,                          rect.y,                           
                          rect.width, thickness),                   color);                  // Up
}


// **************************************************************************** Shape_DrawCircleInImage

// Draw a filled circle in the image
void Shape_DrawCircleInImage(Image* im, Point center, float radius, Color color){
    for (float dx = 0; dx <= radius; dx += 0.5f){
        float dy = Math_RightTriangleSide(dx, radius);
        Point p1 = POINT(center.x + dx, center.y - dy);
        Point p2 = POINT(center.x + dx, center.y + dy);
        Point p3 = POINT(center.x - dx, center.y - dy);
        Point p4 = POINT(center.x - dx, center.y + dy);
        ImageDrawLineV(im, p1, p2, color);
        ImageDrawLineV(im, p3, p4, color);
    }
}


// **************************************************************************** Shape_DrawRodInImage

// Draw a rod, with the given number of legs, in the image
void Shape_DrawRodInImage(Image* im, int legs, Color color){
    legs = PUT_IN_RANGE(legs, 0x0, 0xF);

    float tileSize = MIN(im->width, im->height);
    if (tileSize <= 0.0f){
        return;
    }

    float halfTileSize = tileSize * 0.5f;
    float legWidth = ROD_LEG_WIDTH_RATIO * tileSize;
    float halfLegWidth = legWidth * 0.5f;

    Point center = POINT(halfTileSize, halfTileSize);

    Shape_DrawCircleInImage(im, center, halfLegWidth, color);

    const float extraW = 2.0f;

    if (legs & LEGDIR_RIGHT){
        ImageDrawRectangleRec(im, RECT(center.x, center.y - halfLegWidth, halfTileSize, legWidth + extraW), color);
    }

    if (legs & LEGDIR_DOWN){
        ImageDrawRectangleRec(im, RECT(center.x - halfLegWidth, center.y, legWidth + extraW, halfTileSize), color);
    }

    if (legs & LEGDIR_LEFT){
        ImageDrawRectangleRec(im, RECT(0.0f, center.y - halfLegWidth, halfTileSize, legWidth + extraW), color);
    }

    if (legs & LEGDIR_UP){
        ImageDrawRectangleRec(im, RECT(center.x - halfLegWidth, 0.0f, legWidth + extraW, halfTileSize), color);
    }
}


// **************************************************************************** Shape_DrawSource

// Draw the source as an outlined circle
void Shape_DrawSource(Point pos, float tileSize, Color color){
    Point center = Geo_MovePoint(pos, tileSize * 0.5f, tileSize * 0.5f);
    float radius = ROD_LEG_WIDTH_RATIO * tileSize;
    Shape_DrawOutlinedCircle(center, radius, ROD_SOURCE_OUTLINE_THICKNESS, color, COL_SOURCE_OUTLINE);
}


// **************************************************************************** Shape_DrawSelBoxInImage

// Draw a selection box in the image
void Shape_DrawSelBoxInImage(Image* im, Color color){
    float tileSize = MIN(im->width, im->height);
    
    float cornerSize = SELBOX_CORNER_SIZE_RATIO * tileSize;
    float outerRadius = SELBOX_CORNER_RADIUS_RATIO * tileSize;
    float cornerEdge = SELBOX_EDGE_RATIO * tileSize;

    float thickness = SELBOX_THICKNESS_RATIO * tileSize;
    thickness = PUT_IN_RANGE(thickness, SELBOX_MIN_THICKNESS, SELBOX_MAX_THICKNESS);

    float innerRadius = outerRadius - thickness;

    // Draw the rounded part of the corner
    for (float y = 0.0f; y <= outerRadius; y += 0.5f){
        float x0 = outerRadius - Math_RightTriangleSide(outerRadius - y, outerRadius);
        float x1 = cornerSize;
        if (y > thickness){
            x1 = outerRadius - Math_RightTriangleSide(innerRadius - (y - thickness), innerRadius);
        }

        // Top Left
        Point p0 = POINT(x0, y);
        Point p1 = POINT(x1, y);
        ImageDrawLineV(im, p0, p1, color);

        // Top Right
        p0 = POINT(tileSize - x1, y);
        p1 = POINT(tileSize - x0, y);
        ImageDrawLineV(im, p0, p1, color);

        // Bottom Left
        p0 = POINT(x0, tileSize - y);
        p1 = POINT(x1, tileSize - y);
        ImageDrawLineV(im, p0, p1, color);

        // Bottom Right
        p0 = POINT(tileSize - x1, tileSize - y);
        p1 = POINT(tileSize - x0, tileSize - y);
        ImageDrawLineV(im, p0, p1, color);
    }

    const float extraW = 1.0f;
    const float extraH = 2.0f;

    // Draw the corner vertical edges
    Rect topLeftEdge     = RECT(0.0f,                 outerRadius,           
                                thickness + extraW,   cornerEdge + extraH);
    Rect topRightEdge    = RECT(tileSize - thickness, outerRadius,           
                                thickness + extraW,   cornerEdge + extraH);
    Rect bottomLeftEdge  = RECT(0.0f,                 tileSize - cornerSize, 
                                thickness + extraW,   cornerEdge + extraH);
    Rect bottomRightEdge = RECT(tileSize - thickness, tileSize - cornerSize, 
                                thickness + extraW,   cornerEdge + extraH);
    ImageDrawRectangleRec(im, topLeftEdge,     color);
    ImageDrawRectangleRec(im, topRightEdge,    color);
    ImageDrawRectangleRec(im, bottomLeftEdge,  color);
    ImageDrawRectangleRec(im, bottomRightEdge, color);
}



