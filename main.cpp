//**************************
//Start of File main.cpp
//**************************

//**************************
//Libraries and header files.
//**************************
#include <GL/glut.h>			
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Ball.h"
#include "Tile.h"
#include "TileManager.h"
#include "Particle.h"
#include "ParticleManager.h"
#include "BallMan.h"
//end

//**************************
//Structures
//**************************
//Another structure is used in this program
//it is called "Vector" and is used to store
//x y and z coordinates. This is defined in
//"Base.h" which all the other header files are
//inheriting from.

//
//this is a cheap excuse for storing
//the paddles position as a global
struct Paddle
{
	float x;
};
//end

//
//The camera structure that stores
//the camera variables.
struct Camera	
{
	float heading;
	float inclination;
	Vector pos;
	float distance;
};
//end

//
//A structure that can be used to store
//2D points or texture coordinates
struct VectorS
{
	float u;
	float v;
};
//end

//
//these structures are used to store
//the information about each shape
//

//this structure stores the data for
//each vertex, ie positon, normal
//and texture coordinates
struct Vertex
{
	Vector pos;
	Vector normal;	
	VectorS tex;
};

//this structure is used to store the
//data which makes a triangle
//ie the first, second and third vertexes
//that should be joined to make a triangle
struct Index
{
	unsigned int first;
	unsigned int second;
	unsigned int third;
};

//this is the structure that holds the information
//for each object, all the vertex information is 
//stored in pVertexArray, the index of the verteces
//is stored in pIndexArray. iVertexCount stores the
//amound of vertexes in the corresponding array and
//iIndexCount does the same for the index array.
struct TriMesh
{
	Vertex *pVertexArray;		
	unsigned int iVertexCount;		
	Index *pIndexArray;			
	unsigned int iIndexCount;		
	char meshName[10];				
};

//this structure is the access point for the above structures
//related to the loading of meshes from files.
//pTriMeshArray is the array of meshes that have been loaded
//from files and the count is the amount of meshes that are stored.
struct MeshList
{
	TriMesh *pTriMeshArray;
	unsigned int iTriMeshCount;
};
//end

//
//store mouse positions
//for use when manouvering the camera
//and to ensure it doesn't teleport when you click
//on the screen in one location and the in
//another.
struct Mouse
{
	VectorS newPos;
	VectorS oldPos;
};
//end

//
// Store "level" information.
// Loaded from files by the "loadLevels()" function
struct LevelData
{
	Vector *pPositionArray;
	int iNumberOfTiles;
};

struct GameData
{
	LevelData *pLevelArray;
	int iNumberOfLevels;
};
//end

//
//The following are all the structures related to
//animation, with KEYFRAME storing the information
//for each keyframe. There are two timeDiff's because
//to keep the animations repeating i needed to know
//what the new times would be and the olds to keep the
//ratio the same each time it repeated
struct KEYFRAME
{
	Vector pos;
	float timeDiff;
	float newTimeDiff;
};

//ANIMATION stores all the information for an individual
//animation.
struct ANIMATION
{
	KEYFRAME *pkeyList;
	float simTime;
	Vector result;
	int iNumberOfKeyFrames;
	unsigned int object;
};

//ANIMMANAGER contains an array of ANIMATION
//allowing for multiple animations...
struct ANIMMANAGER
{
	ANIMATION *pAnimation;
	int iNumberOfAnimations;
};
//end

//***********************
//Function Definitions
//***********************
void display(void);	
void reshape(int, int);
void init(void);
void loadFile(void);
bool drawObject(int);
void motion(int, int);
void mouse(int, int, int, int);
void Keyboard(unsigned char, int, int);
void drawOutline(void);
void drawPaddle(void);
void Anim(void);
void drawBall(int );
void rotateBall(void);
void keyBoxes(void);
void drawCity(void);
void createBall(void);
void initBallManager(void);
void checkPosition(float, int);
void drawTiles(int );
void setTilePositions(int);
int checkForAllDestroyed(void);
void loadLevels(void);
void drawParticles(void);
void drawAnimation(void);
void updateAnimation(void);
void loadAnimations(void);
//end

//*******************************
//Global Variables (Your friends)
//*******************************
MeshList meshList;	
GameData gameData;		
Camera camera;
Paddle paddle;
Mouse mouseMove;
ANIMMANAGER anim;
CBallMan manager;
CTileManager tileManager;
CParticleManager particleManager;
float sideLength;

