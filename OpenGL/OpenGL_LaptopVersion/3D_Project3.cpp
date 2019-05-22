//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
// those colors across the triangles.  We us an orthographic projection
// as the default projetion.

#include "Angel.h"
#pragma comment( lib, "glew32" )

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

//point4 points[NumVertices];
//color4 colors[NumVertices];

////Vertices of a unit cube centered at origin, sides aligned with axes
//point4 vertices[8] = 
//{
//    point4( -0.5, -0.5,  0.5, 1.0 ),
//    point4( -0.5,  0.5,  0.5, 1.0 ),
//    point4(  0.5,  0.5,  0.5, 1.0 ),
//    point4(  0.5, -0.5,  0.5, 1.0 ),
//    point4( -0.5, -0.5, -0.5, 1.0 ),
//    point4( -0.5,  0.5, -0.5, 1.0 ),
//    point4(  0.5,  0.5, -0.5, 1.0 ),
//    point4(  0.5, -0.5, -0.5, 1.0 )
//};

////RGBA colors; LET THE USER DEFINE WHAT COLORS THEY WANT
////SO LEAVE THIS HERE
//color4 vertex_colors[8] = 
//{
//    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
//    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
//    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
//    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
//    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
//    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
//    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
//    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
//};

////Array of rotation angles (in degrees) for each coordinate axis
//enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
//int Axis = Xaxis;
//GLfloat Theta[NumAxes] = { 0.0, 0.0, 0.0 };
//
//GLuint theta;

////----------------------------------------------------------------------------
//
////quad generates two triangles for each face and assigns colors
////to the vertices
//int Index = 0;
//void quad( int a, int b, int c, int d )
//{
//    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
//    colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
//    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
//    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
//    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
//    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
//}
//
////----------------------------------------------------------------------------
//
////generate 12 triangles: 36 vertices and 36 colors
//void colorcube()
//{
//    quad( 1, 0, 3, 2 );
//    quad( 2, 3, 7, 6 );
//    quad( 3, 0, 4, 7 );
//    quad( 6, 5, 1, 2 );
//    quad( 4, 5, 6, 7 );
//    quad( 5, 4, 0, 1 );
//}

//----------------------------------------------------------------------------

//// OpenGL initialization
//void init()
//{
//    //colorcube();
//
//    //// Create a vertex array object
//    //GLuint vao;//declare the object
//    glGenVertexArrays( 1, &vao );//find an empty name for the object
//    glBindVertexArray( vao );//create the object
//
//    // Create and initialize a buffer object
//    GLuint buffer;//declare the object
//    glGenBuffers( 1, &buffer );//find an empty name for the object
//    glBindBuffer( GL_ARRAY_BUFFER, buffer );//create the object and associate the data type it
//											//it will be associated with (vertices)  
//	  
//	//allocate memory on the GPU, they will be vertices, the memory allocated will be the size of
//	//points and colors combined (in bytes), NULL means the we aren't tying the data to a specific
// 	//array (yet), and we will just draw once
//	glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW );
//
//	//"dividing" the memory in the GPU into two parts and then pointing each section to the specific array
//    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
//    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );
//
//    // Load shaders and use the resulting shader program
//    GLuint program = InitShader( "vshader_a4.glsl", "fshader_a4.glsl" );
//    glUseProgram( program );
//
//    // set up vertex arrays
//    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
//    glEnableVertexAttribArray( vPosition );
//
//	//describe the form of the data in the vertex array. 4D array of GL_FLOATs, don't normalize
//    //data to (0.0, 1.0), data is contiguous, and the data starts at 0 bytes.
//    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
//
//	//describe the form of the data in the vertex array. 4D array of GL_FLOATs, don't normalize
//    //data to (0.0, 1.0), data is contiguous, and the data starts at sizeof(points) bytes.
//    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
//    glEnableVertexAttribArray( vColor );
//    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );
//
//	//declare the uniform variable theta and associate it with program and theta (the one that's
//	//in the vshader.
//    theta = glGetUniformLocation( program, "theta" ); //The location of the "theta" shader uniform variable
//    
//	glEnable(GL_DEPTH_TEST);
//    glClearColor( 1.0, 1.0, 1.0, 1.0 );
//}

