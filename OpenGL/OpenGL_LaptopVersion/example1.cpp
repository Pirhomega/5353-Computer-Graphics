// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection
// Corbin Matamoros

#include "Angel.h"
#pragma comment( lib, "glew32" )
const int NumPoints = 50000;

//----------------------------------------------------------------------------

void init(void)
{

	vec2 points[NumPoints];

	// Specifiy the vertices for a triangle
	vec2 vertices[3] = {
		vec2(-1.0, -1.0), vec2(0.0, 1.0), vec2(1.0, -1.0)
	};

	// Select an arbitrary initial point inside of the triangle
	points[0] = vec2(0.25, 0.50);

	// compute and store N-1 new points
	for (int i = 1; i < NumPoints; ++i) {
		int j = rand() % 3;   // pick a vertex at random

							  // Compute the point halfway between the selected vertex
							  //   and the previous point
		points[i] = (points[i - 1] + vertices[j]) / 2.0;
	}

	// Create a vertex array object
	GLuint vao;//declare it
	glGenVertexArrays(1, &vao);//find an empty name for it
	glBindVertexArray(vao);//create it

	// Create and initialize a buffer object in the GPU
	GLuint buffer;//declare it
	glGenBuffers(1, &buffer);//find an empty name for it

	//create it and associate the type GL_ARRAY_BUFFER to it
	//which means the we will binding an array of vertices to it
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	
	//Allocate the memory in the GPU. sizeof(points) is how much memory to allocate in bytes, 
	//a pointer will point to the data in the CPU (points), and GL_STATIC_DRAW is what we
	//will do with the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Load shaders and use the resulting shader program. Program is the link between the 
	//shader arrays and the client
	GLuint program = InitShader("vshader_a2.glsl", "fshader_a2.glsl");
	glUseProgram(program);

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);

	//describe the form of the data in loc. 2D array of GL_FLOATs, data isn't normalized to the range (0.0, 1.0),
	//the data is contiguous in the array, and the array begins at 0 bytes
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	glClearColor(1.0, 1.0, 1.0, 1.0); // white background
}

//----------------------------------------------------------------------------

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);     // clear the window
	glDrawArrays(GL_POINTS, 0, NumPoints);    // draw the points
	glFlush();//draw as fast as possible
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033:
		exit(EXIT_SUCCESS);
		break;
	}
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	glutInit(&argc, argv);//initialize glut and the system and allows user to use command-line arguments
	glutInitDisplayMode(GLUT_RGBA);//initializes the display properties. Here, we're using RGBA
	glutInitWindowSize(780, 780);//sets the window size in pixels
	glutInitWindowPosition(0, 0);//sets the position the window will be created at (top left corner of screen)

	// If you are using freeglut, the next two lines will check if 
	// the code is truly 3.2. Otherwise, comment them out
	glutInitContextVersion(3, 1);// Does not need glewExperimental = GL_TRUE
								 // 3,2 does.  Discovered by Dr. C
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Sierpinski Gasket with 50,000 Points - Matamoros");
	glewInit();

	init();

	//event creators
	glutDisplayFunc(display);//call this function (display) whenever the screen needs to be redrawn
	glutKeyboardFunc(keyboard);//call this function whenever a keyboard button is pressed

	//event processor
	glutMainLoop();//the main event loop where the two functions (and there subsequent events) can be called
	return 0;
}