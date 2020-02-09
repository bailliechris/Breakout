// Particle.cpp: implementation of the CParticle class.
//
//////////////////////////////////////////////////////////////////////

#include "Particle.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParticle::CParticle()
{
	m_force.x=0;
	m_force.y=-20;
	m_force.z=-20;

	m_killFlag=1;

	m_moveFlag=0;
}

CParticle::~CParticle()
{

}

//check the particle for collisions with a give tiles position
int CParticle::checkTileCollisions(Vector position)
{
	if(m_killFlag==0)
	{
		if(m_moveFlag==0)
		{
			if(m_position.y<(position.y+0.5) && m_position.y>(position.y+0.35))
			{
					if(m_position.x<position.x+1 && m_position.x>position.x-1)
					{
						m_position.y=m_position.y+0.25;
						m_velocity.y=m_velocity.y*0.5;
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
						m_velocity.y=m_velocity.y*0.5;
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
						m_velocity.x=m_velocity.x*0.5;
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
						m_velocity.x=m_velocity.x*0.5;
						m_velocity.x=m_velocity.x*-1;

						m_oldPosition=m_position;

						return 1;
					}
			}

			m_oldPosition=m_position;
		}
	}

	return 0;
}

//update the same as the ball.
int CParticle::update(float sideLength, float paddlePos)
{
	float time, deltaTime, t, y;

	time=glutGet(GLUT_ELAPSED_TIME)/1000.0f;

	deltaTime=time-m_time;

	//if it's not marked as dead
	if(m_killFlag==0)
	{	
		//and is allowed to move, do calculations
		if(m_moveFlag==0)
		{
		m_lifeRemaining=m_lifeRemaining-deltaTime;

		if(m_lifeRemaining<0)
			return 1;

		if(m_bounceFlagX==1)
			m_velocity.x+=m_force.x*deltaTime;
			
		if(m_bounceFlagX==-1)
			m_velocity.x+=m_force.x*deltaTime;
			
		if(m_bounceFlagY==1)
			m_velocity.y+=m_force.y*deltaTime;

		if(m_bounceFlagY==-1)
			m_velocity.y+=m_force.y*deltaTime;

		if(m_bounceFlagZ==1)
			m_velocity.z+=m_force.z*deltaTime;

		if(m_bounceFlagZ==-1)
			m_velocity.z+=m_force.z*deltaTime;

		//calculate the new position
		m_position.x+=(m_velocity.x*deltaTime);
		m_position.y+=(m_velocity.y*deltaTime);
		m_position.z+=(m_velocity.z*deltaTime);

		//collisions against the sides
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
				if(y<paddlePos+1.2 && y>paddlePos-1.2)
				{
					m_position.y=1-sideLength;
					m_position.x=y;
					m_velocity.y=m_velocity.y*-1;
					m_bounceFlagY=1;

					m_velocity.y=m_velocity.y*0.7;
				}
			}
		}

		}

	}
	m_time=time;

	return 0;
}

Vector CParticle::returnOldPosition()
{
	return m_oldPosition;
}

Vector CParticle::returnPosition()
{
	return m_position;
}

Vector CParticle::returnVelocity()
{
	return m_velocity;
}

void CParticle::setMoveFlag(int flag)
{
	m_moveFlag=flag;
}

float CParticle::returnLifeTime()
{
	return m_lifeTime;
}

int CParticle::returnKillFlag()
{
	return m_killFlag;
}

void CParticle::setKillFlag(int flag)
{
	m_killFlag=flag;
}

void CParticle::setPosition(Vector position)
{
	m_position=position;
}

void CParticle::setVelocity(Vector velocity)
{
	m_velocity=velocity;
}

void CParticle::setOldPosition(Vector position)
{
	m_oldPosition=position;
}

void CParticle::setLifeRemaining(float life)
{
	m_lifeRemaining=life;
}

void CParticle::setForce(Vector force)
{
	m_force=force;
}
