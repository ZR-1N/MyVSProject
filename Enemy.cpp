#include "Enemy.h"
#include "Game.h"
#include "Resources.h"
#include <box2d/b2_circle_shape.h>


void Enemy::Begin()
{
	walkLeftAnimation = Animation(0.50f, {
		AnimFrame(0.00f, Resources::textures["enemy3.png"]),
		AnimFrame(0.25f, Resources::textures["enemy4.png"]),
		});
	walkRightAnimation = Animation(0.50f, {
		AnimFrame(0.00f, Resources::textures["enemy1.png"]),
		AnimFrame(0.25f, Resources::textures["enemy2.png"]),
		});
	// 初始朝向右边
	walkAnimation = &walkRightAnimation;
	dieSound.setBuffer(Resources::sounds["die.mp3"]);
	dieSound.setVolume(50.0f);
	tag = "enemy";

	fixtureData.object = this;
	fixtureData.type = FixtureDataType::Object;

	b2BodyDef bodyDef{};
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = true;
	body = Physics::world->CreateBody(&bodyDef);

	b2CircleShape circleShape{};
	circleShape.m_radius = 0.5f;

	b2FixtureDef fixtureDef{};
	fixtureDef.userData.pointer = (uintptr_t)&fixtureData;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	body->CreateFixture(&fixtureDef);
}

void Enemy::Update(float deltaTime)
{

	if (isHit)
	{

		destroyTimer += deltaTime;
		if (destroyTimer >= hitDuration)
			DeleteObject(this);
		return;
	}
	// 物理与朝向 
	b2Vec2 vel = body->GetLinearVelocity();
	if (std::abs(vel.x) <= 0.02f)
		movement *= -1.0f;        // 碰到障碍反向
	vel.x = movement;
	body->SetLinearVelocity(vel);

	// 更新世界坐标与角度
	position = { body->GetPosition().x, body->GetPosition().y - 0.5f };
	angle = body->GetAngle() * (180.0f / M_PI);

	// 切换当前行走动画
	if (movement < 0)
		walkAnimation = &walkLeftAnimation;
	else
		walkAnimation = &walkRightAnimation;

	// 播放行走动画
	walkAnimation->Update(deltaTime);
}

void Enemy::Render(Renderer& renderer)
{

	if (isHit) {
		// 受击效果

		sf::Vector2f pos = { position.x, position.y + 1.0f };
		sf::Vector2f scale = { 1.0f, 0.3f };
		renderer.Draw(walkAnimation->GetTexture(), pos, scale, angle);

	}
	else {
		renderer.Draw(walkAnimation->GetTexture(), position, { 2.0f,2.0f }, angle);

	}
}

void Enemy::Die()
{

	if (!isHit) {
		isHit = true;
		Physics::world->DestroyBody(body);  // 立即移除物理体
		destroyTimer = 0.0f;                // 重置倒计时
		dieSound.play();

	}
}

bool Enemy::IsDead()
{
	return isDead;
}