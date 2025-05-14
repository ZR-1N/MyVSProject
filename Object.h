#pragma once
#include "Renderer.h"

class Object
{
public:
	virtual void Begin() {}
	virtual void Update(float deltaTime) {}
	virtual void Render(Renderer& renderer) {}

	std::string tag{};

	sf::Vector2f position{};
	float angle{};
};
