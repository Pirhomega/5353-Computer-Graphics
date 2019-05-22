// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include "Angel.h"
#pragma comment( lib, "glew32" )
const int NumLoops = 10;
const int NumPoints = NumLoops * 4;
const float increments = (1.0 / NumLoops) - (0.5 / NumLoops);
//----------------------------------------------------------------------------

void
init(void)
{
	vec2 points[NumPoints];

	// Specifiy the vertices for a triangle
	/*vec2 vertices[3] = {
		vec2(-1.0, -1.0), vec2(0.0, 1.0), vec2(1.0, -1.0)
	};*/

	// Select an arbitrary initial point inside of the triangle
	points[0] = vec2(-0.0, -0.0);

	int j = 1;

	// compute and store N-1 new points
	for (int i = 0; i < NumLoops; ++i)
	{
		points[j] = points[j - 1] * vec2(1.0, -1.0);
		points[j].x += 0.0;
		points[j].y += increments;
		j++;
		points[j] = points[j - 1] * vec2(-1.0, 1.0);
		points[j].x += increments;
		points[j].y += 0.0;
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
	glDrawArrays(GL_POINTS, 0, NumPoints);    // draw the points
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
	glutCreateWindow("Corbin's Sierpinski Gasket");
	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}