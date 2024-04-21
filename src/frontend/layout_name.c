/*******************************************************************************************
*
*   LayoutName v1.0.0 - Tool Description
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 raylib technologies. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "layout_name");

    // layout_name: controls initialization
    //----------------------------------------------------------------------------------
    bool WindowBox000Active = true;
    bool btnDivPressed = false;
    bool btnEqlPressed = false;
    bool btn0Pressed = false;
    bool btnDecPressed = false;
    bool btnFacPressed = false;
    bool btn1Pressed = false;
    bool btn8Pressed = false;
    bool btn5Pressed = false;
    bool btnFcePressed = false;
    bool btn4Pressed = false;
    bool brnSqrtPressed = false;
    bool btnPowPressed = false;
    bool btn7Pressed = false;
    bool btn2Pressed = false;
    bool btnDelPressed = false;
    bool btn9Pressed = false;
    bool btnPlusPressed = false;
    bool btn3Pressed = false;
    bool btnCEPressed = false;
    bool btn6Pressed = false;
    bool btnClearPressed = false;
    bool btnMinPressed = false;
    bool btnMulPressed = false;
    bool ValueBoxEditMode = false;
    int ValueBoxValue = 0;
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Implement required update logic
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            // raygui: controls drawing
            //----------------------------------------------------------------------------------
            if (WindowBox000Active)
            {
                WindowBox000Active = !GuiWindowBox((Rectangle){ 352, 184, 296, 368 }, "SAMPLE TEXT");
                btnDelPressed = GuiButton((Rectangle){ 384, 296, 88, 40 }, "Del"); 
                if (GuiValueBox((Rectangle){ 384, 232, 232, 40 }, "SAMPLE TEXT", &ValueBoxValue, 0, 100, ValueBoxEditMode)) ValueBoxEditMode = !ValueBoxEditMode;
            }
            btnDivPressed = GuiButton((Rectangle){ 576, 400, 40, 40 }, "/"); 
            btnEqlPressed = GuiButton((Rectangle){ 528, 448, 88, 40 }, "="); 
            btn0Pressed = GuiButton((Rectangle){ 480, 448, 40, 40 }, "0"); 
            btnDecPressed = GuiButton((Rectangle){ 432, 448, 40, 40 }, "."); 
            btnFacPressed = GuiButton((Rectangle){ 384, 448, 40, 40 }, "n!"); 
            btn1Pressed = GuiButton((Rectangle){ 432, 400, 40, 40 }, "1"); 
            btn8Pressed = GuiButton((Rectangle){ 480, 304, 40, 40 }, "8"); 
            btn5Pressed = GuiButton((Rectangle){ 480, 352, 40, 40 }, "5"); 
            btnFcePressed = GuiButton((Rectangle){ 384, 304, 40, 40 }, "sin(x)"); 
            btn4Pressed = GuiButton((Rectangle){ 432, 352, 40, 40 }, "4"); 
            brnSqrtPressed = GuiButton((Rectangle){ 384, 352, 40, 40 }, "Sq(x)"); 
            btnPowPressed = GuiButton((Rectangle){ 384, 400, 40, 40 }, "x^y"); 
            btn7Pressed = GuiButton((Rectangle){ 432, 304, 40, 40 }, "7"); 
            btn2Pressed = GuiButton((Rectangle){ 480, 400, 40, 40 }, "2"); 
            btn9Pressed = GuiButton((Rectangle){ 528, 304, 40, 40 }, "9"); 
            btnPlusPressed = GuiButton((Rectangle){ 576, 256, 40, 40 }, "+"); 
            btn3Pressed = GuiButton((Rectangle){ 528, 400, 40, 40 }, "3"); 
            btnCEPressed = GuiButton((Rectangle){ 480, 256, 40, 40 }, "CE"); 
            btn6Pressed = GuiButton((Rectangle){ 528, 352, 40, 40 }, "6"); 
            btnClearPressed = GuiButton((Rectangle){ 528, 256, 40, 40 }, "C"); 
            btnMinPressed = GuiButton((Rectangle){ 576, 304, 40, 40 }, "-"); 
            btnMulPressed = GuiButton((Rectangle){ 576, 352, 40, 40 }, "*"); 
            //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------

