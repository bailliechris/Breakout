// Ball.cpp: implementation of the CBall class.
//
//////////////////////////////////////////////////////////////////////

#include "Ball.h"
#include <stdio.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBall::CBall()
{
	g_time=glutGet(GLUT_ELAPSED_TIME)/1000.0f;
	g_force.x=0;
	g_force.y=-10;
	g_force.z=0;

	m_bounceFlagX=1;
	m_bounceFlagY=1;
	m_bounceFlagZ=0;

	m_accelFlag=0;

	g_moveFlag=1;

	g_maxSpeed=20;
}

CBall::~CBall()
{

}

void CBall::update(float sideLength, float paddlePos)
{
	float time, deltaTime, t, y;

	time=glutGet(GLUT_ELAPSED_TIME)/1000.0f;

	deltaTime=time-g_time;

	if(m_killFlag==0)
	{

	//if ball is allowed to move (time is not stopped)
	//then calculate new position and collisions with
	//the sides
	if(g_moveFlag==1)
	{

		if(m_accelFlag==0)
		{
			// update ball
			m_accel.x=g_force.x;
			m_accel.y=g_force.y;
			m_accel.z=g_force.z;
			m_accelFlag=1;
		}
		else
			{
				g_force.y=g_force.y;
				m_accelFlag=0;
			}
		
		//calculate bounce and keep within a specified max speed
		if(m_bounceFlagX==1)
		{
			m_velocity.x+=m_accel.x*deltaTime;
			if(m_velocity.x>g_maxSpeed)
				m_velocity.x=g_maxSpeed;
		}
			
		if(m_bounceFlagX==-1)
		{
			m_velocity.x+=m_accel.x*deltaTime;
			if(m_velocity.x<-g_maxSpeed)
				m_velocity.x=-g_maxSpeed;
		}
			
		if(m_bounceFlagY==1)
		{
			m_velocity.y+=m_accel.y*deltaTime;
			if(m_velocity.y>g_maxSpeed)
				m_velocity.y=g_maxSpeed;
		}

		if(m_bounceFlagY==-1)
		{
			m_velocity.y+=m_accel.y*deltaTime;
			if(m_velocity.y<-g_maxSpeed)
				m_velocity.y=-g_maxSpeed;
		}

		if(m_bounceFlagZ==1)
		{
			m_velocity.z+=m_accel.z*deltaTime;
			if(m_velocity.z>g_maxSpeed)
				m_velocity.z=g_maxSpeed;
		}

		if(m_bounceFlagZ==-1)
		{
			m_velocity.z-=m_accel.z*deltaTime;
			if(m_velocity.z<-g_maxSpeed)
				m_velocity.z=-g_maxSpeed;
		}


		//calculate new position from velocity
		m_position.x+=(m_velocity.x*deltaTime);
		m_position.y+=(m_velocity.y*deltaTime);
		m_position.z+=(m_velocity.z*deltaTime);


		//calculate collisions against the side of the arena
		if(m_position.x>(sideLength-1))
		{
			t=((sideLength-1)-m_oldPosition.x)/(m_position.x-m_oldPosition.x);
			if(t>=0 && t<=1)
			{
				y=m_oldPosition.y+(t*(m_position.y-m_oldPosition.y));
				m_position.y=y;
				m_position.x=sideLength-1;
			}

			m_velocity.x=m_velocity.x*-1;
			m_bounceFlagX=-1;
		}
		if(m_position.x<(1-sideLength))
		{
			t=((1-sideLength)-m_oldPosition.x)/(m_position.x-m_oldPosition.x);
			if(t>=0 && t<=1)
			{
				y=m_oldPosition.y+(t*(m_position.y-m_oldPosition.y));
				m_position.y=y;
				m_position.x=1-sideLength;
			}

			m_velocity.x=m_velocity.x*-1;
			m_bounceFlagX=1;
		}

		if(m_position.y>(sideLength-1))
		{
			t=((sideLength-1)-m_oldPosition.y)/(m_position.y-m_oldPosition.y);
			if(t>=0 && t<=1)
			{
				y=m_oldPosition.x+(t*(m_position.x-m_oldPosition.x));
				m_position.y=sideLength-1;
				m_position.x=y;
			}

			m_velocity.y=m_velocity.y*-1;
			m_bounceFlagY=-1;
		}

		if(m_position.y<(1-sideLength) && m_position.y>(-sideLength))
		{
			t=((1-sideLength)-m_oldPosition.y)/(m_position.y-m_oldPosition.y);
			if(t>=0 && t<=1)
			{
				y=m_oldPosition.x+(t*(m_position.x-m_oldPosition.x));
				if(y<paddlePos+1.2 && y>paddlePos+0.5)
				{
					m_position.y=1-sideLength;
					m_position.x=y;
					m_velocity.y=m_velocity.y*-1;
					m_bounceFlagY=1;

					m_velocity.y+=0.7;
					m_velocity.x+=1.0;
				}
				if(y<paddlePos+0.5 && y>paddlePos-0.5)
				{
					m_position.y=1-sideLength;
					m_position.x=y;
					m_velocity.y=m_velocity.y*-1;
					m_bounceFlagY=1;

					m_velocity.y+=0.7;
				}
				if(y>paddlePos-1.2 && y<paddlePos-0.5)
				{
					m_position.y=1-sideLength;
					m_position.x=y;
					m_velocity.y=m_velocity.y*-1;
					m_bounceFlagY=1;

					m_velocity.y+=0.7;
					m_velocity.x-=1.0;
				}
			}
		}
		//end collision tests

	}
	}
	g_time=time;
}

