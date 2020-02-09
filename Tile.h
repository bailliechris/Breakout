// Tile.h: interface for the CTile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILE_H__7D7F9EBE_29AE_4E86_9729_2958320637AD__INCLUDED_)
#define AFX_TILE_H__7D7F9EBE_29AE_4E86_9729_2958320637AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base.h"

class CTile : public CBase
{
public:
	int returnKillFlag(void);
	void setKillFlag(int flag);
	void setArrayPosition(unsigned int position);
	unsigned int getArrayPosition(void);
	Vector getPosition(void);
	void setPosition(Vector position);
	CTile();
	virtual ~CTile();

private:
	int m_killFlag;
	unsigned int m_arrayPosition;
	Vector m_position;
};

#endif // !defined(AFX_TILE_H__7D7F9EBE_29AE_4E86_9729_2958320637AD__INCLUDED_)