//******************************
//Code Starts Here
//******************************

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Chris Baillie's Breakout");

	init();

	glutIdleFunc(Anim);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	glutKeyboardFunc(Keyboard);

	glutMainLoop();

	free(meshList.pTriMeshArray);
	free(gameData.pLevelArray);
	free(anim.pAnimation);

	return 0;
}

//
//Load Animation files from animations.txt   
//containing the keyframes for various animations
//
void loadAnimations(void)
{
	FILE *fp;
	FILE *fpOne;
	int count, countOne;
	char fileName[12];
	
	if((fp=fopen("animations.txt", "r")) == NULL)
	{
		fprintf(stderr, "Error opening list of animation files (animations.txt).\n");
		exit(1);
	}

	fscanf(fp, "NUMBER OF ANIMATIONS %d\n", &anim.iNumberOfAnimations);

	anim.pAnimation=(struct ANIMATION *)malloc(sizeof(struct ANIMATION)*anim.iNumberOfAnimations);

	for(countOne=0; countOne<(anim.iNumberOfAnimations); countOne++)
	{
		fscanf(fp, "    %s\n", &fileName);
		if((fpOne=fopen(fileName, "r")) == NULL)
		{
			fprintf(stderr, "Error opening level file.\n");
			exit(1);
		}

		fscanf(fpOne, "NUMBER OF KEYFRAMES %d\n", &anim.pAnimation[countOne].iNumberOfKeyFrames);
		fscanf(fpOne, "OBJECT %d\n", &anim.pAnimation[countOne].object);

		anim.pAnimation[countOne].pkeyList=(struct KEYFRAME *)malloc(sizeof(struct KEYFRAME)*anim.pAnimation[countOne].iNumberOfKeyFrames);

		for(count=0; count<(anim.pAnimation[countOne].iNumberOfKeyFrames); count++)
		{
			fscanf(fpOne, "    POSITION %f %f %f TIME DIFFERENCE %f\n", &anim.pAnimation[countOne].pkeyList[count].pos.x, &anim.pAnimation[countOne].pkeyList[count].pos.y, &anim.pAnimation[countOne].pkeyList[count].pos.z, &anim.pAnimation[countOne].pkeyList[count].timeDiff);
			anim.pAnimation[countOne].pkeyList[count].newTimeDiff=anim.pAnimation[countOne].pkeyList[count].timeDiff;
		}

		fclose(fpOne);
	}

	fclose(fp);
}
//end

//
//Load the list of levels from "levels.txt" 
//and store the positions of each tile in the levels
//
//This information is stored using the "gameData" structure
//
void loadLevels(void)
{
	FILE *fp;
	FILE *fpOne;
	int count, countOne;
	char fileName[12];
	
	if((fp=fopen("levels.txt", "r")) == NULL)
	{
		fprintf(stderr, "Error opening list of level files (levels.txt).\n");
		exit(1);
	}

	fscanf(fp, "NUMBER OF LEVELS %d\n", &gameData.iNumberOfLevels);

	gameData.pLevelArray=(struct LevelData *)malloc(sizeof(struct LevelData)*gameData.iNumberOfLevels);

	for(countOne=0; countOne<(gameData.iNumberOfLevels+1); countOne++)
	{
		fscanf(fp, "    %s\n", &fileName);
		if((fpOne=fopen(fileName, "r")) == NULL)
		{
			fprintf(stderr, "Error opening level file.\n");
			exit(1);
		}

		fscanf(fpOne, "NUMBER OF TILES %d\n", &gameData.pLevelArray[countOne].iNumberOfTiles);

		gameData.pLevelArray[countOne].pPositionArray=(struct Vector *)malloc(sizeof(struct Vector)*gameData.pLevelArray[countOne].iNumberOfTiles);

		for(count=0; count<(gameData.pLevelArray[countOne].iNumberOfTiles+1); count++)
		{
			fscanf(fpOne, "    POSITION %f %f %f\n", &gameData.pLevelArray[countOne].pPositionArray[count].x, &gameData.pLevelArray[countOne].pPositionArray[count].y, &gameData.pLevelArray[countOne].pPositionArray[count].z);
		}

		fclose(fpOne);
	}

	fclose(fp);
}
//end


