// Particle.h: interface for the CParticle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTICLE_H__13A00DA3_8EE9_464D_A321_C0A97538DEDD__INCLUDED_)
#define AFX_PARTICLE_H__13A00DA3_8EE9_464D_A321_C0A97538DEDD__INCLUDED_

#include "Base.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base.h"
#include <stdlib.h>
#include <GL/glut.h>

class CParticle : public CBase
{
public:
	void setForce(Vector force);
	void setLifeRemaining(float life);
	void setOldPosition(Vector position);
	void setVelocity(Vector velocity);
	void setPosition(Vector position);
	void setKillFlag(int flag);
	int returnKillFlag(void);
	float returnLifeTime(void);
	void setMoveFlag(int flag);
	Vector returnVelocity(void);
	Vector returnPosition(void);
	Vector returnOldPosition(void);
	int update(float sideLength, float paddlePos);
	int checkTileCollisions(Vector position);
	CParticle();
	virtual ~CParticle();
		int m_killFlag;

private:
	float m_lifeRemaining;

	float m_lifeTime;
	int m_moveFlag;
	float m_time;
	int m_bounceFlagZ;
	int m_bounceFlagY;
	int m_bounceFlagX;
	Vector m_oldPosition;
	Vector m_velocity;
	Vector m_force;
	Vector m_position;
};

#endif // !defined(AFX_PARTICLE_H__13A00DA3_8EE9_464D_A321_C0A97538DEDD__INCLUDED_)
