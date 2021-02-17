#include "ShapeStorage.h"

ShapeStorage::ShapeStorage()
{
	objectTable = new string[DEFAULT_STORAGE_SIZ];
	//for(int i = 0; i < objectTable;)
	shapeTable.assign(DEFAULT_STORAGE_SIZ, NULL);
	tableSize = DEFAULT_STORAGE_SIZ;
}

void ShapeStorage::initShapeType(int type, string objectFilePath)
{
	if (type >= tableSize)
	{
		string * tempTable = new string[type + 1];
		for (int i = 0; i < tableSize; i++)
			tempTable[i] = objectTable[i];
		objectTable = tempTable;
	}
	if (objectTable[type] == "")
	{
		objectTable[type] = objectFilePath;
	}
}

shared_ptr<Shape> ShapeStorage::getShape(int type)
{
	shared_ptr<Shape> shape;
	if (type > shapeTable.size() || shapeTable.at(type) == NULL) {
		 shape = make_shared<Shape>();
		shape->loadMesh(objectTable[type]);
		shape->resize();
		shape->init();
		shapeTable[type] = shape;
	}
	else
	{
		shape = shapeTable[type];
	}
	return shape;
}