//----------------------------------------------------------------------------

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUniform3fv( theta, 1, Theta );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
    switch( key ) 
	{
		case 033: // Escape Key
		case 'q': case 'Q':
			exit( EXIT_SUCCESS );
			break;
    }
}

//----------------------------------------------------------------------------

void mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) 
	{
		switch( button ) 
		{
			case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
			case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
			case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
		}
    }
}

//----------------------------------------------------------------------------

void idle( void )
{
    Theta[Axis] += 0.01;

    if ( Theta[Axis] > 360.0 ) {
	Theta[Axis] -= 360.0;
    }
    
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
	Object Cube(NumVertices, 8, 8, 8);

	glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Corbin's Color Cube" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );

    glutMainLoop();
    return 0;
}

//this is the copy of the main program that I will reference should the modified one go south

/*//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
// those colors across the triangles.  We us an orthographic projection
// as the default projetion.

#include "Angel.h"
#pragma comment( lib, "glew32" )

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];

//Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = 
{
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
};

//RGBA colors
color4 vertex_colors[8] = 
{
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};

//Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int Axis = Xaxis;
GLfloat Theta[NumAxes] = { 0.0, 0.0, 0.0 };

GLuint theta;

//----------------------------------------------------------------------------

//quad generates two triangles for each face and assigns colors
//to the vertices
int Index = 0;
void quad( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

//----------------------------------------------------------------------------

//generate 12 triangles: 36 vertices and 36 colors
void colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

//----------------------------------------------------------------------------

// OpenGL initialization
void init()
{
    colorcube();

    // Create a vertex array object
    GLuint vao;//declare the object
    glGenVertexArrays( 1, &vao );//find an empty name for the object
    glBindVertexArray( vao );//create the object

    // Create and initialize a buffer object
    GLuint buffer;//declare the object
    glGenBuffers( 1, &buffer );//find an empty name for the object
    glBindBuffer( GL_ARRAY_BUFFER, buffer );//create the object and associate the data type it
											//it will be associated with (vertices)  
	  
	//allocate memory on the GPU, they will be vertices, the memory allocated will be the size of
	//points and colors combined (in bytes), NULL means the we aren't tying the data to a specific
 	//array (yet), and we will just draw once
	glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW );

	//"dividing" the memory in the GPU into two parts and then pointing each section to the specific array
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader_a4.glsl", "fshader_a4.glsl" );
    glUseProgram( program );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );

	//describe the form of the data in the vertex array. 4D array of GL_FLOATs, don't normalize
    //data to (0.0, 1.0), data is contiguous, and the data starts at 0 bytes.
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	//describe the form of the data in the vertex array. 4D array of GL_FLOATs, don't normalize
    //data to (0.0, 1.0), data is contiguous, and the data starts at sizeof(points) bytes.
    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );

	//declare the uniform variable theta and associate it with program and theta (the one that's
	//in the vshader.
    theta = glGetUniformLocation( program, "theta" ); //The location of the "theta" shader uniform variable
    
	glEnable(GL_DEPTH_TEST);
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------------------

void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUniform3fv( theta, 1, Theta );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
    switch( key ) 
	{
		case 033: // Escape Key
		case 'q': case 'Q':
			exit( EXIT_SUCCESS );
			break;
    }
}

//----------------------------------------------------------------------------

void mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) 
	{
		switch( button ) 
		{
			case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
			case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
			case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
		}
    }
}

//----------------------------------------------------------------------------

void idle( void )
{
    Theta[Axis] += 0.01;

    if ( Theta[Axis] > 360.0 ) {
	Theta[Axis] -= 360.0;
    }
    
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Corbin's Color Cube" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );

    glutMainLoop();
    return 0;
}
*/