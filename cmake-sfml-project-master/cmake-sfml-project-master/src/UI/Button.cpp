#include "Button.hpp"

Button::Button(sf::Vector2f position, std::string buttonText, const sf::Font& font, sf::Vector2f buttonSize)
	: buttonText(position, buttonText, font), sound(buffer)
{
	this->colorIdle = sf::Color::White;
	this->colorHover = sf::Color::Green;
	this->colorPressed = sf::Color::Red;
	this->buttonState = IDLE;

    if (!buffer.loadFromFile("Assets/Sounds/thump.mp3"))
    {
        std::cout << "Failed to load buton sound!" << std::endl;
    }

    this->sound.setVolume(2);
	shape.setSize({ buttonSize });
	shape.setFillColor(colorIdle);
	shape.setOrigin({ shape.getSize().x / 2.f, shape.getSize().y / 2.f });
	shape.setPosition(position);
}

Button::~Button()
{
}

bool Button::IsClicked() {
    if (this->buttonState == PRESSED) {
        // Degrade back to HOVER or IDLE so it doesn't fire again next frame
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
        // If it's not pressed, it's at least hovering
        if (this->buttonState != PRESSED) {
            this->buttonState = HOVER;
            this->shape.setFillColor(colorHover);
        }

        // Handle the unique click release event
        if (event.is<sf::Event::MouseButtonReleased>())
        {
            const auto* mouseEvent = event.getIf<sf::Event::MouseButtonReleased>();
            if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left)
            {
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

// Reset helper so the button doesn't stay permanently locked to PRESSED
void Button::ResetClickState() {
    this->buttonState = IDLE;
    this->shape.setFillColor(colorIdle);
}

void Button::Draw(sf::RenderTarget& target) const
{
	target.draw(this->shape);
	buttonText.Draw(target);
}