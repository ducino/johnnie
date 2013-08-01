#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "turret.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Johnnie v0.0");

    std::auto_ptr<Game> game(new Turret(window));

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::JoystickButtonPressed)
            {
                std::cout << "joystick button pressed!" << std::endl;
                std::cout << "joystick id: " << event.joystickButton.joystickId << std::endl;
                std::cout << "button: " << event.joystickButton.button << std::endl;
            }
        }
        //std::cout << sf::Joystick::getAxisPosition(0, sf::Joystick::X) << ", " << sf::Joystick::getAxisPosition(0, sf::Joystick::Y) << std::endl;
        window.clear();
        sf::Time elapsed = clock.restart();
        //std::cout << elapsed.asMicroseconds() << std::endl;
        float elapsed_s = ((float)elapsed.asMicroseconds())/1000000.f;
        game->update(elapsed_s);
        window.display();
    }

    return 0;
}
