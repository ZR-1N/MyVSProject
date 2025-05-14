#pragma once
#include <SFML/Graphics.hpp>

class Renderer
{
public:
	Renderer(sf::RenderTarget& target);

	void Draw(const sf::Texture& texture, const sf::Vector2f& position,
		const sf::Vector2f& size, float angle = 0.0f);//可以调整渲染位置，大小，角度

	sf::RenderTarget& target;
	sf::Sprite sprite{};
};
