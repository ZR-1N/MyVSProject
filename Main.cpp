#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Renderer.h"
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 900), "Sasuke");
    windowPtr = &window;
    sf::Clock deltaClock;
    Renderer renderer(window);
    window.setFramerateLimit(60);

    try {
        Begin();
    }
    catch (const std::exception& e) {
        std::cerr << "初始化失败: " << e.what() << std::endl;
        return 1;
    }

    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();

        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized)
            {
                // 当窗口大小改变时，
                // 重置默认视图的大小和中心点，使其等同于新窗口尺寸
                sf::View dv = window.getDefaultView();
                dv.setSize((float)event.size.width, (float)event.size.height);
                dv.setCenter((float)event.size.width / 2.0f,
                    (float)event.size.height / 2.0f);
                window.setView(dv);
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                paused = !paused;
        }

        Update(deltaTime);
        window.clear(sf::Color::Black);

        if (state == GameState::Menu)
        {
            // 仅菜单：播放菜单背景动画 + Press to Start
            menu->Render(window, camera);
        }
        else if (state == GameState::Playing)
        {
            // 游戏进行中：渲染关卡、角色、金币
            window.setView(camera.GetView(window.getSize()));
            Render(renderer);
            window.setView(window.getDefaultView());
            RenderUI(renderer);
        }
        else if (state == GameState::GameOver)
        {
            // 游戏结束：重画最后一帧游戏场景 + 半透遮罩 + Restart
            window.setView(camera.GetView(window.getSize()));
            Render(renderer);
            sf::Vector2u sz = window.getSize();
            sf::View uiView(sf::FloatRect(0.f, 0.f, (float)sz.x, (float)sz.y));
            uiView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));  // 占满整个窗口
            window.setView(uiView);

            // 然后绘制 UI
            RenderUI(renderer);
        }

        window.display();


    }
    return 0;
}