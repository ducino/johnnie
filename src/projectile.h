#include <SFML/Graphics.hpp>

class Projectile
{
private:
    sf::Vector2f m_position;
    sf::Vector2f m_direction;
    float m_speed;

public:
    Projectile(sf::Vector2f position, sf::Vector2f direction, float speed);

    void update(float elapsed_s);

    void draw(sf::RenderWindow& window);

    float distSquared(sf::Vector2f position);
};
