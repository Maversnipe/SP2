#include "StudioProject.h"
#include "Money.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include "IK\irrKlang.h"
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")
ISoundEngine* sfx1 = createIrrKlangDevice();

StudioProject::StudioProject()
{
}

StudioProject::~StudioProject()
{
}

void StudioProject::Init()
{
	// Init VBO here
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // Set background color to dark blue

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	// Blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_indexBuffer[0]);

	// Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	// Get a handle for out "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");

	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
	m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
	m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
	m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
	m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
	m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
	m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID, "lights[3].exponent");

	m_parameters[U_LIGHT4_POSITION] = glGetUniformLocation(m_programID, "lights[4].position_cameraspace");
	m_parameters[U_LIGHT4_COLOR] = glGetUniformLocation(m_programID, "lights[4].color");
	m_parameters[U_LIGHT4_POWER] = glGetUniformLocation(m_programID, "lights[4].power");
	m_parameters[U_LIGHT4_KC] = glGetUniformLocation(m_programID, "lights[4].kC");
	m_parameters[U_LIGHT4_KL] = glGetUniformLocation(m_programID, "lights[4].kL");
	m_parameters[U_LIGHT4_KQ] = glGetUniformLocation(m_programID, "lights[4].kQ");
	m_parameters[U_LIGHT4_TYPE] = glGetUniformLocation(m_programID, "lights[4].type");
	m_parameters[U_LIGHT4_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[4].spotDirection");
	m_parameters[U_LIGHT4_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[4].cosCutoff");
	m_parameters[U_LIGHT4_COSINNER] = glGetUniformLocation(m_programID, "lights[4].cosInner");
	m_parameters[U_LIGHT4_EXPONENT] = glGetUniformLocation(m_programID, "lights[4].exponent");

	m_parameters[U_LIGHT5_POSITION] = glGetUniformLocation(m_programID, "lights[5].position_cameraspace");
	m_parameters[U_LIGHT5_COLOR] = glGetUniformLocation(m_programID, "lights[5].color");
	m_parameters[U_LIGHT5_POWER] = glGetUniformLocation(m_programID, "lights[5].power");
	m_parameters[U_LIGHT5_KC] = glGetUniformLocation(m_programID, "lights[5].kC");
	m_parameters[U_LIGHT5_KL] = glGetUniformLocation(m_programID, "lights[5].kL");
	m_parameters[U_LIGHT5_KQ] = glGetUniformLocation(m_programID, "lights[5].kQ");
	m_parameters[U_LIGHT5_TYPE] = glGetUniformLocation(m_programID, "lights[5].type");
	m_parameters[U_LIGHT5_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[5].spotDirection");
	m_parameters[U_LIGHT5_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[5].cosCutoff");
	m_parameters[U_LIGHT5_COSINNER] = glGetUniformLocation(m_programID, "lights[5].cosInner");
	m_parameters[U_LIGHT5_EXPONENT] = glGetUniformLocation(m_programID, "lights[5].exponent");

	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


	// Use our shader
	glUseProgram(m_programID);

	// Set default values
	rotateCaro = 0;
	translateX = 0;
	scaleAll = 1;

	light[0].type = Light::LIGHT_SPOT;
	light[0].position.Set(-77, 20, 25);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	light[1].type = Light::LIGHT_SPOT;
	light[1].position.Set(3, 20, 25);
	light[1].color.Set(1, 1, 1);
	light[1].power = 1;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	light[2].type = Light::LIGHT_SPOT;
	light[2].position.Set(83, 20, 25);
	light[2].color.Set(1, 1, 1);
	light[2].power = 1;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = cos(Math::DegreeToRadian(45));
	light[2].cosInner = cos(Math::DegreeToRadian(30));
	light[2].exponent = 3.f;
	light[2].spotDirection.Set(0.f, 1.f, 0.f);

	light[3].type = Light::LIGHT_SPOT;
	light[3].position.Set(163, 20, 25);
	light[3].color.Set(1, 1, 1);
	light[3].power = 1;
	light[3].kC = 1.f;
	light[3].kL = 0.01f;
	light[3].kQ = 0.001f;
	light[3].cosCutoff = cos(Math::DegreeToRadian(45));
	light[3].cosInner = cos(Math::DegreeToRadian(30));
	light[3].exponent = 3.f;
	light[3].spotDirection.Set(0.f, 1.f, 0.f);

	light[4].type = Light::LIGHT_POINT;
	light[4].position.Set(0, 250, 0);
	light[4].color.Set(1, 1, 1);
	light[4].power = 50;
	light[4].kC = 1.f;
	light[4].kL = 0.01f;
	light[4].kQ = 0.001f;
	light[4].cosCutoff = cos(Math::DegreeToRadian(45));
	light[4].cosInner = cos(Math::DegreeToRadian(30));
	light[4].exponent = 3.f;
	light[4].spotDirection.Set(0.f, 1.f, 0.f);

	light[5].type = Light::LIGHT_POINT;
	light[5].position.Set(0, -250, 0);
	light[5].color.Set(1, 1, 1);
	light[5].power = 50;
	light[5].kC = 1.f;
	light[5].kL = 0.01f;
	light[5].kQ = 0.001f;
	light[5].cosCutoff = cos(Math::DegreeToRadian(45));
	light[5].cosInner = cos(Math::DegreeToRadian(30));
	light[5].exponent = 3.f;
	light[5].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 5);

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

	glUniform1i(m_parameters[U_LIGHT3_TYPE], light[3].type);
	glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &light[3].color.r);
	glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
	glUniform1f(m_parameters[U_LIGHT3_KC], light[3].kC);
	glUniform1f(m_parameters[U_LIGHT3_KL], light[3].kL);
	glUniform1f(m_parameters[U_LIGHT3_KQ], light[3].kQ);
	glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], light[3].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT3_COSINNER], light[3].cosInner);
	glUniform1f(m_parameters[U_LIGHT3_EXPONENT], light[3].exponent);

	glUniform1i(m_parameters[U_LIGHT4_TYPE], light[4].type);
	glUniform3fv(m_parameters[U_LIGHT4_COLOR], 1, &light[4].color.r);
	glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
	glUniform1f(m_parameters[U_LIGHT4_KC], light[4].kC);
	glUniform1f(m_parameters[U_LIGHT4_KL], light[4].kL);
	glUniform1f(m_parameters[U_LIGHT4_KQ], light[4].kQ);
	glUniform1f(m_parameters[U_LIGHT4_COSCUTOFF], light[4].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT4_COSINNER], light[4].cosInner);
	glUniform1f(m_parameters[U_LIGHT4_EXPONENT], light[4].exponent);

	glUniform1i(m_parameters[U_LIGHT5_TYPE], light[5].type);
	glUniform3fv(m_parameters[U_LIGHT5_COLOR], 1, &light[5].color.r);
	glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
	glUniform1f(m_parameters[U_LIGHT5_KC], light[5].kC);
	glUniform1f(m_parameters[U_LIGHT5_KL], light[5].kL);
	glUniform1f(m_parameters[U_LIGHT5_KQ], light[5].kQ);
	glUniform1f(m_parameters[U_LIGHT5_COSCUTOFF], light[5].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT5_COSINNER], light[5].cosInner);
	glUniform1f(m_parameters[U_LIGHT5_EXPONENT], light[5].exponent);

	// Initialise Camera
	camera3.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		meshList[i] = NULL;
	}

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("reference", Color(1, 1, 1), 1, 1);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//color2.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1, 1);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//MainScene//front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1, 1);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//MainScene//back.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1, 1);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//MainScene//left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1, 1);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//MainScene//right.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1, 1);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//MainScene//bottom.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1, 1);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//MainScene//top.tga");

	meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("ground", Color(1, 1, 1), 1, 1);
	meshList[GEO_GROUND]->textureID = LoadTGA("Image//MainScene//ground.tga");

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 0), 1, 1, 1);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightSphere", Color(1, 1, 1), 18, 36, 1);