//
//This function is used when "loading" the next
//level from memory. It simply searches the
//Array of levels till it finds the appropriate
//one and creates tiles at the positions stated 
//by the level data.
void setTilePositions(int level)
{
	Vector position;
	int count;
	
	for(count=0; count<gameData.pLevelArray[level].iNumberOfTiles; count++)
	{
		position.x=gameData.pLevelArray[level].pPositionArray[count].x;
		position.y=gameData.pLevelArray[level].pPositionArray[count].y;
		position.z=gameData.pLevelArray[level].pPositionArray[count].z;
		tileManager.createTile(position);
	}
}
//end

/*keyboard input*/
//Please ensure caps lock is OFF
void Keyboard(unsigned char key, int x, int y)
{
	Vector temp;
	int i;

	switch (key)
	{
		//a and d allow you to move the paddle across the bottom
		//save the ball!
		case 'a':	paddle.x-=0.75;
					if(paddle.x<-(sideLength-1.5))
						paddle.x=-(sideLength-1.5);
					break;
		case 'd':	paddle.x+=0.75;
					if(paddle.x>(sideLength-1.5))
						paddle.x=(sideLength-1.5);
					break;
		//the following allow you to move the point of interest
		case 'i':	camera.pos.y++;
					if(camera.pos.y>30)
						camera.pos.y=30;
					break;
		case 'k':	camera.pos.y--;
					if(camera.pos.y<-30)
						camera.pos.y=-30;
					break;
		case 'j':	camera.pos.x--;
					if(camera.pos.x<-30)
						camera.pos.x=-30;
					break;
		case 'l':	camera.pos.x++;
					if(camera.pos.x>30)
						camera.pos.x=30;
					break;
		case 'u':	camera.pos.z++;
					if(camera.pos.z>30)
						camera.pos.z=30;
					break;
		case 'o':	camera.pos.z--;
					if(camera.pos.z<-30)
						camera.pos.z=-30;
					break;
		//b stops all simulations that are currently happening
		//n restarts all the current simulations
		case 'n':	for(i=0; i<manager.returnArraySize(); i++)
					{
						manager.balls[i].g_moveFlag=1;
					}
					for(i=0; i<particleManager.returnArraySize(); i++)
					{
						particleManager.particle[i].setMoveFlag(0);
					}
					break;
		case 'b':	for(i=0; i<manager.returnArraySize(); i++)
					{
						manager.balls[i].g_moveFlag=0;
					}
					for(i=0; i<particleManager.returnArraySize(); i++)
					{
						particleManager.particle[i].setMoveFlag(1);
					}
					break;
		//w, s, q, e, z and c allow you to change the strengh of gravity in the x z and y axis.
		case 'w':	for(i=0; i<manager.returnArraySize(); i++)
					{
						temp=manager.balls[i].returnForce();
						temp.y=temp.y+1.0;
						manager.balls[i].setForce(temp);
					}
					for(i=0; i<particleManager.returnArraySize(); i++)
					{
						particleManager.particle[i].setForce(temp);
					}
					break;
		case 's':	for(i=0; i<manager.returnArraySize(); i++)
					{
						temp=manager.balls[i].returnForce();
						temp.y=temp.y-1.0;
						manager.balls[i].setForce(temp);
					}
					for(i=0; i<particleManager.returnArraySize(); i++)
					{
						particleManager.particle[i].setForce(temp);
					}
					break;
		case 'q':	for(i=0; i<manager.returnArraySize(); i++)
					{
						temp=manager.balls[i].returnForce();
						temp.x=temp.x+1.0;
						manager.balls[i].setForce(temp);
					}
					for(i=0; i<particleManager.returnArraySize(); i++)
					{
						particleManager.particle[i].setForce(temp);
					}
					break;
		case 'e':	for(i=0; i<manager.returnArraySize(); i++)
					{
						temp=manager.balls[i].returnForce();
						temp.x=temp.x-1.0;
						manager.balls[i].setForce(temp);
					}
					for(i=0; i<particleManager.returnArraySize(); i++)
					{
						particleManager.particle[i].setForce(temp);
					}
					break;
		//x resets the gravity to "down" by -10...
		case 'x':	temp.x=0.0; 
					temp.y=-10.0;
					temp.z=0.0;
					for(i=0; i<manager.returnArraySize(); i++)
					{
						manager.balls[i].setForce(temp);
					}
					for(i=0; i<particleManager.returnArraySize(); i++)
					{
						particleManager.particle[i].setForce(temp);
					}
		case 'z':	for(i=0; i<manager.returnArraySize(); i++)
					{
						temp=manager.balls[i].returnForce();
						temp.z=temp.z+1.0;
						manager.balls[i].setForce(temp);
					}
					for(i=0; i<particleManager.returnArraySize(); i++)
					{
						particleManager.particle[i].setForce(temp);
					}
		case 'c':	for(i=0; i<manager.returnArraySize(); i++)
					{
						temp=manager.balls[i].returnForce();
						temp.z=temp.z-1.0;
						manager.balls[i].setForce(temp);
					}
					for(i=0; i<particleManager.returnArraySize(); i++)
					{
						particleManager.particle[i].setForce(temp);
					}
					break;
		case ' ':	manager.createBall(sideLength, paddle.x);
					break;
		default:	break;
	}
   
	glutPostRedisplay();
}
//end

