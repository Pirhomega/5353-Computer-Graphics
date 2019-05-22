/*
Matamoros, Corbin
CMPS 5353 - Graphics - Prof. Simpson
Project #1 - Drawing an N x N Maze
2/8/2019
This program will generate a maze by creating an N by N grid and 
assigning each grid cell with three
bools (east wall - e_wall - , south wall - s_wall - , and visited). 
The depth-first search algorithm will traverse the grid one cell at a time,
choosing randomly which direction it will travel. 
Whatever cell wall the algorithm crosses, it sets to false. All walls left true
will be drawn (loaded as points into the mazePoints array).
If false, they won't be drawn.
*/

#include "Angel.h"
#include<vector>
#include<random>

#pragma comment( lib, "glew32" )
const int NumCells = 44;//grid will be a NumCells by NumCells grid
const int NumPoints = (NumCells * NumCells) * 5;
const double CellWall = 2.0 / (NumCells + 2);//length of a cell wall in grid

/*
Each cell in the N by N grid will have an east wall and a south wall.
If either wall is true, then the wall exists and it can be drawn.
If the cell's 'visited' is true, then the cell has been passed over
by the depth first search algorithm. 
*/
struct cell
{
	bool visited, e_wall, s_wall;

	cell()
	{
		visited = true;
		e_wall = false;
		s_wall = false;
	}
};

/*
	Name:		genGrid
	Parameters:	maze
	Output:		void
	Purpose:	to generate the maze border by setting the west and north side
				cells' e_wall and s_wall to true. Generates grid by setting all
				e_wall and s_wall of cells within border to true 
	Notes:		2D vector is passed by reference
*/
void genGrid(std::vector<std::vector<cell>> &maze)
{
	int i, j = 0;
	//generate border
	for (i = NumCells; i > 0; i--)
	{
		maze[i][j].e_wall = true;
	}
	i = 0;
	for (j = 1; j <= NumCells; j++)
	{
		maze[i][j].s_wall = true;
	}

	//generate grid
	for (i = 1; i <= NumCells; i++)
	{
		for (j = 1; j <= NumCells; j++)
		{
			maze[i][j].e_wall = true;
			maze[i][j].s_wall = true;
			maze[i][j].visited = false;
		}
	}
}

