#include "Text.hpp"

Text::Text(sf::Vector2f position, std::string text, const sf::Font& font, float textSize,sf::Color color)
    : text(font, text, textSize)
{
    this->color = color;
    this->text.setFillColor(sf::Color::Black);

    sf::FloatRect textBounds = this->text.getLocalBounds();
    this->text.setOrigin({
        textBounds.position.x + (textBounds.size.x / 2.f),
        textBounds.position.y + (textBounds.size.y / 2.f)
        });

    this->text.setPosition(position);
}

Text::~Text()
{
}

void Text::SetText(std::string text)
{
    this->text.setString(text);
}

void Text::Update(const sf::Event& event, const sf::RenderWindow& window)
{
    text.setFillColor(color);
}

void Text::Draw(sf::RenderTarget& target) const
{
	target.draw(this->text);
}