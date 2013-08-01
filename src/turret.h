#include "game.h"
#include <list>

#define TARGET_RADIUS 15

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

class Turret: public Game
{
private:
    float m_turretAngleDegrees;

    std::list<Projectile> m_projectiles;
    std::list<sf::Vector2f> m_targets;

    sf::Clock m_fireClock;
    int m_firePeriod;

public:
    Turret(sf::RenderWindow& window);

    virtual void update(float elapsed_s);

private:
    void drawTurret();

    void drawProjectiles();

    void cleanupProjectiles();

    void spawnTargets();

    void drawTargets();
};
