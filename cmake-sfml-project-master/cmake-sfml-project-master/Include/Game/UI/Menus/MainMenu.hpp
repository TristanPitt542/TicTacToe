#pragma once

#include "MenuScreen.hpp"
#include "Button.hpp"
#include "Text.hpp"

class MainMenu : public MenuScreen {
private:
	/// Pointer to the single player button for click checking
	Button* m_singlePlayerButton = nullptr;
	/// Pointer to the exit button for click checking
	Button* m_ExitButton = nullptr;

public:
	/** Creates the title and main menu buttons. */
	void Init(const sf::Vector2u& windowSize, const sf::Font& font) override {
		m_elements.clear();
		float centerX = windowSize.x / 2.f;

		auto title = std::make_unique<Text>(sf::Vector2f(centerX, 100.f), "MAIN MENU", font, 60.0f);

		auto singlePlayer = std::make_unique<Button>(sf::Vector2f(centerX, 250.f), "Singleplayer", font);

		auto exit = std::make_unique<Button>(sf::Vector2f(centerX, 450.f), "Exit", font);

		// Keep non-owning pointers for click checks; ownership remains in m_elements.
		m_singlePlayerButton = singlePlayer.get();
		m_ExitButton = exit.get();

		m_elements.push_back(std::move(title));
		m_elements.push_back(std::move(singlePlayer));
		m_elements.push_back(std::move(exit));
	}

	virtual void Update(const sf::Event& event, const sf::RenderWindow& window) {
		for (auto& element : m_elements) {
			element->Update(event, window);
		}
	}

	/** Clears button state after menu input has been handled. */
	void ResetClickState() {
		for (Button* button : { m_singlePlayerButton, m_ExitButton }) {
			if (button) {
				button->ResetClickState();
			}
		}
	}

	/** Returns whether Singleplayer was clicked. */
	bool IsSinglePlayerPressed() const { return m_singlePlayerButton && m_singlePlayerButton->IsClicked(); }

	/** Alias for IsSinglePlayerPressed. */
	bool IsPlayPressed() const { return m_singlePlayerButton && m_singlePlayerButton->IsClicked(); }

	/** Returns whether Exit was clicked. */
	bool IsExitPressed() const { return m_ExitButton && m_ExitButton->IsClicked(); }
};