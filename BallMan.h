// BallMan.h: interface for the CBallMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BALLMAN_H__48FC5492_86CC_433F_9AD0_45A765F46D9E__INCLUDED_)
#define AFX_BALLMAN_H__48FC5492_86CC_433F_9AD0_45A765F46D9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base.h"
#include "Ball.h"

class CBall;

class CBallMan : public CBase  
{
public:
	CBall balls[250];
	int checkPosition(float sideLength, float y, int i);
	int checkVelocity(int i);
	int createBall(float sideLength, float paddlePosition);
	int killDeadBall(int i);
	int returnArraySize(void);
	CBallMan();
	virtual ~CBallMan();
private:
	int m_arraySize;
};

#endif // !defined(AFX_BALLMAN_H__48FC5492_86CC_433F_9AD0_45A765F46D9E__INCLUDED_)
