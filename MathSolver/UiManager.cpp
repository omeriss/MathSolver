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

void UiManager::DoLogIn()
{
    std::string email = ((TextBox*)screens["01logIn"]->GetElementByName("email"))->GetText();
    std::string password = ((TextBox*)screens["01logIn"]->GetElementByName("password"))->GetText();
    pythonContext.post([this, email, password]() {
        ((TextBox*)screens["01logIn"]->GetElementByName("error"))->SetString("");
        auto log_in = fireBaseModule.attr("log_in");
        std::string& userName = meeting->GetUsername();
        userName = pybind11::cast<std::string>(log_in(email.c_str(), password.c_str()));
        if (userName == "") {
            ((TextBox*)screens["01logIn"]->GetElementByName("error"))->SetString("Wrong email or password");
        }
        else {
            screens["01logIn"]->SetActive(false); screens["00main"]->SetActive(true);
        }
    });
}

void UiManager::DoSignUp()
{
    std::string tempUserName = ((TextBox*)screens["01signUp"]->GetElementByName("username"))->GetText();
    std::string email = ((TextBox*)screens["01signUp"]->GetElementByName("email"))->GetText();
    std::string password = ((TextBox*)screens["01signUp"]->GetElementByName("password"))->GetText();
    std::string confirmPassword = ((TextBox*)screens["01signUp"]->GetElementByName("confirmPassword"))->GetText();
    pythonContext.post([this, tempUserName, email, password, confirmPassword]() {
        ((TextBox*)screens["01signUp"]->GetElementByName("error"))->SetString("");
        auto sign_up = fireBaseModule.attr("sign_up");
        std::string errorString = pybind11::cast<std::string>(sign_up(tempUserName.c_str(), email.c_str(), password.c_str(), confirmPassword.c_str()));
        if (errorString == "") {
            std::string& userName = meeting->GetUsername();
            userName = tempUserName;
            screens["01signUp"]->SetActive(false); screens["00main"]->SetActive(true);
        }
        else {
            ((TextBox*)screens["01signUp"]->GetElementByName("error"))->SetString(errorString);
        }
        });
}

