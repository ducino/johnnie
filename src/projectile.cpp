#include "projectile.h"

Projectile::Projectile(sf::Vector2f position, sf::Vector2f direction, float speed)
: m_position(position), m_direction(direction), m_speed(speed)
{

}

void Projectile::update(float elapsed_s)
{
    m_position += m_direction*m_speed*elapsed_s;
}

void Projectile::draw(sf::RenderWindow& window)
{
    float radius = 5.f;
    sf::CircleShape shape(radius);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(sf::Vector2f(radius, radius));
    shape.setPosition(m_position);

    window.draw(shape);
}

float Projectile::distSquared(sf::Vector2f position)
{
    sf::Vector2f d = m_position - position;

    return d.x*d.x + d.y*d.y;
}