//draws the object at position "object" in the array of pTriMeshArray
bool drawObject(unsigned int object)
{
	unsigned int count;
	float x, y, z, xn, yn, zn;
		
	if(object>=meshList.iTriMeshCount)
		return false;

	glBegin(GL_TRIANGLES);

		for(count=0; count<meshList.pTriMeshArray[object].iIndexCount; count++)
		{

			xn=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].first].normal.x;
			yn=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].first].normal.y;
			zn=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].first].normal.z;
			glNormal3f(xn, yn, zn);
			x=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].first].pos.x;
			y=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].first].pos.y;
			z=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].first].pos.z;
			glVertex3f(x, y, z);
			xn=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].second].normal.x;
			yn=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].second].normal.y;
			zn=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].second].normal.z;
			glNormal3f(xn, yn, zn);
			x=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].second].pos.x;
			y=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].second].pos.y;
			z=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].second].pos.z;
			glVertex3f(x, y, z);
			xn=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].third].normal.x;
			yn=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].third].normal.y;
			zn=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].third].normal.z;
			glNormal3f(xn, yn, zn);
			x=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].third].pos.x;
			y=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].third].pos.y;
			z=meshList.pTriMeshArray[object].pVertexArray[meshList.pTriMeshArray[object].pIndexArray[count].third].pos.z;
			glVertex3f(x, y, z);
		}
	

	glEnd();

	return true;
}
//end

//
//loads mesh files defined in open.txt
void loadFile(void)
{
	FILE *fp;
	FILE *fpOne;
	unsigned int count, countOne;
	char fileName[12];
	
	if((fp=fopen("open.txt", "r")) == NULL)
	{
		fprintf(stderr, "Error opening list of mesh files (open.txt).\n");
		exit(1);
	}

	fscanf(fp, "LENGTH OF SIDES %f\n", &sideLength);
	fscanf(fp, "MESH NUMBER %d\n", &meshList.iTriMeshCount);

	meshList.pTriMeshArray=(struct TriMesh *)malloc(sizeof(struct TriMesh)*meshList.iTriMeshCount);

	for(countOne=0; countOne<(meshList.iTriMeshCount+1); countOne++)
	{
		fscanf(fp, "    %s\n", &fileName);
		if((fpOne=fopen(fileName, "r")) == NULL)
		{
			fprintf(stderr, "Error opening mesh file.\n");
			exit(1);
		}

		fscanf(fpOne, "TRIMESH %s\n", &meshList.pTriMeshArray[countOne].meshName);
		fscanf(fpOne, "    VERTEXCOUNT %d\n", &meshList.pTriMeshArray[countOne].iVertexCount);

		meshList.pTriMeshArray[countOne].pVertexArray=(struct Vertex *)malloc(sizeof(struct Vertex)*meshList.pTriMeshArray[countOne].iVertexCount);

		for(count=0; count<(meshList.pTriMeshArray[countOne].iVertexCount+1); count++)
		{
			fscanf(fpOne, "       POSITION: %f %f %f NORMAL: %f %f %f TEXCOORD: %f %f\n", &meshList.pTriMeshArray[countOne].pVertexArray[count].pos.x, &meshList.pTriMeshArray[countOne].pVertexArray[count].pos.y, &meshList.pTriMeshArray[countOne].pVertexArray[count].pos.z, &meshList.pTriMeshArray[countOne].pVertexArray[count].normal.x, &meshList.pTriMeshArray[countOne].pVertexArray[count].normal.y, &meshList.pTriMeshArray[countOne].pVertexArray[count].normal.z, &meshList.pTriMeshArray[countOne].pVertexArray[count].tex.u, &meshList.pTriMeshArray[countOne].pVertexArray[count].tex.v);
		}

		fscanf(fpOne, "    TRIANGLECOUNT %d\n", &meshList.pTriMeshArray[countOne].iIndexCount);

		meshList.pTriMeshArray[countOne].pIndexArray=(struct Index *)malloc(sizeof(struct Index)*meshList.pTriMeshArray[countOne].iIndexCount);

		for(count=0; count<(meshList.pTriMeshArray[countOne].iIndexCount+1); count++)
		{
			fscanf(fpOne, "        %d %d %d\n", &meshList.pTriMeshArray[countOne].pIndexArray[count].first, &meshList.pTriMeshArray[countOne].pIndexArray[count].second, &meshList.pTriMeshArray[countOne].pIndexArray[count].third);
		}

		fclose(fpOne);
	}

	fclose(fp);
}
//end

