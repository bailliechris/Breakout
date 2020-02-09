// TileManager.h: interface for the CTileManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILEMANAGER_H__A02D2228_8DBC_4B2B_9ABF_265BCE44DFF9__INCLUDED_)
#define AFX_TILEMANAGER_H__A02D2228_8DBC_4B2B_9ABF_265BCE44DFF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base.h"
#include "Tile.h"

class CTile;

class CTileManager : public CBase
{
public:
	int getLevelNumber(void);
	void incrementLevel(int numberOfLevels);
	Vector getTilePosition(int i);
	int returnKillFlag(int j);
	int returnAmountInArray(void);
	int initManager(int amountOfTiles);
	int destroyTile(int i);
	int createTile(Vector position);
	CTileManager();
	virtual ~CTileManager();
private:
	int m_score;
	int m_levelNumber;
	int m_arraySize;
	CTile Tile[10];

};

#endif // !defined(AFX_TILEMANAGER_H__A02D2228_8DBC_4B2B_9ABF_265BCE44DFF9__INCLUDED_)
