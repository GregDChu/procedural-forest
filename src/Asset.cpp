#include "Asset.h"

Asset::Asset(shared_ptr<Program> drawProgram, AssetType type, shared_ptr<Shape> assetShape)
{
	// Init all resources used for drawing the asset
	this->drawProgram = drawProgram;
	//objectFilePath = ASSET_RESOURCE_PATH + objFile;
	// Init the model's transformation data
	size = vec3(1, 1, 1);
	position = vec3(0, 0, 0);
	rotation = mat4(1.0f);
	// Load the asset's mesh data
	meshData = assetShape;
	/*meshData = make_shared<Shape>();
	meshData->loadMesh(objectFilePath);
	meshData->resize();
	meshData->init();*/
	// Store meta data about asset type
	this->type = type;
	if (type == MUSHROOM)
		removable = true;
	else
		removable = false;
	if (type == MUSHROOM || type == MARKER || type == NONE)
		solid = false;
	else
		solid = true;
	// Set spacing data
	spacingRadius = 1.0;
}

void Asset::setID(unsigned int id)
{
	this->id = id;
}

unsigned int Asset::getID()
{
	return id;
}

void Asset::setPosition(float x, float y, float z)
{
	position = vec3(x, y, z);
}

vec3 Asset::getPosition()
{
	return position;
}

void Asset::setSpacingRadius(float spacingRadius)
{
	this->spacingRadius = spacingRadius;
}

float Asset::getSpacingRadius()
{
	return this->spacingRadius;
}

void Asset::uniformScale(float scalar)
{
	size = vec3(scalar, scalar, scalar);
}

void Asset::nonUniformScale(float scalar_x, float scalar_y, float scalar_z)
{
	size = vec3(scalar_x, scalar_y, scalar_z);
}

void Asset::rotate(float angle, Axis rotationAxis)
{
	vec3 rotationAxises = vec3(0, 0, 0);
	if (rotationAxis & X_AXIS)
		rotationAxises.x++;
	if (rotationAxis & Y_AXIS)
		rotationAxises.y++;
	if (rotationAxis & Z_AXIS)
		rotationAxises.z++;

	rotation = glm::rotate(mat4(1.0f), angle, rotationAxises);
}

void Asset::drawAsset(mat4 P, mat4 V)
{
	mat4 M;
	// Bind the program used by the asset to draw itself
	drawProgram->bind();
	// Construct the model matrix
	M = getAssetMatrix();
	// Update all shader uniforms
	glUniformMatrix4fv(drawProgram->getUniform("P"), 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(drawProgram->getUniform("V"), 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(drawProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);
	//TODO Add uniform data based on the asset type/the uniforms that are applied to it using
	//a macro flag system such as define ADD_FOG 0x000001, ADD_DEPTH_COLOR 0x000002, etc.
	glUniform1f(drawProgram->getUniform("fogDepth"), FOG_DEPTH);
	if (type == MUSHROOM)
	{
		static float change = 0.0;
		change += 0.001;
		M *= glm::rotate(mat4(1.0f), change * 2, vec3(0, 1, 0));
		//M *= glm::translate(mat4(1.0f), vec3(0, sin(change * 3), 0));
		glUniformMatrix4fv(drawProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		glUniform1f(drawProgram->getUniform("colorType"), (sin(id * (3.141592 / MUSHROOM_COUNT)) + change));
	}
	// Draw the asset
	meshData->draw(drawProgram);
	// Unbind the program used by the asset
	drawProgram->unbind();
}

mat4 Asset::getAssetMatrix()
{
	// Construct the matrices from the asset's
	// transformation data
	mat4 scale = glm::scale(mat4(1.0f), size);
	mat4 translate = glm::translate(mat4(1.0f), position);
	return translate * rotation * scale;
}

bool Asset::isType(AssetType type)
{
	return (this->type == type);
}

AssetType Asset::getType()
{
	return this->type;
}

bool Asset::isRemovable()
{
	return removable;
}

bool Asset::isSolid()
{
	return solid;
}