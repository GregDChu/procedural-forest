#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#ifndef H_CAMERA
#define H_CAMERA

#include "Player.h"

#define PI 3.14159265358
#define E 2.7182818284

#define CONTINUAL_ROTATION 0

#define ROT_SPEED 3.5
#define NO_ROT_ZONE 0.25
//Don't touch!
#define ROTATION_SENSITIVITY 3

#define START_POS_X 0
#define START_POS_Y 1.0
#define START_POS_Z 0

#define START_ROT_X 0
#define START_ROT_Y 0
#define START_ROT_Z 0

//enum MoveType {WALKING, RUNNING, STOPPED};
//enum Direction {X, Y, Z};

class Camera
{

#define PAN_MAT 0
#define TILT_MAT 1
#define MAX_ROT_SENSITIVITY 4

public:

	Camera();
	mat4 update(float currentTime, vec2 mousePos, vec3 direction, Player player);
	void limitMovement(vec3 direction);
	vec3 getPosition();
	vec3 getRotation();
	void unlimitMovement();

private:
	vec3 position;
	vec3 rotation;
	int disabledDirections[2];
	bool movementLimited;
	vec3 collisionRotation;

	float clip(float value, float min, float max);
	mat4 * createRotationMatrices(float elapsedFrames, vec2 mousePos);
	mat4 createTranslationMatrix(float elapsedTime, vec3 direction, mat4 panMatrix, Player player);
};

#endif