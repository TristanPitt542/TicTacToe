/** @file Button.hpp */

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "UIElement.hpp"
#include "Text.hpp"

/**
 * @enum BUTTON_STATE
 * @brief Visual state of a button for rendering
 */
enum BUTTON_STATE { 
	IDLE = 0,      ///< Button is not interacting with mouse
	HOVER = 1,     ///< Mouse is hovering over the button
	PRESSED = 2    ///< Button was just clicked
};

/** Clickable menu button with hover state and optional sound feedback. */
class Button : public UIElement
{
private:
	// Appearance.
	/// Rectangle shape for the button background
	sf::RectangleShape shape;
	/// Color when button is idle (not hovered)
	sf::Color colorIdle;
	/// Color when mouse is hovering over button
	sf::Color colorHover;
	/// Color when button is pressed/clicked
	sf::Color colorPressed;

	// Label.
	/// Text displayed on the button
	Text buttonText;

	// Audio feedback.
	/// Sound buffer for click sound effect
	sf::SoundBuffer buffer;
	/// Sound object for playing click sound
	sf::Sound sound;
	/// Flag indicating if click sound was successfully loaded
	bool soundLoaded = false;

	// Current visual state.
	/// Current visual state (IDLE, HOVER, or PRESSED)
	int buttonState;

public:
	/** Creates a button centered at position. */
	Button(sf::Vector2f position, std::string buttonText, const sf::Font& font, sf::Vector2f buttonSize = sf::Vector2f(200, 75));

	~Button();

	/** Returns whether the button has been clicked. */
	bool IsClicked();

	/** Returns whether the cursor is over the button. */
	bool IsHover() const;

	/** Updates hover and pressed state from an event. */
	virtual void Update(const sf::Event& event, const sf::RenderWindow& window) override;

	/** Clears the pressed state after the click has been handled. */
	void ResetClickState();

	/** Draws the button and its label. */
	void Draw(sf::RenderTarget& target) const override;
};