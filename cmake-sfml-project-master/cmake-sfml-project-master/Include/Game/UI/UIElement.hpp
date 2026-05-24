#pragma once
#include <iostream>
#include <SFML/graphics.hpp>

class UIElement
{
private:

public:
	UIElement() = default;
	~UIElement() = default;

	virtual void Update(const sf::Event& event, const sf::RenderWindow& window) = 0;
	virtual void Draw(sf::RenderTarget& target) const = 0;
};