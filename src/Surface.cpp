#include "Surface.h"

Surface::Surface(SurfaceType type, string vShader, string fShader, string texture, float range, int density)
{
	// Set surface type
	this->type = type;
	// Set surface shaders
	vertexShader = vShader;
	fragmentShader = fShader;
	// Set texture
	this->texture = texture;
	// Set surface range
	this->range = range;
	// Set suraface position
	position = vec3(0, 0, 0);
	// Set surface matrices
	scale = mat4(1.0f);
	rotate = mat4(1.0f);
	translate = mat4(1.0f);
	parentModelMatrix = mat4(1.0f);
	// Create surface mesh
	generateSurfaceMesh(range, range, density);
	// Create shader program
	if (type != BILLBOARD) {
		program = std::make_shared<Program>();
		program->setVerbose(true);
		program->setShaderNames(SHADER_RESOURCE_PATH + vertexShader, SHADER_RESOURCE_PATH + fragmentShader);
		program->init();
		program->addUniform("P");
		program->addUniform("V");
		program->addUniform("M");
		program->addUniform("offset");
		program->addUniform("cameraPos");
		program->addUniform("fogDepth");
		program->addUniform("nearestWater");
		program->addUniform("waterHoleRadius");
		program->addUniform("waterHoleDepth");
		program->addAttribute("vertPos");
		program->addAttribute("vertNor");
		program->addAttribute("vertText");
	}
	else
	{
		// Get vertical
		rotate = glm::rotate(mat4(1.0f), (float)(PI / 2), vec3(1, 0, 0));
	}
}

void Surface::generateSurfaceMesh(float width, float length, int density)
{

	// Calculate how much screen space is between
	// each vertex (based on density)
	widthPerRect = width / (density + 1);
	lengthPerRect = length / (density + 1);
	this->density = density;
	// Generate the vertices and store them
	// in the vertex array
	for (int z = 0; z < (density + 2); z ++)
	{
		for (int x = 0; x < (density + 2); x ++)
		{
			float xCoord = (x * widthPerRect) - (width / 2);// X coordinate
			float yCoord = 0.0;			    // Y coordinate
			float zCoord = z * lengthPerRect - (length / 2);// Z coordinate
			// Store the vertex coordinates
			vertices.push_back(xCoord);
			vertices.push_back(yCoord);
			vertices.push_back(zCoord);
	// Generate the normals for each vertex and
	// store them in the normals array
			normals.push_back(0); // X normal
			normals.push_back(1.0); // Y normal
			normals.push_back(0); // Z normal
		}
	}
	// Generate the indices for each face and
	// store them in the index array
	for (int z = 0; z < (density + 1); z++)
	{
		for (int x = 0; x < (density + 1); x++)
		{
			int rowStartIndex = z * (density + 2);
			indices.push_back(rowStartIndex + x);
			indices.push_back(rowStartIndex + x + 1);
			indices.push_back(rowStartIndex + density + 2 + x);
			indices.push_back(rowStartIndex + density + 2 + x);
			indices.push_back(rowStartIndex + x + 1);
			indices.push_back(rowStartIndex + density + 3 + x);
		}
	}

	// Check for surface type and add textures depending on type
	if (type == BILLBOARD)
	{
		textures.push_back(0.0);
		textures.push_back(0.0);

		textures.push_back(1.0);
		textures.push_back(0.0);

		textures.push_back(0.0);
		textures.push_back(1.0);

		textures.push_back(1.0);
		textures.push_back(1.0);
	}

	int bufferOffset;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Prepare the array buffer for all mesh data
	bufferOffset = 0;
	glBufferData(GL_ARRAY_BUFFER, (vertices.size() * sizeof(GLfloat)) + \
		(normals.size() * sizeof(GLfloat)) + \
		(textures.size() * sizeof(GLfloat)), \
		0, GL_DYNAMIC_DRAW);
	// Store all vertices and assign them to an attribute
	glBufferSubData(GL_ARRAY_BUFFER, bufferOffset, vertices.size() * sizeof(GLfloat), &vertices[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0);
	bufferOffset += vertices.size() * sizeof(GLfloat);
	// Store all normals and assign them to an attribute
	glBufferSubData(GL_ARRAY_BUFFER, bufferOffset, normals.size() * sizeof(GLfloat), &normals[0]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const void*)bufferOffset);
	bufferOffset += normals.size() * sizeof(GLfloat);
	// Store all texture coordinates and assign them to an attribute
	if (textures.size() > 0)
	{
		glBufferSubData(GL_ARRAY_BUFFER, bufferOffset, textures.size() * sizeof(GLfloat), &textures[0]);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const void*)bufferOffset);
	}

	// Generate an element array buffer
	glGenBuffers(1, &EB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB);
	// Store all element indices in the element buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_DYNAMIC_DRAW);
	//generateWaterHole(0);
}

