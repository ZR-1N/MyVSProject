#pragma once

#include "Animation.h"
#include "Object.h"
#include "Physics.h"
#include <SFML/Audio.hpp>  
class Enemy
	: public Object
{
public:
	virtual void Begin() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(Renderer& renderer) override;

	void Die();
	bool IsDead();

private:
	sf::Sound dieSound{};
	Animation walkLeftAnimation{};   // 左行走动画
	Animation walkRightAnimation{};  // 右行走动画
	Animation* walkAnimation{};      // 当前行走动画指针

	float movement = 5.0f;


	float destroyTimer = 0.0f;      // 被击后计时
	bool isDead = false;            // 是否已被击
	bool isHit = false;            // 是否处于受击状态
	const float hitDuration = 0.5f; // 受击后持续时长

	FixtureData fixtureData{};
	b2Body* body{};
	
};