//
//initiate anything that needs initiating
void init(void)
{
	int i;

	glClearColor(0.0, 0.0, 0.4, 0);

	GLfloat matSpecular[]={0.5, 0.5, 0.5, 1.0};
	GLfloat matShininess[]={50.0};
	GLfloat lightPosition[]={5.0, 10.0, 0.0, 0.0};
	GLfloat whiteLight[]={1.0, 1.0, 1.0, 1.0};
	GLfloat purpleLight[]={0.85, 0.85, 0.85, 1.0};
	GLfloat ambientLight[]={0.75, 0.75, 0.75, 1.0};
	GLfloat lightPos[]={0.0, 0.0, 0.0, 0.0};
	GLfloat yellowLight[]={0.25, 0.25, 25.0, 1.0};

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, purpleLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, yellowLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, whiteLight);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);

	for(i=0; i<manager.returnArraySize(); i++)
	{
		manager.balls[i].setKillFlag(1);
		manager.balls[i].g_moveFlag=1;
		manager.balls[i].setAccelFlag(0);
	}

	tileManager.initManager(20);

	loadFile();
	loadLevels();	
	loadAnimations();

	setTilePositions(0);
	createBall();

	mouseMove.oldPos.u=0;
	mouseMove.oldPos.v=0;

	camera.inclination=-5.0;
	camera.heading=362.5;
	camera.distance=50;
	camera.pos.x=0;
	camera.pos.y=0;
	camera.pos.z=25;
}
//end

//upon trigger creates a ball...shocking
void createBall(void)
{
	manager.createBall(sideLength, paddle.x);
}
//end

//draw all the surviving tiles in the scene
void drawTiles(int i)
{
	Vector position;
	unsigned int object=8;

	if(tileManager.returnKillFlag(i)==0)
	{
		position=tileManager.getTilePosition(i);
		
		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);	
		glRotatef(90, 0, 0, 1);

		if(!drawObject(object))
			printf("Failed to draw object No. : %d", object);
		glPopMatrix();
	}
}	
//end	

//you guessed it, draws all non-dead particles
void drawParticles(void)
{
	Vector position;
	int i;

	glBegin(GL_POINTS);

	for(i=0; i<particleManager.returnArraySize(); i++)
	{
		if(particleManager.particle[i].returnKillFlag()==0)
		{
			position=particleManager.particle[i].returnPosition();
			
			glPushMatrix();

			glPointSize(5.0);

				glColor3f(1.0, 1.0, 1.0);
				glVertex3f(position.x, position.y, position.z);

			glPopMatrix();
		}
	}

	glEnd();
}		
//end