void UiManager::InitScreens()
{
    Screen* EnterScreen = new Screen(BASE_SCREEN_W, BASE_SCREEN_H, window, AlignCenter);
    screens.insert({ "00Enter", EnterScreen });

    Screen* LogInScreen = new Screen(BASE_SCREEN_W, BASE_SCREEN_H, window, AlignCenter);
    LogInScreen->SetActive(false);
    screens.insert({ "01logIn", LogInScreen });

    Screen* SignUpScreen = new Screen(BASE_SCREEN_W, BASE_SCREEN_H, window, AlignCenter);
    SignUpScreen->SetActive(false);
    screens.insert({ "01signUp", SignUpScreen });

    Screen* MainScreen = new Screen(BASE_SCREEN_W, BASE_SCREEN_H, window,AlignCenter);
    MainScreen->SetActive(false);
    screens.insert({ "00main", MainScreen });

    Screen* subScreenEq = new Screen(BASE_SCREEN_W,+ BASE_SCREEN_H, { 0.3, 0.3,0.5,0.5 }, window, SubScreen);
    subScreenEq->SetActive(false);
    screens.insert({ "09SubWindow", subScreenEq });

    //Screen* Open = new Screen(BASE_SCREEN_W, BASE_SCREEN_H, {0,0,1,1}, window);
    //Open->SetActive(false);
    //screens.insert({ "01open", Open });

    Screen* Join = new Screen(BASE_SCREEN_W, BASE_SCREEN_H, { 0,0,1,1 }, window);
    screens.insert({ "01join", Join });
    Join->SetActive(false);

    Screen* Draw = new Screen(BASE_DRAW_SCREEN_W, BASE_DRAW_SCREEN_H, { 0,1/6.0,5/6.0, 5 / 6.0 }, window, ScaleByWith);
    Draw->SetActive(false);
    screens.insert({ "03Draw", Draw });

    Screen* tools = new Screen(BASE_DRAW_SCREEN_W*5.0/6.0, BASE_DRAW_SCREEN_H/6.0, { 0,0.0,5 / 6.0, 1 / 6.0 }, window, AlignLeft);
    tools->SetActive(false);
    screens.insert({ "03Tools", tools });

    Screen* participents = new Screen(BASE_DRAW_SCREEN_W * 1.0 / 6, BASE_DRAW_SCREEN_H, { 5 / 6.0,0,1 / 6.0, 1 }, window, ScaleByWith);
    participents->SetActive(false);
    screens.insert({ "03Participents", participents });

    Screen* settingsScreen = new Screen(BASE_SCREEN_W, BASE_SCREEN_H, window, AlignCenter);
    settingsScreen->SetActive(false);
    screens.insert({ "10settings", settingsScreen });

    // enter screen
    Image* mathLinkImg = new Image(*UiElement::textureMap["MathLinkLogo"], { BASE_SCREEN_W / 2, BASE_SCREEN_H / 5 }, { 700, 350 });
    mathLinkImg->SetOrigin(Center);
    EnterScreen->AddElement(mathLinkImg);
    Button* singUpButton = new Button(*UiElement::textureMap["logInButton"],
        [this](){screens["00Enter"]->SetActive(false); screens["01logIn"]->SetActive(true); Sleep(100); },
        { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 3 }, { 200 * 4.3, 200 });
    singUpButton->SetOrigin(Center);
    EnterScreen->AddElement(singUpButton);
    Button* logInButon = new Button(*UiElement::textureMap["signUpButton"],
        [this]() {screens["00Enter"]->SetActive(false); screens["01signUp"]->SetActive(true); Sleep(100); },
        { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 4.5 }, { 200 * 4.3, 200 });
    logInButon->SetOrigin(Center);
    EnterScreen->AddElement(logInButon);

    // sign up screen
    TextBox* userNameTextBox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 4 * 0.5 }, { 800, 130 }, "Enter UserName", sf::Color::Black, 60);
    userNameTextBox->SetOrigin(Center);
    userNameTextBox->SetName("username");
    SignUpScreen->AddElement(userNameTextBox);
    TextBox* signUpEmailTextBox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 4 * 1.1 }, { 800, 130 }, "Enter Email", sf::Color::Black, 60);
    signUpEmailTextBox->SetOrigin(Center);
    signUpEmailTextBox->SetName("email");
    SignUpScreen->AddElement(signUpEmailTextBox);
    TextBox* ogPasswordTextBox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 4 * 1.7 }, { 800, 130 }, "Enter Password", sf::Color::Black, 60, Password);
    ogPasswordTextBox->SetOrigin(Center);
    ogPasswordTextBox->SetName("password");
    SignUpScreen->AddElement(ogPasswordTextBox);
    SignUpScreen->AddElement(new Button(*UiElement::textureMap["backButton"], [this]() {screens["00Enter"]->SetActive(true); screens["01logIn"]->SetActive(false); }, { 150, 50 }, { 100, 100 }));
    TextBox* confirmPasswordTextBox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 4 * 2.3 }, { 800, 130 }, "Confirm Password", sf::Color::Black, 60, Password);
    confirmPasswordTextBox->SetOrigin(Center);
    confirmPasswordTextBox->SetName("confirmPassword");
    SignUpScreen->AddElement(confirmPasswordTextBox);
    SignUpScreen->AddElement(new Button(*UiElement::textureMap["backButton"], [this]() {screens["00Enter"]->SetActive(true); screens["01signUp"]->SetActive(false); }, { 150, 50 }, { 100, 100 }));
    TextBox* signUpError = new TextBox(*UiElement::textureMap["Empty"], UiElement::baseFont, { BASE_SCREEN_W / 2.3, BASE_SCREEN_H / 4 * 2.8 }, { 800/1.6, 130/1.6 }, "", sf::Color::Red, 40, NoEdit);
    signUpError->SetOrigin(Center);
    signUpError->SetName("error");
    SignUpScreen->AddElement(signUpError);
    Button* signUpButton = new Button(*UiElement::textureMap["signUpButton"],
        [this]() {DoSignUp(); },
        { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 5 }, { 200 * 4.3, 200 });
    signUpButton->SetOrigin(Center);
    SignUpScreen->AddElement(signUpButton);

    // log in screen
    TextBox* emailTextBox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 4 * 1.2  }, { 800, 130 }, "Enter Email", sf::Color::Black, 60);
    emailTextBox->SetOrigin(Center);
    emailTextBox->SetName("email");
    LogInScreen->AddElement(emailTextBox);
    TextBox* passwordTextBox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 4 * 2 }, { 800, 130 }, "Enter Password", sf::Color::Black, 60, Password);
    passwordTextBox->SetOrigin(Center);
    passwordTextBox->SetName("password");
    LogInScreen->AddElement(passwordTextBox);
    LogInScreen->AddElement(new Button(*UiElement::textureMap["backButton"], [this]() {screens["00Enter"]->SetActive(true); screens["01logIn"]->SetActive(false); }, { 150, 50 }, { 100, 100 }));
    TextBox* logInError = new TextBox(*UiElement::textureMap["Empty"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 4 * 2.8 }, { 800/1.6, 130/1.6 }, "", sf::Color::Red, 40, NoEdit);
    logInError->SetOrigin(Center);
    logInError->SetName("error");
    LogInScreen->AddElement(logInError);
    Button* logInButton = new Button(*UiElement::textureMap["logInButton"],
        [this]() {DoLogIn(); },
        { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 5 }, { 200 * 4.3, 200 });
    logInButton->SetOrigin(Center);
    LogInScreen->AddElement(logInButton);

    // main screen
    auto openRoomFunc = [this]() mutable {this->pythonContext.post([this]() {
        auto get_create_server = fireBaseModule.attr("get_create_server");
        pybind11::tuple tup = get_create_server();
        std::string ip = pybind11::cast<string>(*tup.begin());
        int port = pybind11::cast<int>(*(tup.begin() + 1));
        if (ip != "") {
            meeting->AppendRoomCode(pybind11::cast<std::string>(*(tup.begin() + 2)), true);
            cl.Connect(ip, port, this->GetMeeting()->GetUsername());
        }
    }); };
    Button* b = new Button(*UiElement::textureMap["HostMeeting"], openRoomFunc, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 3 }, {200*4.3, 200});
    b->SetOrigin(Center);
    MainScreen->AddElement(b);
    b = new Button(*UiElement::textureMap["JoinMeeting"], 
        [this]() mutable {screens["00main"]->SetActive(false); screens["01join"]->SetActive(true); Sleep(100); return; }, 
        { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 4.2 }, { 200 * 4.3, 200 }
    );
    b->SetOrigin(Center);
    MainScreen->AddElement(b);
    b = new Button(*UiElement::textureMap["Settings"],
        [this]() mutable {screens["10settings"]->SetActive(true); Sleep(100);},
        { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 5.4 }, { 200 * 4.3, 200 }
    );
    b->SetOrigin(Center);
    MainScreen->AddElement(b);
    MainScreen->AddElement(mathLinkImg);

    // open room screen
    //TextBox* textbox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, {BASE_SCREEN_W / 2, BASE_SCREEN_H/4}, {800, 130}, "Enter Room Code", sf::Color::Black, 60);
    //textbox->SetOrigin(Center);
    //textbox->SetName("RoomCode");
    //Open->AddElement(textbox);
    //textbox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H/4*2 }, { 800, 130 }, "Enter Name", sf::Color::Black, 60);
    //textbox->SetOrigin(Center);
    //Open->AddElement(textbox);
    //auto confunc = [this]() {string ip = ((TextBox*)screens["01open"]->GetElementByName("RoomCode"))->GetText();
    //                        cl.Connect(ip, 16016); };
    //b = new Button(*UiElement::textureMap["HostMeeting"], confunc, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 5 }, { 200 * 4.3, 200 });
    //b->SetOrigin(Center);
    //Open->AddElement(b);
    //Open->AddElement(new Button(*UiElement::textureMap["backButton"], [this](){screens["00main"]->SetActive(true); screens["01open"]->SetActive(false); }, { 150, 50 }, { 100, 100 }));


    // join room screen
    //TextBox* textbox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 4 }, { 800, 130 }, "Enter IP", sf::Color::Black, 60);
    //textbox->SetOrigin(Center);
    //textbox->SetName("ip");
    //Join->AddElement(textbox);
    TextBox* textbox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 4 * 1.5 }, { 800, 130 }, "Enter Room Code", sf::Color::Black, 60);
    textbox->SetOrigin(Center);
    textbox->SetName("RoomCode");
    Join->AddElement(textbox);
    auto confunc1 = [this]() {this->pythonContext.post([this]() {
        string roomCode = ((TextBox*)screens["01join"]->GetElementByName("RoomCode"))->GetText();
        auto get_join_server = fireBaseModule.attr("get_join_server");
        std::string room_pref = roomCode.substr(0, 4);
        pybind11::tuple tup = get_join_server(room_pref.c_str());
        std::string ip = pybind11::cast<string>(*tup.begin());
        int port = pybind11::cast<int>(*(tup.begin() + 1));
        if (ip != "") {
            meeting->AppendRoomCode(room_pref, true);
            cl.Connect(ip, 16016, this->GetMeeting()->GetUsername(), roomCode.substr(4));
        }
        }); };
    b = new Button(*UiElement::textureMap["JoinMeeting"], confunc1, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 5 }, { 200 * 4.3, 200 });
    b->SetOrigin(Center);
    Join->AddElement(b);
    Join->AddElement(new Button(*UiElement::textureMap["backButton"], [this]() {screens["00main"]->SetActive(true); screens["01join"]->SetActive(false); }, { 150, 50 }, { 100, 100 }));

    
    // draw screen
    DrawScreen* dsc = new DrawScreen({ 0,0 }, { BASE_DRAW_SCREEN_W, BASE_DRAW_SCREEN_H });
    dsc->SetName("board");
    Draw->AddElement(dsc);

    // tools screen
    TextBox* roomCodeTextBox = new TextBox(*UiElement::textureMap["Empty"], UiElement::baseFont, { 850, 30 }, { 200, 500 }, "room code:\n----------------", sf::Color::Red, 16, NoEdit);;
    roomCodeTextBox->SetName("RoomCodeText");
    tools->AddElement(roomCodeTextBox);
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
        { 780, 35 }, { 50, 50 });
    muteToggle->SetName("MuteButton");
    tools->AddElement(muteToggle);

    tools->AddElement(new ScrollBar([this](float val) { this->GetMeeting()->SetRadius(val); },
        {1, 10}, { 450, 49 }, { 1476 / 5.0,110 / 5.0 }));


    // create meeting class
    meeting = new Meeting(Draw, participents, tools, subScreenEq, &cl);


    // settings screen
    auto settingBack = new Image(*UiElement::textureMap["settingsRec"], { BASE_SCREEN_W / 2.0, BASE_SCREEN_H / 2.0 }, { BASE_SCREEN_W * 0.75, BASE_SCREEN_H * 0.75 });
    settingBack->SetOrigin(Center);
    settingsScreen->AddElement(settingBack);
    auto selectedDot = new Image(*UiElement::textureMap["greenDot"], { SETTING_DOT_START_X, 0 }, { 30,30 });
    selectedDot->SetOrigin(Center);
    settingsScreen->AddElement(selectedDot);
    settingsScreen->AddElement(new Button(*UiElement::textureMap["backButton"], [this]() {screens["10settings"]->SetActive(false); }, { 340, 220 }, { 100, 100 }));
    settingsScreen->AddElement(new TextBox(*UiElement::textureMap["Empty"], UiElement::baseFont, { 700, 300 }, {100, 500 }, "Choose your mic", sf::Color::Red, 50, NoEdit));

    auto refMics = [this, settingsScreen, selectedDot]() {
        static vector<Button*> micButtons = {};
        for (auto b : micButtons) {
            settingsScreen->RemoveElement(b);
        }
        auto mics = sf::SoundRecorder::getAvailableDevices();
        float yPos = SETTINGS_TEXT_START_Y;
        for (auto micS : mics) {
            auto lastBrac = micS.begin();
            while (std::find(lastBrac + 1, micS.end(), ')') != micS.end())
                lastBrac = std::find(lastBrac + 1, micS.end(), ')');
            std::string cleanS = std::string((std::find(micS.begin(), micS.end(), '(') == micS.end()) ? micS.begin(): std::find(micS.begin(), micS.end(), '(') + 1, lastBrac);
            Button* b = new Button(*UiElement::textureMap["Empty"],
                [this, selectedDot, micS, yPos]() mutable {
                    meeting->GetRecorder().setDevice(micS);
                    selectedDot->SetPosition({ selectedDot->GetPosition().x, yPos + SETTING_DOT_OFFSET_Y });
                },
                { SETTINGS_TEXT_START_X, yPos}, { SETTING_BUTTON_W, SETTINGS_TEXT_JUMP_Y }
            );

            b->SetString(cleanS, UiElement::baseFont);
            settingsScreen->AddElement(b);
            micButtons.push_back(b);

            if (micS == meeting->GetRecorder().getDevice())
                selectedDot->SetPosition({ selectedDot->GetPosition().x, yPos + SETTING_DOT_OFFSET_Y });

            yPos += SETTINGS_TEXT_JUMP_Y;
        }
    };
    refMics();

    settingsScreen->AddElement(
        new Button(*UiElement::textureMap["redReload"],
        refMics, { 1100, 300 }, { 60, 60 }
    ));
}

void UiManager::Start()
{
    asio::io_context::work pythonWork(pythonContext);

    pybind11::gil_scoped_release release();
    std::thread* pythonThread = new std::thread([this]() {
        pybind11::gil_scoped_acquire acquire();
        pybind11::scoped_interpreter gard{};
        pythonContext.run();
     });
    pythonThread->detach();

    pythonContext.post([this]() {
        this->fireBaseModule = pybind11::module_::import("fireBaseModule");
    });

    pythonContext.post([this]() {
        auto initFireBase = fireBaseModule.attr("init_firebase");
        initFireBase();
    });
    
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
    // udate the window defult evetns
    UpdateSfmlEvents();

    // execute all the packets from the server
    PacketExecutor::GetInstance()->Udate();

    // find the screen on top
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

    // update and print all the screens
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
