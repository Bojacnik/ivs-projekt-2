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
#include "operation.h"

enum operation{plus = 1, minus, mult, division, sinus, fact, root, power };
//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------
void addNumberToCurrNum(char *currNum, char* number){
    if(strlen(currNum) == 1 && currNum[0] == '0'){
        currNum[0] = number[0];
        currNum[1] = '\0';
        return;
    }

    strcat(currNum, number);
    return;
}

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

    InitWindow(screenWidth, screenHeight, "Calculator");

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
    bool btnSqrtPressed = false;
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


    char currNum[50] = "";
    char prevNum[50] = "";
    int oper=0;
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
                GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
            }

            //buttons 1-9
            if(btn1Pressed || IsKeyPressed(KEY_KP_1)){ addNumberToCurrNum(currNum, "1"); }
            if(btn2Pressed || IsKeyPressed(KEY_KP_2)){ addNumberToCurrNum(currNum, "2"); }
            if(btn3Pressed || IsKeyPressed(KEY_KP_3)){ addNumberToCurrNum(currNum, "3"); }
            if(btn4Pressed || IsKeyPressed(KEY_KP_4)){ addNumberToCurrNum(currNum, "4"); }
            if(btn5Pressed || IsKeyPressed(KEY_KP_5)){ addNumberToCurrNum(currNum, "5"); }
            if(btn6Pressed || IsKeyPressed(KEY_KP_6)){ addNumberToCurrNum(currNum, "6"); }
            if(btn7Pressed || IsKeyPressed(KEY_KP_7)){ addNumberToCurrNum(currNum, "7"); }
            if(btn8Pressed || IsKeyPressed(KEY_KP_8)){ addNumberToCurrNum(currNum, "8"); }
            if(btn9Pressed || IsKeyPressed(KEY_KP_9)){ addNumberToCurrNum(currNum, "9"); }
            //--

            //nula special ofc
            if((btn0Pressed || IsKeyPressed(KEY_KP_0))) {
                if(strlen(currNum) == 0){
                    strcat(currNum, "0");
                }
                else if(strlen(currNum) == 1 && currNum[0] == '0'){}
                else{
                    strcat(currNum, "0");
                }
            }
            //---

            //removes newest digit
            if(btnDelPressed || IsKeyPressed(KEY_BACKSPACE)){
                currNum[strlen(currNum)-1] = '\0';
            }
            //clears current number, (FIX NOT HOW STRINGS WORK) actually no this works, because static size pole :]]]]
            if(btnCEPressed){
                currNum[0] = '\0';
            }
            //clears all,
            if(btnClearPressed || IsKeyPressed(KEY_DELETE)){
                currNum[0] = '\0';
                prevNum[0] = '\0';
                oper = 0;
            }

            //functions
            if(btnPlusPressed || IsKeyPressed(KEY_KP_ADD)){
                strcpy(prevNum,currNum);
                currNum[0] = '\0';
                oper = plus;
            }
            if(btnMinPressed || IsKeyPressed(KEY_KP_SUBTRACT)){
                if(strlen(currNum) == 0){
                    strcat(currNum,"-");
                }else if(strlen(currNum) == 1 &&  currNum[0] == '-'){}
                else{
                strcpy(prevNum,currNum);
                currNum[0] = '\0';
                oper = minus;
                }
            }
            if(btnDivPressed || IsKeyPressed(KEY_KP_DIVIDE)){
                strcpy(prevNum,currNum);
                currNum[0] = '\0';
                oper = division;
            }
            if(btnMulPressed || IsKeyPressed(KEY_KP_MULTIPLY)){
                strcpy(prevNum,currNum);
                currNum[0] = '\0';
                oper = mult;
            }
            if(btnFacPressed){
                oper = fact;
                btnEqlPressed = true;
            }
            if(btnSinPressed){
                oper = sinus;
                btnEqlPressed = true;
            }
            if(btnSqrtPressed){
                strcpy(prevNum,currNum);
                currNum[0] = '\0';
                oper = root;
            }
            if(btnPowPressed){
                strcpy(prevNum,currNum);
                currNum[0] = '\0';
                oper = power;
            }


        //pain and suffering
            if(btnEqlPressed || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)){
                if(oper && strlen(currNum)){
                    char* tmpStr;
                    switch (oper) {
                        case plus:
                            tmpStr = op_add(prevNum, currNum);
                            strcpy(currNum, tmpStr);
                            break;
                        case minus:
                            tmpStr = op_sub(prevNum, currNum);
                            strcpy(currNum, tmpStr);
                            break;
                        case division:
                            tmpStr = op_div(prevNum, currNum);
                            strcpy(currNum, tmpStr);
                            break;
                        case mult:
                            tmpStr = op_mul(prevNum, currNum);
                            strcpy(currNum, tmpStr);
                            break;
                        case sinus:
                            tmpStr = op_sin(currNum);
                            strcpy(currNum, tmpStr);
                            break;
                        case fact:
                            tmpStr = op_factorial(currNum);
                            strcpy(currNum, tmpStr);
                            break;
                        case root:
                            tmpStr = op_root(prevNum, currNum);
                            strcpy(currNum, tmpStr);
                            break;
                        case power:
                            tmpStr = op_root(prevNum, currNum);
                            strcpy(currNum, tmpStr);
                            break;

                    }
                    free(tmpStr);

                    prevNum[0] = '\0';
                    oper = 0;

                }
            }

            //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        // raygui: controls drawing
        //----------------------------------------------------------------------------------
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
            btnSqrtPressed = GuiButton((Rectangle){ 32, 208, 40, 40 }, "Root");
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
            if (GuiTextBox((Rectangle){ 32, 48, 232, 40 }, currNum, 256, false)) {
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

