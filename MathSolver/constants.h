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

// Meeting

#define Start_Offset_x 20
#define Start_Offset_y 20
#define User_Lable_Size_x (365/2)
#define User_Lable_Size_y (83/2)
#define Skip_Size_Factor 1.5f

// voice chat

#define ChannelCount 1
#define SampleRate 10000
#define BufferSize 10000
#define BufferPlay 3000
#define Recorder_Processing_Interval 0.05