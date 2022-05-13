#include "UiManager.h"

UiManager::UiManager()
{
}

UiManager* UiManager::GetInstance()
{
    if (instance == NULL)
        UiManager::instance = new UiManager();
    return instance;
}

void UiManager::UpdateSfmlEvents()
{

    UiElement::input.clear();
    UiElement::MouseWheelDelta = 0;


    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window->close();
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode < 128) {
                if (event.text.unicode == 22) {
                    char* text = NULL;
                    void* cdata = NULL;
                    if (OpenClipboard(NULL))
                        cdata = GetClipboardData(CF_TEXT);
                    text = (char*)cdata;
                    while (text && *text) {
                        std::cout << *text << std::endl;
                        UiElement::input.push_back(*text);
                        text++;
                    }
                    GlobalUnlock(cdata);
                    CloseClipboard();
                }
                else {
                    UiElement::input.push_back(event.text.unicode);
                }
            }
        }
        if (event.type == sf::Event::Resized) {
            for (auto it = screens.begin(); it != screens.end(); it++) {
                it->second->ReScale(event.size.width, event.size.height, lastWindowSize.x, lastWindowSize.y);
            }
            this->lastWindowSize = this->window->getSize();
        }
        if (event.type == sf::Event::MouseEntered) {
            UiElement::MouseInScreen = true;
        }
        if (event.type == sf::Event::MouseLeft) {
            UiElement::MouseInScreen = false;
        }
        if (event.type == sf::Event::MouseWheelMoved) {
            UiElement::MouseWheelDelta = event.mouseWheel.delta;
        }
    }
}