void CBall::setForce(Vector force)
{
	g_force=force;
}

void CBall::setAccelFlag(int flag)
{
	m_accelFlag=flag;
}

Vector CBall::returnPosition()
{
	return m_position;
}

//when each ball is created this function is called
//so it starts with the following predefined variables
void CBall::Init(float sideLength, float paddlePosition)
{
 	m_bounceFlagX=1;
	m_bounceFlagY=1;
	m_bounceFlagZ=0;

	m_accelFlag=0;

	g_moveFlag=1;

	g_maxSpeed=20;

	g_force.x=0;
	g_force.y=-10;
	g_force.z=0;

	g_time=glutGet(GLUT_ELAPSED_TIME)/1000.0f;
}

//check for collisions against a given tiles position
int CBall::checkTileCollisions(Vector position)
{
	if(m_killFlag==0)
	{

		if(m_position.y<(position.y+0.5) && m_position.y>(position.y+0.35))
		{
				if(m_position.x<position.x+1 && m_position.x>position.x-1)
				{
					m_position.y=m_position.y+0.25;
					m_velocity.y=m_velocity.y*1.2;
					m_velocity.y=m_velocity.y*-1;

					m_oldPosition=m_position;

					return 1;
				}
		}

		if(m_position.y<(position.y-0.35) && m_position.y>(position.y-0.5))
		{
				if(m_position.x<position.x+1 && m_position.x>position.x-1)
				{
					m_position.y=m_position.y-0.25;
					m_velocity.y=m_velocity.y*1.2;
					m_velocity.y=m_velocity.y*-1;

					m_oldPosition=m_position;

					return 1;
				}
		}

		if(m_position.x<(position.x+1.0) && m_position.x>(position.x+0.85))
		{
				if(m_position.y<position.y+0.5 && m_position.y>position.y-0.5)
				{
					m_position.x=m_position.x+0.1;
					m_velocity.x=m_velocity.x*1.2;
					m_velocity.x=m_velocity.x*-1;

					m_oldPosition=m_position;

					return 1;
				}
		}

		if(m_position.x<(position.x-0.85) && m_position.x>(position.x-1.0))
		{
				if(m_position.y<position.y+0.5 && m_position.y>position.y-0.5)
				{
					m_position.x=m_position.x-0.1;
					m_velocity.x=m_velocity.x*1.2;
					m_velocity.x=m_velocity.x*-1;

					m_oldPosition=m_position;

					return 1;
				}
		}

	m_oldPosition=m_position;
	}
	return 0;
}

Vector CBall::returnVelocity()
{
	return m_velocity;
}

Vector CBall::returnOldPosition()
{
	return m_oldPosition;
}

Vector CBall::returnForce()
{
	return g_force;
}

Vector CBall::getForce()
{
	return g_force;
}

void CBall::setKillFlag(int flag)
{
	m_killFlag=flag;
}

int CBall::returnKillFlag()
{
	return m_killFlag;
}

void CBall::setPosition(Vector position)
{
	m_position=position;
}

void CBall::setVelocity(Vector velocity)
{
	m_velocity=velocity;
}
