#pragma once

#ifndef H_BILLBOARD
#define H_BILLBOARD

#include <string>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Program.h"
#include "Shape.h"
using namespace std;

class Billboard
{
public:
	Billboard(string textureFilePath, float width, float height)
	{
		texturePath = textureFilePath;
		generateBillboardSurface(width, height);
	}
private:
	GLuint VAO, VBO, EB;
	string texturePath;
	float * vertices;
	short * indices;
	shared_ptr<Program> program;
	shared_ptr<Shape> drawShape;

	void generateBillboardSurface(float width, float height)
	{
		indices = new short[6];
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 1;
		indices[4] = 2;
		indices[5] = 3;

		vertices = new float[4 * 3];
		vertices[0] = -width / 2;
		vertices[1] = -height / 2;
		vertices[2] = 0;

		vertices[3] = width / 2;
		vertices[4] = -height / 2;
		vertices[5] = 0;

		vertices[6] = -width / 2;
		vertices[7] = height / 2;
		vertices[8] = 0;

		vertices[9] = width / 2;
		vertices[10] = height / 2;
		vertices[11] = 0;

		int bufferOffset;

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// Prepare the array buffer for all mesh data
		bufferOffset = 0;
		glBufferData(GL_ARRAY_BUFFER, (3 * 4 * sizeof(GLfloat)), \
			0, GL_DYNAMIC_DRAW);
		// Store all vertices and assign them to an attribute
		glBufferSubData(GL_ARRAY_BUFFER, bufferOffset, 3 * 4 * sizeof(GLfloat), &vertices[0]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0);

		// Generate an element array buffer
		glGenBuffers(1, &EB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB);
		// Store all element indices in the element buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), &indices[0], GL_DYNAMIC_DRAW);
	}
};

#endif