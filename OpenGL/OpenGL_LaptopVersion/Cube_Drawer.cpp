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

//enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };

// Retrieve transformation and shader uniform variable locations
GLuint program, ModelView, Projection;

vector<Object> models;//vector to hold all models to be drawn

//each planet has the three angles (rotations about x, y, and z) held in the 2D vectors 
//columns:
//							11 objects, 11 different rotations, 11 columns
//							 v  v  v  v  v  v  v  v  v  v  v 
//x-axis rotation angles -> [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
//y-axis rotation angles -> [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
//z-axis rotation angles -> [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
std::vector<std::vector<GLfloat>> angles(3, vector<GLfloat>(11, 0.0));

GLfloat xxx = 0.0, yyy = 100.0, zzz = 0.0, //will be used in LookAt function
		iii = 0.0, jjj = 0.0,//will be used to control the rocket
		lx = 0.0, ly = 0.0, lz = -100.0,//will keep the light position at center of sun
		eg = 1.0;//we'll use these to make the sun glow different colors
int decider = 0;

//NOTE: LookAt function parameters (eye, at, up) are defined in world coordinates
vec4 up = vec4(0.0, 0.0, 1.0, 0.0); //up will be intuitive, parallel to the positive y

int frame = 0, fps = 0, time = 0, timebase = 0;
//int currentModel = 0, currentAngle = 1;
std::stack<mat4> modelViewMatrices;

//----------------------------------------------------------------------------
//Declare all objects to be drawn right here
//Format: <class> <variable name>(<# of vertices>, <# of colors>, <# of edges>);
//Note: # of vertices is the value AFTER triangulation. # of edges is count before
//		triangulation
//Object Cube(36, 8, 8);

//----------------------------------------------------------------------------

