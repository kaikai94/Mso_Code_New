




// Tile.h: interface for the CTile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILE_H__7312558F_B883_49C4_8AAA_69CA0AE2D8F4__INCLUDED_)
#define AFX_TILE_H__7312558F_B883_49C4_8AAA_69CA0AE2D8F4__INCLUDED_

#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000
//[自动寻路][By:慌张的油条][QQ:112582793][2018/9/15]
#include "IPathNode.h"
#define TFA_COLLISON	0x00000001

typedef WORD TILEATTR;


struct TILE_ATTR
{
	TILEATTR uAttr : 8;			//  0 ~ 255
	TILEATTR uSkillObjectBlock : 8;		//	0 ~ 255
};

class CTile  : public IPathNode
{

	TILE_ATTR m_Attr;
public:
	//[自动寻路][By:慌张的油条][QQ:112582793][2018/9/15]
	CTile();
	virtual ~CTile();


	void AddTileAttrib(AREATILE areatile);

	void RemoveTileAttrib(AREATILE areatile);

	inline void SetTileAttrib(TILE_ATTR Attr)
	{
		m_Attr = Attr;
	}
	inline BOOL IsCollisonTile()

	{
		return m_Attr.uAttr == TFA_COLLISON || m_Attr.uSkillObjectBlock != 0;
	}
	//[自动寻路][By:慌张的油条][QQ:112582793][2018/9/15]
	virtual void Calc_Cost(IPathNode* p_DestinationNode);

	virtual void Astar_MakeOpened( IPathNode* p_NowSelectedNode, IPathNode* p_DestinationNode );
	virtual BOOL Astar_CompareParentCost( IPathNode* p_NowSelectedNode, int p_Index );	
private:
	DWORD		m_PosX;
	DWORD		m_PosY;
	TILE_ATTR	m_NodeAttrib;

public:
	DWORD		GetPosX() const { return m_PosX; }
	void		SetPosX(DWORD val) { m_PosX = val; }
	DWORD		GetPosY() const { return m_PosY; }
	void		SetPosY(DWORD val) { m_PosY = val; }

};



#endif // !defined(AFX_TILE_H__7312558F_B883_49C4_8AAA_69CA0AE2D8F4__INCLUDED_)


