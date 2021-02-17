/*
ZJ Wood CPE 471 Lab 3 base code
*/

#include <iostream>
#include <glad/glad.h>
//#include <irrKlang.h>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "WindowManager.h"
#include "Shape.h"
#include "Camera.h"
#include "Surface.h"
#include "Asset.h"
#include "Map.h"
#include "MapPopulator.h"
#include "Bag.h"
#include "Player.h"
#include "ShapeStorage.h"
#include "ProgramStorage.h"
#include "sound.h"
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
using namespace glm;
//using namespace irrklang;

//#pragma comment(lib, "irrKlang.lib")

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	//Shape
	shared_ptr<Shape> shape;

	// Our shader program
	std::shared_ptr<Program> prog;

	// Sound Engine
	//ISoundEngine *soundEngine;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our triangle to OpenGL
	GLuint VertexBufferID;

	// Camera
	Camera camera;

	// Player
	Player player;

	// User keyboard direction
	vec3 userKeyboard;

	// User keyboard pick up
	bool userPickup;

	// User marker
	bool placeMarker;

	ShapeStorage shapeStorage;

	ProgramStorage programStorage;

	//texture data
	GLuint grass, ground, glow;

	//Surface ground;

	// User movement type
	//MoveType userMovementType;

	// Music object
	music_ soundObj;
	bool noSound;

	// Sounds
	vector<int> soundSamples;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			userKeyboard += vec3(0, 0, 1);
		}
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			userKeyboard -= vec3(0, 0, 1);
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			userKeyboard += vec3(0, 0, -1);
		}
		if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			userKeyboard -= vec3(0, 0, -1);
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			userKeyboard += vec3(-1, 0, 0);
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			userKeyboard -= vec3(-1, 0, 0);
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			userKeyboard += vec3(1, 0, 0);
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			userKeyboard -= vec3(1, 0, 0);
		}
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
		{
			player.setMovementType(player.RUNNING);
		}
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
		{
			player.setMovementType(player.WALKING);
		}
		if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
		{
			placeMarker = true;
		}
	}

	// callback for the mouse when clicked move the triangle when helper functions
	// written
	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT &&
			action == GLFW_PRESS)
		{
			userPickup = true;
		}
		/*if (button == GLFW_MOUSE_BUTTON_RIGHT &&
			action == GLFW_PRESS)
		{
			userMovementType = RUNNING;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT
			&& action == GLFW_RELEASE)
		{
			userMovementType = WALKING;
		}*/
		/*double posX, posY;

		glfwGetCursorPos(window, &posX, &posY);
		mousePos.x = (float)posX;
		mousePos.y = (float)posY;
		*/
		/*
		float newPt[2];
		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			mousePos.x = (float)posX;
			mousePos.y = (float)posY;
			std::cout << "Pos X " << posX <<  " Pos Y " << posY << std::endl;

			//change this to be the points converted to WORLD
			//THIS IS BROKEN< YOU GET TO FIX IT - yay!
			newPt[0] = 0;
			newPt[1] = 0;

			std::cout << "converted:" << newPt[0] << " " << newPt[1] << std::endl;
			glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
			//update the vertex array with the updated points
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*6, sizeof(float)*2, newPt);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}*/
	}

	//if the window is resized, capture the new size and reset the viewport
	void resizeCallback(GLFWwindow *window, int in_width, int in_height)
	{
		//get the window size - may be different then pixels for retina
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}

	/*Note that any gl calls must always happen after a GL state is initialized */
	void initGeom()
	{
		string resourceDirectory = "../resources";
		// Initialize mesh.
		shape = make_shared<Shape>();
		shape->loadMesh(resourceDirectory + "/sphere.obj");
		shape->resize();
		shape->init();

		int width, height, channels;
		char filepath[1000];

		//texture 1
		//TODO Implement this inside the class
		string str = "../resources/grasspatch1.png";
		strcpy(filepath, str.c_str());
		unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &grass);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, grass);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		GLuint grassTextureLoc = glGetUniformLocation(programStorage.getProgram(GRASS)->pid, "grassTexture");
		glUseProgram(programStorage.getProgram(GRASS)->pid);
		glUniform1i(grassTextureLoc, 0);


		//texture 2