/*
	Name:		mazeCutter
	Parameters:	maze
	Output:		void
	Purpose:	traverses grid using depth first search (dfs) algorithm,
				setting all cell's e_wall's and/or s_wall's to false as it
				passes through them. Direction of travel is chosen at random
				(a 0 means up, 1 means right, 2 means down, and 3 means left).
	Notes:		2D vector is passed by reference
*/
void mazeCutter(std::vector<std::vector<cell>> &maze)
{
	std::vector<std::pair<int, int>> dfsPath;

	//once a path has been chosen and the algorithm has moved to the next cell,
	//the direction that would lead back to the previous cell can be removed
	//from pathChoser. This is done to prevent algorithm from checking a
	//previously visited cell AND to prevent the same invalid direction from
	//being selected again
	std::vector<int> pathChoser{ 0,1,2,3 };

	int i, j = 0, failureCnt = 0, visitedCntr = 1, nextPnt, tempI, tempJ;
	unsigned int randVar;
	std::random_device rd;
	
	//randomly select a starting point from the west side of grid
	i = (rd() % (maze.size() - 3)) + 1;
	maze[i][j].e_wall = false;

	//erase previous cell from pathChoser
	pathChoser.erase(pathChoser.begin() + 3);

	j++;

	//cells set to 'visited = true' once algorithm has passed over them
	maze[i][j].visited = true;

	//push visited cell to dfs queue
	dfsPath.push_back(std::make_pair(i, j));

	//once all cells in grid are visited, stop algorithm
	while (visitedCntr != (NumCells*NumCells))
	{
		//failureCnt increments when a chosen direction (0,1,2,3) leads to a
		//cell that's already visited. 
		while (failureCnt != 3)
		{
			if (pathChoser.size() != 1)
			{
				randVar = rd() % (pathChoser.size() - 1);
				nextPnt = pathChoser[randVar];
			}
			else
				nextPnt = pathChoser[0];
			
			//to travel up in grid
			if (nextPnt == 0 && !maze[i - 1][j].visited)
			{
				i--;
				dfsPath.push_back(std::make_pair(i, j));
				maze[i][j].s_wall = false;
				maze[i][j].visited = true;
				failureCnt = 0;
				pathChoser.resize(3);
				pathChoser[0] = 1;
				pathChoser[1] = 0;
				pathChoser[2] = 3;
				visitedCntr++;
			}

			//to travel right in grid
			else if (nextPnt == 1 && !maze[i][j + 1].visited)
			{
				maze[i][j].e_wall = false;
				j++;
				maze[i][j].visited = true;
				dfsPath.push_back(std::make_pair(i, j));
				failureCnt = 0;
				pathChoser.resize(3);
				pathChoser[0] = 0;
				pathChoser[1] = 2;
				pathChoser[2] = 1;
				visitedCntr++;
			}

			//to travel down in grid
			else if (nextPnt == 2 && !maze[i + 1][j].visited)
			{
				maze[i][j].s_wall = false;
				i++;
				maze[i][j].visited = true;
				dfsPath.push_back(std::make_pair(i, j));
				failureCnt = 0;
				pathChoser.resize(3);
				pathChoser[0] = 3;
				pathChoser[1] = 1;
				pathChoser[2] = 2;
				visitedCntr++;
			}

			//to travel left in grid
			else if (nextPnt == 3 && !maze[i][j - 1].visited)
			{
				j--;
				dfsPath.push_back(std::make_pair(i, j));
				maze[i][j].e_wall = false;
				maze[i][j].visited = true;
				failureCnt = 0;
				pathChoser.resize(3);
				pathChoser[0] = 2;
				pathChoser[1] = 3;
				pathChoser[2] = 0;
				visitedCntr++;
			}

			//if selected direction leads to a visited cell, erase direction 
			//from pathChoser and try again until an unvisited cell is found or
			//no unvisited cells remain from the algorithm's current cell 
			//location
			else
			{
				failureCnt++;
				pathChoser.erase(pathChoser.begin() + randVar);
			}
		}

		//will run if all surrounding cells from the algorithm's
		//current position have been visited
		tempI = std::get<0>(dfsPath.back());
		tempJ = std::get<1>(dfsPath.back());

		//pops cell from queue, basically retreating the algorithm back to the 
		//previous cell
		dfsPath.pop_back();

		//gets the previously visited cell's coordinates from the dfs vector
		i = std::get<0>(dfsPath.back());
		j = std::get<1>(dfsPath.back());

		//determines the direction of the cell from which the algorithm just
		//came from and eliminates the direction (0,1,2,3) that would lead back
		//to it
		pathChoser.resize(3);

		//if previous cell is below current
		if (tempI - i == 1)
		{
			pathChoser[0] = 3;
			pathChoser[1] = 0;
			pathChoser[2] = 1;
		}
		//if previous cell is above current
		else if (tempI - i == -1)
		{
			pathChoser[0] = 2;
			pathChoser[1] = 1;
			pathChoser[2] = 3;
		}
		//if previous cell is to the right of current
		else if (tempJ - j == 1)
		{
			pathChoser[0] = 0;
			pathChoser[1] = 3;
			pathChoser[2] = 2;
		}
		//if previous cell is to the left of current
		else if (tempJ - j == -1)
		{
			pathChoser[0] = 1;
			pathChoser[1] = 2;
			pathChoser[2] = 0;
		}
		failureCnt = 0;
	}

	//creates the ending point of the maze
	j = NumCells;
	i = 3;
	maze[i][j].e_wall = false;
}

/*
	Name:		drawMaze
	Parameters:	maze, pathChoser, and dfsPath
	Output:		void
	Purpose:	passes over the maze (2D vector) and draws lines to represent all
				e_walls and s_walls set to true. 
	Notes:		2D vector and vec2 object is passed by reference
*/
void drawMaze(std::vector<std::vector<cell>> maze, vec2 mazePoints[])
{
	double x = -1.0, y = 1.0 - CellWall;
	int k = 0;

	for (int i = 0; i <= NumCells; i++)
	{
		for (int j = 0; j <= NumCells; j++)
		{
			//if s_wall is true, load end points of wall into mazePoints
			if (maze[i][j].s_wall == true)
			{
				mazePoints[k] = vec2(x, y);
				k++;
				x += CellWall;
				mazePoints[k] = vec2(x, y);
				k++;
			}
			else
				x += CellWall;
			//if e_wall is true, load end points of wall into mazePoints
			if (maze[i][j].e_wall == true)
			{
				mazePoints[k] = vec2(x, y);
				k++;
				y += CellWall;
				mazePoints[k] = vec2(x, y);
				k++;
				y -= CellWall;
			}
		}
		//repeat at the beginning of the next row
		x = -1.0;
		y -= CellWall;
	}
	return;
}

void
init(void)
{
	std::vector<std::vector<cell>> maze(NumCells + 2, std::vector<cell>(NumCells + 2));
	vec2 mazePoints[NumPoints];
	
	//generate the border and grid
	genGrid(maze);

	//create the maze
	mazeCutter(maze);

	//draw the maze
	drawMaze(maze, mazePoints);

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mazePoints), mazePoints, GL_STATIC_DRAW);

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