//=======================OBJECTS================================================================
	meshList[GEO_BUILDING] = MeshBuilder::GenerateOBJ("building1", "OBJ//MainScene//building.obj");
	meshList[GEO_BUILDING]->textureID = LoadTGA("Image//MainScene//building.tga");

	meshList[GEO_BUILDING2] = MeshBuilder::GenerateOBJ("building2", "OBJ//MainScene//building.obj");
	meshList[GEO_BUILDING2]->textureID = LoadTGA("Image//MainScene//building2.tga");

	meshList[GEO_BUILDING3] = MeshBuilder::GenerateOBJ("building3", "OBJ//MainScene//building.obj");
	meshList[GEO_BUILDING3]->textureID = LoadTGA("Image//MainScene//building3.tga");

	meshList[GEO_BUILDING4] = MeshBuilder::GenerateOBJ("building4", "OBJ//MainScene//building.obj");
	meshList[GEO_BUILDING4]->textureID = LoadTGA("Image//MainScene//building4.tga");

	meshList[GEO_CAROTOP] = MeshBuilder::GenerateOBJ("carotop", "OBJ//MainScene//CaroTop.obj");
	meshList[GEO_CAROTOP]->textureID = LoadTGA("Image//MainScene//Caro.tga");

	meshList[GEO_CAROBOTTOM] = MeshBuilder::GenerateOBJ("carobottom", "OBJ//MainScene//CaroBase.obj");
	meshList[GEO_CAROBOTTOM]->textureID = LoadTGA("Image//MainScene//Caro.tga");

	meshList[GEO_FENCE] = MeshBuilder::GenerateOBJ("fence", "OBJ//MainScene//fence.obj");
	meshList[GEO_FENCE]->textureID = LoadTGA("Image//MainScene//fence.tga");
	//=======================================ROCK===========================================
	meshList[GEO_ROCKS] = MeshBuilder::GenerateQuad("rock", Color(1, 1, 1), 1, 1);
	meshList[GEO_ROCKS]->textureID = LoadTGA("Image//starRocks.tga");
	//======================================================================================

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//PrestigeElite.tga");
	//======================================================================================
	meshList[GEO_LOAD1] = MeshBuilder::GenerateQuad("load", Color(1, 1, 1), 1, 1);
	meshList[GEO_LOAD1]->textureID = LoadTGA("Image//loading1.tga");

	// Building Coords
	building[0].Set(-80, 0, -23);
	building[1].Set(0, 0, -23);
	building[2].Set(80, 0, -23);
	building[3].Set(0, -2, 200);
	building[4].Set(160, -2, -23); 

	// Building Names
	buildingName[0] = "Shooter";
	buildingName[1] = "Platformer";
	buildingName[2] = "Driving";
	buildingName[3] = "Space Invaders";

	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 2000.0f);
	projectionStack.LoadMatrix(projection);

	changeScene = 0;

	sfx1->play2D("audio/main.mp3", GL_TRUE);
}

