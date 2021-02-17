#pragma once

#ifndef H_SHAPESTORAGE
#define H_SHAPESTORAGE

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shape.h"

#define DEFAULT_STORAGE_SIZ 20

using namespace std;

class ShapeStorage
{
public:
	ShapeStorage();
	void initShapeType(int type, string objectFilePath);
	shared_ptr<Shape> getShape(int type);
private:
	string * objectTable;
	vector<shared_ptr<Shape>> shapeTable;
	int tableSize;
};

#endif