// Ball.h: interface for the CBall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BALL_H__B1E85636_310B_4DBB_9C30_D1A5F5158569__INCLUDED_)
#define AFX_BALL_H__B1E85636_310B_4DBB_9C30_D1A5F5158569__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include <GL/glut.h>
#include "Base.h"

class CBall : public CBase 
{
public:
	void setVelocity(Vector velocity);
	void setPosition(Vector position);
	int returnKillFlag(void);
	void setKillFlag(int flag);
	Vector getForce(void);
	Vector returnForce(void);
	Vector returnOldPosition(void);
	Vector returnVelocity(void);
	int checkTileCollisions(Vector position);
	void Init(float sideLength, float paddlePosition);
	float g_maxSpeed;
	int g_moveFlag;
	Vector returnPosition(void);
	void setAccelFlag(int flag);
	void setForce(Vector force);
	void update(float sideLength, float paddlePos);
	float g_mass;
	float g_time;
	CBall();
	virtual ~CBall();

private:
	int m_killFlag;
	Vector m_oldPosition;
	Vector g_force;
	int m_accelFlag;
	int m_bounceFlagZ;
	int m_bounceFlagY;
	int m_bounceFlagX;
	Vector m_accel;
	Vector m_velocity;
	Vector m_position;
};

#endif // !defined(AFX_BALL_H__B1E85636_310B_4DBB_9C30_D1A5F5158569__INCLUDED_)