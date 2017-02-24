#ifndef SHOOTING_H
#define SHOOTING_H

#include "Scene.h"
#include "Mtx44.h"
#include "ShootingCamera.h"
#include "Mesh.h"
#include <vector>
#include "AABB.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Bullet.h"
#include <vector>

class Shooting : public Scene
{
public:
	Shooting();
	~Shooting();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	ShootingCamera Camera;

	enum GEOMETRY_TYPE
	{
		GEO_TRIANGLE_1 = 0,
		GEO_AXES,
		GEO_QUAD,

		GEO_TEXT,
		GEO_LASER0,
		GEO_LASER1,
		GEO_LASER2,
		GEO_LASER3,
		GEO_LASER4,
		GEO_LASER5,
		GEO_HEALTH,
		GEO_ROCKS,
		GEO_CUBE,
		GEO_LIGHTBALL,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_FLOOR,
		GEO_ENEMY,
		GEO_TABLE,
		GEO_GUN,
		GEO_GUN2,
		GEO_BULLET,
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
	bool dead = false;
	int money = 0;

	float horizontalRotation;
	float verticalRotation;
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


	//============Enemies============
	int enemySize;
	int objectSize;
	float enemySpeed;
	float enemyRadius;
	float ObjectRadius;
	float enemyRotation1[100] = {};

	Vector3 dirVec;
	Vector3 enemyPos[100] = {};
	bool enemyDead[50];
	std::vector <int> enemyMarking;
	//========Tutorial enemy=======
	Vector3 enemyTutPos = {2,0,-20};
	bool enemyTutDead = false;
	float enemyTutRotation1 = 0.f;
	//===========Bullet=============
	Bullet bullet[5];
	Bullet original[5];
	int bulletCount = 0;
	bool moveLaser[5] = {};
	bool reload = false;
	float elapsed_time = 0.f;
	float bounce_time = 0.f;
	float bounce_time_enemy_hit = 0.0;
	float rotateLasHori = 0.f;
	float rotateLasVert = 0.f;
    //==========Tutorial============
	bool pickUpGun = false;
	bool disappearTable = false;
	bool disappearEnemy = true;
	bool tutorialEnd = false;
	bool openTreasure = false;
	//============Treasure===========
	Vector3 ObjectPos[2] = {};
	bool getMoney = false;
	int amtMoney = 0;
	bool getHealth = false;
	int health = 3;
	float rotateTreasure = 0.f;
	bool treasureAnimation = false;
	bool playMoney = false;
	//===============================
	float RandomNumber(float min, float max);


	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	std::string X = "";
	std::string Y = "";
	std::string Z = "";
};


#endif