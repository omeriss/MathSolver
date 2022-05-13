#pragma once

// general

#define BASE_SCREEN_W 1920
#define BASE_SCREEN_H 1080
#define SPRITE_PATH "Sprites\\"
#define Backround_Color sf::Color(28, 21, 73)

// graph

#define StartGraphXW 10
#define GraphDensity 10000
#define AsymptoteThreshold 3000

// draw screen

#define BASE_DRAW_SCREEN_W 1280
#define BASE_DRAW_SCREEN_H 720
#define Draw_Screen_H_Mul 6
#define MouseWheelSensitivity 10 
#define BaseDrawScreenColor sf::Color(255, 255, 255)
#define MarkedColor sf::Color(100, 100, 100, 100)
//#define FolderToSaveImgs ""
#define FolderToSaveImgs "C:\\Users\\user\\source\\repos\\MathSolver\\x64\\Debug\\nning"

// Meeting

#define Start_Offset_x 20
#define Start_Offset_y 20
#define User_Lable_Size_x (365/2)
#define User_Lable_Size_y (83/2)
#define Skip_Size_Factor 1.5f
#define UserName_Offset_From_Button_Top 5
#define Draw_Button_Offset_From_Button_Top 5
#define User_Name_Font_Size 23
#define Draw_Button_Size 30
#define Draw_Button_Offset_X 20
#define User_Name_Offset_X 60

// voice chat

#define ChannelCount 1
#define SampleRate 10000
#define SoundBufferSize 10000
#define BufferPlay 3000
#define Recorder_Processing_Interval 0.05

// NN

#define CharImageH 45
#define CharImageW 45
#define NumOfHiddenLayers 3
#define NumOfNodesInHidden 40
#define NumOfNodesInInput (CharImageH*CharImageW)
#define NumOfNodesInOutput 10
#define SetSize 10
#define LEARNRATE 2
#define NNSaveFile "with14.dat"
#define NNUploadFile "with14.dat"