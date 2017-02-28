#ifndef PLATFORMER_H
#define PLATFORMER_H

#include <map>
#include <vector>
#include "Scene.h"
#include "Mtx44.h"
#include "PlatformerCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Platforms.h"

class Platformer : public Scene
{
public:
	Platformer();
	~Platformer();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void setPlatforms();
	void renderPlatforms();
	float RandomNumber(float, float);

	PlatformerCamera camera;

	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		ONE_BLOCK,
		TWO_BLOCK,
		THREE_BLOCK,
		FOUR_BLOCK,
		FIVE_BLOCK,
		START_END,
		TREASURE_BLOCK,
		GEO_TEXT,
		GEO_CUBE,
		GEO_LIGHTBALL,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_GROUND,
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

	enum PLATFORMER_GAMESTATE
	{
		ABILITY_SELECT,
		GAME,
		GAMEOVER,
		PAUSE,
		COUNT
	};

	unsigned m_parameters[U_TOTAL];

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

	void RenderSkybox();

	// Time
	double elapsed_time = 0.0;
	double bounce_time = 0.0;

	// Map (Platform location)
	int mapData[9][40][100];
	int mapHeight = 20;
	bool haveSpace = false;
	std::vector<Platforms> platformID[6];

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	std::string X = "";
	std::string Y = "";
	std::string Z = "";
};

#endif