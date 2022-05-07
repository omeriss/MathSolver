#include "UiElement.h"
#include <functional>
class ToggleButton :
    public UiElement
{
public:
    ToggleButton(sf::Texture& texture1, sf::Texture& texture2, std::function<void(bool)> onClick, sf::Vector2f pos = { 0,0 }, sf::Vector2f size = { 200, 50 });
    void Update(sf::RenderWindow& window);
    void Print(sf::RenderWindow& window);
    void SetButtonMode(bool buttonMode);
    void ChangeState(bool state);
private:
    bool state;
    bool isClicked;
    bool buttonMode;
    std::atomic_bool disabled;
    sf::Texture& texture1;
    sf::Texture& texture2;
    std::function<void(bool)> onClick;
};

