#pragma once
#include "Screen.h"
#include "Button.h"
#include "TextBox.h"
#include "Image.h"
#include "Client.h"
#include "DrawScreen.h"
#include "Packet.h"
#include "ToggleButton.h"
#include <SFML/Audio.hpp>

enum class ParticipentType :uint16_t {
    WatchOnly = 0, Editor = 1, Host = 2
};

class Participant {
public:
    Participant(uint32_t uid, int pos, Screen* participentsScreen){
        this->uid = uid;
        this->pos = pos;

        UserRec = new Image(*UiElement::textureMap["UserRec"], {Start_Offset_x, Start_Offset_y + pos * Skip_Size_Factor * User_Lable_Size_y}, { User_Lable_Size_x, User_Lable_Size_y});
        participentsScreen->AddElement(UserRec);

        userName = new TextBox(*UiElement::textureMap["Empty"], UiElement::baseFont, { Start_Offset_x + User_Name_Offset_X, Start_Offset_y + pos * Skip_Size_Factor * User_Lable_Size_y + UserName_Offset_From_Button_Top},
            { User_Lable_Size_x, User_Lable_Size_y - User_Name_Offset_X }, "omeriss", sf::Color::Black, User_Name_Font_Size, NoEdit);
        participentsScreen->AddElement(userName);

        canDraw = new ToggleButton(*UiElement::textureMap["notEditorButton"], *UiElement::textureMap["editorButton"], [](bool b) {return; }, 
            { Start_Offset_x + Draw_Button_Offset_X, Start_Offset_y + pos * Skip_Size_Factor * User_Lable_Size_y + Draw_Button_Offset_From_Button_Top }, { Draw_Button_Size, Draw_Button_Size});
        participentsScreen->AddElement(canDraw);

        canDraw->SetButtonMode(false);

        this->sampleCount = 0;

    }

    void RemoveFromScreen(Screen* participentsScreen) {
        participentsScreen->RemoveElement(UserRec);
        delete UserRec;
        participentsScreen->RemoveElement(userName);
        delete userName;
        participentsScreen->RemoveElement(canDraw);
        delete canDraw;
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
        }
    }

    int GetPosition() {
        return pos;
    }

    void SetParticipentType(ParticipentType participentType) {
        this->participentType = participentType;
        if(participentType == ParticipentType::WatchOnly)
            canDraw->ChangeState(false);
        else
            canDraw->ChangeState(true);
    }

    void SetButtonMode(bool buttonMode) {
        canDraw->SetButtonMode(buttonMode);
    }
private:
    uint32_t uid;
    ParticipentType participentType;
    int pos;
    Image* UserRec;
    ToggleButton* canDraw;
    TextBox* userName;
    sf::Sound userSound;
    sf::SoundBuffer userSoundBuffer;
    sf::Int16 userSamplesBuffer[SoundBufferSize];
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
        //static sf::SoundBuffer bf;
        //static sf::Sound so;
        //sf::Int16* samples2 = new sf::Int16[sampleCount];
        //std::memcpy(samples2, samples, sampleCount * sizeof(sf::Int16));
        //bf.loadFromSamples(samples2, sampleCount, ChannelCount, SampleRate);
        //so.setBuffer(bf);
        //so.play();
        //for (int i = 0; i < sampleCount; i++) {
        //    std::cout << ((const sf::Int16*)samples)[i] << std::endl;
        //}
        tr = new std::thread([samples, sampleCount, this]() {
            std::shared_ptr<Packet> packet = make_shared<Packet>();
            packet->GetHeader().packetType = Audiop;
            packet->WriteArr(samples , sampleCount);
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
    void AddParticipant(uint32_t uid, ParticipentType pType);
    void RemoveParticipant(uint32_t uid);
    void PlayAudioData(uint32_t uid, Byte* soundData, int dataSize);
    void SetColor(sf::Color color);
    void SetSelfId(uint32_t selfId);
    void SetRadius(float r);
    void SetParticipentType(uint32_t uid, ParticipentType participentType);
private:
    bool isActive;
    Screen *boardScreen, *participentsScreen, *toolsScreen;
    ParticipentType participentType;
    DrawScreen* board;
    Client* client;
    int participantsCount;
    std::map<uint32_t, Participant*> participants;
    VoiceChatSoundRecorder voiceRecorder;
    uint32_t selfId;
};