//calls function that will draw the object on the screen
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear the screen

	vec4	eye = vec4(xxx, yyy, zzz, 1.0),//these will adjust my camera's position
			at = vec4(xxx, 0.0, zzz, 1.0);

	mat4 modelChangerViewer =	LookAt(eye, at, up) * //position the camera
								RotateX(90.0);	//models load in on their sides 
												//for some reason, so this fixes it

	//"remember" the original model view matrix.
	//NOTE: Saving the model view matrix before concatenating the
	//rotation matrices allows each planet to rotate idependently.
	//They'll each rotate at different speeds
	modelViewMatrices.push(modelChangerViewer);

	//NOTE: for some reason, rotateY rotates about Z and rotate Z rotates about Y. Keep that in mind...
	modelChangerViewer *= RotateY(angles[1][0]) * RotateZ(angles[2][0]); //rotate object
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelChangerViewer);//link model_view matrix to the shader variable
	
	//putting emission here will only have the sun emit a color
	glUniform4fv(glGetUniformLocation(program, "Emission"), 1, vec4(1.0, eg, 0.0, 1.0));

	models[0].Draw(program);//draw the Sun

	//-------------------------------------------------------------------------

	modelChangerViewer = modelViewMatrices.top();

	modelChangerViewer *=	RotateY(angles[1][1]) * RotateZ(angles[2][1]) * //rotates planet about sun
							Translate(3.0, 0.0, 0.0) * //moves planet away from sun
							RotateZ(1.5 * angles[2][1]);//rotates the planet about its own axis
	//link model_view matrix to the shader variable
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelChangerViewer);

	//we undo the emissive term so the rest of planets dont emit
	glUniform4fv(glGetUniformLocation(program, "Emission"), 1, vec4(0.0, 0.0, 0.0, 1.0));

	models[1].Draw(program);//draw Mercury

	//-------------------------------------------------------------------------

	modelChangerViewer = modelViewMatrices.top();

	modelChangerViewer *=	RotateY(angles[1][2]) * RotateZ(angles[2][2]) * 
							Translate(5.0, 0.0, 0.0) *
							RotateZ(-0.925 * angles[2][2]);
	//link model_view matrix to the shader variable
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelChangerViewer);
	models[2].Draw(program);//draw Venus

	//-------------------------------------------------------------------------

	modelChangerViewer = modelViewMatrices.top();

	modelChangerViewer *=	RotateY(angles[1][3]) * RotateZ(angles[2][3]) * 
							Translate(7.0, 0.0, 0.0) *
							RotateZ(365.0 * angles[2][3]);
	//link model_view matrix to the shader variable
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelChangerViewer);
	models[3].Draw(program);//draw Earth

	//-------------------------------------------------------------------------

	modelChangerViewer *=	RotateY(angles[1][4]) * RotateZ(angles[2][4]) * 
							Translate(1.0, 0.0, 0.0) *
							RotateZ(angles[2][4]);
	//link model_view matrix to the shader variable
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelChangerViewer);
	models[4].Draw(program);//draw the moon

	//-------------------------------------------------------------------------

	modelChangerViewer = modelViewMatrices.top();

	modelChangerViewer *=	RotateY(angles[1][5]) * RotateZ(angles[2][5]) *	
							Translate(9.0, 0.0, 0.0) *
							RotateZ(668.7 * angles[2][5]);
	//link model_view matrix to the shader variable
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelChangerViewer);
	models[5].Draw(program);//draw Mars

	//-------------------------------------------------------------------------

	modelChangerViewer = modelViewMatrices.top();

	modelChangerViewer *=	RotateY(angles[1][6]) * RotateZ(angles[2][6]) * 
							Translate(12.0, 0.0, 0.0) *
							RotateZ(10476.8 * angles[2][6]);
	//link model_view matrix to the shader variable
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelChangerViewer);
	models[6].Draw(program);//draw Jupiter

	//-------------------------------------------------------------------------

	modelChangerViewer = modelViewMatrices.top();

	modelChangerViewer *=	RotateY(angles[1][7]) * RotateZ(angles[2][7]) * 
							Translate(20.0, 0.0, 0.0) *
							RotateZ(24475.5 * angles[2][7]);
	//link model_view matrix to the shader variable
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelChangerViewer);
	models[7].Draw(program);//draw Saturn

	//-------------------------------------------------------------------------

	modelChangerViewer = modelViewMatrices.top();


	modelChangerViewer *=	RotateZ(angles[2][8]) * Translate(27.0, 0.0, 0.0) * RotateZ(-angles[2][8]) *
							RotateY(90.0) * //this'll get Uranus on its side
							RotateZ(42721.8 * angles[2][8]);
	//link model_view matrix to the shader variable
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelChangerViewer);
	models[8].Draw(program);//draw Uranus

	//-------------------------------------------------------------------------

	modelChangerViewer = modelViewMatrices.top();

	modelChangerViewer *=	RotateY(angles[1][9]) * RotateZ(angles[2][9]) * 
							Translate(35.0, 0.0, 0.0) *
							RotateZ(89665.9 * angles[2][9]);
	//link model_view matrix to the shader variable
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelChangerViewer);
	models[9].Draw(program);//draw Neptune

	//-------------------------------------------------------------------------

	modelChangerViewer = modelViewMatrices.top();

	modelChangerViewer *=	RotateY(angles[1][10]) * RotateZ(angles[2][10]) * 
							Translate(40.0, 0.0, 0.0) *
							RotateZ(14179.4 * angles[2][10]);
	//link model_view matrix to the shader variable
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelChangerViewer);
	models[10].Draw(program);//draw Pluto

	//-------------------------------------------------------------------------

	modelChangerViewer = modelViewMatrices.top();

	modelChangerViewer *= Translate(iii, jjj, -5.0) * RotateX(-90.0);
	//link model_view matrix to the shader variable
	glUniformMatrix4fv(ModelView, 1, GL_TRUE, modelChangerViewer);
	models[11].Draw(program);//draw rocket

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
	//at last, bind the position of the light source to the shader and give it a
	//position in camera coordinates
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, vec4(lx, ly, lz, 0.0));

	glutSwapBuffers();
}

//----------------------------------------------------------------------------

