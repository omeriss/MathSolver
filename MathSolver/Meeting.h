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

    /// <summary>
    /// constructor
    /// </summary>
    /// <param name="uid"> user id</param>
    /// <param name="pos"> pos in queue</param>
    /// <param name="participentsScreen"> p screen</param>
    /// <param name="cl"> client instance</param>
    Participant(uint32_t uid, std::string userName, int pos, Screen* participentsScreen, Client* cl){
        this->uid = uid;
        this->pos = pos;

        UserRec = new Image(*UiElement::textureMap["UserRec"], {Start_Offset_x, Start_Offset_y + pos * Skip_Size_Factor * User_Lable_Size_y}, { User_Lable_Size_x, User_Lable_Size_y});
        participentsScreen->AddElement(UserRec);

        userNameTextBox = new TextBox(*UiElement::textureMap["Empty"], UiElement::baseFont, { Start_Offset_x + User_Name_Offset_X, Start_Offset_y + pos * Skip_Size_Factor * User_Lable_Size_y + UserName_Offset_From_Button_Top},
            { User_Lable_Size_x, User_Lable_Size_y - User_Name_Offset_X }, userName, sf::Color::Black, User_Name_Font_Size, NoEdit);
        participentsScreen->AddElement(userNameTextBox);

        canDraw = new ToggleButton(*UiElement::textureMap["notEditorButton"], *UiElement::textureMap["editorButton"],
            [cl, this](bool b) {
                std::shared_ptr<Packet> packet = make_shared<Packet>();
                packet->GetHeader().packetType = ChangeParticipentType;
                (*packet) << this->uid;
                (*packet) << (b ? ParticipentType::Editor : ParticipentType::WatchOnly);
                cl->SendTcp(packet);
            }, 
            { Start_Offset_x + Draw_Button_Offset_X, Start_Offset_y + pos * Skip_Size_Factor * User_Lable_Size_y + Draw_Button_Offset_From_Button_Top }, { Draw_Button_Size, Draw_Button_Size});
        participentsScreen->AddElement(canDraw);

        canDraw->SetButtonMode(false);

        this->sampleCount = 0;

    }
    
    /// <summary>
    /// remove from p screen
    /// </summary>
    /// <param name="participentsScreen">p screen </param>
    void RemoveFromScreen(Screen* participentsScreen) {
        participentsScreen->RemoveElement(UserRec);
        delete UserRec;
        participentsScreen->RemoveElement(userNameTextBox);
        delete userNameTextBox;
        participentsScreen->RemoveElement(canDraw);
        delete canDraw;
    }

    /// <summary>
    /// set new pos because one was removed
    /// </summary>
    /// <param name="RemovedPos"> the removed pos</param>
    void CorrectPosition(int RemovedPos) {
        if (pos < RemovedPos)
            return;
        pos--;
        UserRec->SetPosition({ Start_Offset_x, Start_Offset_y + pos * Skip_Size_Factor * User_Lable_Size_y });
        userNameTextBox->SetPosition({ Start_Offset_x + User_Name_Offset_X, Start_Offset_y + pos * Skip_Size_Factor * User_Lable_Size_y + UserName_Offset_From_Button_Top });
        canDraw->SetPosition({ Start_Offset_x + Draw_Button_Offset_X, Start_Offset_y + pos * Skip_Size_Factor * User_Lable_Size_y + Draw_Button_Offset_From_Button_Top });
    }
    
    /// <summary>
    /// load sound data
    /// </summary>
    /// <param name="soundData">data</param>
    /// <param name="dataSize">size</param>
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

    /// <summary>
    /// set the p type
    /// </summary>
    /// <param name="participentType">new ptype</param>
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
    TextBox* userNameTextBox;
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

    /// <summary>
    /// pcocess sound
    /// </summary>
    /// <param name="samples"></param>
    /// <param name="sampleCount"></param>
    /// <returns></returns>
    virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
    {
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
    /// <summary>
    /// constructor
    /// </summary>
    /// <param name="boardS">board screen</param>
    /// <param name="paricipentsS">participents screen </param>
    /// <param name="toolsS">tools screen</param>
    /// <param name="subScreen">sub s</param>
    /// <param name="cl">cleint</param>
    Meeting(Screen* boardS, Screen* paricipentsS, Screen* toolsS, Screen* subScreen, Client* cl);
    void SetActive(bool state);
    void SetCell(int x, int y, sf::Color color);
    void SetLine(int y, array<sf::Color, BASE_DRAW_SCREEN_W>& pixels);
    /// <summary>
    /// add new participent
    /// </summary>
    /// <param name="uid"> uid</param>
    /// <param name="pType"> the ptype</param>
    void AddParticipant(uint32_t uid, ParticipentType pType, std::string userName);
    void RemoveParticipant(uint32_t uid);
    /// <summary>
    /// play audio of client of voice chat 
    /// </summary>
    /// <param name="uid"> the uid of the client</param>
    /// <param name="soundData">data</param>
    /// <param name="dataSize">size</param>
    void PlayAudioData(uint32_t uid, Byte* soundData, int dataSize);
    void SetColor(sf::Color color);
    void SetSelfId(uint32_t selfId);
    void SetRadius(float r);
    void SetParticipentType(uint32_t uid, ParticipentType participentType);
    void ChangeRecorderState(bool active);
    void AppendRoomCode(std::string code, bool restart);
    std::string& GetUsername();

private:
    bool isActive;

    /// <summary>
    /// screens
    /// </summary>
    Screen *boardScreen, *participentsScreen, *toolsScreen;
    ParticipentType participentType;
    DrawScreen* board;

    /// <summary>
    /// client instance
    /// </summary>
    Client* client;
    int participantsCount;

    /// <summary>
    /// participents map
    /// </summary>
    std::map<uint32_t, Participant*> participants;
    VoiceChatSoundRecorder voiceRecorder;
    uint32_t selfId;
    std::string roomCode;
    std::string userName;
};

