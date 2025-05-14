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
	void Begin();
	void Update(float deltaTime);
	void Draw(Renderer& renderer);
	size_t GetCoins();
	virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
	virtual void OnEndContact(b2Fixture* self, b2Fixture* other) override;
	sf::Vector2f position{};
	float angle{};
	bool isDead{};
	// 近战攻击相关 
	float attackCooldown = 1.0f;    // 攻击间隔
	float timeSinceLastAtk = 0.0f;    // 距离上次攻击时间
	float attackRange = 3.0f;    // 攻击距离（Box2D 世界单位）
	// 天照攻击相关
	Animation tianzhaoAnimation;    // 天照动画
	float tianzhaoCooldown = 5.0f;  // 天照冷却时间
	float timeSinceLastTianzhao = 0.0f; // 距离上次天照时间
	float tianzhaoDuration = 2.0f;  // 天照动画持续时间
	float tianzhaoTime = 0.0f;      // 天照动画当前播放时间
	bool tianzhaoActive = false;    // 天照动画是否正在播放
	sf::Vector2f tianzhaoPosition;  // 天照动画播放位置

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
	b2Fixture* groundFixture;
	size_t coins{};
	size_t isGrounded{};
	bool facingLeft{};
};