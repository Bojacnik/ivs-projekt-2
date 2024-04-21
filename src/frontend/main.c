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
#include "style_jungle.h"

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
    int screenWidth = 296;
    int screenHeight = 368;
    int firstStart = 1;

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
    bool btnSinPressed = false;
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
    bool ScreenBoxEditMode = false;
    int ScreenBoxValue = 0;
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Implement required update logic
            if(firstStart){
                GuiLoadStyleJungle();
                firstStart = 0;
            }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        // raygui: controls drawing
        //----------------------------------------------------------------------------------
        if (WindowBox000Active)
        {
            WindowBox000Active = !GuiWindowBox((Rectangle){ 0, 0, 296, 368 }, "SAMPLE TEXT");
            btnDivPressed = GuiButton((Rectangle){ 224, 256, 40, 40 }, "/");
            btnEqlPressed = GuiButton((Rectangle){ 176, 304, 88, 40 }, "=");
            btn0Pressed = GuiButton((Rectangle){ 128, 304, 40, 40 }, "0");
            btnDecPressed = GuiButton((Rectangle){ 80, 304, 40, 40 }, ".");
            btnFacPressed = GuiButton((Rectangle){ 32, 304, 40, 40 }, "n!");
            btn1Pressed = GuiButton((Rectangle){ 80, 256, 40, 40 }, "1");
            btn8Pressed = GuiButton((Rectangle){ 128, 160, 40, 40 }, "8");
            btn5Pressed = GuiButton((Rectangle){ 128, 208, 40, 40 }, "5");
            btnSinPressed = GuiButton((Rectangle){ 32, 160, 40, 40 }, "Sin");
            btn4Pressed = GuiButton((Rectangle){ 80, 208, 40, 40 }, "4");
            brnSqrtPressed = GuiButton((Rectangle){ 32, 208, 40, 40 }, "Sq");
            btnPowPressed = GuiButton((Rectangle){ 32, 256, 40, 40 }, "x^y");
            btn7Pressed = GuiButton((Rectangle){ 80, 160, 40, 40 }, "7");
            btn2Pressed = GuiButton((Rectangle){ 128, 256, 40, 40 }, "2");
            btnDelPressed = GuiButton((Rectangle){ 32, 112, 88, 40 }, "Del");
            btn9Pressed = GuiButton((Rectangle){ 176, 160, 40, 40 }, "9");
            btnPlusPressed = GuiButton((Rectangle){ 224, 112, 40, 40 }, "+");
            btn3Pressed = GuiButton((Rectangle){ 176, 256, 40, 40 }, "3");
            btnCEPressed = GuiButton((Rectangle){ 128, 112, 40, 40 }, "CE");
            btn6Pressed = GuiButton((Rectangle){ 176, 208, 40, 40 }, "6");
            btnClearPressed = GuiButton((Rectangle){ 176, 112, 40, 40 }, "C");
            btnMinPressed = GuiButton((Rectangle){ 224, 160, 40, 40 }, "-");
            btnMulPressed = GuiButton((Rectangle){ 224, 208, 40, 40 }, "*");
            if (GuiValueBox((Rectangle){ 32, 48, 232, 40 }, NULL, &ScreenBoxValue, 0, 100, ScreenBoxEditMode)) ScreenBoxEditMode = !ScreenBoxEditMode;
        }
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

