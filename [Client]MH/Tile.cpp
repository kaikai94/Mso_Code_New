











// Tile.cpp: implementation of the CTile class.


//

//////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Tile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTile::CTile()
:m_PosX(0)
,m_PosY(0)
{

}

CTile::~CTile()
{

}
void CTile::AddTileAttrib(AREATILE areatile)

{
	if(areatile == SKILLAREA_ATTR_BLOCK)

	{

		++m_Attr.uSkillObjectBlock;
	}

}



void CTile::RemoveTileAttrib(AREATILE areatile)

{
	if(areatile == SKILLAREA_ATTR_BLOCK)
	{
		ASSERT(m_Attr.uSkillObjectBlock > 0);
		if(m_Attr.uSkillObjectBlock != 0)

			--m_Attr.uSkillObjectBlock;

	}

}
void CTile::Calc_Cost(IPathNode* p_DestinationNode)
{
	CTile* t_DestinationNode = static_cast<CTile*>(p_DestinationNode);

	if (m_AstarParentNode == NULL)
	{
		m_GCost = 0.f;
	}
	else
	{
		int t_DistInt = abs( (int)m_PosX - (int)m_AstarParentNode->GetPosX()) + abs( (int)m_PosY - (int)m_AstarParentNode->GetPosY());
		float t_Dist = (t_DistInt == 1) ? 1.0f : 1.4142f;
		m_GCost =  m_AstarParentNode->GetGCost() + t_Dist;
	}

	m_HCost = sqrt( pow( (float)((int)m_PosX - (int)t_DestinationNode->GetPosX()), 2) + 
		pow( (float)((int)m_PosY - (int)t_DestinationNode->GetPosY()), 2) );

	m_FCost = m_GCost + m_HCost;
}

void CTile::Astar_MakeOpened( IPathNode* p_NowSelectedNode, IPathNode* p_DestinationNode )
{
	m_AstarParentNode = p_NowSelectedNode;
	Calc_Cost(p_DestinationNode);
	m_AStarStatus = eOpened;	
}

BOOL CTile::Astar_CompareParentCost( IPathNode* p_NowSelectedNode, int p_Index )
{
	float t_Dist = 0;
	if (p_Index < 4) t_Dist = 1;
	if (p_Index >= 4) t_Dist = 1.4142f;
	if ( p_NowSelectedNode->GetGCost() + t_Dist < m_GCost )
	{
		m_AstarParentNode = p_NowSelectedNode;
		return TRUE;
	}

	return FALSE;
}