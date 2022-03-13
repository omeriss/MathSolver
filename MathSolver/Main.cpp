#define WIN32_LEAN_AND_MEAN
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <filesystem>
#include <vector>
#include <string>
#include <Windows.h>
#include "UiElement.h"
#include "TextBox.h"
#include "Screen.h"
#include "constants.h"
#include "Button.h"
#include "UiManager.h"
#include "HandlePacketFunctions.h"



#pragma region define static members
std::vector<char> UiElement::input = std::vector<char>();
static std::map<std::string, sf::Texture*>* LoadTextures(std::string dirPath) {
    std::map<std::string, sf::Texture*>* textureMap = new std::map < std::string, sf::Texture*>();
    for (const auto& file : std::filesystem::directory_iterator(dirPath)) {
        std::string filePath = file.path().string();
        std::string name = std::string(filePath.begin() + dirPath.length(), find(filePath.begin(), filePath.end(), '.'));
        sf::Texture* tempTex = new sf::Texture();
        tempTex->loadFromFile(filePath);
        textureMap->insert({ name, tempTex });
    }
    return textureMap;
}
std::map<std::string, sf::Texture*>& UiElement::textureMap = *LoadTextures(SPRITE_PATH);
UiManager* UiManager::instance = new UiManager();
PacketExecutor* PacketExecutor::instance;
#pragma endregion



int main(){

    PacketExecutor::GetInstance()->SetFunctions(CreatePacketVector);
    UiManager::instance->Start();
    return 0;
}