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
    int screenWidth = 376;
    int screenHeight = 456;
    int firstStart = 1;

    InitWindow(screenWidth, screenHeight, "Calculator");

    // layout_name: controls initialization
    //----------------------------------------------------------------------------------
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

    short hasDecimal = 0;

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
                if(currNum[strlen(currNum)-1] == '.') hasDecimal = 0;
                currNum[strlen(currNum)-1] = '\0';
            }
            //clears current number, (FIX NOT HOW STRINGS WORK) actually no this works, because static size pole :]]]]
            if(btnCEPressed){
                currNum[0] = '\0';
                hasDecimal = 0;
            }
            //clears all,
            if(btnClearPressed || IsKeyPressed(KEY_DELETE)){
                currNum[0] = '\0';
                prevNum[0] = '\0';
                oper = 0;
                hasDecimal = 0;
            }

            //functions
            if((btnPlusPressed || IsKeyPressed(KEY_KP_ADD)) && strlen(currNum) != 0){
                strcpy(prevNum,currNum);
                currNum[0] = '\0';
                oper = plus;
                hasDecimal = 0;
            }
            if((btnMinPressed || IsKeyPressed(KEY_KP_SUBTRACT))  && strlen(currNum) != 0){
                if(strlen(currNum) == 0){
                    strcat(currNum,"-");
                }else if(strlen(currNum) == 1 &&  currNum[0] == '-'){}
                else{
                strcpy(prevNum,currNum);
                currNum[0] = '\0';
                oper = minus;
                hasDecimal = 0;
                }
            }
            if((btnDivPressed || IsKeyPressed(KEY_KP_DIVIDE))  && strlen(currNum) != 0){
                strcpy(prevNum,currNum);
                currNum[0] = '\0';
                oper = division;
                hasDecimal = 0;
            }
            if((btnMulPressed || IsKeyPressed(KEY_KP_MULTIPLY)) && strlen(currNum) != 0){
                strcpy(prevNum,currNum);
                currNum[0] = '\0';
                oper = mult;
                hasDecimal = 0;
            }
            if(btnFacPressed && strlen(currNum) != 0){
                oper = fact;
                btnEqlPressed = true;
            }
            if(btnSinPressed && strlen(currNum) != 0){
                oper = sinus;
                btnEqlPressed = true;
            }
            if(btnSqrtPressed && strlen(currNum) != 0){
                strcpy(prevNum,currNum);
                currNum[0] = '\0';
                oper = root;
                hasDecimal = 0;
            }
            if(btnPowPressed && strlen(currNum) != 0){
                strcpy(prevNum,currNum);
                currNum[0] = '\0';
                oper = power;
                hasDecimal = 0;
            }
            if(btnDecPressed || IsKeyPressed(KEY_PERIOD ) || IsKeyPressed(KEY_KP_DECIMAL)){
                if(!hasDecimal && strlen(currNum) != 0){
                    strcat(currNum, ".");
                    hasDecimal = 1;
                }
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
                            tmpStr = op_pow(prevNum, currNum);
                            strcpy(currNum, tmpStr);
                            break;
                        default:
                            fprintf(stderr ,"no operation error, closing");
                            exit(1);
                    }

                    if(strstr(currNum, ".") != NULL) hasDecimal = 1;
                    else hasDecimal = 0;

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
        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
        btnDivPressed = GuiButton((Rectangle){ 288, 304, 56, 56 }, "/");
        btnEqlPressed = GuiButton((Rectangle){ 224, 368, 120, 56 }, "=");
        btn0Pressed = GuiButton((Rectangle){ 160, 368, 56, 56 }, "0");
        btnDecPressed = GuiButton((Rectangle){ 96, 368, 56, 56 }, ".");
        btnFacPressed = GuiButton((Rectangle){ 32, 368, 56, 56 }, "n!");
        btn1Pressed = GuiButton((Rectangle){ 96, 304, 56, 56 }, "1");
        btn8Pressed = GuiButton((Rectangle){ 160, 176, 56, 56 }, "8");
        btn5Pressed = GuiButton((Rectangle){ 160, 240, 56, 56 }, "5");
        btnSinPressed = GuiButton((Rectangle){ 32, 176, 56, 56 }, "Sin");
        btn4Pressed = GuiButton((Rectangle){ 96, 240, 56, 56 }, "4");
        btnSqrtPressed = GuiButton((Rectangle){ 32, 240, 56, 56 }, "Root");
        btnPowPressed = GuiButton((Rectangle){ 32, 304, 56, 56 }, "x^y");
        btn7Pressed = GuiButton((Rectangle){ 96, 176, 56, 56 }, "7");
        btn2Pressed = GuiButton((Rectangle){ 160, 304, 56, 56 }, "2");
        btnDelPressed = GuiButton((Rectangle){ 32, 112, 120, 56 }, "Del");
        btn9Pressed = GuiButton((Rectangle){ 224, 176, 56, 56 }, "9");
        btnPlusPressed = GuiButton((Rectangle){ 288, 112, 56, 56 }, "+");
        btn3Pressed = GuiButton((Rectangle){ 224, 304, 56, 56 }, "3");
        btnCEPressed = GuiButton((Rectangle){ 160, 112, 56, 56 }, "CE");
        btn6Pressed = GuiButton((Rectangle){ 224, 240, 56, 56 }, "6");
        btnClearPressed = GuiButton((Rectangle){ 224, 112, 56, 56 }, "C");
        btnMinPressed = GuiButton((Rectangle){ 288, 176, 56, 56 }, "-");
        btnMulPressed = GuiButton((Rectangle){ 288, 240, 56, 56 }, "*");
        GuiSetStyle(DEFAULT, TEXT_SIZE, 28);
        if (GuiTextBox((Rectangle){ 32, 24, 312, 72 }, currNum, 128, false)){}
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

