#include "Game.h"
#include "Resources.h"
#include "Physics.h"
#include "Sasuke.h"
#include "Map.h"
#include <SFML/Audio.hpp>
#include <filesystem>
#include <iostream>

Map map(1.0f);
Camera camera(20.0f);
Sasuke sasuke{};
std::vector<Object*> objects{};
bool paused{};
GameState state = GameState::Menu;
sf::RenderWindow* windowPtr = nullptr;
std::unique_ptr<Menu> menu;

sf::Image mapImage{};
sf::Font font{};
sf::Text coinsText("Coins", font);
sf::RectangleShape backgroundShape(sf::Vector2f(1.0f, 1.0f));

void Restart()
{
    for (auto& obj : objects)
    {
        delete obj;
    }
    objects.clear();
    

    Physics::Init();
    sasuke = Sasuke();
    sasuke.position = map.CreateFromImage(mapImage, objects);
    sasuke.isDead = false;
    paused = false;
    state = GameState::Playing;
    sasuke.Begin();
    for (auto& object : objects)
    {
        object->Begin();
    }
    std::cout << "Game restarted" << std::endl; // 调试
}

void Begin()
{
    // 加载纹理
    for (auto& file : std::filesystem::directory_iterator("./resources/textures/"))
    {
        if (file.is_regular_file() && (file.path().extension() == ".png" || file.path().extension() == ".jpg"))
        {
            Resources::textures[file.path().filename().string()].loadFromFile(
                file.path().string());
        }
    }

    // 加载音效
    for (auto& file : std::filesystem::directory_iterator("./resources/sounds/"))
    {
        if (file.is_regular_file() && (file.path().extension() == ".ogg" || file.path().extension() == ".wav" || file.path().extension() == ".mp3"))
        {
            Resources::sounds[file.path().filename().string()].loadFromFile(
                file.path().string());
        }
    }



    if (!font.loadFromFile("./resources/font.ttf"))
    {
        throw std::runtime_error("无法加载字体");
    }

    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setOrigin(0.5f, 0.5f);

    mapImage.loadFromFile("./resources/map.png");

    menu = std::make_unique<Menu>();
    //Restart();
    state = GameState::Menu;
}

void Update(float deltaTime)
{
    if (state == GameState::Menu)
    {
        if (windowPtr)
        {
            menu->Update(*windowPtr);
            if (menu->ShouldStart())
            {
                menu->shouldStart = false; // 重置 shouldStart
                Restart();
            }
        }
    }
    else if (state == GameState::Playing)
    {
        
        if (sasuke.isDead)
        {
            menu->Update(*windowPtr);
            state = GameState::GameOver;
            return;
        }

        if (paused)
            return;

        Physics::Update(deltaTime);
        sasuke.Update(deltaTime);
        camera.position = sasuke.position;

        for (auto& object : objects)
        {
            object->Update(deltaTime);
        }
    }
    
    if (state == GameState::GameOver)
    {
        
        windowPtr->setView(windowPtr->getDefaultView());
        menu->Update(*windowPtr);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || menu->ShouldStart())
        {
            menu->shouldStart = false;
            Restart();
        }
    }
}

void Render(Renderer& renderer)
{
    renderer.Draw(Resources::textures["background.png"], sasuke.position, camera.GetViewSize());
    map.Draw(renderer);
    for (auto& object : objects)
    {
        object->Render(renderer);
    }
    sasuke.Draw(renderer);
    Physics::DebugDraw(renderer);
}

void RenderUI(Renderer& renderer)
{
    if (state == GameState::Playing)
    {
        // 正常游戏时只显示 Coins
        coinsText.setPosition(-camera.GetViewSize() / 2.0f + sf::Vector2f(20.0f, 1.0f));
        coinsText.setString("Coins: " + std::to_string(sasuke.GetCoins()));
        renderer.target.draw(coinsText);
    }
    else if (state == GameState::GameOver)
    {
        // 在 UI 视图下，用屏幕像素大小构造遮罩
        sf::Vector2u sz = windowPtr->getSize();
        sf::RectangleShape overlay(sf::Vector2f((float)sz.x, (float)sz.y));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        overlay.setPosition(0.f, 0.f);
        renderer.target.draw(overlay);

        //  在屏幕中心绘制 Restart
        sf::Text restart = menu->restartText;
        float x = sz.x * 0.5f - restart.getGlobalBounds().width * 0.5f;
        float y = sz.y * 0.5f - restart.getGlobalBounds().height * 0.5f;
        restart.setPosition(x, y);
        
        renderer.target.draw(restart);
    }
}

