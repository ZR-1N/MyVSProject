#pragma once

#include <SFML/Graphics.hpp>

class Camera
{
public:
	Camera(float zoomLevel = 5.0f);
	sf::Vector2f GetViewSize();
	sf::View GetView(sf::Vector2u windowSize);
	sf::View GetUIView(float uiWidth = 800.0f); // 默认宽度可调

	float zoomLevel;
	sf::Vector2f position;

private:
	sf::Vector2f viewSize{};
};
