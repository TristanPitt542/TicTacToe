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

bool Button::IsClicked() const
{
    return this->buttonState == PRESSED;
}

bool Button::IsHover() const
{
    return this->buttonState == HOVER;
}

void Button::Update(const sf::Event& event, const sf::RenderWindow& window)
{
    // Get the current mouse position in world space
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // Check if the mouse is hovering over the button bounds
    if (this->shape.getGlobalBounds().contains(mousePos))
    {
        // If it was clicked on a previous frame, don't instantly downgrade it to hover
        if (this->buttonState != PRESSED) {
            this->buttonState = HOVER;
            this->shape.setFillColor(colorHover);
        }

        // Check if the user just LET GO of the left mouse button while hovering inside
        if (event.is<sf::Event::MouseButtonReleased>())
        {
            const auto* mouseEvent = event.getIf<sf::Event::MouseButtonReleased>();
            if (mouseEvent && mouseEvent->button == sf::Mouse::Button::Left)
            {
                // Play Sound
                sound.play();

                this->buttonState = PRESSED;
                this->shape.setFillColor(colorPressed);
            }
        }
    }
    else
    {
        // If the mouse left the button boundaries, reset completely
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