void Surface::drawBillboard(mat4 P, mat4 V, vec3 camPosition, shared_ptr<Program> program, int type)
{
	mat4 M;
	program->bind();

	//parentModelMatrix * translate * rotate * 
	M = getModelMatrix(); //* rotateVertical * scale;
	//M = getModelMatrix();

	glUniformMatrix4fv(program->getUniform("P"), 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(program->getUniform("V"), 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(program->getUniform("M"), 1, GL_FALSE, &M[0][0]);

	if (type == GLOW)
	{
		static float change = 0.0;
		change += 0.001;
		//TODO This divide by three is BAD, get rid of this hard coding
		glm::mat4 MToView = glm::transpose(V);
		MToView[0][3] = 0;
		MToView[1][3] = 0;
		MToView[2][3] = 0;
		MToView[3][0] = 0;
		MToView[3][1] = 0;
		MToView[3][2] = 0;
		
		//mat4 THover = glm::translate(mat4(1.0f), vec3(0, sin(change * 3) / 3, 0));
		//M *= glm::translate(mat4(1.0f), vec3(0 ,0, sin(change * 3)/ 3));
		mat4 R = glm::rotate(mat4(1.0f), 1.0f, vec3(0, 1, 0));
		//translate = glm::translate(mat4(1.0f), position);
		M = parentModelMatrix * translate * MToView * rotate * scale;
		glUniformMatrix4fv(program->getUniform("M"), 1, GL_FALSE, &M[0][0]);
	}
	else
	{
		glUniform1f(program->getUniform("fogDepth"), FOG_DEPTH);
	}

	// Bind the surface's vertex array object
	glBindVertexArray(VAO);
	// Bind the surface's element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB);
	// Draw the surface
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
	// Unbind the current program used to render the surface

	program->unbind();
}

void Surface::drawSurface(mat4 P, mat4 V, vec3 camPosition)
{
	mat4 M;
	vec3 offset, offsetFloat;

	// Bind the current program used to render the surface
	program->bind();
	// Init the surface's model matrix
	M = getModelMatrix();
	// Get camera offset to emulate infinite terrain
	offset = vec3((int)camPosition.x, 0, (int)camPosition.z);

	//fprintf(stderr, "Offset: (%f, %f)\n", offset.x, offset.z);
	// Get the position of the nearest water hole
	vec3 nearestWaterPosition = vec3(range + camPosition.x, 0, range + camPosition.z);//vec3(FOG_DEPTH + WATER_HOLE_RADIUS + camPosition.x, 0, camPosition.z);
	for(int i = 0; i < waterPositions.size(); i++)
	{
		vec3 waterPos = waterPositions.at(i);
		float tempDistance = distance(vec3(camPosition.x, 0, camPosition.z), \
			waterPos);
		if (tempDistance <= abs(distance(vec3(camPosition.x, 0, camPosition.z), nearestWaterPosition)))
			nearestWaterPosition = waterPos;
	}
	//fprintf(stderr, "Distance to nearest water pos: %f\n", abs(distance(vec3(position.x, 0, position.z), nearestWaterPosition)));
	// Assign the appropriate matrices to the corresponding
	// shader uniform variable
	glUniformMatrix4fv(program->getUniform("P"), 1, GL_FALSE, &P[0][0]);
	glUniformMatrix4fv(program->getUniform("V"), 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(program->getUniform("M"), 1, GL_FALSE, &M[0][0]);
	if (type != GRASS) {
		glUniform3fv(program->getUniform("offset"), 1, &offset[0]);
		glUniform3fv(program->getUniform("cameraPos"), 1, &camPosition[0]);
		if (type == GROUND) {
			glUniform1f(program->getUniform("fogDepth"), FOG_DEPTH);
			glUniform3fv(program->getUniform("nearestWater"), 1, &nearestWaterPosition[0]);
			glUniform1f(program->getUniform("waterHoleRadius"), WATER_HOLE_RADIUS);
			glUniform1f(program->getUniform("waterHoleDepth"), WATER_HOLE_DEPTH);
			glUniform1f(program->getUniform("ratio"), lengthPerRect);
			glUniform1f(program->getUniform("density"), (density + 1.0));
		}
	}
	// Bind the surface's vertex array object
	glBindVertexArray(VAO);
	// Bind the surface's element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB);
	// Draw the surface
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
	// Unbind the current program used to render the surface
	program->unbind();
}

void Surface::setParentModelMatrix(mat4 parentMatrix)
{
	parentModelMatrix = parentMatrix;
}

mat4 Surface::getModelMatrix()
{
	translate = glm::translate(mat4(1.0f), position);
	return parentModelMatrix * translate * rotate * scale;
}

//
//void Surface::generateWaterHole(float theta, vec3 pos)
//{
//	/*float rand_theta = ((float)rand() / RAND_MAX) * 2 * 3.141592;
//	vec2 waterPos = vec2(WATER_HOLE_SPREAD_RADIUS * cos(rand_theta), \
//		WATER_HOLE_SPREAD_RADIUS * sin(rand_theta));*/
//	float variation = ((((float)rand() / RAND_MAX) * 2) - 1.0);
//	vec2 waterPos = vec2((WATER_HOLE_SPREAD_RADIUS * cos(theta + variation)) + pos.x, \
//		(WATER_HOLE_SPREAD_RADIUS * sin(theta + variation)) + pos.z);
//	holes.push_back(waterPos);
//	cout << "Generated water hole at: (" << waterPos.x << ", " << waterPos.y << ")" << endl;
//
//	for(int i = 0; i < vertices.size() / 3; i++)
//	{
//		float xCoord = vertices.at((i * 3));
//		float zCoord = vertices.at((i * 3) + 2);
//		float yCoord;
//		float distToWaterPos = distance(vec2(xCoord, zCoord), waterPos);
//		if (distToWaterPos <= WATER_HOLE_RADIUS) {
//			//yCoord = -WATER_HOLE_DEPTH * pow((WATER_HOLE_RADIUS - abs(distToWaterPos)) / WATER_HOLE_RADIUS, 0.5);
//			yCoord = -WATER_HOLE_DEPTH * ((1.0 - tanh(10 * (abs(distToWaterPos/WATER_HOLE_RADIUS) - 0.7))) / 2);
//			vertices[(i * 3) + 1] = yCoord;
//
//			float xDist, zDist;
//			xDist = distance(xCoord, waterPos.x);
//			zDist = distance(zCoord, waterPos.y);
//
//			float zPlaneTheta = atan(xDist / abs(yCoord));
//			float yPlanetheta = atan(zDist / abs(xDist));
//			normals[(i * 3)] = (sin(zPlaneTheta) * pow(abs(yCoord) / WATER_HOLE_DEPTH, 0.5));
//			normals[(i * 3) + 1] = ((cos(zPlaneTheta) * (abs(yCoord) / WATER_HOLE_DEPTH)) + \
//				(1.0 * ((WATER_HOLE_DEPTH + yCoord) / WATER_HOLE_DEPTH)));
//			normals[(i * 3) + 2] = (sin(yPlanetheta) * pow(abs(yCoord) / WATER_HOLE_DEPTH, 0.5));
//		}
//	}
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), &vertices[0]);
//}

void Surface::translateSurface(vec3 position)
{
	this->position = position;
	//translate = glm::translate(mat4(1.0f), position);
}

void Surface::scaleSurface(vec3 scale)
{
	this->scale = glm::scale(mat4(1.0f), scale);
}

void Surface::rotateSurface(float theta, Axis axis)
{
	vec3 axisVec;
	if (axis == X_AXIS)
		axisVec = vec3(1, 0, 0);
	if (axis == Y_AXIS)
		axisVec = vec3(0, 1, 0);
	if (axis == Z_AXIS)
		axisVec = vec3(0, 0, 1);
	rotate = glm::rotate(mat4(1.0f), theta, axisVec) * rotate;
}

shared_ptr<Program> Surface::getProgram()
{
	return program;
}

void Surface::setProgram(shared_ptr<Program> program)
{
	this->program = program;
}

vec3 Surface::getPosition()
{
	return position;
}

void Surface::update(vec3 position)
{
	// For implementing infinite terrain
	//this->position = vec3((int)position.x, this->position.y, (int)position.z);
	static vec3 prevPos = vec3(0, 0, 0);

	if (type == GROUND)
	{
		float minWaterSpacing = WATER_HOLE_SPREAD_RADIUS;
		float nextWaterDistance = minWaterSpacing + 1.0;
		int i = 0;

		while(i < waterPositions.size() && nextWaterDistance > minWaterSpacing)
		{
			vec3 waterPos = waterPositions.at(i);
			nextWaterDistance = distance(vec3(position.x, 0, position.z), \
				waterPos);
			i++;
		}
		if (nextWaterDistance > minWaterSpacing && i == waterPositions.size())
		{
			float xDist = abs(position.x - prevPos.x);
			float zDist = abs(prevPos.z - position.z);
			float theta;
			//if (xDist == 0)
			//{
			//	theta = PI / 2;
			//	/*if (zDist < 0)
			//		theta *= -1;*/
			//}
			//else {
			//	theta = atan(zDist / xDist);
			//}
			if (position.x == 0)
				theta = ((float)rand() / RAND_MAX);
			else
				theta = atan(position.z / position.x);
			fprintf(stderr, "Theta: %f\n", theta);
			generateNewWaterPosition(position, theta);
		}
		prevPos = position;
	}

	//if (type == GROUND)
	//{
	//	float nearestWater = 2 * (WATER_HOLE_SPREAD_RADIUS + WATER_HOLE_RADIUS);
	//	float theta = 0;
	//	for (int i = 0; i < holes.size(); i++)
	//	{
	//		vec2 waterPos = holes.at(i);
	//		float tempDist = distance(vec2(position.x, position.z), \
	//			waterPos);
	//		if (tempDist < nearestWater)
	//		{
	//			nearestWater = tempDist;
	//			theta = atan((waterPos.y - position.z) / (position.x - waterPos.x));
	//		}
	//	}
	//	if (nearestWater >= 2 * (WATER_HOLE_SPREAD_RADIUS + WATER_HOLE_RADIUS))
	//		generateWaterHole(theta, position);
	//}
}

void Surface::generateNewWaterPosition(vec3 position, float theta)
{
	srand(350);
	float random_theta = ((float)rand() / RAND_MAX) * WATER_POS_VARIATION;
	float newWaterPosDistance = NEW_WATER_HOLE_RADIUS;
	/*vec3 newWaterPos = vec3(cos(random_theta + theta), // X coordinate
							0, // Y coordinate
							sin(random_theta + theta)); // Z coordinate*/
	vec3 newWaterPos = vec3(cos(theta), // X coordinate
		0, // Y coordinate
		sin(theta)); // Z coordinate
	newWaterPos *= newWaterPosDistance;
	newWaterPos.x += position.x;
	newWaterPos.z += position.z;
	waterPositions.push_back(newWaterPos);
	fprintf(stderr, "New water hole generated at (%f, %f)\n", newWaterPos.x, newWaterPos.z);
}

vec3 Surface::getClosestWater(vec3 position)
{
	vec3 groundPos = vec3(position.x, 0, position.z);
	vec3 closestPos = vec3(FOG_DEPTH, FOG_DEPTH, 0); // A place holder
	float closestWaterDistance = WATER_HOLE_SPREAD_RADIUS;
	for (int i = 0; i < waterPositions.size(); i++)
	{
		vec3 currentWaterPosition = waterPositions.at(i);
		float waterDistance = abs(distance(currentWaterPosition, groundPos));
		if (waterDistance <= closestWaterDistance)
		{
			closestWaterDistance = waterDistance;
			closestPos = currentWaterPosition;
		}
	}
	return closestPos;
}

void Surface::setID(int id)
{
	this->id = id;
}

int Surface::getID()
{
	return id;
}