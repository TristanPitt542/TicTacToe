/** @file UIElement.hpp */

#pragma once
#include <iostream>
#include <SFML/graphics.hpp>

/** Common interface for drawable and updateable UI elements. */
class UIElement
{
private:

public:
	UIElement() = default;
	~UIElement() = default;

	/** Updates the element from the current event. */
	virtual void Update(const sf::Event& event, const sf::RenderWindow& window) = 0;

	/** Draws the element. */
	virtual void Draw(sf::RenderTarget& target) const = 0;
};