#include "Scene4.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"


Scene4::Scene4()
{
}

Scene4::~Scene4()
{
}

void Scene4::Init()
{
	// Init VBO here
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // Set background color to dark blue

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Load vertex and fragment shaders
	m_programID = LoadShaders(
		"Shader//TransformVertexShader.vertexshader",
		"Shader//SimpleFragmentShader.fragmentshader");

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_indexBuffer[0]);

	// Get a handle for out "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	// Use our shader
	glUseProgram(m_programID);

	// Set default values
	rotateAngle = 0;
	translateX = 0;
	scaleAll = 1;

	// Initialise Camera
	camera2.Init(Vector3(40, 30, 30), Vector3(0, 0, 0), Vector3(0, 1, 0));

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("reference", Color(0, 1, 1), 1, 1);
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 0), 1, 1, 1);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(0, 1, 0), 36, 1);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("circle", Color(1, 0, 0), 20, 1, 0.7, 6);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 1), 18, 36, 1);
	meshList[GEO_SPHERE2] = MeshBuilder::GenerateSphere("sphere2", Color(0, 1, 1), 18, 36, 1);
	meshList[GEO_SPHERE3] = MeshBuilder::GenerateSphere("sphere3", Color(1, 0.5, 0.5), 18, 36, 1);
	meshList[GEO_SPHERE4] = MeshBuilder::GenerateSphere("sphere4", Color(0.5, 0.5, 1), 18, 36, 1);
	meshList[GEO_SPHERE5] = MeshBuilder::GenerateSphere("sphere5", Color(0.5, 1, 0.5), 18, 36, 1);
	meshList[GEO_SUN] = MeshBuilder::GenerateSphere("sun", Color(1, 1, 0), 18, 36, 1);
	meshList[GEO_MOON] = MeshBuilder::GenerateSphere("moon", Color(1, 1, 1), 18, 36, 1);
	meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("hemisphere", Color(1, 0, 1), 18, 36, 1);
	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("cylinder", Color(0, 1, 1), 6, 5, 36, 1.0);
	/*meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0, 1, 1), 3.0, 18, 180, 1.0);*/
	
	Mtx44 projection;
	projection.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);
}

void Scene4::Update(double dt)
{
	static float direction = 1;
	static float translateDir = 1;
	static float rotateDir = 1;

	if (Application::IsKeyPressed(VK_SPACE))
		direction *= -1;
	if ((translateX < -40) || (translateX > 40))
		translateDir *= -1;
	if ((rotateAngle < -180) || (rotateAngle > 180))
		rotateDir *= 1;
	if (scaleAll > 10)
		scaleAll = 0;
	else if (scaleAll < 0)
		scaleAll = 10;

	rotateAngle += (float)(90 * rotateDir * dt * direction);
	translateX += (float)(40 * translateDir * dt * direction);
	scaleAll += (float)(2 * dt * direction);

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

void Scene4::Render()
{
	// Render VBO here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color & depth buffer every frame

	Mtx44 MVP;

	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera2.position.x, camera2.position.y, camera2.position.z,
		camera2.target.x, camera2.target.y, camera2.target.z,
		camera2.up.x, camera2.up.y, camera2.up.z);
	viewStack.Rotate(camera2.getPhi(), 0, 1, 0);
	viewStack.Rotate(camera2.getTheta(), 1, 0, 0);
	modelStack.LoadIdentity();
	
	//---------------------------------------------------------------
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_AXES]->Render();

	//--------------------------SUN--------------------------------
	{
		modelStack.PushMatrix();
		modelStack.Scale(10, 10, 10);
		modelStack.Rotate(-rotateAngle, 0, 1, 0);
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]); // Update shader with new MVP

		glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
		meshList[GEO_HEMISPHERE]->Render();
		modelStack.PopMatrix();
	}

	//----------------------Planet1--------------------------------
	{
		modelStack.PushMatrix();
		modelStack.Rotate(rotateAngle, 0, 1, 0);
		modelStack.Translate(30, 0, 0);
		modelStack.Scale(2, 2, 2);
		
		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]); // Update shader with new MVP
		glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
		meshList[GEO_SPHERE]->Render();

		{
			modelStack.PushMatrix();
			modelStack.Rotate(-rotateAngle * 2, 1, 0, 0);
			modelStack.Translate(0, 5, 0);
			modelStack.Scale(0.5, 0.5, 0.5);

			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]); // Update shader with new MVP
			glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
			meshList[GEO_MOON]->Render();
			modelStack.PopMatrix();
		}

		modelStack.PopMatrix();
	}

	//--------------------------Planet2-------------------------------
	{
		modelStack.PushMatrix();
		modelStack.Rotate(rotateAngle, 0, 1, 0);
		modelStack.Translate(55, 0, 0);
		modelStack.Scale(3, 3, 3);

		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]); // Update shader with new MVP
		glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
		meshList[GEO_SPHERE2]->Render();
		modelStack.PopMatrix();
	}

	//--------------------------Planet3-------------------------------
	{
		modelStack.PushMatrix();
		modelStack.Rotate(rotateAngle * 1.5, 0, 1, 0);
		modelStack.Translate(-80, 10, 0);
		modelStack.Scale(7, 7, 7);

		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]); // Update shader with new MVP
		glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
		meshList[GEO_SPHERE3]->Render();

		{
			modelStack.PushMatrix();
			modelStack.Rotate(-rotateAngle * 2, 1, 0, 0);
			modelStack.Scale(2, 2, 2);

			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]); // Update shader with new MVP
			glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
			meshList[GEO_RING]->Render();
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}

	//--------------------------Planet4-------------------------------
	{
		modelStack.PushMatrix();
		modelStack.Rotate(rotateAngle * 1.5, 0, 1, 0);
		modelStack.Translate(100, 40, 0);
		modelStack.Scale(7, 7, 7);

		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]); // Update shader with new MVP
		glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
		meshList[GEO_SPHERE4]->Render();

		{
			modelStack.PushMatrix();
			modelStack.Rotate(-rotateAngle * 2, 1, 0, 0);
			modelStack.Translate(0, 3, 0);
			modelStack.Scale(0.5, 0.5, 0.5);

			MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
			glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
				&MVP.a[0]); // Update shader with new MVP
			glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
			meshList[GEO_MOON]->Render();
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}

	//--------------------------Planet5-------------------------------
	{
		modelStack.PushMatrix();
		modelStack.Rotate(rotateAngle * 0.5, 0, 1, 0);
		modelStack.Translate(140, 40, 0);
		modelStack.Scale(15, 15, 15);

		MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]); // Update shader with new MVP
		glEnableVertexAttribArray(0); // 1st attribute buffer : vertices
		meshList[GEO_SPHERE5]->Render();
		modelStack.PopMatrix();
	}
}

void Scene4::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