//defines keyboard strikes that will affect the drawn object
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	//-------------------------------------------------------------------------
	//Camera controls
	case 'w'://move up
		zzz += 0.5;
		ly -= 0.5;
		break;
	case 'a'://move to the left of model
		xxx += 0.5;
		lx += 0.5;
		break;
	case 's'://move down
		zzz -= 0.5;
		ly += 0.5;
		break;
	case 'd'://move to right of model
		xxx -= 0.5;
		lx -= 0.5;
		break;
	//-------------------------------------------------------------------------
	//Rocket Controls
	case 'i'://move up
		jjj += 0.5;
		break;
	case 'j'://move to the left of model
		iii += 0.5;
		break;
	case 'k'://move down
		jjj -= 0.5;
		break;
	case 'l'://move to right of model
		iii -= 0.5;
		break;
	case 'z'://move closer to model
		yyy -= 0.5;
		lz += 0.5;
		break;
	case 'Z'://move farther from model
		yyy += 0.5;
		lz -= 0.5;
		break;
	case ' '://reset camera to starting position
		xxx = 0; yyy = 100; zzz = 0;
		lx = 0.0; ly = 0.0; lz = -100.0;
		break;

		////change model being viewed
		//case 'n'://next model
		//	currentModel++;
		//	break;
		//case 'N'://previous model
		//	currentModel--;
		//	break;
	}

	//if (currentModel < 0)//keeps program from running outside model vector indices
	//	currentModel = models.size() - 1;
	//else if (currentModel >= models.size())
	//	currentModel = 0;
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

////defines mouse clicks that will affect the drawn object
//void mouse(int button, int state, int x, int y)
//{
//	if (state == GLUT_DOWN)
//	{
//		switch (button)
//		{
//			case GLUT_LEFT_BUTTON://rotate about the x axis
//				cout << "Changing rotation to x-axis\n";
//				currentAngle = 0;
//				break;
//			case GLUT_MIDDLE_BUTTON://rotate about the y axis
//				cout << "Changing rotation to y-axis\n";
//				currentAngle = 1;
//				break;
//			case GLUT_RIGHT_BUTTON://rotate about the z axis
//				cout << "Changing rotation to z-axis\n";
//				currentAngle = 2;
//				break;
//		}
//	}
//}

//----------------------------------------------------------------------------

//calls function that will determine what the object does when no buttons on 
//the keyboard or mouse are being clicked, i.e. when nothing is in the event
//queue
void idle(void)
{
	//orbit rotations
	//----------------------------
	angles[2][0] += GLfloat(0.01);//sun
	angles[2][1] += GLfloat(0.207);//mercury
	angles[2][2] += GLfloat(0.08);//venus
	angles[2][3] += GLfloat(0.05);//earth
	angles[2][4] += GLfloat(0.5);//moon
	angles[2][5] += GLfloat(0.0266);//mars
	angles[2][6] += GLfloat(0.00422);//jupiter
	angles[2][7] += GLfloat(0.001696);//saturn
	angles[2][8] += GLfloat(0.0005947);//uranus
	angles[2][9] += GLfloat(0.0003032);//neptune
	angles[2][10] += GLfloat(0.0002017);//pluto
	//-----------------------------

	//this code will make the sun's emission color transition between a 
	//red-orange and yellow
	if (decider == 0)
		eg += GLfloat(0.001);
	else
		eg -= GLfloat(0.001);
	if (eg > 0.5)
		decider = 1;
	else if (eg < 0.0)
		decider = 0;
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
	glutCreateWindow("Corbin's Wonderful Models");//names the display window

	glewInit();//initializes glew

	//compile shaders and bind
	program = InitShader("vshader_a6.glsl", "fshader_a5.glsl");
	glUseProgram(program);

	//all the models to be display
	models.push_back(Object("SUN.ply"));
	models.push_back(Object("Mercury.ply"));
	models.push_back(Object("Venus.ply"));
	models.push_back(Object("Earth.ply"));
	models.push_back(Object("Moon.ply"));
	models.push_back(Object("Mars.ply"));
	models.push_back(Object("Jupiter.ply"));
	models.push_back(Object("Saturn.ply"));
	models.push_back(Object("Uranus.ply"));
	models.push_back(Object("Neptune.ply"));
	models.push_back(Object("Pluto.ply"));
	models.push_back(Object("Rocket.ply"));

	//using class member "Load" to load models onto the graphics card
	for (int i = 0; i < models.size(); i++)
		models[i].Load(program);

	//tying a uniform variable to the respective variable in the shader
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);//paint screen white

	glutDisplayFunc(display);//the display function callback
	glutKeyboardFunc(keyboard);//keyboard function callback
	//glutMouseFunc(mouse);//mouse function callback
	glutIdleFunc(idle);//idle function callback
	glutReshapeFunc(reshape);//reshape the model if window is reshaped


	glutMainLoop();//begins the main event loop processor
	return 0;
}