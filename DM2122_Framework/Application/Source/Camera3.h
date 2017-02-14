#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"

class Camera3 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);

	bool buildingCollide(Vector3, Vector3);

	Vector3 buildings[4] = {};
	bool collideBuilding = false;
	Vector3 buildingSlide;

private:
	Vector3 view;
	Vector3 right;
};

#endif