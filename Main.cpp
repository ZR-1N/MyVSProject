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
        std::cerr << "��ʼ��ʧ��: " << e.what() << std::endl;
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
                // �����ڴ�С�ı�ʱ��
                // ����Ĭ����ͼ�Ĵ�С�����ĵ㣬ʹ���ͬ���´��ڳߴ�
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
            // ���˵������Ų˵��������� + Press to Start
            menu->Render(window, camera);
        }
        else if (state == GameState::Playing)
        {
            // ��Ϸ�����У���Ⱦ�ؿ�����ɫ�����
            window.setView(camera.GetView(window.getSize()));
            Render(renderer);
            window.setView(window.getDefaultView());
            RenderUI(renderer);
        }
        else if (state == GameState::GameOver)
        {
            // ��Ϸ�������ػ����һ֡��Ϸ���� + ��͸���� + Restart
            window.setView(camera.GetView(window.getSize()));
            Render(renderer);
            sf::Vector2u sz = window.getSize();
            sf::View uiView(sf::FloatRect(0.f, 0.f, (float)sz.x, (float)sz.y));
            uiView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));  // ռ����������
            window.setView(uiView);

            // Ȼ����� UI
            RenderUI(renderer);
        }

        window.display();


    }
    return 0;
}