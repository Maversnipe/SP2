#ifndef CAMERA2_H
#define CAMERA2_H

#include "Camera.h"

class Camera2 : public Camera
{
public:

	Vector3 view;
	Vector3 right;

	Camera2();
	~Camera2();

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);
	float getPhi();
	float getTheta();

private:

	float phi;
	float theta;
};

#endif