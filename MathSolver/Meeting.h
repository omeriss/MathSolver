#pragma once
#include "Screen.h"
#include "Button.h"
#include "TextBox.h"
#include "Image.h"
#include "Client.h"
#include "DrawScreen.h"
#include "Packet.h"
#include <SFML/Audio.hpp>

typedef enum ParticipentType {
    User, Host
};

class Participant {
public:
    Participant(uint32_t uid, int pos, Screen* participentsScreen) {
        this->uid = uid;
        this->pos = pos;
        UserRec = new Image(*UiElement::textureMap["UserRec"], {Start_Offset_x, Start_Offset_y + pos * Skip_Size_Factor * User_Lable_Size_y}, { User_Lable_Size_x, User_Lable_Size_y});
        participentsScreen->AddElement(UserRec);
        this->sampleCount = 0;
    }
    void RemoveFromScreen(Screen* participentsScreen) {
        participentsScreen->RemoveElement(UserRec);
    }
    void CorrectPosition(int RemovedPos) {
        if (pos < RemovedPos)
            return;
        pos--;
        UserRec->SetPosition({ Start_Offset_x, Start_Offset_y + pos * Skip_Size_Factor * User_Lable_Size_y });
    }
    void LoadSoundData(Byte* soundData, int dataSize) {
        if (sampleCount < BufferPlay) {
            std::memcpy(userSamplesBuffer + sampleCount, (const sf::Int16*)soundData, dataSize);
            sampleCount += dataSize / sizeof(sf::Int16);
        }
        else {
            std::memcpy(userSamplesBuffer + sampleCount, (const sf::Int16*)soundData, dataSize);
            sampleCount += dataSize / sizeof(sf::Int16);
            userSoundBuffer.loadFromSamples(userSamplesBuffer, sampleCount, ChannelCount, SampleRate);
            sampleCount = 0;
            userSound.setBuffer(userSoundBuffer);
            userSound.play();
            std::cout << 2;
        }
    }
    int GetPosition() {
        return pos;
    }
private:
    uint32_t uid;
    int pos;
    Image* UserRec;
    TextBox* userName;
    sf::Sound userSound;
    sf::SoundBuffer userSoundBuffer;
    sf::Int16 userSamplesBuffer[BufferSize];
    int sampleCount;
};


class VoiceChatSoundRecorder : public sf::SoundRecorder
{
public:
    VoiceChatSoundRecorder(Client* clientInstance) :SoundRecorder() {
        this->clinet = clientInstance;
    }
    ~VoiceChatSoundRecorder()
    {
        stop();
    }
private:
    virtual bool onStart()
    {
        this->setProcessingInterval(sf::Time(sf::seconds(Recorder_Processing_Interval)));
        return true;
    }
    virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
    {
        tr = new std::thread([samples, sampleCount, this]() {
            std::shared_ptr<Packet> packet = make_shared<Packet>();
            packet->GetHeader().packetType = Audiop;
            packet->WriteArr(samples ,sizeof(sf::Int16) * sampleCount);
            clinet->SendUdp(packet);
            });
        tr->detach();
        return true;
    }
    std::thread* tr;
    Client* clinet;
};

class Meeting
{
public:
    Meeting(Screen* boardS, Screen* paricipentsS, Screen* toolsS, Client* cl);
    void SetActive(bool state);
    void SetCell(int x, int y, sf::Color color);
    void SetLine(int y, array<sf::Color, BASE_DRAW_SCREEN_W>& pixels);
    void AddParticipant(uint32_t uid);
    void RemoveParticipant(uint32_t uid);
private:
    bool isActive;
    Screen *boardScreen, *participentsScreen, *toolsScreen;
    ParticipentType type; 
    DrawScreen* board;
    Client* client;
    int participantsCount;
    std::map<uint32_t, Participant*> participants;
    VoiceChatSoundRecorder voiceRecorder;
};

