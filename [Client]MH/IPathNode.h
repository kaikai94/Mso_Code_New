#pragma once

enum eAStarStatus
{
	eUnidentified, eOpened, eClosed,
};


class IPathNode
{
public:
	IPathNode()
		: m_GCost(0)
		, m_HCost(0)
		, m_FCost(0)
		, m_AstarParentNode(NULL)
		, m_AstarNextNode(NULL)
		, m_AStarStatus(eUnidentified)
	{
	}

	void Reset()
	{
		m_GCost = 0;
		m_HCost = 0;
		m_FCost = 0;
		m_AstarParentNode = NULL;
		m_AstarNextNode = NULL;
		m_AStarStatus = eUnidentified;
	}

	float	GetGCost() const	{ return m_GCost; }
	void	SetGCost(float val) { m_GCost = val; }
	float	GetHCost() const	{ return m_HCost; }
	void	SetHCost(float val) { m_HCost = val; }
	float	GetFCost() const	{ return m_FCost; }
	void	SetFCost(float val) { m_FCost = val; }

	virtual void Calc_Cost(IPathNode* p_DestinationNode) PURE;


	virtual void Astar_MakeOpened( IPathNode* p_NowSelectedNode, IPathNode* p_DestinationNode ) PURE;
	virtual BOOL Astar_CompareParentCost( IPathNode* p_NowSelectedNode, int p_Index ) PURE;				

	eAStarStatus GetAStarStatus() const { return m_AStarStatus; }
	void SetAStarStatus(eAStarStatus val) { m_AStarStatus = val; }


public:
	virtual DWORD	GetPosX() const PURE;
	virtual void	SetPosX(DWORD val) PURE;
	virtual DWORD	GetPosY() const PURE;
	virtual void	SetPosY(DWORD val) PURE;

	IPathNode* GetAstarParentNode() const { return m_AstarParentNode; }
	void SetAstarNextNode(IPathNode* val) { m_AstarNextNode = val; }

protected:
	float			m_GCost;			
	float			m_HCost;			
	float			m_FCost;		

	IPathNode*		m_AstarParentNode;	
	IPathNode*		m_AstarNextNode;	
	eAStarStatus	m_AStarStatus;		
};

