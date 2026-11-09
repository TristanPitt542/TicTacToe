/** @file Button.cpp */

#include "Button.hpp"

Button::Button(sf::Vector2f position, std::string buttonText, const sf::Font& font, sf::Vector2f buttonSize)
	: buttonText(position, buttonText, font), sound(buffer)
{
	this->colorIdle = sf::Color::White;
	this->colorHover = sf::Color::Green;
	this->colorPressed = sf::Color::Red;
	this->buttonState = IDLE;

	soundLoaded = buffer.loadFromFile("Assets/Sounds/thump.mp3")
		&& buffer.getChannelCount() > 0
		&& buffer.getSampleCount() > 0;
	if (!soundLoaded)
	{
		std::cout << "Failed to load button sound!" << std::endl;
	}

	this->sound.setVolume(2);

	shape.setSize({ buttonSize });
	shape.setFillColor(colorIdle);
    // Center the shape so position refers to its midpoint.
	shape.setOrigin({ shape.getSize().x / 2.f, shape.getSize().y / 2.f });
	shape.setPosition(position);
}

Button::~Button()
{
	sound.stop();
}

bool Button::IsClicked() {
    if (this->buttonState == PRESSED) {
        this->buttonState = HOVER;
        this->shape.setFillColor(colorHover);
        return true;
    }
    return false;
}

bool Button::IsHover() const
{
    return this->buttonState == HOVER;
}

void Button::Update(const sf::Event& event, const sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (this->shape.getGlobalBounds().contains(mousePos))
    {
        if (this->buttonState != PRESSED) {
            this->buttonState = HOVER;
            this->shape.setFillColor(colorHover);
        }

        if (event.is<sf::Event::MouseButtonReleased>())
        {
            const auto* mouseEvent = event.getIf<sf::Event::MouseButtonReleased>();
            if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left)
            {
                if (soundLoaded)
                    sound.play();
                this->buttonState = PRESSED;
                this->shape.setFillColor(colorPressed);
            }
        }
    }
    else
    {
        this->buttonState = IDLE;
        this->shape.setFillColor(colorIdle);
    }
}

// Clear the one-frame pressed state.
void Button::ResetClickState() {
    this->buttonState = IDLE;
    this->shape.setFillColor(colorIdle);
}

void Button::Draw(sf::RenderTarget& target) const
{
	target.draw(this->shape);
	buttonText.Draw(target);
}