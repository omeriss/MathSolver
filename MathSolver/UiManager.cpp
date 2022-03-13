#include "UiManager.h"

UiManager::UiManager()
{
}

void UiManager::UpdateSfmlEvents()
{
    UiElement::input.clear();
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode < 128) {
                UiElement::input.push_back(event.text.unicode);
            }
        }
        if (event.type == sf::Event::Resized) {
            for (auto it = screens.begin(); it != screens.end(); it++) {
                it->second->ReScale(event.size.width, event.size.height, lastWindowSize.x, lastWindowSize.y);
            }
            this->lastWindowSize = this->window->getSize();
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            Packet p;
            p.GetHeader().packetType = 1;
            cl.SendUdp(p);
        }
    }
}

void UiManager::Init()
{
    baseFont.loadFromFile("arial.ttf");
    InitScreens();
}

std::vector<CalcElement> testFunc(string s) {
    std::vector<CalcElement> v;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] <= '9' && s[i] >= '0') {
            CalcElement t;
            t.IsNum = true;
            t.Element.num = s[i] - '0';
            v.push_back(t);
        }
        else {
            CalcElement t;
            t.IsNum = false;
            t.Element.op = s[i];
            v.push_back(t);
        }
    }
    return v;
}

void UiManager::InitScreens()
{
    Screen* MainScreen = new Screen(BASE_SCREEN_W, BASE_SCREEN_H, window,AlignCenter);

    screens.insert({ "00main", MainScreen });
    Screen* Open = new Screen(BASE_SCREEN_W, BASE_SCREEN_H, {0,0,1,1}, window);
    screens.insert({ "01open", Open });
    Open->SetActive(false);

    auto func = [this]() mutable {screens["00main"]->SetActive(false); screens["01open"]->SetActive(true); Sleep(100); return; };
    Button* b = new Button(*UiElement::textureMap["HostMeeting"], func, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 3 }, {200*4.3, 200});
    b->SetOrigin(Center);
    MainScreen->AddElement(b);
    b = new Button(*UiElement::textureMap["JoinMeeting"], func, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 4.2 }, { 200 * 4.3, 200 });
    b->SetOrigin(Center);
    MainScreen->AddElement(b);
    b = new Button(*UiElement::textureMap["Settings"], func, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 5.4 }, { 200 * 4.3, 200 });
    b->SetOrigin(Center);
    MainScreen->AddElement(b);
    Image* m = new Image(*UiElement::textureMap["MathLinkLogo"], { BASE_SCREEN_W / 2, BASE_SCREEN_H / 5 }, {700, 350});
    m->SetOrigin(Center);
    MainScreen->AddElement(m);


    //std::vector<CalcElement> v = testFunc("1x2^3-/");
    //std::vector<CalcElement> v = testFunc("x3^");
    //Screen* Sub1 = new Screen(BASE_SCREEN_W,+ BASE_SCREEN_H, { 0.3, 0.3,0.5,0.5 }, window, SubScreen);
    //screens.insert({ "02SubWindow", Sub1 });
    //auto closesub = [this]() mutable {screens["02SubWindow"]->SetActive(false); return; };
    //auto maximize = [this]() mutable {printf("not yet\n"); return; };
    //Image* m1 = new Image(*UiElement::textureMap["SubWindow"], { 0, 0 }, { BASE_SCREEN_W, BASE_SCREEN_H });
    //Sub1->AddElement(m1);
    //b = new Button(*UiElement::textureMap["xButton"], closesub, { 100, 15 }, { 100,100 });
    //Sub1->AddElement(b);
    //b = new Button(*UiElement::textureMap["MMButton"], maximize, { 220, 15 }, { 100,100 });
    //Sub1->AddElement(b);
    //DragBar* dr = new DragBar(*UiElement::textureMap["Empty"], Sub1, { 340, 0 }, { BASE_SCREEN_W - 450, 130 });
    //Sub1->AddElement(dr);
    //Graph* g = new Graph({ BASE_SCREEN_W/2, BASE_SCREEN_H/3*1.7 }, { 1300, 800 }, v);
    //g->SetOrigin(Center);
    //Sub1->AddElement(g);
    

    TextBox* textbox = new TextBox(*UiElement::textureMap["TextBox"], baseFont, {BASE_SCREEN_W / 2, BASE_SCREEN_H/4}, {800, 130}, "Enter Room Code", sf::Color::Black, 60);
    textbox->SetOrigin(Center);
    textbox->SetName("RoomCode");
    Open->AddElement(textbox);
    textbox = new TextBox(*UiElement::textureMap["TextBox"], baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H/4*2 }, { 800, 130 }, "Enter Name", sf::Color::Black, 60);
    textbox->SetOrigin(Center);
    Open->AddElement(textbox);
    auto confunc = [this]() {string ip = ((TextBox*)screens["01open"]->GetElementByName("RoomCode"))->GetText();
                            cl.Connect(ip, 16016); };
    b = new Button(*UiElement::textureMap["JoinMeeting"], confunc, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 5 }, { 200 * 4.3, 200 });
    b->SetOrigin(Center);
    Open->AddElement(b);
    
}

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
void UiManager::Start()
{
    //string ip = "127.0.0.1";
    //cl.Connect(ip, 16016);



    window = new sf::RenderWindow(sf::VideoMode(BASE_SCREEN_W, BASE_SCREEN_H), "Math Link");
    this->lastWindowSize = window->getSize();
    window->setFramerateLimit(60);
    Init();
    sf::Clock c;
    float m = 0;
    c.restart();
    while (window->isOpen())
    {
        //cout <<( m = c.restart().asSeconds())<<endl;
        UpdateSfmlEvents();
        PacketExecutor::GetInstance()->Udate();
        window->clear(sf::Color(28, 21, 73));
        for (auto it = screens.begin(); it != screens.end(); it++) {
            it->second->UdpateScreen();
            it->second->DrawScreen();
        }
        window->display();
    }
}

void UiManager::Update()
{
}
