// BallMan.cpp: implementation of the CBallMan class.
//
//////////////////////////////////////////////////////////////////////

#include "BallMan.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBallMan::CBallMan()
{
	m_arraySize=250;
}

CBallMan::~CBallMan()
{

}

int CBallMan::returnArraySize()
{
	return m_arraySize;
}

int CBallMan::killDeadBall(int i)
{
	if(i<m_arraySize && i>-1)
	{
		balls[i].setKillFlag(1);
		return 0;
	}

	return 1;
}

int CBallMan::createBall(float sideLength, float paddlePosition)
{
	int marker=0;
	Vector velocity;
	Vector position;

	for(marker=0; marker<m_arraySize; marker++)
	{
		if(balls[marker].returnKillFlag()==1)
		{
			position.x=paddlePosition;
			position.y=1.25-sideLength;
			position.z=0;

			//randomise everything except for the starting position
			//which is the position of the block that's just been
			//destroyed
			velocity.x=9*sin(rand())+3;
			velocity.y=18*cos(rand())+10;
			if(velocity.y<0)
				velocity.y=velocity.y*-1;
			velocity.z=0.0;

			//set these random variables
			balls[marker].setPosition(position);
			balls[marker].setVelocity(velocity);
			balls[marker].setKillFlag(0);

			return 0;
		}
	}
	return 0;
}

int CBallMan::checkVelocity(int i)
{
	Vector velocity;

	velocity=balls[i].returnVelocity();

	if(velocity.x<0.5 && velocity.x>-0.5)
	{
		if(velocity.y<0.5 && velocity.y>-0.5)
		{
			killDeadBall(i);
			return 1;
		}
	}
	return 0;
}

int CBallMan::checkPosition(float sideLength, float y, int i)
{
	if(y<(-sideLength-5))
	{
		killDeadBall(i);
		return 1;
	}
	return 0;
}
