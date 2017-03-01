#ifndef CAMERA_AZIMUTH_H
#define CAMERA_AZIMUTH_H

#include "Camera.h"

class CameraAzimuth : public Camera
{
public:
	CameraAzimuth();
	~CameraAzimuth();
	void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	void Reset();
	void Update(double dt);
};

#endif