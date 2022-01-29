// PtrList.cpp: implementation of the cPtrList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "PtrList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define ASSERT assert
void* pNullPoint	= NULL;

//////////////////////////////////////////////////////////////////
//				CPlex	class									//
//////////////////////////////////////////////////////////////////
CPlex* PASCAL CPlex::Create(CPlex*& pHead, UINT nMax, UINT cbElement)
{
	CPlex* p = (CPlex*) new BYTE[sizeof(CPlex) + nMax * cbElement];
	p->pNext = pHead;
	pHead = p;
	return p;
}

void CPlex::FreeDataChain()
{
	CPlex* p = this;
	while (p != NULL)
	{
		BYTE* bytes = (BYTE*) p;
		CPlex* pNext = p->pNext;
		delete[] bytes;

		p = pNext;
	}
}


cPtrList::cPtrList(int nBlockSize)
{
	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;

	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
}

void cPtrList::RemoveAll()
{
	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

cPtrList::~cPtrList()
{
	RemoveAll();
}


cPtrList::CNode*
cPtrList::NewNode(cPtrList::CNode* pPrev, cPtrList::CNode* pNext)
{

	if (m_pNodeFree == NULL)
	{
		CPlex* pNewBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CNode));

		CNode* pNode = (CNode*) pNewBlock->data();
		pNode += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pNode--)
		{

			pNode->pNext = m_pNodeFree;
			m_pNodeFree = pNode;
		}
	}

	cPtrList::CNode* pNode = m_pNodeFree;
	m_pNodeFree = m_pNodeFree->pNext;
	pNode->pPrev = pPrev;
	pNode->pNext = pNext;
	m_nCount++;

	pNode->data = NULL;

	return pNode;
}

