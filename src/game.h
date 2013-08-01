#include <SFML/Graphics.hpp>

class Game
{
protected:
    sf::RenderWindow& m_window;

public:
    Game(sf::RenderWindow& window);

    virtual void update(float elapsed_s) =0;
};
