#ifndef SP_GABRIEL_H
#define SP_GABRIEL_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera2.h"
#include "Camera3.h"
#include "CameraAzimuth.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

#include <math.h>

class SP_Gabriel : public Scene
{
public:
	SP_Gabriel();
	~SP_Gabriel();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	CameraAzimuth camera3; // cameraAzimuth;
	//Camera2 camera2;
	//Camera3 camera3;

	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_QUAD,

		GEO_TEXT,
		GEO_LIGHTBALL,

		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		GEO_UFO,
		GEO_UFO_TEST,
		GEO_UFO_ON_SCREEN,

		GEO_PROJECTILE,
		GEO_LOAD1,
		GEO_LOAD2,
		GEO_LOAD3,
		GEO_LOAD4,

		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,

	};



	unsigned m_parameters[U_TOTAL];

	float rotateAngle;
	float translateX;
	float scaleAll;

	// all used variables for UFO player/enemy
	float enemyUFOdirectionX[10], enemyUFOdirectionY, enemyUFOdirectionZ[10];
	float randomDirectionX[10], randomDirectionZ[10];
	int maxSpawn;
	int currentSpawned;
	int spawnCounter;
	int forCount;
	int spawnTimer;
	float valX_Z, val;
	float prevValX;
	bool isPlayerAlive, isEnemyAlive[10];
	bool renderRef;
	bool isShooting[10];
	float projectileDirectionX[10], projectileDirectionZ[10];
	float projStartX[10];
	float projStartZ[10];
	int bulletCount;
	int maxBullets;
	float bulletCD;		// bullet cooldown
	float totalDistOfBulletAndObject;
	float distanceOfBulletAndObject[10];	//DOBAO
	float bulletRadius[10];
	float enemyRadius[10];

private:
	unsigned m_vertexArrayID;
	unsigned m_vertexBuffer[NUM_GEOMETRY];

	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_indexBuffer[NUM_GEOMETRY];
	unsigned m_programID;

	MS modelStack, viewStack, projectionStack;

	Light light[1];

	void RenderMesh(Mesh *mesh, bool enableLight);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, int sizex, int sizey);


	bool lightSwitch = false;

	void RenderSkybox();

	float fps = 0.0;
	std::string framesPerSec = "";
	//Loading screen
	float load_time = 0;
	bool playLoading = true;

};

#endif