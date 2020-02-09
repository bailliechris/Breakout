// TileManager.cpp: implementation of the CTileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "TileManager.h"
#include "Tile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTileManager::CTileManager()
{
	m_levelNumber=0;
	m_score=0;
}

CTileManager::~CTileManager()
{

}

//this function searches through the array for
//an entry marked as "dead" and then resets
//the information with new positions, velocity
//etc and marks it as "alive". 
int CTileManager::createTile(Vector position)
{
	int marker=0;

	while(marker!=-1)
	{
		if(marker<(m_arraySize-1) && marker>-1)
		{
			if(Tile[marker].returnKillFlag()==1)
			{
				Tile[marker].setPosition(position);
				Tile[marker].setKillFlag(0);
				marker=-1;
				return 0;
			}
		}
		marker++;
	}
	return 1;
}

int CTileManager::destroyTile(int i)
{
	if(i<m_arraySize && i>-1)
	{
		Tile[i].setKillFlag(1);
		m_score++;
		return 0;
	}

	return 1;
}

int CTileManager::initManager(int amountOfTiles)
{
	m_arraySize=10;

	return 0;
}

int CTileManager::returnAmountInArray()
{
	return m_arraySize;
}

int CTileManager::returnKillFlag(int j)
{
	return Tile[j].returnKillFlag();
}

Vector CTileManager::getTilePosition(int i)
{
	return Tile[i].getPosition();
}

void CTileManager::incrementLevel(int numberOfLevels)
{
	m_levelNumber++;
	if(m_levelNumber>numberOfLevels-1)
		m_levelNumber=0;
}

int CTileManager::getLevelNumber()
{
	return m_levelNumber;
}
