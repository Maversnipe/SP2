#include "MeshBuilder.h"
#include "Vertex.h"
#include "LoadOBJ.h"
#include <vector>
#include <MyMath.h>
#include <GL\glew.h>

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	// An array of 3 vectors which represents 3 vertices
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	Vertex v;

	
	v.pos.Set(-1000, 0, 0); v.color.Set(1, 0, 0); v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(1000, 0, 0); v.color.Set(1, 0, 0); v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0, -1000, 0); v.color.Set(0, 1, 0); v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0, 1000, 0); v.color.Set(0, 1, 0); v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, -1000); v.color.Set(0, 0, 1); v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, 1000); v.color.Set(0, 0, 1); v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < 6; i++)
		index_buffer_data.push_back(i);

	Mesh *mesh = new Mesh(meshName);

	// Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);

	//Transer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
		&vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned),
		&index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthY)
{
	// An array of 3 vectors which represents 3 vertices
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	Vertex v;

	// Top
	v.pos.Set(-0.5f, 0.5f, 0.f); v.texCoord.Set(0, 1); v.color = color; v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.f); v.texCoord.Set(0, 0); v.color = color; v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, 0.f); v.texCoord.Set(1, 1); v.color = color; v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f, 0.5f, 0.f); v.texCoord.Set(1, 1); v.color = color; v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.f); v.texCoord.Set(0, 0); v.color = color; v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.f); v.texCoord.Set(1, 0); v.color = color; v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < 6; i++)
		index_buffer_data.push_back(i);

	Mesh *mesh = new Mesh(meshName);

	// Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);

	//Transer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
		&vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned),
		&index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ)
{
	// An array of 3 vectors which represents 3 vertices
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	Vertex v;
	
		// Top
	v.pos.Set(-0.5f, 0.5f, -0.5f); v.color = color; v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, 0.5f); v.color = color; v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, -0.5f); v.color = color; v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f, 0.5f, 0.5f); v.color = color; v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, 0.5f); v.color = color; v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, -0.5f); v.color = color; v.normal.Set(0, 1, 0); vertex_buffer_data.push_back(v);

		// Right
	v.pos.Set(0.5f, 0.5f, -0.5f); v.color = color; v.normal.Set(1, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, 0.5f); v.color = color; v.normal.Set(1, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.5f); v.color = color; v.normal.Set(1, 0, 0); vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f, -0.5f, 0.5f); v.color = color; v.normal.Set(1, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, -0.5f); v.color = color; v.normal.Set(1, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, -0.5f); v.color = color; v.normal.Set(1, 0, 0); vertex_buffer_data.push_back(v);

		// Back
	v.pos.Set(0.5f, 0.5f, -0.5f); v.color = color; v.normal.Set(0, 0, -1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, -0.5f); v.color = color; v.normal.Set(0, 0, -1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, -0.5f); v.color = color; v.normal.Set(0, 0, -1); vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f, -0.5f, -0.5f); v.color = color; v.normal.Set(0, 0, -1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, -0.5f); v.color = color; v.normal.Set(0, 0, -1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, -0.5f); v.color = color; v.normal.Set(0, 0, -1); vertex_buffer_data.push_back(v);

		// Left
	v.pos.Set(-0.5f, 0.5f, -0.5f); v.color = color; v.normal.Set(-1, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, -0.5f); v.color = color; v.normal.Set(-1, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, 0.5f); v.color = color; v.normal.Set(-1, 0, 0); vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f, -0.5f, -0.5f); v.color = color; v.normal.Set(-1, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.5f); v.color = color; v.normal.Set(-1, 0, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, 0.5f); v.color = color; v.normal.Set(-1, 0, 0); vertex_buffer_data.push_back(v);

		// Front
	v.pos.Set(-0.5f, -0.5f, 0.5f); v.color = color; v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.5f); v.color = color; v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, 0.5f, 0.5f); v.color = color; v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f, 0.5f, 0.5f); v.color = color; v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, 0.5f, 0.5f); v.color = color; v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, 0.5f); v.color = color; v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v);
		// Bottom
	v.pos.Set(-0.5f, -0.5f, 0.5f); v.color = color; v.normal.Set(0, -1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f, -0.5f, -0.5f); v.color = color; v.normal.Set(0, -1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.5f); v.color = color; v.normal.Set(0, -1, 0); vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f, -0.5f, -0.5f); v.color = color; v.normal.Set(0, -1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, -0.5f); v.color = color; v.normal.Set(0, -1, 0); vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f, -0.5f, 0.5f); v.color = color; v.normal.Set(0, -1, 0); vertex_buffer_data.push_back(v);
	
	for (unsigned i = 0; i < 36; i++)
		index_buffer_data.push_back(i);

	Mesh *mesh = new Mesh(meshName);

	// Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);

	//Transer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
		&vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned),
		&index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateCircle(const std::string &meshName, Color color, unsigned numSlices, float radius)
{
	// An array of 3 vectors which represents 3 vertices
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	Vertex v;
	
	float anglePerSlice = 360.f / numSlices;

	for (int slice = 0; slice < numSlices + 1; slice++)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, 0, 0);
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (unsigned i = 0; i < 2 * (numSlices + 1); i++)
		index_buffer_data.push_back(i);

	Mesh *mesh = new Mesh(meshName);

	// Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);

	//Transer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
		&vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned),
		&index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string &meshName, Color color, unsigned numSlices, float outerRadius, float innerRadius, float height)
{
	// An array of 3 vectors which represents 3 vertices
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	Vertex v;

	float anglePerSlice = 360.f / numSlices;

	// bottom
	for (unsigned slice = 0; slice < numSlices + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(outerRadius * cos(Math::DegreeToRadian(theta)), -height * 0.5, outerRadius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(innerRadius * cos(Math::DegreeToRadian(theta)), -height * 0.5, innerRadius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (unsigned i = 0; i < 2 * (numSlices + 1); i++)
		index_buffer_data.push_back(i);

	// curve
	for (unsigned slice = 0; slice < numSlices + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(outerRadius * cos(Math::DegreeToRadian(theta)), height * 0.5f, outerRadius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(cos(Math::DegreeToRadian(theta)), height * 0.5f, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);

		v.pos.Set(outerRadius * cos(Math::DegreeToRadian(theta)), -height * 0.5f, outerRadius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(cos(Math::DegreeToRadian(theta)), -height * 0.5f, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}

	for (unsigned i = 0; i < 2 * (numSlices + 1); i++)
		index_buffer_data.push_back(i + 2 * (numSlices + 1));

	for (unsigned slice = 0; slice < numSlices + 1; ++slice)
	{
	float theta = slice * anglePerSlice;
	v.pos.Set(innerRadius * cos(Math::DegreeToRadian(theta)), height * 0.5f, innerRadius * sin(Math::DegreeToRadian(theta)));
	v.color = color;
	v.normal.Set(-cos(Math::DegreeToRadian(theta)), height * 0.5f, -sin(Math::DegreeToRadian(theta)));
	vertex_buffer_data.push_back(v);

	v.pos.Set(innerRadius * cos(Math::DegreeToRadian(theta)), -height * 0.5f, innerRadius * sin(Math::DegreeToRadian(theta)));
	v.color = color;
	v.normal.Set(-cos(Math::DegreeToRadian(theta)), -height * 0.5f, -sin(Math::DegreeToRadian(theta)));
	vertex_buffer_data.push_back(v);
	}

	for (unsigned i = 0; i < 2 * (numSlices + 1); i++)
		index_buffer_data.push_back(i + 2 * (numSlices + 1) + 2 * (numSlices + 1));

	// top
	for (unsigned slice = 0; slice < numSlices + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(outerRadius * cos(Math::DegreeToRadian(theta)), height * 0.5, outerRadius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(innerRadius * cos(Math::DegreeToRadian(theta)), height * 0.5, innerRadius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (unsigned i = 0; i < 2 * (numSlices + 1); i++)
		index_buffer_data.push_back(i + 2 * (numSlices + 1) + 2 * (numSlices + 1) + 2 * (numSlices + 1));

	Mesh *mesh = new Mesh(meshName);

	// Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);

	//Transer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
		&vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned),
		&index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices, float radius)
{
	// An array of 3 vectors which represents 3 vertices
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	Vertex v;

	float anglePerSlice = 360.f / numSlices;
	float anglePerStack = 180.f / numStacks;

	for (signed stack = 0; stack < numStacks + 1; stack++)
	{
		float phi = -90.f + stack * anglePerStack;
		for (unsigned slice = 0; slice < numSlices + 1; slice++)
		{
			float theta = slice * anglePerSlice;
			v.pos.Set(radius * cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)), 
				radius * sin(Math::DegreeToRadian(phi)), 
				radius * cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta)));
			v.color = color;
			v.normal.Set(cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)),
				sin(Math::DegreeToRadian(phi)),
				cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta)));
			vertex_buffer_data.push_back(v);
		}
	}

	for (unsigned stack = 0; stack < numStacks + 1; stack++)
	{
		for (unsigned slice = 0; slice < numSlices + 1; slice++)
		{
			index_buffer_data.push_back(stack * (numSlices + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlices + 1) + slice);
		}
	}
		

	Mesh *mesh = new Mesh(meshName);

	// Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);

	//Transer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
		&vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned),
		&index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateHemisphere(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices, float radius)
{
	// An array of 3 vectors which represents 3 vertices
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	Vertex v;

	float anglePerSlice = 360.f / numSlices;
	float anglePerStack = 180.f / numStacks;

	for (signed stack = 0; stack < numStacks + 1; stack++)
	{
		float phi = 0 + stack * anglePerStack;
		for (unsigned slice = 0; slice < numSlices + 1; slice++)
		{
			float theta = slice * anglePerSlice;
			v.pos.Set(radius * cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta)),
				radius * sin(Math::DegreeToRadian(phi)),
				radius * cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta)));
			v.color = color;
			v.normal.Set(0, 1, 0);
			vertex_buffer_data.push_back(v);
		}
	}

	for (int slice = 0; slice < numSlices + 1; slice++)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);

		v.pos.Set(0, 0, 0);
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (signed stack = 0; stack < numStacks + 1; stack++)
	{
		for (unsigned slice = 0; slice < numSlices + 1; slice++)
		{
			index_buffer_data.push_back(stack * (numSlices + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSlices + 1) + slice);
		}
	}

	Mesh *mesh = new Mesh(meshName);

	// Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);

	//Transer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
		&vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned),
		&index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateCylinder(const std::string &meshName, Color color, float height, unsigned numStacks, unsigned numSlices, float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float anglePerSlice = 360.0f / numSlices;

	// bottom
	for (unsigned slice = 0; slice < numSlices + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), -height * 0.5f, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
	}

	v.pos.Set(0, -height * 0.5f, 0);
	v.color = color;
	v.normal.Set(0, -1, 0);
	vertex_buffer_data.push_back(v);

	// curve
	for (unsigned slice = 0; slice < numSlices + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), height * 0.5f, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(cos(Math::DegreeToRadian(theta)), height * 0.5f, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);

		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), -height * 0.5f, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(cos(Math::DegreeToRadian(theta)), -height * 0.5f, sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}

	// top
	for (unsigned slice = 0; slice < numSlices + 1; ++slice)
	{
		float theta = slice * anglePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), height * 0.5f, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}

	v.pos.Set(0, height * 0.5f, 0);
	v.color = color;
	v.normal.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < numSlices + 1; ++i)
	{
		index_buffer_data.push_back(numSlices + 1);
		index_buffer_data.push_back(i);
	}
	for (unsigned i = 0; i < numSlices + 1; ++i)
	{
		index_buffer_data.push_back(numSlices + 2 + i * 2 + 1);
		index_buffer_data.push_back(numSlices + 2 + i * 2);
	}
	for (unsigned i = 0; i < numSlices + 1; ++i)
	{
		index_buffer_data.push_back(numSlices + 2 + (numSlices + 1) * 2 + i);
		index_buffer_data.push_back(numSlices + 2 + (numSlices + 1) * 2 + numSlices + 1);
	}
	
	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const std::string &file_path)
{
	//Read vertices, texcoords & normals from OBJ
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if (!success)
		return NULL;
	//Index the vertices, texcoords & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);
	//Create the mesh and call glBindBuffer, glBufferData
	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
	//Use triangle list and remember to set index size
}

Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol)
{
	// An array of 3 vectors which represents 3 vertices
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	Vertex v;

	float width = 1.f / numCol;
	float height = 1.f / numRow;
	for (unsigned row = 0; row < numRow; ++row)
	{
		for (unsigned col = 0; col < numCol; ++col)
		{
			//Task: Add 4 vertices into vertex_buffer_data
			v.pos.Set(-0.5f, 0.5f, 0.f); v.texCoord.Set(col * width, (16 - row) * height); vertex_buffer_data.push_back(v);
			v.pos.Set(-0.5f, -0.5f, 0.f); v.texCoord.Set(col * width, (16 - row - 1) * height); vertex_buffer_data.push_back(v);
			v.pos.Set(0.5f, -0.5f, 0.f); v.texCoord.Set((col + 1) * width, (16 - row - 1) * height); vertex_buffer_data.push_back(v);
			v.pos.Set(0.5f, 0.5f, 0.f); v.texCoord.Set((col + 1) * width, (16 - row) * height); vertex_buffer_data.push_back(v);


			//Task: Add 6 indices into index_buffer_data
			index_buffer_data.push_back((row * numCol + col) * 4 + 0);
			index_buffer_data.push_back((row * numCol + col) * 4 + 1);
			index_buffer_data.push_back((row * numCol + col) * 4 + 2);
			index_buffer_data.push_back((row * numCol + col) * 4 + 0);
			index_buffer_data.push_back((row * numCol + col) * 4 + 2);
			index_buffer_data.push_back((row * numCol + col) * 4 + 3);
		}
	}

	Mesh *mesh = new Mesh(meshName);

	// Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);

	//Transer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
		&vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(unsigned),
		&index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}