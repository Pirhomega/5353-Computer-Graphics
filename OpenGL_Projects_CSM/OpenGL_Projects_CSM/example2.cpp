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
#pragma comment( lib, "glew32" )
const int NumLoops = 10;//change this value
const int NumPoints = (NumLoops * 4) + 1;//don't touch this one
const float increments = 0.95 / NumLoops;//nor this one
//----------------------------------------------------------------------------

void
init(void)
{
	vec2 points[NumPoints];
	int j = 1;
	points[0] = vec2(-0.0, -0.0);
	/*
	compute and store N-1 new points with the pattern
	as follow - example with NumLoops = 10 in parantheses:
		1. from starting point, negate y value (-0.0, 0.0)
		2. increment y value (-0.0, 0.095). Store in array
		3. negate x value (0.0, 0.095)
		4. increment x value (0.095, 0.095). Store.
		5. negate y value (0.095, -0.095). Store.
		6. negate x value (-0.095, -0.095). Store.
		7. repeat until loop finishes
	*/
	for (int i = 0; i < NumLoops; ++i)
	{
		points[j] = points[j - 1] * vec2(1.0, -1.0);
		points[j].y += increments;
		j++;
		points[j] = points[j - 1] * vec2(-1.0, 1.0);
		points[j].x += increments;
		j++;
		points[j] = points[j - 1] * vec2(1.0, -1.0);
		j++;
		points[j] = points[j - 1] * vec2(-1.0, 1.0);
		j++;
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
	glDrawArrays(GL_LINE_STRIP, 0, NumPoints);    // draw the points
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
	glutCreateWindow("Corbin's OpenGL 10-Loop Spiral");//I changed this
	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}