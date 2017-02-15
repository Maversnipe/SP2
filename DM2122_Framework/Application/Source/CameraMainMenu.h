#ifndef CAMERAMAINMENU_H
#define CAMERAMAINMENU_H

#include "Camera.h"

class CameraMainMenu : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	CameraMainMenu();
	~CameraMainMenu();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);

private:
	Vector3 view;
	Vector3 right;
};

#endif