void DeleteObject(Object* object)
{
    const auto& it = std::find(objects.begin(), objects.end(), object);
    if (it != objects.end())
    {
        delete* it;
        objects.erase(it);
    }
}

// Menu 类实现
Menu::Menu()
{
    // 加载 11 帧动画
    for (int i = 1; i <= 11; ++i)
    {
        sf::Texture frame;
        std::string filename = "./resources/textures/menu_background." + std::to_string(i) + ".png";
        if (!frame.loadFromFile(filename))
        {
            std::cerr << "警告: 无法加载动画帧: " << filename << std::endl;
            backgroundSprite.setColor(sf::Color::Blue);
            break;
        }
        backgroundFrames.push_back(frame);
    }

    if (!backgroundFrames.empty())
    {
        backgroundSprite.setTexture(backgroundFrames[0]);
        backgroundSprite.setPosition(0, 0);
    }

    if (!font.loadFromFile("./resources/font.ttf"))
    {
        std::cerr << "警告: 无法加载字体: ./resources/font.ttf" << std::endl;
    }
    else
    {
        std::cout << "字体加载成功" << std::endl;
        startButton.setFont(font);
        startButton.setString("Press To Start Game");
        startButton.setCharacterSize(60);
        startButton.setFillColor(sf::Color::Black);
        startButton.setOutlineColor(sf::Color::White);
        startButton.setOutlineThickness(2.0f);

        // 初始化死亡 UI 文本
        restartText.setFont(font);
        restartText.setString("Restart");
        restartText.setCharacterSize(80);
        restartText.setFillColor(sf::Color::Red);
        restartText.setOutlineColor(sf::Color::White);
        restartText.setOutlineThickness(3.0f);
    }
}

void Menu::UpdateBackgroundScale(sf::RenderWindow& window)
{
    if (backgroundFrames.empty()) return;

    sf::Vector2u windowSize = window.getSize();
    float windowAspect = static_cast<float>(windowSize.x) / windowSize.y;
    sf::Vector2u textureSize = backgroundFrames[0].getSize();
    float textureAspect = static_cast<float>(textureSize.x) / textureSize.y;

    float scale;
    if (windowAspect > textureAspect)
    {
        scale = static_cast<float>(windowSize.y) / textureSize.y;
    }
    else
    {
        scale = static_cast<float>(windowSize.x) / textureSize.x;
    }

    backgroundSprite.setScale(scale, scale);
    sf::FloatRect spriteBounds = backgroundSprite.getGlobalBounds();
    backgroundSprite.setPosition(
        (windowSize.x - spriteBounds.width) / 2.0f,
        (windowSize.y - spriteBounds.height) / 2.0f
    );

    startButton.setPosition(
        windowSize.x / 2.0f - startButton.getGlobalBounds().width / 2.0f,
        windowSize.y * 0.75f
    );

    // 调整死亡 UI 文本位置（屏幕中心）
    restartText.setPosition(
        windowSize.x / 2.0f - restartText.getGlobalBounds().width / 2.0f,
        windowSize.y / 2.0f - restartText.getGlobalBounds().height / 2.0f
    );
}

void Menu::Update(sf::RenderWindow& window)
{
    if (!backgroundFrames.empty() && animationClock.getElapsedTime().asSeconds() >= frameDuration)
    {
        currentFrame = (currentFrame + 1) % backgroundFrames.size();
        backgroundSprite.setTexture(backgroundFrames[currentFrame]);
        animationClock.restart();
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos, window.getView());

        sf::FloatRect buttonBounds = startButton.getGlobalBounds();
        if (buttonBounds.contains(worldPos))
        {
            shouldStart = true;
        }

        sf::FloatRect restartBounds = restartText.getGlobalBounds();
        if (restartBounds.contains(worldPos))
        {
            shouldStart = true;
        }
    }
}

void Menu::Render(sf::RenderWindow& window, Camera& camera)
{
    UpdateBackgroundScale(window);

    sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(view);
    window.draw(backgroundSprite);
    window.draw(startButton);
    if (state==GameState::GameOver)//仅在角色死亡时显示 "Restart"
    {
        window.draw(restartText);
    }
}