void display(void)
{
	int i;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		glTranslatef(-camera.pos.x, -camera.pos.y, -camera.pos.z);
		glRotatef(camera.inclination, 1, 0 ,0);
		glRotatef(camera.heading, 0, 1, 0);

	glPushMatrix();
	glRotatef(-50, 1, 0, 0);

		drawOutline();
		drawPaddle();

		for(i=0; i<tileManager.returnAmountInArray(); i++)
			drawTiles(i);

		for(i=0; i<manager.returnArraySize(); i++)
			drawBall(i);

			drawParticles();

	glPopMatrix();

	drawCity();

	drawAnimation();

	glutSwapBuffers();
}

//runs though all tiles in the current level
//if all are destroyed return 0
int checkForAllDestroyed(void)
{
	int k, state;

	for(k=0; k<tileManager.returnAmountInArray(); k++)
	{
		state=tileManager.returnKillFlag(k);
		if(state==0)
			return 0;

	}

	return 1;
}

//
//the idle function
void Anim(void)
{
	int i, j;
	int destroyTile=0;
	int createTiles=0;
	int destroyParticle=0;
	int exitLoop=0;
	Vector position;

	//for each ball...
	for(i=0; i<manager.returnArraySize(); i++)
	{
		//update balls in the scene
		if(manager.balls[i].returnKillFlag()==0)
		{
			//if ball move to slow, nuke it.
			if(manager.checkVelocity(i)==1)
				manager.createBall(sideLength, paddle.x);
			manager.balls[i].update(sideLength, paddle.x);
	

			//...check position against each tile...
			for(j=0; j<tileManager.returnAmountInArray(); j++)
			{
				destroyTile=0;
				if(tileManager.returnKillFlag(j)==0)
				{
					//check collisions
					position=tileManager.getTilePosition(j);
					destroyTile=manager.balls[i].checkTileCollisions(position);
					particleManager.particle[i].checkTileCollisions(position);
					if(destroyTile==1)
					{
						tileManager.destroyTile(j);
						particleManager.createParticle(position);
						
						createTiles=checkForAllDestroyed();

						//...if all tiles are destroyed
						if(createTiles==1)
						{
							//...load the next level
							tileManager.incrementLevel(gameData.iNumberOfLevels);
							setTilePositions(tileManager.getLevelNumber());
						}
					}
				}
			}
		}
	}

	updateAnimation();

	//update particles
	for(i=0; i<particleManager.returnArraySize(); i++)
	{
		destroyParticle=0;
		particleManager.particle[i].checkTileCollisions(position);
		destroyParticle=particleManager.particle[i].update(sideLength, paddle.x);
		if(destroyParticle==1)
			particleManager.killDeadParticle(i);
	}

	glutPostRedisplay();
}
//end

//if you fail to save the current ball, create a new one
void checkPosition(float y, int i)
{
	if(manager.checkPosition(sideLength, y, i)==1)
	{
		createBall();
	}	
}
//end

void drawBall(int i)
{
	Vector position;
	unsigned int object=1;

	if(manager.balls[i].returnKillFlag()==0)
	{
	
		position=manager.balls[i].returnPosition();
		position.z=0;

		checkPosition(position.y, i);

		glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glRotatef(-50, 1, 0, 0);
		if(!drawObject(object))
			printf("Failed to draw object No. : %d", object);
		glPopMatrix();
	}
}

void drawPaddle(void)
{
	unsigned int object=2;

	glPushMatrix();
	glTranslatef(paddle.x, 0.5-sideLength,0);
	glRotatef(90, 0, 0, 1);
	if(!drawObject(object))
		printf("Failed to draw object No. : %d", object);
	glPopMatrix();
}

//the reshape function
void reshape(int iNewWidth, int iNewHeight)
{
	if(iNewHeight == 0)
		iNewHeight = 1;

	glViewport(0, 0, iNewWidth, iNewHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)iNewWidth / (float)iNewHeight, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}
//end

//
//mouse functions follow for getting user input
//and manouvering camera
void motion(int x, int y)
{
	int dx, dy;

	dx=0;
	dy=0;

	dx=mouseMove.oldPos.u-x;
	dy=mouseMove.oldPos.v-y;

	camera.heading-=dx;
	camera.inclination+=dy;

	if(camera.inclination>45)
		camera.inclination=45;

	if(camera.inclination<-45)
		camera.inclination=-45;

	camera.distance=25;

	mouseMove.oldPos.u=x;
	mouseMove.oldPos.v=y;

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	switch(button)
	{
	case GLUT_LEFT_BUTTON : switch(state)
							{
								case GLUT_DOWN :
													mouseMove.oldPos.u=x;
													mouseMove.oldPos.v=y;

													if(camera.inclination>45)
														camera.inclination=45;

													if(camera.inclination<-45)
														camera.inclination=-45;

													break;
								default			:	break;
							}
	default				:	break;
	}

	camera.distance=25;

	glutPostRedisplay();
}
//end

