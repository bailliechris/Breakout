// ParticleManager.cpp: implementation of the CParticleManager class.
//
//////////////////////////////////////////////////////////////////////

#include "ParticleManager.h"
#include "Particle.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParticleManager::CParticleManager()
{
	m_arraySize=2000;
}

CParticleManager::~CParticleManager()
{

}

int CParticleManager::createParticle(Vector position)
{
	int marker=0;
	Vector velocity;
	float time;

	for(marker=0; marker<m_arraySize; marker++)
	{
		if(particle[marker].returnKillFlag()==1)
		{
			particle[marker].setOldPosition(position);

			//randomise everything except for the starting position
			//which is the position of the block that's just been
			//destroyed
			velocity.x=5*sin(randMinMax(-10, 10));
			velocity.y=5*cos(randMinMax(-10, 10));
			velocity.z=5*sin(randMinMax(-10, 10));

			time=5*sin(randMinMax(-10, 10));

			//set these random variables
			particle[marker].setPosition(position);
			particle[marker].setVelocity(velocity);
			particle[marker].setLifeRemaining(time);
			particle[marker].setKillFlag(0);
		}
	}
	return 0;
}

int CParticleManager::killDeadParticle(int i)
{
	if(i<m_arraySize && i>-1)
	{
		particle[i].setKillFlag(1);
		return 0;
	}

	return 1;
}

//used to create the random numbers
int CParticleManager::randMinMax(int min, int max)
{
	long rnum = (max - min + 1) * rand()/100 + min;

	return rnum;
}

int CParticleManager::returnArraySize()
{
	return m_arraySize;
}

void CParticleManager::setNumberOfParticles(int number)
{
	m_arraySize=number;
}
