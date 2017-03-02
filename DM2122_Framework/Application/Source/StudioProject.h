#ifndef STUDIOPROJECT_H
#define STUDIOPROJECT_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"

class StudioProject : public Scene
{
public:
	StudioProject();
	~StudioProject();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	Camera3 camera3;

	enum GEOMETRY_TYPE
	{
		GEO_TRIANGLE_1 = 0,
		GEO_AXES,
		GEO_QUAD,

		GEO_BB8,
		GEO_MODEL1,
		GEO_MODEL2,
		GEO_BUILDING,
		GEO_BUILDING2,
		GEO_BUILDING3,
		GEO_BUILDING4,
		GEO_CAROTOP,
		GEO_CAROBOTTOM,

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
		GEO_ROCKS,
		GEO_LOAD1,
		GEO_LOAD2,
		GEO_LOAD3,
		GEO_LOAD4,
		GEO_FENCE,
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

	float rotateCaro;
	float translateX;
	float scaleAll;

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

	Vector3 building[5] = {};
	//==Carousell==
	Vector3 prevPos = {};
	Vector3 prevTarget = {};
	int increment = 0;
	float theta = 0.f;
	//=============

	float elapsed_time = 0.0;
	float bounce_time = 0.0;
	float bounce_time_money = 0.0;
	float flash_money_time = 0.0;
	bool noMoney = false;
	bool displayMoney = false;
	float load_time = 0; 
	bool playLoading = true;

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	std::string X = "";
	std::string Y = "";
	std::string Z = "";
};

#endif