void drawOutline(void)
{
	unsigned int object=0;

	//object 0 is the side of the area
	//object 1 is the ball
	//object 2 is the paddle

	glPushMatrix();
	glTranslatef(-sideLength, 0, 0);
	
	glPushMatrix();
	glTranslatef(0,0,0);
	if(!drawObject(object))
		printf("Failed to draw object No. : %d", object);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(sideLength, sideLength, 0);
	glRotatef(90, 0, 0, 1);
	if(!drawObject(object))
		printf("Failed to draw object No. : %d", object);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(sideLength*2,0,0);
	if(!drawObject(object))
		printf("Failed to draw object No. : %d", object);
	glPopMatrix();

	glPopMatrix();
}

void updateAnimation(void)
{
	float appTime = glutGet(GLUT_ELAPSED_TIME) / 1000.f;
	float t;
	int i, j, k;

	for(i=0; i<anim.iNumberOfAnimations; i++)
	{
		anim.pAnimation[i].simTime=appTime;

		if(appTime>anim.pAnimation[i].pkeyList[anim.pAnimation[i].iNumberOfKeyFrames-1].newTimeDiff)
		{
			for(j=0; j<anim.pAnimation[i].iNumberOfKeyFrames; j++)
			{
				anim.pAnimation[i].pkeyList[j].newTimeDiff=appTime+anim.pAnimation[i].pkeyList[j].timeDiff;
			}
		}

		for(j=0; j<anim.pAnimation[i].iNumberOfKeyFrames; j++)
		{
			k=j+1;
			if(k<anim.pAnimation[i].iNumberOfKeyFrames)
			{
				if(appTime>anim.pAnimation[i].pkeyList[j].newTimeDiff && appTime<anim.pAnimation[i].pkeyList[k].newTimeDiff)
				{
					t=(anim.pAnimation[i].simTime-anim.pAnimation[i].pkeyList[j].newTimeDiff)/(anim.pAnimation[i].pkeyList[k].newTimeDiff-anim.pAnimation[i].pkeyList[j].newTimeDiff);

					anim.pAnimation[i].result.x=(anim.pAnimation[i].pkeyList[j].pos.x*(1-t))+(anim.pAnimation[i].pkeyList[k].pos.x*t);
					anim.pAnimation[i].result.y=(anim.pAnimation[i].pkeyList[j].pos.y*(1-t))+(anim.pAnimation[i].pkeyList[k].pos.y*t);
					anim.pAnimation[i].result.z=(anim.pAnimation[i].pkeyList[j].pos.z*(1-t))+(anim.pAnimation[i].pkeyList[k].pos.z*t);
				}
			}
		}
	}
}

void drawAnimation(void)
{
	int i;
	glPushMatrix();
//	glRotatef(90, 1, 0, 0);

	for(i=0; i<anim.iNumberOfAnimations; i++)
	{
		glTranslatef(anim.pAnimation[i].result.x, anim.pAnimation[i].result.y, anim.pAnimation[i].result.z);
		if(!drawObject(anim.pAnimation[i].object))
			printf("Failed to draw object No. : %d", anim.pAnimation[i].object);
	}

	glPopMatrix();
}

//
//this draws all those strange and random
//blocks you can see behind the level
//i thought it gave the place a bit of
//atmosphere
void drawCity(void)
{
	unsigned int object=3;

	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	glTranslatef(0,0,-35);
	if(!drawObject(object))
		printf("Failed to draw object No. : %d", object);

	object=4;
	if(!drawObject(object))
		printf("Failed to draw object No. : %d", object);

	object=5;
	if(!drawObject(object))
		printf("Failed to draw object No. : %d", object);

	object=6;
	if(!drawObject(object))
		printf("Failed to draw object No. : %d", object);

	object=7;
	if(!drawObject(object))
		printf("Failed to draw object No. : %d", object);
	
	glPopMatrix();

}
//end

//end of file
