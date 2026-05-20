#include "Game.hpp"
#include "Board.hpp"
#include <iostream>

void Game::InitialiseWindow()
{
    window = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(sf::Vector2u(static_cast<unsigned int>(windowSize.x),
            static_cast<unsigned int>(windowSize.y))),
        WindowTitle
    );
    window->setFramerateLimit(FPSLimit);
}

void Game::Run()
{
    //Initialize Window
    InitialiseWindow();

    Board board(sf::Vector2i(3,4), windowSize);

    //Window Loop
    while (window->isOpen()) {
        while (const std::optional event = window->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window->close();
            }
        }

        window->clear();

        //draw
        board.Draw(*window);

        window->display();
    }
}

void Game::Menu()
{

    sf::RectangleShape button;
    button.setSize({ 100.f, 100.f });
    button.setOrigin({ 50.f, 50.f });
    button.setPosition({ 300.f, 300.f });
    button.setFillColor(sf::Color::White);
}

void Game::Input()
{
}