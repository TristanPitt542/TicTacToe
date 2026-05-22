#pragma once

#include <SFML/Graphics.hpp>

enum BUTTON_STATE{ 
	IDLE = 0,
	HOVER = 1,
	PRESSED = 2 
};

class Button
{
private:
	sf::RectangleShape shape;
	sf::Color colorIdle;
	sf::Color colorHover;
	sf::Color colorPressed;
	//sf::Texture texIdle;
	//sf::Texture texHover;
	//sf::Texture texPressed;	
	int buttonState;

public:
	Button(sf::Vector2f position);
	~Button();

	const bool& IsClicked() const;
	const bool& IsHover() const;

	void Update(sf::RenderWindow& window);

	void RenderButton(sf::RenderTarget& target);
};