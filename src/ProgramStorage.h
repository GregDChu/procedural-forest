#pragma once

#ifndef H_PROGRAMSTORAGE
#define H_PROGRAMSTORAGE

#define DEFAULT_PROG_STORAGE_SIZ 20

#include <string>
#include <vector>
#include "Program.h"
#include "config.h"

using namespace std;

class ProgramStorage
{
public:
	ProgramStorage();
	void initProgram(int type, string vertexShader, string fragmentShader, vector<string> uniforms);
	shared_ptr<Program> getProgram(int type);
private:
	string * vShaderTable;
	string * fShaderTable;
	vector<vector<string>> uniformVariables;
	vector<shared_ptr<Program>> programTable;
	int tableSize;
};

#endif