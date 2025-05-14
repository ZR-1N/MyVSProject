#pragma once
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "Object.h"
#include "Camera.h"
#include <memory>
#include <vector>

// 游戏状态
enum class GameState { Menu, Playing, Paused, GameOver };

class Menu {
public:
    Menu();
    void Update(sf::RenderWindow& window); // 更新菜单状态，处理鼠标点击
    void Render(sf::RenderWindow& window, Camera& camera); // 渲染菜单
    void UpdateBackgroundScale(sf::RenderWindow& window); // 动态调整背景图缩放

    bool ShouldStart() const { return shouldStart; }
    std::vector<sf::Texture> backgroundFrames; 
    sf::Sprite backgroundSprite;
    int currentFrame = 0; // 当前帧
    sf::Clock animationClock; // 动画计时器
    float frameDuration = 0.1f; // 每帧持续时间

    sf::Font font; 
    sf::Text startButton; // 开始游戏按钮
    sf::Text restartText; // 角色死亡时的重启文本
    bool shouldStart = false; // 标记是否点击开始
};

extern Camera camera;
extern bool paused;
extern GameState state;
extern sf::RenderWindow* windowPtr;
extern std::vector<Object*> objects;
extern std::unique_ptr<Menu> menu;

void Begin();
void Update(float deltaTime);
void Render(Renderer& renderer);
void RenderUI(Renderer& renderer);
void DeleteObject(Object* object);