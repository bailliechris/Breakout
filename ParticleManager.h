// ParticleManager.h: interface for the CParticleManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTICLEMANAGER_H__45D5536D_F4BC_4882_84EE_CF92A5CB8C67__INCLUDED_)
#define AFX_PARTICLEMANAGER_H__45D5536D_F4BC_4882_84EE_CF92A5CB8C67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base.h"
#include <stdlib.h>
#include <GL/glut.h>
#include "Particle.h"

class CParticle;

class CParticleManager : public CBase  
{
public:
	void setNumberOfParticles(int number);
	CParticle particle[2000];
	int returnArraySize(void);
	int randMinMax(int min, int max);
	int killDeadParticle(int i);
	int createParticle(Vector position);
	CParticleManager();
	virtual ~CParticleManager();

private:
	int m_arraySize;

};

#endif // !defined(AFX_PARTICLEMANAGER_H__45D5536D_F4BC_4882_84EE_CF92A5CB8C67__INCLUDED_)
