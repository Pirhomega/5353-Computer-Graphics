#pragma once
#include "Angel.h"
#include "plyloader.h"
#include "SOIL.h"
#include <string>
#include <iostream>
#include <vector>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#pragma comment( lib, "SOIL" )

using namespace std;
class Object
{
private:
	//std::vector<vec4> points;//holds all points after triangulation
	//std::vector<vec4> colors;//holds colors for all points after triangulation
	//std::vector<vec4> vertices;//holds all points before triangulation
	//std::vector<vec4> vertex_colors;//holds color wheel
	//int numVertices;
	string name;
	PLYModel model;
	//unsigned int sizeOfPoints, sizeOfColors;//# of bytes in points and colors
											//vectors (above)
	//GLuint theta;
	//GLfloat Theta[3] = { 0.0, 0.0, 0.0 };
	//int frame = 0, fps = 0, time = 0, timebase = 0;
public:
	//int Axis = 0;
	//vertex array object, the model view object, and the projection object
	GLuint vao;
	GLuint tex;

//public functions
public:
	// Parameratized constructor that reads in the model rply file
	Object(string);

	//Object(int, int, int);//overloaded constructor
	~Object();
	//void Load();

	// This method loads the model into OpenGL(ie onto the GPU)
	bool Load(GLuint);

	//void fillColors_and_Points();
	//void Draw();
	void Draw(GLuint);
	//void _idle();
};

