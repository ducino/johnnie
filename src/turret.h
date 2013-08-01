#include "game.h"
#include <SFML/Audio.hpp>
#include "projectile.h"
#include <list>

#define TARGET_RADIUS 15

class Turret: public Game
{
private:
    float m_turretAngleDegrees;

    std::list<Projectile> m_projectiles;
    std::list<sf::Vector2f> m_targets;

    sf::Clock m_fireClock;
    int m_firePeriod;

    sf::Clock m_spawnClock;
    int m_spawnPeriod;

    sf::SoundBuffer m_shotBuffer;
    sf::Sound m_shotSound;

    sf::SoundBuffer m_hitBuffer;
    sf::Sound m_hitSound;

public:
    Turret(sf::RenderWindow& window);

    virtual void update(float elapsed_s);

private:

    void fireTurret();

    void updateProjectiles(float elapsed_s);

    void drawTurret();

    void drawProjectiles();

    void cleanupProjectiles();

    void spawnTargets();

    void drawTargets();

    void fireSound();
    void hitSound();
};
