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
		GEO_LIGHTSPOT,
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
		GEO_FLASHLIGHT,
		GEO_FLASHLIGHT2,
		GEO_BULLET,
		GEO_HITNOTITOP,
		GEO_HITNOTIBOTTOM,
		GEO_HITNOTIRIGHT,
		GEO_HITNOTILEFT,
		GEO_LOAD1,
		GEO_LOAD2,
		GEO_LOAD3,
		GEO_LOAD4,
		PAUSE_SCREEN,
		PAUSE2_SCREEN,
		PAUSE3_SCREEN,
		GAME_OVER,
		GAME_OVER2,
		GAME_OVER3,

		GEO_TUTNPC,
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
		//Light one
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
		//===
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,

	};

	enum SHOOTING_GAMESTATE
	{
		GAME_SELECT,
		TUTORIAL,
		GAME_START,
		GAME_PAUSE,
		GAMEOVER,
		COUNT,
	}game_state;

	unsigned m_parameters[U_TOTAL];

	float rotateAngle;
	float translateX;
	float scaleAll;


	float horizontalRotation;
	float verticalRotation;
private:
	unsigned m_vertexArrayID;
	unsigned m_vertexBuffer[NUM_GEOMETRY];

	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_indexBuffer[NUM_GEOMETRY];
	unsigned m_programID;

	MS modelStack, viewStack, projectionStack;

	Light light[2];

	void RenderMesh(Mesh *mesh, bool enableLight);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);


	bool lightSwitch = false;

	void RenderSkybox();

	float fps = 0.0;
	std::string framesPerSec = "";

	//======Gamestates functions=====
	void UpdateSelect(double);
	void UpdateTutorial(double);
	void UpdateGame(double);
	void UpdateBullet(double);
	void UpdateTreasure(double);
	//============Enemies============
	int enemySize = 70;
	int objectSize;
	float enemySpeed;
	float enemyRadius;
	float ObjectRadius;
	float enemyRotation1[100];
	//===========Pause===============
	double pause_bounce_time = 0.0;
	int pauseSelect = 1;
	void RenderPause();
	//==========Gameover===============
	void RenderGameover();
	double gameover_bounce_time = 0.0;
	int gameoverSelect = 1;

	Vector3 dirVec;
	Vector3 enemyPos[100] = {};
	bool enemyDead[100];
	std::vector <int> enemyMarking;
	int notifyPlayer = 0;
	//========Tutorial enemy=======
	Vector3 enemyTutPos = {2,0,-20};
	bool enemyTutDead = false;
	float enemyTutRotation1 = 0.f;
	//===========Bullet=============
	Bullet bullet[5];
	Bullet original[5];
	int bulletCount = 0;
	bool moveLaser[5];
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
	bool tutorialStart = false;
	bool display1 = false;
	bool display2 = false;
	bool display3 = false;
	bool display4 = false;
	float bounce_time_text_display = 0.f;

	//============Treasure===========
	Vector3 ObjectPos[1] = {};
	bool getMoney = false;
	int amtMoney = 0;
	bool getHealth = false;
	int health = 3;
	float rotateTreasure = 0.f;
	float bounce_time_treasure = 0.f;
	float bounce_time_taken_treasure = 0.f;
	bool treasureAnimation = false;
	bool playMoney = false;
	bool makeNewTreasure = false;
	bool treasureTaken = false;
	//===============================
	float RandomNumber(float min, float max);
	//========Loading screen=========
	double load_time = 0;
	bool playLoading = true;
	//========================
	bool dead = false;
	int money = 0;

	double xpos, ypos;
	int width, height;
};


#endif