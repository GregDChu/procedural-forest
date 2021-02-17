#include "ProgramStorage.h"

ProgramStorage::ProgramStorage()
{
	vShaderTable = new string[DEFAULT_PROG_STORAGE_SIZ];
	fShaderTable = new string[DEFAULT_PROG_STORAGE_SIZ];
	uniformVariables.assign(DEFAULT_PROG_STORAGE_SIZ, vector<string>());
	programTable.assign(DEFAULT_PROG_STORAGE_SIZ, NULL);
	tableSize = DEFAULT_PROG_STORAGE_SIZ;
}
void ProgramStorage::initProgram(int type, string vertexShader, string fragmentShader, vector<string> uniforms)
{
	
	if (type >= tableSize)
	{
		string * tempVTable = new string[type + 1];
		string * tempFTable = new string[type + 1];
		//vector<vector<s ** tempUTable = new string*[type + 1];
		for (int i = 0; i < tableSize; i++)
		{
			tempVTable[i] = vShaderTable[i];
			tempFTable[i] = fShaderTable[i];
			//tempUTable[i] = uniformVariables[i];
		}
		vShaderTable = tempVTable;
		fShaderTable = tempFTable;
	}
	if (vShaderTable[type] == "")
	{
		vShaderTable[type] = vertexShader;
		fShaderTable[type] = fragmentShader;
		uniformVariables[type] = uniforms;
	}
}
shared_ptr<Program> ProgramStorage::getProgram(int type)
{
	//shared_ptr<Shape> shape;
	//if (type > shapeTable.size() || shapeTable.at(type) == NULL) {
	//	shape = make_shared<Shape>();
	//	shape->loadMesh(objectTable[type]);
	//	shape->resize();
	//	shape->init();
	//	shapeTable[type] = shape;
	//}
	//else
	//{
	//	shape = shapeTable[type];
	//}
	//return shape;
	shared_ptr<Program> program;
	//TODO Fix this coniditon where the type is larger than the table...
	if (type > programTable.size() || programTable.at(type) == NULL) {
		program = make_shared<Program>();
		program->setVerbose(true);
		program->setShaderNames(vShaderTable[type], fShaderTable[type]);
		program->init();
		program->addUniform("P");
		program->addUniform("V");
		program->addUniform("M");
		for (int i = 0; i < uniformVariables[type].size(); i++)
			program->addUniform(uniformVariables[type].at(i));
		program->addAttribute("vertPos");
		program->addAttribute("vertNor");
		if (type == GRASS)
		{
			program->addAttribute("vertTex");
		}
		programTable[type] = program;
	}
	else
	{
		program = programTable[type];
	}
	return program;
}