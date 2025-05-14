#include "Animation.h"
#include <iostream> 
#include <algorithm> 

Animation::Animation(float length, std::vector<AnimFrame> frames)
    : length(length), frames(frames) {
    // ��ʱ������֡
    std::sort(frames.begin(), frames.end(),
        [](const AnimFrame& a, const AnimFrame& b) { return a.time < b.time; });
}

void Animation::Update(float deltaTime) {
    time += deltaTime;
    while (time > length) {
        time -= length;
        currentFrame = 0; // ѭ���ص���һ֡
    }
    // ���µ�ǰ֡
    while (currentFrame < frames.size() - 1 && time >= frames[currentFrame + 1].time) {
        currentFrame++;
    }
}

sf::Texture& Animation::GetTexture() {
    if (frames.empty()) {
        static sf::Texture emptyTexture;
        return emptyTexture;
    }
    return frames[currentFrame].texture;
}
