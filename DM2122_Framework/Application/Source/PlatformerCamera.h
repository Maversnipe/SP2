#ifndef PLATFORMER_CAMERA_H
#define PLATFORMER_CAMERA_H

#include "Camera.h"
#include "AABB.h"
#include <vector>
#include "Platforms.h"

class PlatformerCamera : public Camera
{
public:

	enum PLATFORM_GAMESTATES
	{
		SELECT_ABILITIES,
		PAUSE,
		GAMEOVER,
		GAME,
		COUNT
	};

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	PlatformerCamera();
	~PlatformerCamera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, std::vector<Platforms> []);

	float mouseY = 0;
	float mouseX = 0;
	double xpos, ypos;
	float verticalAngle = 0.0f;
	float rotateVert = 0;
	float rotateHori = 0;

	// Platform Stuff
	bool onPlatform(AABB, AABB);
	AABB charAABB;
	Platforms currPlatform;
	Platforms nextPlatform;
	Platforms noPlatform;

	// Character Movement
		// Moving Direction
			float velocity = 0;
			void charMovement(double, std::vector<Platforms> []);
			
		// Jumping
			void jumping(double);
			bool jump = false;
			double playerOriginalHeight = 0.0;
			bool onGround = true;
			double gravity = 19.6;
			float fallingVelocity = 0;

	// Special Abilities
	bool doubleJump = false;
	bool superSpeed = false;
	bool jumpHigher = false;

private:
	Vector3 view;
	Vector3 right;
};

#endif