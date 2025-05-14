#include "Sasuke.h"
#include "Object.h"
#include "Resources.h"
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>
#include <iostream>
#include "Game.h"
#include "Enemy.h"

const float movementSpeed = 7.0f;
const float jumpVelocity = 10.0f;



void Sasuke::Begin()
{

    // �����ܲ�����
    tpAnimation = Animation(0.10f,
        {
            AnimFrame(0.00f,Resources::textures["tp_1.png"]),
            AnimFrame(0.05f,Resources::textures["tp_2.png"]),
        });
    runAnimation = Animation(0.90f,
        {
            AnimFrame(0.00f, Resources::textures["run_1.png"]),
            AnimFrame(0.15f, Resources::textures["run_2.png"]),
            AnimFrame(0.30f, Resources::textures["run_3.png"]),
            AnimFrame(0.45f, Resources::textures["run_4.png"]),
            AnimFrame(0.60f, Resources::textures["run_5.png"]),
            AnimFrame(0.75f, Resources::textures["run_6.png"]),
        });
    //���幥������
    attackAnimation = Animation(0.50f,
        {
        AnimFrame(0.00f, Resources::textures["attack_1.png"]),
        AnimFrame(0.10f, Resources::textures["attack_2.png"]),
        AnimFrame(0.30f, Resources::textures["attack_3.png"]),

        });
    // �������ն���
    tianzhaoAnimation = Animation(2.0f, {
        AnimFrame(0.00f, Resources::textures["tianzhao_1.png"]),
        AnimFrame(0.17f, Resources::textures["tianzhao_2.png"]),
        AnimFrame(0.34f, Resources::textures["tianzhao_3.png"]),
        AnimFrame(0.51f, Resources::textures["tianzhao_4.png"]),
        AnimFrame(0.68f, Resources::textures["tianzhao_5.png"]),
        AnimFrame(0.85f, Resources::textures["tianzhao_6.png"]),
        AnimFrame(1.02f, Resources::textures["tianzhao_7.png"]),
        AnimFrame(1.19f, Resources::textures["tianzhao_8.png"]),
        AnimFrame(1.36f, Resources::textures["tianzhao_9.png"]),
        AnimFrame(1.53f, Resources::textures["tianzhao_10.png"]),
        AnimFrame(1.70f, Resources::textures["tianzhao_11.png"]),
        AnimFrame(1.87f, Resources::textures["tianzhao_12.png"]),
        });
    // ����վ������
    stanceAnimation = Animation(0.8f,
        {
            AnimFrame(0.00f, Resources::textures["stance_1.png"]),
            AnimFrame(0.25f, Resources::textures["stance_2.png"]),
            AnimFrame(0.50f, Resources::textures["stance_3.png"]),
            AnimFrame(0.75f, Resources::textures["stance_4.png"]),



        });

    // ������Ծ����
    jumpAnimation = Animation(1.5f,
        {
            AnimFrame(0.00f, Resources::textures["jump_1.png"]),
            AnimFrame(0.50f, Resources::textures["jump_2.png"]),
            AnimFrame(1.00f, Resources::textures["jump_3.png"]),
        });

    jumpSound.setBuffer(Resources::sounds["jump.wav"]);
    attackSound.setBuffer(Resources::sounds["attack.mp3"]);
    jumpSound.setVolume(20);
    tianzhaoSound.setBuffer(Resources::sounds["tianzhao.mp3"]);
    tianzhaoSound.setVolume(30);
    aoyiSound.setBuffer(Resources::sounds["aoyi.mp3"]);
    aoyiSound.setVolume(40);
    danshiSound.setBuffer(Resources::sounds["danshi.wav"]);
    danshiSound.setVolume(40);

    fixtureData.listener = this;
    fixtureData.sasuke = this;
    fixtureData.type = FixtureDataType::Sasuke;

    b2BodyDef bodyDef{};
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x, position.y);
    bodyDef.fixedRotation = true;
    body = Physics::world->CreateBody(&bodyDef);

    b2FixtureDef fixtureDef{};
    fixtureDef.userData.pointer = (uintptr_t)&fixtureData;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;

    b2CircleShape circleShape{};
    circleShape.m_radius = 0.5f;
    circleShape.m_p.Set(0.0f, -0.5f);
    fixtureDef.shape = &circleShape;
    body->CreateFixture(&fixtureDef);

    circleShape.m_p.Set(0.0f, 0.5f);
    body->CreateFixture(&fixtureDef);

    b2PolygonShape polygonShape{};
    polygonShape.SetAsBox(0.5f, 0.5f);
    fixtureDef.shape = &polygonShape;
    body->CreateFixture(&fixtureDef);

    polygonShape.SetAsBox(0.4f, 0.2f, b2Vec2(0.0f, 1.0f), 0.0f);
    fixtureDef.isSensor = true;
    groundFixture = body->CreateFixture(&fixtureDef);
}

