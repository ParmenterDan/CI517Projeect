#ifndef __MY_ENGINE_H__
#define __MY_ENGINE_H__

#include "../EngineCommon.h"

class PhysicsObjects {
public:
	float x = 0.0, y = 0.0, w = 0.0, h = 0.0, lastX = 0.0, lastY = 0.0;
	float speedX = 0.0, speedY = 2.0, multiplier = 1.0, mass = 1.0, velocity = 0, lastSpeedX = 0, lastSpeedY = 0;
	bool isActive = false, immovable = false;
	std::string side;

	// Functions
	void enablePhysicsObject();
	void disablePhysicsObject();
	bool setImmovable(bool setter);

	PhysicsObjects();
	~PhysicsObjects();
};

class MyEngineSystem {
	friend class XCube2Engine;
private: 

public:

	/*void checkOverlap();
	std::string boxCollideX(int x, int boxWidth, int screenWidth);
	std::string boxCollideY(int y, int boxHeight, int screenHeight);
	bool boxCollision(int x, int y, int width, int height, int x2, int y2, int width2, int height2);
	std::string collisionSide(int x, int y, int width, int height, int x2, int y2, int width2, int height2);
	std::string collisionSideComplex(int x, int y, int lastX, int lastY, int width, int height, int x2, int y2, int lastX2, int lastY2, int width2, int height2);
	float calcVelocityX(float speedX1, float mass1, float speedX2, float mass2);
	float calcVelocityY(float speedY1, float mass1, float speedY2, float mass2);*/

	// Enable/Disable objects
	bool disableObject();
	bool enableObject();
	bool enablePhysicsObject();
	bool disablePhysicsObject();

	// Check for collision
	bool boxCollisionPhysics(PhysicsObjects object1, PhysicsObjects object2);
	std::string boxCollisionSide(PhysicsObjects object1, PhysicsObjects object2);
	std::string borderCollideX(PhysicsObjects object, int screenwidth, int borderOffsetL, int borderOffsetR);
	std::string borderCollideY(PhysicsObjects object, int screenheight, int borderOffsetT, int borderOffsetB);
	float calcVelocityX(PhysicsObjects object1, PhysicsObjects object2);
	float calcVelocityY(PhysicsObjects object1, PhysicsObjects object2);

};

#endif