#include "SceneLight.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Utility.h"


SceneLight::SceneLight()
{
}

SceneLight::~SceneLight()
{
}

void SceneLight::Init()
{
	// Init VBO here
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // Set background color to dark blue

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//Shading.vertexshader", 
		"Shader//Shading.fragmentshader");

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_indexBuffer[0]);

	// Get a handle for out "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	// Use our shader
	glUseProgram(m_programID);

	// Set default values
	rotateAngle = 0;
	translateX = 0;
	scaleAll = 1;

	light[0].position.Set(0, 20, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);


	// Initialise Camera
	camera2.Init(Vector3(40, 30, 30), Vector3(0, 0, 0), Vector3(0, 1, 0));

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("reference", Color(0, 0, 0), 1, 1);
	meshList[GEO_QUAD]->material.kAmbient.Set(0.25f, 0.25f, 0.25f);
	meshList[GEO_QUAD]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_QUAD]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_QUAD]->material.kShininess = 1.f;

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 0), 1, 1, 1);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(0, 1, 0), 36, 1);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("circle", Color(1, 0, 0), 20, 1, 0.7, 6);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightSphere", Color(1, 1, 1), 18, 36, 1);
	meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("hemisphere", Color(1, 0, 1), 18, 36, 1);
	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("cylinder", Color(0, 1, 1), 6, 5, 36, 1.0);
	/*meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0, 1, 1), 3.0, 18, 180, 1.0);*/

	// Sphere 1
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1);
	meshList[GEO_SPHERE]->material.kAmbient.Set(0.25f, 0.25f, 0.25f);
	meshList[GEO_SPHERE]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPHERE]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE]->material.kShininess = 1.f;

	// Sphere 2
	meshList[GEO_SPHERE1] = MeshBuilder::GenerateSphere("sphere1", Color(0, 1, 0), 18, 36, 1);
	meshList[GEO_SPHERE1]->material.kAmbient.Set(0.25f, 0.25f, 0.25f);
	meshList[GEO_SPHERE1]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPHERE1]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE1]->material.kShininess = 1.f;

	// Sphere 3
	meshList[GEO_SPHERE2] = MeshBuilder::GenerateSphere("sphere2", Color(0, 0, 1), 18, 36, 1);
	meshList[GEO_SPHERE2]->material.kAmbient.Set(0.25f, 0.25f, 0.25f);
	meshList[GEO_SPHERE2]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPHERE2]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE2]->material.kShininess = 1.f;

	// Sphere 4
	meshList[GEO_SPHERE3] = MeshBuilder::GenerateSphere("sphere3", Color(1, 1, 0), 18, 36, 1);
	meshList[GEO_SPHERE3]->material.kAmbient.Set(0.25f, 0.25f, 0.25f);
	meshList[GEO_SPHERE3]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPHERE3]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE3]->material.kShininess = 1.f;

	// Sphere 5
	meshList[GEO_SPHERE4] = MeshBuilder::GenerateSphere("sphere4", Color(1, 0, 1), 18, 36, 1);
	meshList[GEO_SPHERE4]->material.kAmbient.Set(0.25f, 0.25f, 0.25f);
	meshList[GEO_SPHERE4]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPHERE4]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE4]->material.kShininess = 1.f;

	// Sphere 6
	meshList[GEO_SPHERE5] = MeshBuilder::GenerateSphere("sphere5", Color(0, 1, 1), 18, 36, 1);
	meshList[GEO_SPHERE5]->material.kAmbient.Set(0.25f, 0.25f, 0.25f);
	meshList[GEO_SPHERE5]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPHERE5]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE5]->material.kShininess = 1.f;

	// Sphere 7
	meshList[GEO_SPHERE6] = MeshBuilder::GenerateSphere("sphere6", Color(1, 0.5, 0.5), 18, 36, 1);
	meshList[GEO_SPHERE6]->material.kAmbient.Set(0.25f, 0.25f, 0.25f);
	meshList[GEO_SPHERE6]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPHERE6]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE6]->material.kShininess = 1.f;

	// Sphere 8
	meshList[GEO_SPHERE7] = MeshBuilder::GenerateSphere("sphere7", Color(0.5, 1, 0.5), 18, 36, 1);
	meshList[GEO_SPHERE7]->material.kAmbient.Set(0.25f, 0.25f, 0.25f);
	meshList[GEO_SPHERE7]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPHERE7]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE7]->material.kShininess = 1.f;

	// Sphere 9
	meshList[GEO_SPHERE8] = MeshBuilder::GenerateSphere("sphere8", Color(0.5, 0.5, 1), 18, 36, 1);
	meshList[GEO_SPHERE8]->material.kAmbient.Set(0.25f, 0.25f, 0.25f);
	meshList[GEO_SPHERE8]->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPHERE8]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE8]->material.kShininess = 1.f;


	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);
}

void SceneLight::Update(double dt)
{
	float LSPEED = 10.f;
	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);

	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	else if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	else if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	camera2.Update(dt);
}

void SceneLight::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color & depth buffer every frame

	Mtx44 MVP;

	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera2.position.x, camera2.position.y, camera2.position.z,
		camera2.target.x, camera2.target.y, camera2.target.z,
		camera2.up.x, camera2.up.y, camera2.up.z);
	viewStack.Rotate(camera2.getPhi(), 0, 1, 0);
	viewStack.Rotate(camera2.getTheta(), 1, 0, 0);
	modelStack.LoadIdentity();

	//---------------------------------------------------------------
	RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_QUAD], true);
	modelStack.PopMatrix();

	//---------------------------------------------------------------
	// Sphere 1
	modelStack.PushMatrix();
	modelStack.Translate(0, 10, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_HEMISPHERE], true);
	modelStack.PopMatrix();

	// Sphere 2
	modelStack.PushMatrix();
	modelStack.Translate(-20, 10, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SPHERE1], true);
	modelStack.PopMatrix();

	// Sphere 3
	modelStack.PushMatrix();
	modelStack.Translate(20, 10, 0);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SPHERE2], true);
	modelStack.PopMatrix();

	// Sphere 4
	modelStack.PushMatrix();
	modelStack.Translate(20, 10, 20);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SPHERE3], true);
	modelStack.PopMatrix();

	// Sphere 5
	modelStack.PushMatrix();
	modelStack.Translate(-20, 10, 20);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SPHERE4], true);
	modelStack.PopMatrix();

	// Sphere 6
	modelStack.PushMatrix();
	modelStack.Translate(0, 10, 20);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SPHERE5], true);
	modelStack.PopMatrix();

	// Sphere 7
	modelStack.PushMatrix();
	modelStack.Translate(0, 10, -20);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SPHERE6], true);
	modelStack.PopMatrix();

	// Sphere 8
	modelStack.PushMatrix();
	modelStack.Translate(20, 10, -20);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SPHERE7], true);
	modelStack.PopMatrix();

	// Sphere 9
	modelStack.PushMatrix();
	modelStack.Translate(-20, 10, -20);
	modelStack.Scale(10, 10, 10);
	RenderMesh(meshList[GEO_SPHERE8], true);
	modelStack.PopMatrix();


}

void SceneLight::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneLight::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (Application::IsKeyPressed('9'))
		lightSwitch = true;
	else if (Application::IsKeyPressed('0'))
		lightSwitch = false;

	if ((enableLight) && (lightSwitch == true))
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
	mesh->Render();
}

