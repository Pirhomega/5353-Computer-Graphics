/*
Matamoros, Corbin
CMPS 5353 - Graphics - Prof. Simpson
Project #1 - Drawing a 2D Spiral
1/30/2019
This program will generate a set of points, a function of the number of
spiral loops, and store them in an array to send to the GPU to be drawn.
Lines will be drawn from point to point.
*/

#include "Angel.h"
#include<vector>

#pragma comment( lib, "glew32" )
const int NumCells = 44;
const int NumPoints = ((NumCells + 1) * (NumCells + 1) * (NumCells + 1)) + 1;
const double CellWall = 2.0 / (NumCells + 2);
//----------------------------------------------------------------------------

struct cell
{
	bool visited, e_wall, s_wall;

	cell()
	{
		visited = false;
		e_wall = false;
		s_wall = false;
	}
};

void
init(void)
{
	std::vector<std::vector<cell>> maze(NumCells + 2, std::vector<cell>(NumCells + 2));
	vec2 points[NumPoints];

	int k = 0, i, j;
	double x = -1.0, y = -1.0;
	x += CellWall;
	y += CellWall;

	//-------------------------------------------------------------------------
	//							Generate border
	//-------------------------------------------------------------------------

	for (i = 0; i < NumCells; i++)
	{	
		points[k] = vec2(x, y);
		k++;
		y += CellWall;
		points[k] = vec2(x, y);
		k++;
	}

	for (i = 0; i < NumCells; i++)
	{	
		points[k] = vec2(x, y);
		k++;
		x += CellWall;
		points[k] = vec2(x, y);
		k++;
	}

	x = -1.0 + CellWall; 
	y = 1.0 - CellWall - CellWall;

	for (i = 1; i <= NumCells; i++)
	{
		for (j = 1; j <= NumCells; j++)
		{
			maze[i][j].visited = true;
			maze[i][j].e_wall = true;
			maze[i][j].s_wall = true;

			points[k] = vec2(x,y);
			x += CellWall;
			k++;
			points[k] = vec2(x, y);
			k++;
			points[k] = vec2(x, y);
			y += CellWall;
			k++;
			points[k] = vec2(x, y);
			y -= CellWall;
			k++;
		}
		x = -1.0 + CellWall;
		y -= CellWall;
	}

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader_a2.glsl", "fshader_a2.glsl");
	glUseProgram(program);

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}

//----------------------------------------------------------------------------

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);     // clear the window
	glDrawArrays(GL_LINES, 0, NumPoints);    // draw the points
	glFlush();
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033:
		exit(EXIT_SUCCESS);
		break;
	}
}

//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(780, 780);
	// If you are using freeglut, the next two lines will check if 
	// the code is truly 3.2. Otherwise, comment them out
	glutInitContextVersion(3, 1);// Does not need glewExperimental = GL_TRUE
	// 3,2 does.  Discovered by Dr. C
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Corbin's Maze");//I changed this
	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}