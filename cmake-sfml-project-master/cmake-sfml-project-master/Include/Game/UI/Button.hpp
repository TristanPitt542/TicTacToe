#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "UIElement.hpp"
#include "Text.hpp"

enum BUTTON_STATE{ 
	IDLE = 0,
	HOVER = 1,
	PRESSED = 2 
};

class Button : public UIElement
{
private:
	// Colors
	sf::RectangleShape shape;
	sf::Color colorIdle;
	sf::Color colorHover;
	sf::Color colorPressed;
	
	// Text
	Text buttonText;
	
	// Sound
	sf::SoundBuffer buffer;
	sf::Sound sound;
	
	// Button State(IDLE ,HOVER ,PRESSED)
	int buttonState;

public:
	Button(sf::Vector2f position, std::string buttonText, const sf::Font& font, sf::Vector2f buttonSize = sf::Vector2f(200, 75));
	~Button();

	bool IsClicked() const;
	bool IsHover() const;

	virtual void Update(const sf::Event& event, const sf::RenderWindow& window) override;

	void ResetClickState();

	void Draw(sf::RenderTarget& target) const override;
};