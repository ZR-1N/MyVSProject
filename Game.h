#pragma once
#include <SFML/Graphics.hpp>
#include "Renderer.h"
#include "Object.h"
#include "Camera.h"
#include <memory>
#include <vector>

// ��Ϸ״̬
enum class GameState { Menu, Playing, Paused, GameOver };

class Menu {
public:
    Menu();
    void Update(sf::RenderWindow& window); // ���²˵�״̬�����������
    void Render(sf::RenderWindow& window, Camera& camera); // ��Ⱦ�˵�
    void UpdateBackgroundScale(sf::RenderWindow& window); // ��̬��������ͼ����

    bool ShouldStart() const { return shouldStart; }
    std::vector<sf::Texture> backgroundFrames; 
    sf::Sprite backgroundSprite;
    int currentFrame = 0; // ��ǰ֡
    sf::Clock animationClock; // ������ʱ��
    float frameDuration = 0.1f; // ÿ֡����ʱ��

    sf::Font font; 
    sf::Text startButton; // ��ʼ��Ϸ��ť
    sf::Text restartText; // ��ɫ����ʱ�������ı�
    bool shouldStart = false; // ����Ƿ�����ʼ
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