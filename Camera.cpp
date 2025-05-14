#include "Camera.h"

Camera::Camera(float zoomLevel)
	: zoomLevel(zoomLevel)
{
}

sf::Vector2f Camera::GetViewSize()
{
	return viewSize;
}

sf::View Camera::GetView(sf::Vector2u windowSize)
{
	float aspect = (float)windowSize.x / (float)windowSize.y;
	if (aspect < 1.0f)
		viewSize = sf::Vector2f(zoomLevel, zoomLevel / aspect);
	else
		viewSize = sf::Vector2f(zoomLevel * aspect, zoomLevel);

	return sf::View(position, viewSize);
}

sf::View Camera::GetUIView(float uiWidth) {
	float aspect = viewSize.x / viewSize.y;
	sf::Vector2f uiSize(uiWidth, uiWidth / aspect);
	sf::View uiView(sf::FloatRect(0, 0, uiWidth, uiWidth / aspect));
	uiView.setViewport(sf::FloatRect(0, 0, 1, 1)); // ¸²¸ÇÕû¸ö´°¿Ú
	return uiView;
}
