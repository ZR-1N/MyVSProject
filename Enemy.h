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
	Animation walkLeftAnimation{};   // �����߶���
	Animation walkRightAnimation{};  // �����߶���
	Animation* walkAnimation{};      // ��ǰ���߶���ָ��

	float movement = 5.0f;


	float destroyTimer = 0.0f;      // �������ʱ
	bool isDead = false;            // �Ƿ��ѱ���
	bool isHit = false;            // �Ƿ����ܻ�״̬
	const float hitDuration = 0.5f; // �ܻ������ʱ��

	FixtureData fixtureData{};
	b2Body* body{};
	
};