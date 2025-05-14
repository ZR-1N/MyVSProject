#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>

class Resources
{
public:
	static std::unordered_map<std::string, sf::Texture> textures;
	static std::unordered_map<std::string, sf::SoundBuffer> sounds;
};
