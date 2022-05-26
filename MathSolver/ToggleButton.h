#include "UiElement.h"
#include <functional>
class ToggleButton :
    public UiElement
{
public:
    /// <summary>
    /// constructor
    /// </summary>
    /// <param name="texture1"></param>
    /// <param name="texture2"></param>
    /// <param name="onClick"></param>
    /// <param name="pos"></param>
    /// <param name="size"></param>
    ToggleButton(sf::Texture& texture1, sf::Texture& texture2, std::function<void(bool)> onClick, sf::Vector2f pos = { 0,0 }, sf::Vector2f size = { 200, 50 });
    
    /// <summary>
    /// update toggle
    /// </summary>
    /// <param name="window"> the window</param>
    void Update(sf::RenderWindow& window);
    void Print(sf::RenderWindow& window);

    /// <summary>
    /// set if can press
    /// </summary>
    /// <param name="buttonMode"></param>
    void SetButtonMode(bool buttonMode);

    /// <summary>
    /// change the state
    /// </summary>
    /// <param name="state">new state</param>
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

