#include "turret.h"
#include <cmath>
#include <iostream>

Turret::Turret(sf::RenderWindow& window)
: Game(window),
  m_turretAngleDegrees(0.f),
  m_firePeriod(100),
  m_spawnPeriod(3000)
{
    if (!m_shotBuffer.loadFromFile("snd/shot1.ogg"))
    {
        std::cout << "Error loading shot1.ogg";
    }
    else
    {
        m_shotSound.setBuffer(m_shotBuffer);
    }

    if (!m_hitBuffer.loadFromFile("snd/hit1.ogg"))
    {
        std::cout << "Error loading hit1.ogg";
    }
    else
    {
        m_hitSound.setBuffer(m_hitBuffer);
    }
}

void Turret::update(float elapsed_s)
{
    fireTurret();

    updateProjectiles(elapsed_s);

    drawTurret();
    drawProjectiles();
    cleanupProjectiles();

    spawnTargets();
    drawTargets();
}

void Turret::fireSound()
{
    m_shotSound.play();
}

void Turret::hitSound()
{
    m_hitSound.play();
}

void Turret::fireTurret()
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

    if ( sf::Joystick::isButtonPressed(0, 0) && m_fireClock.getElapsedTime().asMilliseconds() > m_firePeriod)
    {
        m_fireClock.restart();

        float turretAngleRadians = (m_turretAngleDegrees+90.f)*pi/180.f;
        sf::Vector2f direction(cos(turretAngleRadians), sin(turretAngleRadians));
        m_projectiles.push_back(Projectile(m_window.getView().getCenter()+direction*70.f, direction, 500.f));
        fireSound();
    }
}

void Turret::updateProjectiles(float elapsed_s)
{
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
            hitSound();
        }
        else
        {
            it->update(elapsed_s);
        }
    }
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
    #define SQR(x) (x)*(x)
    if ( m_spawnClock.getElapsedTime().asMilliseconds() > m_spawnPeriod )
    {
        m_spawnClock.restart();

        sf::Vector2f windowSize = m_window.getView().getSize();
        sf::Vector2f center = m_window.getView().getCenter();

        int x, y;
        do
        {
            x = std::rand()% (int)windowSize.x;
            y = std::rand()% (int)windowSize.y;
        }
        while(SQR(x-center.x) + SQR(y-center.y) < SQR(200));

        m_targets.push_back(sf::Vector2f(x, y));
    }
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
