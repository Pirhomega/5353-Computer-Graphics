/*
Corbin Matamoros
CMPS 5333 - Mr. Simpson
Project 5 - The Solar System
4/1/2019
This program loads the ply file data into the graphics card and draws the models.
Each object to be drawn is its own object set in the object class. The models
are vertex painted, modelled in Blender. A point light source is placed in the
world, inside the sun. The sun has an added emission variable that makes it appear
to glow. Planets have their own orbit about the sun and each rotate about their
poles. If calculations were correct, each planet orbits about the sun and their
poles at the same rate as the actual planets if the earth orbited about the sun
once per minute. It's best to run this program at 120 fps, with vsync.

I know, the instructions said to start the camera on the z-axis and look
down the negative end. There's so many planets in this program, I thought it best
to look down from the y-axis, about 100 units up from the origin and orthogonal to
the x-z plane. Plus, the rocket works better this way.

//-----------------------------------------------------------------------------
//							CAMERA AND ROCKET CONTROLS
//
//	CAMERA: 'wasd' to pan, 'z' to zoom in, 'Z' to zoom out, space to reset
//
//	ROCKET: 'ijkl' to pan
//
//-----------------------------------------------------------------------------
*/

#include "Angel.h"
#include "Object.h"
#include "plyloader.h"
#include <vector>
#include <stack>
#pragma comment( lib, "glew32" )

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

using namespace std;
//---------------------------------------------------------------------------

// Retrieve transformation and shader uniform variable locations
GLuint program, ModelView, Projection;

vector<Object> models;//vector to hold all models to be drawn

std::vector<std::vector<GLfloat>> angles(3, vector<GLfloat>(11, 0.0));

GLfloat xxx = 0.0, yyy = 0.0, zzz = 10.0; //will be used in LookAt function
int decider = 0;

//NOTE: LookAt function parameters (eye, at, up) are defined in world coordinates
vec4 up = vec4(0.0, 1.0, 0.0, 0.0); //up will be intuitive, parallel to the positive y

int frame = 0, fps = 0, time = 0, timebase = 0;
std::stack<mat4> modelViewMatrices;

//----------------------------------------------------------------------------

//calls function that will draw the object on the screen
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear the screen

	vec4	eye = vec4(xxx, yyy, zzz, 1.0),//these will adjust my camera's position
			at = vec4(xxx, 0.0, zzz, 1.0);

	mat4 modelChangerViewer = LookAt(eye, at, up) * //position the camera
		RotateZ(90.0);	//models load in on their sides 
						//for some reason, so this fixes it

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelChangerViewer);//link model_view matrix to the shader variable

	models[0].Draw(program);//draw the Sun

	//-------------------------------------------------------------------------

	// Timing etc to display the frame rate of the rotating cube
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	char display_string[100];
	if (time - timebase > 1000)
	{
		fps = frame * 1000 / (time - timebase);
		sprintf(display_string, "Corbin\'s Planetary System : FPS:%d ", fps);
		glutSetWindowTitle(display_string);
		timebase = time;
		frame = 0;
	}

	glutSwapBuffers();
}

//----------------------------------------------------------------------------

//defines keyboard strikes that will affect the drawn object
void keyboard(unsigned char key, int x, int y)
{
	if (key == 'q')
		exit(0);
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

//calls function that will determine what the object does when no buttons on 
//the keyboard or mouse are being clicked, i.e. when nothing is in the event
//queue
void idle(void)
{
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

//adjusts the aspect ratio of the window to prevent scretching of the model
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	//adjusts aspect ratio and perspective matrices to prevent the stretching of the model
	GLfloat aspectRatio = GLfloat(width) / height;
	mat4 per_spec = Perspective(GLfloat(45.0), aspectRatio, GLfloat(0.5), GLfloat(110.0));

	glUniformMatrix4fv(Projection, 1, GL_TRUE, per_spec);
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	glutInit(&argc, argv);//initializes glut

	//initializes the display. It'll use RGBA coloring, double buffering, and
	//allows us to use the depth buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);//sets window size in pixels
	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Corbin's Sodie Coke");//names the display window

	glewInit();//initializes glew

	//compile shaders and bind
	program = InitShader("SodieCokeVert.glsl", "SodieCokeFrag.glsl");
	glUseProgram(program);

	//all the models to be display
	models.push_back(Object("Sodie_Coke.ply"));
	
	for (auto i : models)
		i.Load(program);

	//tying a uniform variable to the respective variable in the shader
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);//paint screen white

	glutDisplayFunc(display);//the display function callback
	glutKeyboardFunc(keyboard);//keyboard function callback
	glutIdleFunc(idle);//idle function callback
	glutReshapeFunc(reshape);//reshape the model if window is reshaped

	glutMainLoop();//begins the main event loop processor
	return 0;
}