void StudioProject::Update(double dt)
{
	ShowCursor(false);
	elapsed_time += dt;

	x = camera3.position.x;
	y = camera3.position.y;
	z = camera3.position.z;
	X = "X: " + std::to_string(x);
	Y = "Y: " + std::to_string(y);
	Z = "Z: " + std::to_string(z);

	rotateCaro += 40 * dt;

	if ((camera3.position.x - 2 > building[0].x - 10) && (camera3.position.x + 2 < building[0].x + 15)
		&& (camera3.position.z - 2 > building[0].z + 25) && (camera3.position.z + 2 < building[0].z + 35)
		&& (Application::IsKeyPressed('E')))
	{
		if (Money::getInstance()->getMoney() >= 5)
		{
			Money::getInstance()->deductMoney(5);
			changeScene = 2;
		}
	
	}

	if ((camera3.position.x - 2 > building[1].x - 10) && (camera3.position.x + 2 < building[1].x + 15)
		&& (camera3.position.z - 2 > building[1].z + 25) && (camera3.position.z + 2 < building[1].z + 35)
		&& (Application::IsKeyPressed('E')))
	{
		if (Money::getInstance()->getMoney() >= 5)
		{
			Money::getInstance()->deductMoney(5);
			changeScene = 3;
		}
		else 
			Money::getInstance()->notEnoughMoney = true;
	}

	if ((camera3.position.x - 2 > building[2].x - 10) && (camera3.position.x + 2 < building[2].x + 15)
		&& (camera3.position.z - 2 > building[2].z + 25) && (camera3.position.z + 2 < building[2].z + 35)
		&& (Application::IsKeyPressed('E')))
	{
		if (Money::getInstance()->getMoney() >= 5)
		{
			Money::getInstance()->deductMoney(5);
			changeScene = 4;
		}
		else
			Money::getInstance()->notEnoughMoney = true;
	}
	if ((camera3.position.x - 2 > building[4].x - 10) && (camera3.position.x + 2 < building[4].x + 15)
		&& (camera3.position.z - 2 > building[4].z + 25) && (camera3.position.z + 2 < building[4].z + 35)
		&& (Application::IsKeyPressed('E')))
	{
		if (Money::getInstance()->getMoney() >= 5)
		{
			Money::getInstance()->deductMoney(5);
			changeScene = 5;
		}
		else
			Money::getInstance()->notEnoughMoney = true;
	}
	if (Application::IsKeyPressed('E'))
	{ // So that it does not sense that user pressed E when entering mini-games
	}
//=========================================MONEY============================================	
	if (Application::IsKeyPressed('V'))
		Money::getInstance()->deductMoney(3);

	if (Money::getInstance()->notEnoughMoney)
	{
		noMoney = true;
		displayMoney = true;
		Money::getInstance()->notEnoughMoney = false;
	}
	if (flash_money_time < 3 && displayMoney)
	{
		if (bounce_time_money > 0 && !noMoney)
		{
			bounce_time_money -= 1.5 * dt;
			if (bounce_time_money - 1.5 * dt <= 0)
				noMoney = true;
		}

		else if (bounce_time_money < 1 && noMoney)
		{
			bounce_time_money += 1.5 * dt;
			if (bounce_time_money + 1.5 * dt >= 1)
				noMoney = false;
		}
		flash_money_time += 1 * dt;
	}

	else
	{
		flash_money_time = 0.0;
		bounce_time_money = 0.0;
		displayMoney = false;
		noMoney = false;
	}
//==================================CAROUSELL===========================================		
	//Position of carousell
	if (((camera3.position.x >= -26) && (camera3.position.x <= 26) && (camera3.position.z >= 174) && (camera3.position.z <= 230)) && Application::IsKeyPressed('Q') && !camera3.playCarousell && (elapsed_time > bounce_time))
	{
		if (Money::getInstance()->getMoney() >= 5)
		{
			Money::getInstance()->deductMoney(5);
			camera3.playCarousell = true;
			//Saving previous positions
			prevPos = camera3.position;
			prevTarget = camera3.target;
		}
		//If player has not enough money
		else
			Money::getInstance()->notEnoughMoney = true;
		bounce_time = elapsed_time + 10;
	}
	if (Application::IsKeyPressed('Q'))
	{
	}

	if (camera3.playCarousell)
	{
		//Moving in a circle
		theta = (float)increment; //Increment is like the position of slice in a drawn circle. Each slice has 1 degree
		camera3.position.z = 10 * cos(Math::DegreeToRadian(theta)) + 200;
		camera3.position.x = 10 * sin(Math::DegreeToRadian(theta)) + (-5);
		camera3.position.y = 5;
		if (increment < (360 + 1)) //360 is the angle of circle
			increment++;
		else if (increment >= (360 + 1))
			increment = 0;
		if (Application::IsKeyPressed('E'))
		{
			camera3.playCarousell = false;
			//Returning to previous position
			camera3.position = prevPos;
			camera3.target = prevTarget;
			increment = 0;
		}
	}
//===================================================================
	camera3.Update(dt);
}

