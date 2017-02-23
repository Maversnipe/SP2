#ifndef DRIVING_H
#define DRIVING_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera4.h"
#include "Mesh.h"
#include <vector>
#include "AABB.h"
#include "MatrixStack.h"
#include "Light.h"

using std::vector;

class Driving : public Scene
{
public:
	Driving();
	~Driving();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	Camera4 Camera4;

	Car car;

	enum GEOMETRY_TYPE
	{
		GEO_TRIANGLE_1 = 0,
		GEO_AXES,
		GEO_QUAD,

		GEO_BB8,
		GEO_MODEL1,
		GEO_MODEL2,

		GEO_TEXT,
		GEO_CUBE,
		GEO_LIGHTBALL,
		GEO_EXPLOSION,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_ENEMY,
		GEO_CAR,
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
	float scaleExplosion;
	bool dead = false;
	int HP = 100;

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
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);

	bool lightSwitch = false;

	void RenderSkybox();

	float fps = 0.0;
	std::string framesPerSec = "";

	int enemySize;
	float enemySpeed;
	float enemyRadius;
	float ObjectRadius;
	float enemyRotation1[100] = {};
	float enemyRotation2[100] = {};

	Vector3 dirVec;
	Vector3 carVec;
	Vector3 enemyPos[100] = {};
	Vector3 ObjectPos[100] = {};
	Vector3 ExplosionPos[100] = {};
	float ExplosionRadius[50];
	bool enemyDead[50];
	bool enemyExplosion[50];
	float RandomNumber(float Min, float Max);
};


#endif