#pragma once

#ifndef H_SURFACE
#define H_SURFACE

#include <iostream>
#include <vector>
#include <time.h>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

#include "Program.h"
#include "Camera.h"
#include "config.h"

class Surface
{
public:
	Surface(SurfaceType type, string vShader, string fShader, string texture, float range, int density);
	void setParentModelMatrix(mat4 parentMatrix);
	mat4 getModelMatrix();
	void drawSurface(mat4 P, mat4 V, vec3 camPosition);
	void drawBillboard(mat4 P, mat4 V, vec3 camPosition, shared_ptr<Program> program, int type);
	//void generateWaterHole(float theta, vec3 pos);
	void translateSurface(vec3 position);
	void scaleSurface(vec3 scale);
	void rotateSurface(float theta, Axis axis);
	vec3 getPosition();
	shared_ptr<Program> getProgram();
	void setProgram(shared_ptr<Program> program);
	void update(vec3 position);
	vec3 getClosestWater(vec3 position);
	void setID(int id);
	int getID();

private:
	int id;
	SurfaceType type;
	GLuint VAO, VBO, EB, TB, TL;
	mat4 parentModelMatrix;
	mat4 translate, rotate, scale;
	vector<GLfloat> vertices;
	vector<GLushort> indices;
	vector<GLfloat> normals;
	vector<GLfloat> textures;
	string vertexShader, fragmentShader, texture;
	shared_ptr<Program> program;
	vec3 position;
	vector<vec3> waterPositions;
	float range;
	float widthPerRect;
	float lengthPerRect;
	int density;

	void generateSurfaceMesh(float width, float length, int density);
	void generateNewWaterPosition(vec3 position, float theta);
};
#endif