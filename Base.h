// Base.h: interface for the CBase class.
//
//Just used to allow all the other classes use of the
//"Vector" structure.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASE_H__1CA79BEC_07C3_4FC1_B201_5F98B8FCF2EB__INCLUDED_)
#define AFX_BASE_H__1CA79BEC_07C3_4FC1_B201_5F98B8FCF2EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct Vector			
{
	float x;
	float y;
	float z;
};

class CBase  
{
public:
	CBase();
	virtual ~CBase();

};

#endif // !defined(AFX_BASE_H__1CA79BEC_07C3_4FC1_B201_5F98B8FCF2EB__INCLUDED_)