void UiManager::Init()
{
    UiElement::baseFont.loadFromFile("arial.ttf");
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

    auto func = [this]() mutable {screens["00main"]->SetActive(false); screens["01open"]->SetActive(true); Sleep(100); return; };
    auto func1 = [this]() mutable {screens["00main"]->SetActive(false); screens["01join"]->SetActive(true); Sleep(100); return; };
    Button* b = new Button(*UiElement::textureMap["HostMeeting"], func, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 3 }, {200*4.3, 200});
    b->SetOrigin(Center);
    MainScreen->AddElement(b);
    b = new Button(*UiElement::textureMap["JoinMeeting"], func1, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 4.2 }, { 200 * 4.3, 200 });
    b->SetOrigin(Center);
    MainScreen->AddElement(b);
    b = new Button(*UiElement::textureMap["Settings"], func, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 5.4 }, { 200 * 4.3, 200 });
    b->SetOrigin(Center);
    MainScreen->AddElement(b);
    Image* m = new Image(*UiElement::textureMap["MathLinkLogo"], { BASE_SCREEN_W / 2, BASE_SCREEN_H / 5 }, {700, 350});
    m->SetOrigin(Center);
    MainScreen->AddElement(m);

    
    Screen* subScreenEq = new Screen(BASE_SCREEN_W,+ BASE_SCREEN_H, { 0.3, 0.3,0.5,0.5 }, window, SubScreen);
    subScreenEq->SetActive(false);
    screens.insert({ "09SubWindow", subScreenEq });
    
    

    Screen* Open = new Screen(BASE_SCREEN_W, BASE_SCREEN_H, {0,0,1,1}, window);
    screens.insert({ "01open", Open });
    Open->SetActive(false);
    TextBox* textbox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, {BASE_SCREEN_W / 2, BASE_SCREEN_H/4}, {800, 130}, "Enter Room Code", sf::Color::Black, 60);
    textbox->SetOrigin(Center);
    textbox->SetName("RoomCode");
    Open->AddElement(textbox);
    textbox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H/4*2 }, { 800, 130 }, "Enter Name", sf::Color::Black, 60);
    textbox->SetOrigin(Center);
    Open->AddElement(textbox);
    auto confunc = [this]() {string ip = ((TextBox*)screens["01open"]->GetElementByName("RoomCode"))->GetText();
                            cl.Connect(ip, 16016); };
    b = new Button(*UiElement::textureMap["HostMeeting"], confunc, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 5 }, { 200 * 4.3, 200 });
    b->SetOrigin(Center);
    Open->AddElement(b);


    Screen* Join = new Screen(BASE_SCREEN_W, BASE_SCREEN_H, { 0,0,1,1 }, window);
    screens.insert({ "01join", Join });
    Join->SetActive(false);
    textbox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 4 }, { 800, 130 }, "Enter IP", sf::Color::Black, 60);
    textbox->SetOrigin(Center);
    textbox->SetName("ip");
    Join->AddElement(textbox);
    textbox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 4 * 2 }, { 800, 130 }, "Enter Room Code", sf::Color::Black, 60);
    textbox->SetOrigin(Center);
    textbox->SetName("RoomCode");
    Join->AddElement(textbox);
    auto confunc1 = [this]() {string ip = ((TextBox*)screens["01join"]->GetElementByName("ip"))->GetText();
    string roomCode = ((TextBox*)screens["01join"]->GetElementByName("RoomCode"))->GetText();
    cl.Connect(ip, 16016, roomCode); };
    b = new Button(*UiElement::textureMap["JoinMeeting"], confunc1, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 5 }, { 200 * 4.3, 200 });
    b->SetOrigin(Center);
    Join->AddElement(b);
    

    Screen* Draw = new Screen(BASE_DRAW_SCREEN_W, BASE_DRAW_SCREEN_H, { 0,1/6.0,5/6.0, 5 / 6.0 }, window, ScaleByWith);
    Draw->SetActive(false);
    screens.insert({ "03Draw", Draw });
    DrawScreen* dsc = new DrawScreen({ 0,0 }, { BASE_DRAW_SCREEN_W, BASE_DRAW_SCREEN_H });
    dsc->SetName("board");
    Draw->AddElement(dsc);


    Screen* tools = new Screen(BASE_DRAW_SCREEN_W*5.0/6.0, BASE_DRAW_SCREEN_H/6.0, { 0,0.0,5 / 6.0, 1 / 6.0 }, window, AlignLeft);
    tools->SetActive(false);
    screens.insert({ "03Tools", tools });

    Screen* participents = new Screen(BASE_DRAW_SCREEN_W * 1.0 / 6, BASE_DRAW_SCREEN_H, { 5 / 6.0,0,1 / 6.0, 1 }, window, ScaleByWith);
    participents->SetActive(false);
    screens.insert({ "03Participents", participents });

    tools->AddElement(new Button(*UiElement::textureMap["blackButton"], 
        [changecolor = sf::Color(0, 0, 0), this](){ this->GetMeeting()->SetColor(changecolor);
        ((Image*)(screens["03Tools"]->GetElementByName("ImageColor")))->ChangeTexture(*UiElement::textureMap["blackButton"]); },
        { 200, 20 }, {30,30}));
    tools->AddElement(new Button(*UiElement::textureMap["redButton"], 
        [changecolor = sf::Color(255, 0, 0), this](){ this->GetMeeting()->SetColor(changecolor);
        ((Image*)(screens["03Tools"]->GetElementByName("ImageColor")))->ChangeTexture(*UiElement::textureMap["redButton"]); },
        { 200, 70 }, { 30,30 }));
    tools->AddElement(new Button(*UiElement::textureMap["blueButton"], 
        [changecolor = sf::Color(0, 0, 255), this](){ this->GetMeeting()->SetColor(changecolor);
        ((Image*)(screens["03Tools"]->GetElementByName("ImageColor")))->ChangeTexture(*UiElement::textureMap["blueButton"]); },
        { 250, 20 }, { 30,30 }));
    tools->AddElement(new Button(*UiElement::textureMap["greenButton"], 
        [changecolor = sf::Color(0, 255, 0), this](){ this->GetMeeting()->SetColor(changecolor);
        ((Image*)(screens["03Tools"]->GetElementByName("ImageColor")))->ChangeTexture(*UiElement::textureMap["greenButton"]); },
        { 250, 70 }, { 30,30 }));
    tools->AddElement(new Button(*UiElement::textureMap["yellowButton"], 
        [changecolor = sf::Color(255, 255, 0), this](){ this->GetMeeting()->SetColor(changecolor);
        ((Image*)(screens["03Tools"]->GetElementByName("ImageColor")))->ChangeTexture(*UiElement::textureMap["yellowButton"]); },
        { 300, 20 }, { 30,30 }));
    tools->AddElement(new Button(*UiElement::textureMap["orangeButton"], 
        [changecolor = sf::Color(255, 127, 0), this](){ this->GetMeeting()->SetColor(changecolor);
    ((Image*)(screens["03Tools"]->GetElementByName("ImageColor")))->ChangeTexture(*UiElement::textureMap["orangeButton"]); },
        { 300, 70 }, { 30,30 }));
    tools->AddElement(new Button(*UiElement::textureMap["lightBlueButton"], 
        [changecolor = sf::Color(0, 255, 255), this](){ this->GetMeeting()->SetColor(changecolor);
    ((Image*)(screens["03Tools"]->GetElementByName("ImageColor")))->ChangeTexture(*UiElement::textureMap["lightBlueButton"]); },
        { 350, 20 }, { 30,30 }));
    tools->AddElement(new Button(*UiElement::textureMap["pinkButton"], 
        [changecolor = sf::Color(255, 0, 255), this](){ this->GetMeeting()->SetColor(changecolor);
    ((Image*)(screens["03Tools"]->GetElementByName("ImageColor")))->ChangeTexture(*UiElement::textureMap["pinkButton"]); },
        { 350, 70 }, { 30,30 }));
    tools->AddElement(new Button(*UiElement::textureMap["purpleButton"],
        [changecolor = sf::Color(127, 0, 255), this](){ this->GetMeeting()->SetColor(changecolor);
        ((Image*)(screens["03Tools"]->GetElementByName("ImageColor")))->ChangeTexture(*UiElement::textureMap["purpleButton"]); },
        { 400, 20 }, { 30,30 }));
    tools->AddElement(new Button(*UiElement::textureMap["eraserButton"],
        [changecolor = sf::Color(255, 255, 255), this](){ this->GetMeeting()->SetColor(changecolor);
        ((Image*)(screens["03Tools"]->GetElementByName("ImageColor")))->ChangeTexture(*UiElement::textureMap["eraserButton"]); },
        { 400, 70 }, { 30,30 }));

    auto ImageColor = new Image(*UiElement::textureMap["blackButton"],
        { 120, 35 }, { 50,50 });
    ImageColor->SetName("ImageColor");
    tools->AddElement(ImageColor);

    tools->AddElement(new Button(*UiElement::textureMap["leaveButton"],
        [this]() {cl.Disconnect(false); },
        { 10, 10 }, { 50,50 }));

    auto muteToggle = new ToggleButton(*UiElement::textureMap["NotMuted"], *UiElement::textureMap["Muted"], [this](bool state) {this->GetMeeting()->ChangeRecorderState(!state); },
        { 800, 35 }, { 50, 50 });
    muteToggle->SetName("MuteButton");
    tools->AddElement(muteToggle);

    tools->AddElement(new ScrollBar([this](float val) { this->GetMeeting()->SetRadius(val); },
        {1, 10}, { 450, 49 }, { 1476 / 5.0,110 / 5.0 }));

    meeting = new Meeting(Draw, participents, tools, subScreenEq, &cl);
}

