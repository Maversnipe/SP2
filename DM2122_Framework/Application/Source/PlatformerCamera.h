#ifndef PLATFORMER_CAMERA_H
#define PLATFORMER_CAMERA_H

#include "Camera.h"
#include "AABB.h"
#include <vector>
#include "Platforms.h"
#include "PlatformerTreasure.h"

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
	virtual void Update(double dt, std::vector<Platforms> [], std::vector<PlatformerTreasure>);

	float mouseY = 0;
	float mouseX = 0;
	double xpos, ypos;
	float verticalAngle = 0.0f;
	float rotateVert = 0;
	float rotateHori = 0;

	// Platform Stuff
	int boundsCheck(Vector3);
	bool onPlatform(AABB, AABB);
	AABB charAABB;
	Platforms currPlatform;
	Platforms noPlatform;
	
	//============================TIME==========================
	double elapsed_time = 0.0;
	double bounce_time = 0.0;

	//==========================Treasure==========================
	int numPickedUp = 0;

	//======================Character Movement======================
		// Moving Direction
			float velocity = 0;
			float fixedVelocity = 0;
			void charMovement(double, std::vector<Platforms> []);
			
		// Jumping
			void jumping(double);
			bool jump = false;
			float playerOriginalHeight = 0.0;
			bool onGround = true;
			float gravity = 19.6f;
			float fallingVelocity = 0;
			bool doubleJump = false; // This to see if player is doing double jump
			bool landed = true;

	//======================Special Abilities======================
		//(Checking if have special abilities)
		// Only can have one special ability at a time
		bool doubleJumpAbility = false;
		bool superSpeedAbility = false;

	//=====================COLLISION===================================
		int slideAABBobject(AABB, AABB);
		bool collideAABBobject(AABB, AABB);
		int X_Or_Z = 0;

private:
	Vector3 view;
	Vector3 right;
};

#endif