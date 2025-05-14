#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <box2d/b2_body.h>
#include "Animation.h"
#include "Renderer.h"
#include "Physics.h"

class Sasuke
	: public ContactListener
{
public:
	~Sasuke() {
		if (body) {
			Physics::world->DestroyBody(body);
			body = nullptr;
		}
	}
	void Begin();
	void Update(float deltaTime);
	void Draw(Renderer& renderer);
	size_t GetCoins();
	virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
	virtual void OnEndContact(b2Fixture* self, b2Fixture* other) override;
	sf::Vector2f position{};
	float angle{};
	bool isDead{};
	// ��ս������� 
	float attackCooldown = 1.0f;    // �������
	float timeSinceLastAtk = 0.0f;    // �����ϴι���ʱ��
	float attackRange = 3.0f;    // ��������
	// ���չ������
	Animation tianzhaoAnimation;    // ���ն���
	float tianzhaoCooldown = 5.0f;  // ������ȴʱ��
	float timeSinceLastTianzhao = 0.0f; // �����ϴ�����ʱ��
	float tianzhaoDuration = 2.0f;  // ���ն�������ʱ��
	float tianzhaoTime = 0.0f;      // ���ն�����ǰ����ʱ��
	bool tianzhaoActive = false;    // ���ն����Ƿ����ڲ���
	sf::Vector2f tianzhaoPosition;  // ���ն�������λ��

	Animation runAnimation;
	Animation stanceAnimation;
	Animation jumpAnimation;
	Animation attackAnimation;
	Animation tpAnimation;

	sf::Texture textureToDraw{};
	sf::Sound jumpSound{};
	sf::Sound attackSound{};
	sf::Sound tianzhaoSound{};
	sf::Sound aoyiSound{};
	sf::Sound danshiSound{};

	FixtureData fixtureData{};
	b2Body* body{};
	b2Fixture* groundFixture=nullptr;
	size_t coins{};
	size_t isGrounded{};
	bool facingLeft{};
};