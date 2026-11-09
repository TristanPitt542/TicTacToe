/** @file Text.hpp */

#pragma once
#include "UIElement.hpp"

/** UI element that displays centered text. */
class Text : public UIElement
{
private:
	// Appearance.
	/// Color used to render the text
	sf::Color color;

	// SFML text object.
	/// SFML text object used for rendering
	sf::Text text;

public:
	/** Creates text centered at position. */
	Text(sf::Vector2f position, std::string text, const sf::Font& font, float textSize = 30.0f, sf::Color color = sf::Color::White);

	virtual ~Text();

	/** Replaces the displayed string. */
	void SetText(std::string text);

	/** Text has no event-driven state to update. */
	void Update(const sf::Event& event, const sf::RenderWindow& window) override;

	/** Draws the text. */
	void Draw(sf::RenderTarget& target) const override;
};