void Sasuke::Update(float deltaTime)
{
    float move = movementSpeed;

    // �������ж���
    runAnimation.Update(deltaTime);
    stanceAnimation.Update(deltaTime);
    jumpAnimation.Update(deltaTime);
    attackAnimation.Update(deltaTime);
    tpAnimation.Update(deltaTime);
    if (tianzhaoActive) {
        tianzhaoAnimation.Update(deltaTime); // ֻ�ڼ���ʱ�������ն���
        tianzhaoTime += deltaTime;           // �ۼƲ���ʱ��
        if (tianzhaoTime >= tianzhaoDuration) {
            tianzhaoActive = false;          // 2���ֹͣ����
            tianzhaoAnimation.Reset();       // ���ö�������һ֡
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        move *= 2;

    b2Vec2 velocity = body->GetLinearVelocity();
    velocity.x = 0.0f;
    //  ���¹�����ȴ  
    timeSinceLastAtk += deltaTime;
    timeSinceLastTianzhao += deltaTime;  // ������ȴ��ʱ
    //  ��⹥������  
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)
        && timeSinceLastAtk >= attackCooldown)
    {
        timeSinceLastAtk = 0.0f;

        // �������ж���Ѱ�һ��ŵ� Enemy
        for (auto* obj : objects)
        {
            Enemy* e = dynamic_cast<Enemy*>(obj);
            if (!e || e->IsDead())
                continue;

            // �������
            sf::Vector2f toE = e->position - position;
            float dist = std::hypot(toE.x, toE.y);
            // �����жϣ����ҿ��� toE.x>0�������� toE.x<0
            if (dist <= attackRange
                && ((facingLeft && toE.x < 0) || (!facingLeft && toE.x > 0)))
            {
                e->Die();    // ��ɱ
                break;       // ���ι���ֻ����һ��
            }
        }
    }
    // ���չ�����K����
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K) && timeSinceLastTianzhao >= tianzhaoCooldown)
    {
        timeSinceLastTianzhao = 0.0f;
        tianzhaoActive = true;// �������ն���
        tianzhaoTime = 0.0f;// ���ò���ʱ��
        tianzhaoSound.play();// ������Ч
        // ����ǰ����8��λ��
        float direction = facingLeft ? -1.0f : 1.0f;
        tianzhaoPosition = position + sf::Vector2f(direction * 8.0f, -0.5f);

        // ���ǰ��8���ڵĵ���
        Enemy* target = nullptr;
        float minDist = 8.0f;
        for (auto* obj : objects)
        {
            Enemy* e = dynamic_cast<Enemy*>(obj);
            if (!e || e->IsDead())
                continue;

            sf::Vector2f toE = e->position - position;
            float dist = std::hypot(toE.x, toE.y);
            if (dist <= 8.0f && ((facingLeft && toE.x < 0) || (!facingLeft && toE.x > 0)))
            {
                if (dist < minDist)
                {
                    minDist = dist;
                    target = e;
                    tianzhaoPosition = e->position; // �����ڵ���λ�ò���
                }
            }
        }

        // ����е��ˣ�ɱ����
        if (target)
        {
            target->Die();
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        velocity.x += move;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        velocity.x -= move;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && isGrounded)
    {
        velocity.y = -jumpVelocity;
        jumpSound.play();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !isGrounded)
    {

        velocity.y = jumpVelocity;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
    {
        // ����˲�ƣ�ֱ���޸� body �� transform
        b2Vec2 p = body->GetPosition();
        float offset = 1.0f;  // Box2D ��λ��4 �� ���൱�� 4.0 ��
        float newX = facingLeft ? p.x - offset : p.x + offset;
        body->SetTransform(b2Vec2(newX, p.y), 0.0f);

        // ͬ�� position����֤��һ֡��Ⱦ�õ���ֵ
        position = sf::Vector2f(newX, p.y);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
    {
        aoyiSound.play();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
    {
        danshiSound.play();
    }
    // ѡ��Ҫ���Ƶ�����
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
    {
        textureToDraw = tpAnimation.GetTexture();
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
    {
        attackSound.play();
        textureToDraw = attackAnimation.GetTexture();
    }
    else if (velocity.x < -0.02f || velocity.x > 0.02f)
    {
        textureToDraw = runAnimation.GetTexture();
        if (velocity.x < -0.02f)
            facingLeft = true;
        else
            facingLeft = false;
    }
    else if (!isGrounded)
    {
        // ������Ծ״̬ѡ��֡
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && isGrounded) // ����
            textureToDraw = Resources::textures["jump_1.png"];
        else if (!isGrounded && velocity.y != 0) // ����
            textureToDraw = Resources::textures["jump_3.png"];
        else // ���
            textureToDraw = Resources::textures["jump_2.png"];
    }
    else
    {
        textureToDraw = stanceAnimation.GetTexture();
    }

    body->SetLinearVelocity(velocity);

    position = sf::Vector2f(body->GetPosition().x, body->GetPosition().y);
    angle = body->GetAngle() * (180.0f / M_PI);
}

void Sasuke::Draw(Renderer& renderer)
{
    float sizex = textureToDraw.getSize().x / 40.0;
    float sizey = textureToDraw.getSize().y / 80.0;
    renderer.Draw(textureToDraw, position,
        sf::Vector2f(facingLeft ? -(1.0f * sizex) : 1.0f * sizex, 2.0f * sizey), angle);
    // ��Ⱦ���ն�����������
    if (tianzhaoActive)
    {
        sf::Texture& tianzhaoTexture = tianzhaoAnimation.GetTexture();
        float tzSizex = tianzhaoTexture.getSize().x / 40.0f;
        float tzSizey = tianzhaoTexture.getSize().y / 80.0f;
        renderer.Draw(tianzhaoTexture, tianzhaoPosition,
            sf::Vector2f(facingLeft ? -(1.0f * tzSizex) : 1.0f * tzSizex, 2.0f * tzSizey));

    }
}

void Sasuke::OnBeginContact(b2Fixture* self, b2Fixture* other)
{
    FixtureData* data = (FixtureData*)other->GetUserData().pointer;

    if (!data)
        return;

    if (groundFixture == self && data->type == FixtureDataType::MapTile)
        isGrounded++;
    else if (data->type == FixtureDataType::Object && data->object->tag == "coin")
    {
        DeleteObject(data->object);
        std::cout << "coins = " << ++coins << "\n";
    }
    else if (data->type == FixtureDataType::Object && data->object->tag == "enemy")
    {
        Enemy* enemy = dynamic_cast<Enemy*>(data->object);
        if (!enemy)
            return;

        if (groundFixture == self)
        {
            enemy->Die();
        }
        else if (!enemy->IsDead())
        {
            isDead = true;
        }
    }
}

void Sasuke::OnEndContact(b2Fixture* self, b2Fixture* other)
{
    FixtureData* data = (FixtureData*)other->GetUserData().pointer;

    if (!data)
        return;

    if (groundFixture == self &&
        data->type == FixtureDataType::MapTile && isGrounded > 0)
        isGrounded--;
}
size_t Sasuke::GetCoins()
{
    return coins;
}