#include "Camera.h";

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;

Camera::Camera()
{
	position = vec3(START_POS_X, -START_POS_Y, -START_POS_Z);
	rotation = vec3(START_ROT_X, START_ROT_Y, START_ROT_Z);
	disabledDirections[0] = 0;
	disabledDirections[1] = 0;
	movementLimited = false;
}

float Camera::clip(float value, float min, float max)
{
	if (value > max)
		return max;
	if (value < min)
		return min;
	return value;
}

mat4 * Camera::createRotationMatrices(float elapsedFrames, vec2 mousePos)
{
	float rotationDistance, \
		  distanceScalar;
	mat4 * rMatrix;

	//Adjust mouse values

	//Limit the range of mouse values to
	//just the screen (limits the speed)
	mousePos.x = Camera::clip(mousePos.x, -1.0, 1.0);
	mousePos.y = Camera::clip(mousePos.y, -1.0, 1.0);

	//Refactor mouse position so that there
	//is a rotation 'dead zone' within the
	//center of the screen where no rotation
	//will occur if the mouse is moved away
	//from the origin
	//Note: The size of the dead zone
	//can be set using the NO_ROT_ZONE
	//macro (0.0 for now dead zone up to
	//1.0 for entire screen is a dead zone)
	if (mousePos.x < NO_ROT_ZONE && mousePos.x > -NO_ROT_ZONE)
		mousePos.x = 0;
	else
		(mousePos.x > 0) ? mousePos.x -= NO_ROT_ZONE : mousePos.x += NO_ROT_ZONE;
	if (mousePos.y < NO_ROT_ZONE && mousePos.y > -NO_ROT_ZONE)
		mousePos.y = 0;
	else
		(mousePos.y > 0) ? mousePos.y -= NO_ROT_ZONE : mousePos.y += NO_ROT_ZONE;

	//Calculate tilt rotation (vertical) -- limit to 90 degrees up
	//and 90 degress down so the user won't spin upside down
	rotation = vec3(rotation.x, \
		pow(mousePos.y, ROTATION_SENSITIVITY) * (PI / 2), \
		rotation.y);
	//Calculate pan rotation (horizontal) -- allow full/continuous
	//rotation (additive)
	//if (abs(mousePos.y) < NO_ROT_ZONE) {
		rotationDistance = (ROT_SPEED * elapsedFrames) * (PI / 4);
		distanceScalar = pow(mousePos.x, 2);
		if (mousePos.x < 0)
			distanceScalar *= -1;
		rotation += vec3(distanceScalar * rotationDistance, \
			0, \
			0);
	//}
	//Calculate the rotation matrices
	rMatrix = new mat4[2];
	rMatrix[PAN_MAT] = glm::rotate(mat4(1.0f), rotation.x, vec3(0, 1, 0)); //Rotate in x-direction
	rMatrix[TILT_MAT] = glm::rotate(mat4(1.0f), rotation.y, vec3(1, 0, 0)); //Rotate in y-drection

	return rMatrix;
}

mat4 Camera::createTranslationMatrix(float elapsedFrames, vec3 direction, mat4 panMatrix, Player player)
{
	float travelDistance;
	vec3 movePos;
	//if(player.getSpeed() == 0)
	//	return glm::translate(mat4(1.0f), player.collisionPos);
	/*if (moveType == WALKING)
		travelDistance = elapsedFrames * MOVE_WALK_SPEED;
	else if (moveType == RUNNING)
		travelDistance = elapsedFrames * MOVE_RUN_SPEED;
	else if (moveType == STOPPED)
		travelDistance = 0;*/
	travelDistance = elapsedFrames * player.getSpeed();
	//Isolate the x and z directions
	float moveX, moveZ;
	moveX = moveZ = 0;
	if ((disabledDirections[0] <= 0 && direction.x > 0) || \
		(disabledDirections[0] >= 0 && direction.x < 0))
	{
		moveX = direction.x * travelDistance;
	}
	if ((disabledDirections[1] <= 0 && direction.z > 0) || \
		(disabledDirections[1] >= 0 && direction.z < 0))
	{
		moveZ = direction.z * travelDistance;
	}
	movePos = vec3(moveX, \
		0, \
		moveZ);
	//Apply the camera rotation to the move
	//Note: The inverse of the camera rotation is
	//applied since the world movement is 'opposite'
	//to that of the camera rotation. Also, the tilt
	//is not applied so that the user will not
	//move into the ground, or up into the air
	movePos = inverse(panMatrix) * vec4(movePos, 1);
	//Reincorporate the y direction
	movePos.y = direction.y * travelDistance;
	//Finally, update the camera position
	//using the calculated move
	position += movePos;

	//Calculate the position matrix
	return glm::translate(mat4(1.0f), position);
}

mat4 Camera::update(float currentTime, vec2 mousePos, vec3 direction, Player player)
{
	static float prevTime = 0;
	float timeChange;
	mat4 translatePos, \
		*rotateMatrix; //Of size 2, index using PAN_MAT and TILT_MAT

	// Calculate how much time has passed since last update call
	timeChange = currentTime - prevTime;
	
	// Calculate how much to rotate the camera
	rotateMatrix = createRotationMatrices(timeChange, mousePos);
	if (abs((PI / 2) - abs((rotation - collisionRotation).x)) < 0.001)
		unlimitMovement();
	if ((PI / 2) - abs((rotation - collisionRotation).x) < 0)
	{
		disabledDirections[0] *= -1;
		disabledDirections[1] *= -1;
	}//TODO Fix the issue with backwards non-collision

	// Calculate how much to move the camera
	translatePos = createTranslationMatrix(timeChange, direction, rotateMatrix[PAN_MAT], player);

	// Update previous time to current time for next update call
	// to use
	prevTime = currentTime;

	// Return view matrix using the new position and rotation matrices
	return rotateMatrix[TILT_MAT] * rotateMatrix[PAN_MAT] * translatePos;
}

vec3 Camera::getPosition()
{
	return vec3(-1 * position.x, position.y, -1 * position.z);
}

vec3 Camera::getRotation()
{
	return rotation;
}

void Camera::limitMovement(vec3 direction)
{
	if (!movementLimited)
	{
		disabledDirections[0] = direction.x;
		disabledDirections[1] = direction.z;
		movementLimited = true;
		collisionRotation = rotation;
	}
}

void Camera::unlimitMovement()
{
	movementLimited = false;
	disabledDirections[0] = 0;
	disabledDirections[1] = 0;
}