#pragma once

#ifndef H_ASSET
#define H_ASSET

#include "GLSL.h"
#include "Shape.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;

#include "Program.h"
#include "config.h"

#define ASSET_RESOURCE_PATH "../resources/"

class Asset
{
public:
	Asset(shared_ptr<Program> drawProgram, AssetType type, shared_ptr<Shape> assetShape);
	void setID(unsigned int id);
	unsigned int getID();
	void setPosition(float x, float y, float z);
	vec3 getPosition();
	void setSpacingRadius(float spacingRadius);
	float getSpacingRadius();
	void uniformScale(float scalar);
	void nonUniformScale(float scalar_x, float scalar_y, float scalar_z);
	void rotate(float angle, Axis rotationAxis);
	void drawAsset(mat4 P, mat4 V);
	bool isType(AssetType type);
	AssetType getType();
	bool isRemovable();
	bool isSolid();
	//TODO: canPickUp, checkCollision, pickUp(reset all transformation info), 
	//placeDown(reset all transformation info)

private:
	int id;
	vec3 position;
	vec3 size;
	mat4 rotation;
	float spacingRadius;
	AssetType type;
	bool removable;
	bool solid;

	string objectFilePath;
	shared_ptr<Program> drawProgram;
	shared_ptr<Shape> meshData;

	mat4 getAssetMatrix();
};

#endif