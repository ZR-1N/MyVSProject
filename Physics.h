#pragma once
#include <box2d/b2_world.h>
#include <box2d/b2_fixture.h>
#include "Renderer.h"

constexpr float M_PI = 22.0f / 7.0f;

class MyDebugDraw;
class Object;
class Sasuke;

class ContactListener
{
public:
	virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) = 0;
	virtual void OnEndContact(b2Fixture* self, b2Fixture* other) = 0;
};

enum class FixtureDataType
{
	Sasuke,
	MapTile,
	Object
};

struct FixtureData
{
	FixtureDataType type;
	ContactListener* listener;

	union
	{
		Sasuke* sasuke;
		Object* object;
		struct { int mapX, mapY; };
	};
};

class Physics
{
public:
	static void Init();
	static void Update(float deltaTime);
	static void DebugDraw(Renderer& renderer);

	static b2World* world;
	static MyDebugDraw* debugDraw;
};