void cPtrList::FreeNode(cPtrList::CNode* pNode)
{
	pNode->pNext = m_pNodeFree;
	m_pNodeFree = pNode;
	m_nCount--;

	if (m_nCount == 0)
		RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////

PTRLISTPOS cPtrList::AddHead(void* newElement)
{
	CNode* pNewNode = NewNode(NULL, m_pNodeHead);
	pNewNode->data = newElement;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = pNewNode;
	else
		m_pNodeTail = pNewNode;
	m_pNodeHead = pNewNode;
	return (PTRLISTPOS) pNewNode;
}

PTRLISTPOS cPtrList::AddTail(void* newElement)
{
	CNode* pNewNode = NewNode(m_pNodeTail, NULL);
	pNewNode->data = newElement;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = pNewNode;
	else
		m_pNodeHead = pNewNode;
	m_pNodeTail = pNewNode;
	return (PTRLISTPOS) pNewNode;
}

void cPtrList::AddHead(cPtrList* pNewList)
{
	PTRLISTPOS pos = pNewList->GetTailPosition();
	while (pos != NULL)
		AddHead(pNewList->GetPrev(pos));
}

void cPtrList::AddTail(cPtrList* pNewList)
{
	PTRLISTPOS pos = pNewList->GetHeadPosition();
	while (pos != NULL)

		AddTail(pNewList->GetNext(pos));
}

void* cPtrList::RemoveHead()
{
	CNode* pOldNode = m_pNodeHead;
	void* returnValue = pOldNode->data;

	m_pNodeHead = pOldNode->pNext;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = NULL;

	else
		m_pNodeTail = NULL;
	FreeNode(pOldNode);
	return returnValue;
}

void* cPtrList::RemoveTail()
{
	if( this == NULL)
		return pNullPoint;
	if( m_pNodeTail == NULL)
		return pNullPoint;

	CNode* pOldNode = m_pNodeTail;
	void* returnValue = pOldNode->data;

	m_pNodeTail = pOldNode->pPrev;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = NULL;
	else
		m_pNodeHead = NULL;
	FreeNode(pOldNode);
	return returnValue;
}

PTRLISTPOS cPtrList::InsertBefore(PTRLISTPOS position, void* newElement)
{
	if (position == NULL)
		return AddHead(newElement); 

	CNode* pOldNode = (CNode*) position;
	CNode* pNewNode = NewNode(pOldNode->pPrev, pOldNode);
	pNewNode->data = newElement;

	if (pOldNode->pPrev != NULL)
	{
		pOldNode->pPrev->pNext = pNewNode;
	}
	else
	{
		m_pNodeHead = pNewNode;
	}

	pOldNode->pPrev = pNewNode;
	return (PTRLISTPOS) pNewNode;
}

PTRLISTPOS cPtrList::InsertAfter(PTRLISTPOS position, void* newElement)
{
	if (position == NULL)
		return AddTail(newElement);

	CNode* pOldNode = (CNode*) position;
	CNode* pNewNode = NewNode(pOldNode, pOldNode->pNext);
	pNewNode->data = newElement;

	if (pOldNode->pNext != NULL)
	{
		pOldNode->pNext->pPrev = pNewNode;
	}
	else
	{
		m_pNodeTail = pNewNode;
	}
	pOldNode->pNext = pNewNode;
	return (PTRLISTPOS) pNewNode;
}

void cPtrList::RemoveAt(PTRLISTPOS& position)
{
	CNode* pOldNode = (CNode*) position;
	if( pOldNode == NULL)
		return;

	if (pOldNode == m_pNodeHead)
	{
		m_pNodeHead = pOldNode->pNext;

		position	= (PTRLISTPOS)m_pNodeHead;
	}
	else
	{
		pOldNode->pPrev->pNext = pOldNode->pNext;
		position	= (PTRLISTPOS)pOldNode->pPrev->pNext;
	}
	if (pOldNode == m_pNodeTail)
	{
		m_pNodeTail = pOldNode->pPrev;
	}
	else
	{
		pOldNode->pNext->pPrev = pOldNode->pPrev;
	}
	FreeNode(pOldNode);
}


/////////////////////////////////////////////////////////////////////////////

PTRLISTPOS cPtrList::FindIndex(int nIndex) const
{
	if (nIndex >= m_nCount || nIndex < 0)
		return NULL; 

	CNode* pNode = m_pNodeHead;
	while (nIndex--)
	{
		pNode = pNode->pNext;
	}
	return (PTRLISTPOS) pNode;
}

PTRLISTPOS cPtrList::Find(void* searchValue, PTRLISTPOS startAfter) const
{
	CNode* pNode = (CNode*) startAfter;
	if (pNode == NULL)
	{
		pNode = m_pNodeHead; 
	}
	else
	{
		pNode = pNode->pNext; 
	}

	for (; pNode != NULL; pNode = pNode->pNext)
		if (pNode->data == searchValue)
			return (PTRLISTPOS) pNode;
	return NULL;
}


void*& cPtrList::GetHead()
{
	return m_pNodeHead->data;
}

void* cPtrList::GetHead() const
{
	return m_pNodeHead->data;
}

void*& cPtrList::GetTail()
{
	if( m_pNodeTail == NULL)
	{
		return pNullPoint;
	}
	return m_pNodeTail->data;

}

void* cPtrList::GetTail() const
{
	return m_pNodeTail->data;
}

PTRLISTPOS cPtrList::GetHeadPosition() const
{
	return (PTRLISTPOS) m_pNodeHead;
}

PTRLISTPOS cPtrList::GetTailPosition() const
{
	return (PTRLISTPOS) m_pNodeTail;
}


void*& cPtrList::GetNext(PTRLISTPOS& rPosition)
{
	CNode* pNode = (CNode*) rPosition;
	if( pNode == NULL)
		return pNullPoint;

	rPosition = (PTRLISTPOS) pNode->pNext;
	return pNode->data;
}

void* cPtrList::GetNext(PTRLISTPOS& rPosition) const 
{
	CNode* pNode = (CNode*) rPosition;
	rPosition = (PTRLISTPOS) pNode->pNext;
	return pNode->data;
}

void*& cPtrList::GetPrev(PTRLISTPOS& rPosition) 
{
	CNode* pNode = (CNode*) rPosition;
	if( pNode == NULL)
		return pNullPoint;

	rPosition = (PTRLISTPOS) pNode->pPrev;
	return pNode->data;
}


void* cPtrList::GetPrev(PTRLISTPOS& rPosition) const
{
	CNode* pNode = (CNode*) rPosition;
	if( pNode == NULL)
		return pNullPoint;

	rPosition = (PTRLISTPOS) pNode->pPrev;
	return pNode->data;
}

void*& cPtrList::GetAt(PTRLISTPOS position)
{
	CNode* pNode = (CNode*) position;
	return pNode->data;
}

void* cPtrList::GetAt(PTRLISTPOS position) const
{
	CNode* pNode = (CNode*) position;
	return pNode->data;

}

void cPtrList::SetAt(PTRLISTPOS pos, void* newElement)
{
	CNode* pNode = (CNode*) pos;
	pNode->data = newElement;
}


BOOL cPtrList::Remove(void* pRemoveValue)
{
	PTRLISTPOS findpos = NULL;
	CNode* pNode = m_pNodeHead; 
	for (; pNode != NULL; pNode = pNode->pNext)
	{
		if (pNode->data == pRemoveValue)
			findpos = (PTRLISTPOS) pNode;
	}
	if(findpos == NULL)
		return FALSE;
	
	RemoveAt(findpos);
	return TRUE;
}

