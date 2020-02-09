// Tile.cpp: implementation of the CTile class.
//
//////////////////////////////////////////////////////////////////////

#include "Tile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTile::CTile()
{
	m_killFlag=1;
}

CTile::~CTile()
{

}

Vector CTile::getPosition()
{
	return m_position;
}

void CTile::setPosition(Vector position)
{
	m_position=position;
}

unsigned int CTile::getArrayPosition()
{
	return m_arrayPosition;
}

void CTile::setArrayPosition(unsigned int position)
{
	m_arrayPosition=position;
}

void CTile::setKillFlag(int flag)
{
	m_killFlag=flag;
}

int CTile::returnKillFlag()
{
	return m_killFlag;
}
