#include "Button.hpp"

Button::Button(sf::Vector2f position)
{
	this->colorIdle = sf::Color::White;
	this->colorHover = sf::Color::Green;
	this->colorPressed = sf::Color::Red;

	//if (this->texIdle.loadFromFile("")
	//	&& this->texHover.loadFromFile("")
	//	&& this->texPressed.loadFromFile(""))
	//{

	//	this->shape.setTexture(this->texIdle);
	//}
	//else
	//{
	//	//Failed to load texture
	//}
	this->buttonState = IDLE;

	shape.setSize({ 200,75 });
	shape.setFillColor(colorIdle);

	// Set origin to center
	shape.setOrigin({ shape.getSize().x / 2.f, shape.getSize().y / 2.f});

	// Now you can just set the position to the mouse directly
	shape.setPosition(position);
}

Button::~Button()
{
}

const bool& Button::IsClicked() const
{
	return this->buttonState == PRESSED;
}

const bool& Button::IsHover() const
{
	return this->buttonState == HOVER;
}

void Button::Update(sf::RenderWindow& window)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	if (this->shape.getGlobalBounds().contains(mousePos))
	{
		this->buttonState = HOVER;
		this->shape.setFillColor(colorHover);
		//this->shape.setTexture(this->texHover);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			this->buttonState = PRESSED;
			this->shape.setFillColor(colorPressed);
			//this->shape.setTexture(this->texPressed);
		}
	}
	else
	{
		this->buttonState = IDLE;
		//this->shape.setTexture(this->texIdle);
	}
}

void Button::RenderButton(sf::RenderTarget& target)
{
	target.draw(this->shape);
}