#include "turret.h"
#include <cmath>
#include <iostream>

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

Turret::Turret(sf::RenderWindow& window)
: Game(window),
  m_turretAngleDegrees(0.f),
  m_firePeriod(100)
{
    m_targets.push_back(sf::Vector2f(100, 100));
    m_targets.push_back(sf::Vector2f(200, 100));
    m_targets.push_back(sf::Vector2f(200, 500));
    m_targets.push_back(sf::Vector2f(600, 500));
}

void Turret::update(float elapsed_s)
{
    float pi = std::atan(1)*4;//TODO Cache

    float x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
    float y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

    float joystickThreshold = 10.f;
    //float angleDiff = 0.;
    //float angleSpeed = 1.f;
    //float elapsed_s = ((float)elapsed_us)/1000000.f;

    if(abs(x) > joystickThreshold || abs(y) > joystickThreshold)
    {
        float angle = std::atan2(y, x);
        //angleDiff = (angle*180./pi - 90.f) - m_turretAngleDegrees;
        m_turretAngleDegrees = (angle*180./pi - 90.f);
    }

    //m_turretAngleDegrees += angleDiff*angleSpeed*elapsed_s;

    if ( sf::Joystick::isButtonPressed(0, 0) && m_fireClock.getElapsedTime().asMilliseconds() > m_firePeriod)
    {
        m_fireClock.restart();

        float turretAngleRadians = (m_turretAngleDegrees+90.f)*pi/180.f;
        sf::Vector2f direction(cos(turretAngleRadians), sin(turretAngleRadians));
        m_projectiles.push_back(Projectile(m_window.getView().getCenter()+direction*70.f, direction, 500.f));
    }

    for (std::list<Projectile>::iterator it = m_projectiles.begin(); it != m_projectiles.end(); ++it)
    {
        bool hit = false;
        for (std::list<sf::Vector2f>::iterator itTarget = m_targets.begin(); itTarget != m_targets.end(); ++itTarget)
        {
            if(it->distSquared(*itTarget) < TARGET_RADIUS*TARGET_RADIUS)
            {
                itTarget = m_targets.erase(itTarget);
                hit = true;
            }
        }

        if(hit)
        {
            it = m_projectiles.erase(it);
        }
        else
        {
            it->update(elapsed_s);
        }
    }

    drawTurret();
    drawProjectiles();
    cleanupProjectiles();

    spawnTargets();
    drawTargets();
}

void Turret::drawTurret()
{
    float radius = 50.f;
    sf::Vector2f radiusOffset(radius, radius);

    sf::Vector2f center = m_window.getView().getCenter();

    sf::CircleShape shape(radius);
    shape.setFillColor(sf::Color::Green);

    shape.setOrigin(radiusOffset);
    shape.setPosition(center);
    shape.rotate(m_turretAngleDegrees);

    sf::RectangleShape shapeCannon(sf::Vector2f(10, radius+20));
    shapeCannon.setFillColor(sf::Color::Blue);
    shapeCannon.setOrigin(5, 10);
    shapeCannon.setPosition(center);
    shapeCannon.rotate(m_turretAngleDegrees);

    m_window.draw(shape);
    m_window.draw(shapeCannon);
}

void Turret::drawProjectiles()
{
     for (std::list<Projectile>::iterator it = m_projectiles.begin(); it != m_projectiles.end(); ++it)
     {
         it->draw(m_window);
     }
}

void Turret::cleanupProjectiles()
{
    sf::Vector2f center = m_window.getView().getCenter();
    for (std::list<Projectile>::iterator it = m_projectiles.begin(); it != m_projectiles.end(); ++it)
    {
        float distSquared = it->distSquared(center);

        if (distSquared > 2000*2000)
        {
            it = m_projectiles.erase(it);
            std::cout << "Erase" << std::endl;
        }
    }
}

void Turret::spawnTargets()
{

}

void Turret::drawTargets()
{
    for (std::list<sf::Vector2f>::iterator itTarget = m_targets.begin(); itTarget != m_targets.end(); ++itTarget)
    {
        float radius = TARGET_RADIUS;
        sf::CircleShape shape(radius);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(sf::Vector2f(radius, radius));
        shape.setPosition(*itTarget);

        m_window.draw(shape);
    }
}