void UiManager::Start()
{
    //string ip = "127.0.0.1";
    //cl.Connect(ip, 16016);

    
    window = new sf::RenderWindow(sf::VideoMode(BASE_SCREEN_W, BASE_SCREEN_H), "Math Link");
    this->lastWindowSize = window->getSize();
    window->setFramerateLimit(60);
    Init();
    auto windowIcon = UiElement::textureMap["windowIcon"]->copyToImage();
    window->setIcon(windowIcon.getSize().x, windowIcon.getSize().y, windowIcon.getPixelsPtr());
    float m = 0;
    sf::Clock c;
    c.restart();
    while (window->isOpen())
    {
        Update();
    }
}

void UiManager::Update()
{
    UpdateSfmlEvents();
    PacketExecutor::GetInstance()->Udate();
    window->clear(Backround_Color);
    std::map<std::string, Screen*>::iterator screenOnTop = screens.end();
    auto click = sf::Mouse::getPosition(*this->window);
    for (auto it = screens.begin(); it != screens.end(); it++) {
        if (!(it->second->IsActive()))
            continue;
        auto viewPort = it->second->GetPort();
        sf::FloatRect tr = {viewPort.left * this->window->getSize().x, viewPort.top * this->window->getSize().y, 
            viewPort.width* this->window->getSize().x, viewPort.height* this->window->getSize().y };
        if (tr.contains(click.x, click.y)) {
            screenOnTop = it;
        }
    }
    for (auto it = screens.begin(); it != screens.end(); it++) {
        UiElement::enableMouseClick = (it == screenOnTop);
        it->second->UdpateScreen();
        it->second->DrawScreen();
    }
    window->display();
}

Meeting* UiManager::GetMeeting()
{
    return meeting;
}

std::map<std::string, Screen*>& UiManager::GetScreens()
{
    return screens;
}