//TODO Implement this inside the class
		str = "../resources/ground_text.jpg";
		strcpy(filepath, str.c_str());
		data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &ground);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ground);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		GLuint groundTextureLoc = glGetUniformLocation(programStorage.getProgram(10)->pid, "groundTexture");
		glUseProgram(programStorage.getProgram(10)->pid);
		glUniform1i(groundTextureLoc, 1);


		str = "../resources/glow.png";
		strcpy(filepath, str.c_str());
		data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &glow);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, glow);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		GLuint glowTextureLoc = glGetUniformLocation(programStorage.getProgram(GLOW)->pid, "glowTexture");
		glUseProgram(programStorage.getProgram(GLOW)->pid);
		glUniform1i(glowTextureLoc, 2);
		/*int w, h, channels;
		char filepath[1000];


		//texture 1
		GLuint TB, TL;
		string str = "../resources/noise.tif";
		strcpy(filepath, str.c_str());
		//unsigned char* data = stbi_load(filepath, &w, &h, &channels, 4);
		unsigned char* data;
		glGenTextures(1, &TB);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TB);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		TL = glGetUniformLocation(ground.getProgram()->pid, "surfaceTexture");
		// Then bind the uniform samplers to texture units:
		glUseProgram(ground.getProgram()->pid);
		glUniform1i(TL, 0);*/

	}

	//General OGL initialization - set OGL state here
	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(1, 1, 1, 1.0f);
		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);
		// Polygon mode
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		// Initialize the GLSL program.
		prog = std::make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/shader_vertex.glsl", resourceDirectory + "/shader_fragment.glsl");
		prog->init();
		prog->addUniform("P");
		prog->addUniform("V");
		prog->addUniform("M");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");
		prog->addAttribute("vertText");

		camera = Camera();
		userKeyboard = vec3(0, 0, 0);
		userPickup = false;
		placeMarker = false;
		player = Player();
		shapeStorage = ShapeStorage();
		shapeStorage.initShapeType(MUSHROOM, "../resources/mushroom.obj");
		shapeStorage.initShapeType(TREE, "../resources/tree.obj");
		shapeStorage.initShapeType(MARKER, "../resources/cone.obj");
		shapeStorage.initShapeType(ARROW, "../resources/arrow.obj");
		shapeStorage.initShapeType(BUSH, "../resources/bush.obj");
		shapeStorage.initShapeType(ROCK, "../resources/sphere.obj");
		programStorage = ProgramStorage();
		vector<string> grassUniforms;
		grassUniforms.push_back("fogDepth");
		grassUniforms.push_back("grassTexture");
		programStorage.initProgram(GRASS, "../resources/grass_vertex_shader.glsl", "../resources/grass_frag_shader.glsl", grassUniforms);
		vector<string> glowUniforms;
		grassUniforms.push_back("glowTexture");
		programStorage.initProgram(GLOW, "../resources/glow_vertex_shader.glsl", "../resources/glow_frag_shader.glsl", glowUniforms);
		vector<string> mushroomUniforms;
		mushroomUniforms.push_back("fogDepth");
		mushroomUniforms.push_back("colorType");
		programStorage.initProgram(MUSHROOM, "../resources/mush_vertex_shader.glsl", "../resources/mush_frag_shader.glsl", mushroomUniforms);
		vector<string> treeUniforms;
		treeUniforms.push_back("fogDepth");
		programStorage.initProgram(TREE, "../resources/tree_vertex_shader.glsl", "../resources/tree_frag_shader.glsl", treeUniforms);
		vector<string> markerUniforms;
		markerUniforms.push_back("fogDepth");
		programStorage.initProgram(MARKER, "../resources/marker_vertex_shader.glsl", "../resources/marker_frag_shader.glsl", markerUniforms);
		vector<string> bushUniforms;
		bushUniforms.push_back("fogDepth");
		programStorage.initProgram(BUSH, "../resources/bush_vertex_shader.glsl", "../resources/bush_frag_shader.glsl", bushUniforms);
		vector<string> rockUniforms;
		rockUniforms.push_back("fogDepth");
		programStorage.initProgram(ROCK, "../resources/rock_vertex_shader.glsl", "../resources/rock_frag_shader.glsl", rockUniforms);
		vector<string> groundUniforms;
		groundUniforms.push_back("fogDepth");
		groundUniforms.push_back("offset");
		groundUniforms.push_back("cameraPos");
		groundUniforms.push_back("fogDepth");
		groundUniforms.push_back("nearestWater");
		groundUniforms.push_back("waterHoleRadius");
		groundUniforms.push_back("waterHoleDepth");
		groundUniforms.push_back("ratio");
		groundUniforms.push_back("density");
		//TODO Fix this hardcoded 10 to a GROUND macro/enum
		programStorage.initProgram(10, "../resources/ground_vertex_shader.glsl", "../resources/ground_frag_shader.glsl", groundUniforms);
		vector<string> arrowUniforms;
		arrowUniforms.push_back("fogDepth");
		programStorage.initProgram(ARROW, "../resources/arrow_vertex_shader.glsl", "../resources/arrow_frag_shader.glsl", arrowUniforms);

		soundSamples.push_back(soundObj.init_music("../resources/bird-forest.wav"));
		soundObj.play(soundSamples.at(0));
		soundSamples.push_back(soundObj.init_music("../resources/forest-amb.wav"));
		//int z;
		//z = 0;
		//soundEngine = createIrrKlangDevice();
		//if (!soundEngine)
		//	cout << "Error loading IrrKlang sound engine..." << endl;
	}


	/****DRAW
	This is the most important function in your program - this is where you
	will actually issue the commands to draw any geometry you have set up to
	draw
	********/
	void render()
	{
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		float aspect = width / (float)height;
		float time = (float)glfwGetTime();
		glViewport(0, 0, width, height);


		// Read and store mouse position data
		vec2 mousePos;
		double mouseX, mouseY;
		//Get mouse position in pixels (origin at upper left corner of
		//application window
		glfwGetCursorPos(windowManager->getHandle(), &mouseX, &mouseY);
		//Convert mouse pixel position to world-space position
		mousePos = vec2((mouseX / (width / 2)) - 1, (mouseY / (height / 2)) - 1);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Create the matrix stacks - please leave these alone for now

		glm::mat4 V, M, P;
		//V = glm::mat4(1);
		V = glm::mat4(1);
		M = glm::mat4(1);
		P = P = glm::perspective((float)(3.14159 / 4.), (float)((float)width / (float)height), 0.1f, 1000.0f);

		static float theta = 0;
		theta += 0.01;
		static bool randSeeded = false;
		if (!randSeeded)
		{
			float rand_time = glfwGetTime() * 100;
			srand((int)rand_time);
			randSeeded = true;
		}
		static Bag bag = Bag();
		static Map map = Map(MAP_RANGE/2);
		static MapPopulator mp = MapPopulator();
		//TODO Implement unique surface shaders

		static Surface ground = Surface(GROUND, "ground_vertex_shader.glsl", "ground_frag_shader.glsl", "noise.tif", MAP_RANGE, 220);
		static Surface water = Surface(WATER, "water_vertex_shader.glsl", "water_frag_shader.glsl", "", MAP_RANGE, 220);
		water.translateSurface(vec3(0, -0.1, 0));

		ground.setProgram(programStorage.getProgram(10));
		ground.update(camera.getPosition());
		vec3 closestWater = ground.getClosestWater(camera.getPosition());

		static vector<Surface> mushroomGlows;
		static bool placedMushrooms = false;
		if (!placedMushrooms) {
			int mushPerRow = sqrt(MUSHROOM_COUNT);
			for (int i = 0; i < MUSHROOM_COUNT; i++)
			{
				vec2 mushPosition = vec2((i % mushPerRow) * MUSH_GRID_SPACING, (i / mushPerRow) * MUSH_GRID_SPACING);
				mushPosition.x += (MUSH_GRID_SPACING / 4) - ((MUSH_GRID_SPACING / 2) * ((float)rand() / RAND_MAX));
				mushPosition.y += (MUSH_GRID_SPACING / 4) - ((MUSH_GRID_SPACING / 2) * ((float)rand() / RAND_MAX));
				shared_ptr<Program> program = programStorage.getProgram(MUSHROOM);
				Asset mush = Asset(program, MUSHROOM, shapeStorage.getShape(MUSHROOM));
				mush.setPosition(mushPosition.x, 0.3, mushPosition.y);
				mush.uniformScale(0.1);
				mush.setSpacingRadius(0.1);
				//tree.nonUniformScale(0.1, 4, 0.1);
				//tree.setPosition(tree.getPosition().x, 5.7, tree.getPosition().z);
				map.addItem(mush);
				Surface glowA = Surface(BILLBOARD, "", "", "../resources/meh.png", 1.0, 0);
				Surface glowB = Surface(BILLBOARD, "", "", "../resources/meh.png", 1.0, 0);
				//float theta = PI * 2 * ((float)rand() / RAND_MAX);
				glowA.translateSurface(vec3(mushPosition.x, 0.3, mushPosition.y));
				glowA.scaleSurface(vec3(0.3, 0.3, 0.3));
				unsigned int id = map.mapAssets.at(map.mapAssets.size() - 1).getID();
				glowA.setID(id);
				mushroomGlows.push_back(glowA);
				/*
				glowB.translateSurface(vec3(mushPosition.x, 0.3, mushPosition.y));
				glowB.scaleSurface(vec3(0.3, 0.3, 0.3));
				glowB.rotateSurface((PI / 2), Y_AXIS);
				mushroomGlows.push_back(glowB);*/
				
			}
			placedMushrooms = true;
		}
		else
		{
			float closestMushroomDistance = map.range;
			for (int i = 0; i < map.mapAssets.size(); i++)
			{
				Asset asset = map.mapAssets.at(i);
				if (asset.isType(MUSHROOM))
				{
					float dist = abs(distance(camera.getPosition(), asset.getPosition()));
					if (dist < closestMushroomDistance)
						closestMushroomDistance = dist;
				}
			}
			if (closestMushroomDistance < NO_SOUND_ZONE)
			{
				noSound = true;
				soundObj.fade_out(soundSamples.at(0), 4000);
				if (!soundObj.getTrack(soundSamples.at(1))->is_running())
					soundObj.play(soundSamples.at(1));
			}
			else
			{
				noSound = false;
			}
		}
		//TODO Fix bugs here when it clears and another asset gets placed there that wasn't there before
		//map.clearAssetType(TREE);
		//map.clearAssetType(ROCK);
		//map.clearAssetType(BUSH);
		// Populate map with trees
		vector<vec2> fillLocations = map.emptyAssetLocations(camera.getPosition(), TREE_GRID_SPACING, TREE, closestWater);
		for (int i = 0; i < fillLocations.size(); i++)
		{
			vec2 treePosition = fillLocations.at(i);
			shared_ptr<Program> program = programStorage.getProgram(TREE);
			Asset tree = Asset(program, TREE, shapeStorage.getShape(TREE));
			float posVariation = ((float)rand() / RAND_MAX);
			tree.setPosition(treePosition.x, 5.7, treePosition.y);
			tree.nonUniformScale(3.0, 6.0, 3.0);
			tree.setSpacingRadius(3.0);
			//tree.nonUniformScale(0.1, 4, 0.1);
			//tree.setPosition(tree.getPosition().x, 5.7, tree.getPosition().z);
			map.addItem(tree);
		}
		// Populate map with bushes
		fillLocations = map.emptyAssetLocations(camera.getPosition(), BUSH_GRID_SPACING, BUSH, closestWater);
		for (int i = 0; i < fillLocations.size(); i++)
		{
			vec2 bushPosition = fillLocations.at(i);
			shared_ptr<Program> program = programStorage.getProgram(BUSH);
			Asset bush = Asset(program, BUSH, shapeStorage.getShape(BUSH));
			//float posVariation = ((float)rand() / RAND_MAX);
			bush.setPosition(bushPosition.x, 0, bushPosition.y);
			bush.uniformScale(0.4);
			bush.setSpacingRadius(0.5);
			bush.setPosition(bush.getPosition().x, 0.3, bush.getPosition().z);
			map.addItem(bush);
		}
		// Populate map with rocks
		fillLocations = map.emptyAssetLocations(camera.getPosition(), ROCK_GRID_SPACING, ROCK, closestWater);
		for (int i = 0; i < fillLocations.size(); i++)
		{
			vec2 rockPosition = fillLocations.at(i);
			shared_ptr<Program> program = programStorage.getProgram(ROCK);
			Asset rock = Asset(program, ROCK, shapeStorage.getShape(ROCK));
			//float posVariation = ((float)rand() / RAND_MAX);
			srand(((int)(rockPosition.x) % 7)  + ((int)rockPosition.y % 11));
			rock.setPosition(rockPosition.x, abs(sin((float)rand() / RAND_MAX * 2 * 3.1415) * 100) - 0.5, rockPosition.y);
			rock.uniformScale(1.5);
			rock.setSpacingRadius(1.5);
			//rock.setPosition(rock.getPosition().x, -2.0, rock.getPosition().z);
			map.addItem(rock);
		}
		/*if (player.detectCollision(map, camera.getPosition())) {
			camera.limitMovement(userKeyboard);
		}
		else
		{
			camera.unlimitMovement();
		}*/
		V = camera.update(time, mousePos, userKeyboard, player);
		//player.setMovementType(player.WALKING);

		map.drawMap(P, V, camera.getPosition());

		if (userPickup) {
			userPickup = false;
			unsigned int id = map.closestInteractiveAsset(camera.getPosition());//TODO implement this, set attrib of pickupable
			if (id > 0)
			{
				bag.addItem(map, id);
				map.removeItem(id);
				for (int i = 0; i < mushroomGlows.size(); i++)
				{
					if (mushroomGlows.at(i).getID() == id)
					{
						mushroomGlows.erase(mushroomGlows.begin() + i);
						break;
					}
				}
			}
		}
		
		/*map.drawMap(P, V);*/

		static Asset marker = Asset(programStorage.getProgram(MARKER), MARKER, shapeStorage.getShape(MARKER));
		static Asset arrow = Asset(programStorage.getProgram(ARROW), ARROW, shapeStorage.getShape(ARROW));
		if (placeMarker)
		{
			vec3 userPos = camera.getPosition();
			marker.setPosition(userPos.x + sin(camera.getRotation().x), \
				-(userPos.y), \
				userPos.z - cos(camera.getRotation().x));
			marker.nonUniformScale(0.1, 0.2, 0.1);
			marker.rotate(3.14159265, X_AXIS);
			placeMarker = false;
			bag.markerUsed = true;
		}
		if (bag.markerUsed) {
			double xDist = marker.getPosition().x - camera.getPosition().x;
			double dist = abs(distance(marker.getPosition(), camera.getPosition()));
			int sign = 1;
			if (marker.getPosition().z > camera.getPosition().z)
			{
				sign = -1;
			}
			double theta = acos(xDist / dist);
			double cameraTheta = camera.getRotation().x;
			cout << xDist << "  " << dist << " " << camera.getPosition().z << endl;
			cout << "Theta: " << theta << endl;
			arrow.rotate(sign * theta + cameraTheta, Y_AXIS);
			arrow.uniformScale(0.3);
			arrow.setPosition(0, -0.5, -0.4);
			//arrow.setPosition(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
			arrow.drawAsset(P, V);
			marker.drawAsset(P, V);
		}
		ground.drawSurface(P, V, camera.getPosition());
		water.drawSurface(P, V, camera.getPosition());
		// Populate with grass
		static vector<Surface> grassList;
		if (grassList.size() <= 0)
		{
			for (int x = 0; x < FOG_DEPTH * GRASS_GRID_DENSITY * 2; x++)
			{
				for (int z = 0; z < FOG_DEPTH * GRASS_GRID_DENSITY * 2; z++)
				{
					float noise = fract(sin(dot(normalize(vec2(x + 11, z + 7)), vec2(36.636, 7.1146))) * 2.172369238) * 0.5;
					noise += fract(sin(dot(normalize(vec2(x + 43, z + 51)), vec2(6.689, 2.6176))) * 6.17293938) * 0.5;
					if (pow(noise, 3) < 0.5)
						continue;
					float variation = noise * (GRASS_GRID_DENSITY / 4);
					Surface grassA = Surface(BILLBOARD, "", "", "../resources/meh.png", 1.0, 0);
					Surface grassB = Surface(BILLBOARD, "", "", "../resources/meh.png", 1.0, 0);
					float theta = PI * 2 * ((float)rand() / RAND_MAX);
					grassA.translateSurface(vec3((x / GRASS_GRID_DENSITY) - FOG_DEPTH + variation, 0.6, (z/ GRASS_GRID_DENSITY) - FOG_DEPTH) - variation);
					grassA.scaleSurface(vec3(0.3, 0.3, 0.3));
					grassA.rotateSurface(theta, Y_AXIS);
					grassList.push_back(grassA);

					grassB.translateSurface(vec3((x / GRASS_GRID_DENSITY) - FOG_DEPTH + variation, 0.6, (z / GRASS_GRID_DENSITY) - FOG_DEPTH) - variation);
					grassB.scaleSurface(vec3(0.3, 0.3, 0.3));
					grassB.rotateSurface(theta + (PI / 2), Y_AXIS);
					grassList.push_back(grassB);
				}
			}
		}
		for (int i = 0; i < grassList.size(); i++)
		{
			Surface grass = grassList.at(i);
			/*if (abs(distance(grass.getPosition(), camera.getPosition())) > map.range)
			{
				grassList.erase(grassList.begin + i);
				Surface newGrass = Surface(BILLBOARD, "", "", "../resources/meh.png", 1.0, 0);
				grassList[i] = 
			}*/
			//glDisable(GL_DEPTH_TEST);
			//TODO Use variation array for this instead of creating a bunch of surfaces
			grass.drawBillboard(P, V, camera.getPosition(), programStorage.getProgram(GRASS), GRASS);
			//glEnable(GL_DEPTH_TEST);
		}
		for (int i = 0; i < mushroomGlows.size(); i++)
		{
			Surface glow = mushroomGlows.at(i);
			glow.drawBillboard(P, V, camera.getPosition(), programStorage.getProgram(GLOW), GLOW);
		}

/*		for (int i = 0; i < 10; i++)
		{
			Surface grass = Surface(BILLBOARD, "", "", "../resources/grass1.png", 1.0, 0);
			
			
			
		}*/
	}

};
//******************************************************************************************
int main(int argc, char **argv)
{
	std::string resourceDir = "../resources"; // Where the resources are loaded from
	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	/* your main will always include a similar set up to establish your window
		and GL context, etc. */
	WindowManager * windowManager = new WindowManager();
	windowManager->init(1920, 1080);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	/* This is the code that will likely change program to program as you
		may need to initialize or set up different data and state */
	// Initialize scene.
	application->init(resourceDir);
	application->initGeom();
	track_* backgroundTrack = application->soundObj.getTrack(application->soundSamples.at(0));

	// Play background sounds
	//application->soundEngine->play2D("../resources/forest-amb.wav", true);

	// Loop until the user closes the window.
	while(! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Check sound and repeat
		if (!backgroundTrack->is_running() && !application->noSound)
		{
			application->soundObj.play(application->soundSamples.at(0));
		}

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}
	// Quit program.
	//application->soundEngine->drop();
	windowManager->shutdown();
	return 0;
}