void StudioProject::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color & depth buffer every frame
	//LIGHT0
	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	//LIGHT1
	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
	//LIGHT2
	if (light[2].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[2].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[2].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
	}
	//LIGHT3
	if (light[3].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[3].position.x, light[3].position.y, light[3].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[3].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[3].position;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[3].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[3].position;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
	}
	//LIGHT4
	if (light[4].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[4].position.x, light[4].position.y, light[4].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[4].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[4].position;
		glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[4].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT4_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[4].position;
		glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightPosition_cameraspace.x);
	}
	//LIGHT5
	if (light[5].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[5].position.x, light[5].position.y, light[5].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[5].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[5].position;
		glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[5].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT5_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[5].position;
		glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightPosition_cameraspace.x);
	}

	Mtx44 MVP;

	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera3.position.x, camera3.position.y, camera3.position.z,
		camera3.target.x, camera3.target.y, camera3.target.z,
		camera3.up.x, camera3.up.y, camera3.up.z);
	modelStack.LoadIdentity();

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	//---------------------------------------------------------------
	if (changeScene != 0)
		RenderMeshOnScreen(meshList[GEO_LOAD1], 40, 20, 80, 80);//No transform needed
	else
	{

		RenderSkybox();

		modelStack.PushMatrix();
		modelStack.Translate(0, -5, 0);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_GROUND], true);
		modelStack.PopMatrix();
		//===FENCE===
		modelStack.PushMatrix();
		modelStack.Translate(-480, -2, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(480, -2, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, -2, -480);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, -2, 480);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		//===BUILDINGS===

		modelStack.PushMatrix();
		modelStack.Translate(building[0].x, building[0].y, building[0].z);
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 10, -30);
			RenderText(meshList[GEO_TEXT], buildingName[0], Color(1, 0, 0));
			modelStack.PopMatrix();
		}
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BUILDING], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(building[1].x, building[1].y, building[1].z);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BUILDING2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(building[2].x, building[2].y, building[2].z);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BUILDING3], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(building[4].x, building[4].y, building[4].z);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BUILDING4], true);
		modelStack.PopMatrix();

		//====CAROUSELL====
		modelStack.PushMatrix();
		modelStack.Translate(building[3].x, building[3].y, building[3].z);

		modelStack.PushMatrix();
		modelStack.Rotate(rotateCaro, 0, 1, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_CAROBOTTOM], true);
		modelStack.PopMatrix();

		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_CAROTOP], true);
		modelStack.PopMatrix();


		RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(Money::getInstance()->getMoney()), Color(0, 1, 1), 3, 23, 19);
		RenderMeshOnScreen(meshList[GEO_ROCKS], 75, 57, 4, 4);
		if (noMoney)
			RenderTextOnScreen(meshList[GEO_TEXT], "Not enough money!", Color(1, 0, 0), 2, 23, 26);
		RenderTextOnScreen(meshList[GEO_TEXT], X, Color(0, 1, 1), 3, 0.5, 2.5);
		RenderTextOnScreen(meshList[GEO_TEXT], Y, Color(0, 1, 1), 3, 0.5, 1.5);
		RenderTextOnScreen(meshList[GEO_TEXT], Z, Color(0, 1, 1), 3, 0.5, 0.5);
	}
	
}

void StudioProject::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(24.95 * 20, 0, 0);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-24.95 * 20, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -24.95 * 20);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, 24.95 * 20);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, (float)-24.945 * 20, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, (float)24.945 * 20, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
}

void StudioProject::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if ((enableLight))
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	mesh->Render();

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void StudioProject::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void StudioProject::RenderMeshOnScreen(Mesh* mesh, int x, int y, int
	sizex, int sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate((float)x, (float)y, 0);
	modelStack.Scale((float)sizex, (float)sizey, 1);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void StudioProject::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void StudioProject::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; i++)
	{
		if (meshList != NULL)
			delete meshList[i];
	}
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

	sfx1->stopAllSounds();
}