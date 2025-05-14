#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

struct AnimFrame {
    AnimFrame(float time = 0.0f, sf::Texture texture = sf::Texture())
        : time(time), texture(texture) {
    }
    float time;
    sf::Texture texture;
};

class Animation {
public:
    Animation(float length = 0.0f, std::vector<AnimFrame> frames = {});
    void Update(float deltaTime);
    sf::Texture& GetTexture();
    void Reset() 
    { 
        time = 0.0f; 
        currentFrame = 0; 
    }
private:
    float time = 0.0f;
    float length;
    std::vector<AnimFrame> frames;
    size_t currentFrame = 0;
};
