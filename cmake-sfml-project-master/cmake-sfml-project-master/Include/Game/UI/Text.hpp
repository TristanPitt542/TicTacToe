#pragma once
#include "UIElement.hpp"

class Text : public UIElement
{
private:
	// Colors
	sf::Color color;

	// Text
	sf::Text text;

public:
	Text(sf::Vector2f position, std::string text, const sf::Font& font, float textSize = 30.0f, sf::Color color = sf::Color::White);
	virtual ~Text();

	void SetText(std::string text);
	void Update(const sf::Event& event, const sf::RenderWindow& window) override;
	void Draw(sf::RenderTarget& target) const override;
};