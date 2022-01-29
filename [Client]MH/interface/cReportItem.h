



// cReportItem.h: interface for the cReportItem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _cREPORTITEM_H_
#define _cREPORTITEM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cWindowDef.h"
#include "cLooseLinkedList.h"

class RITEM
{

public:
	RITEM(int cols)
	{
		pString = new char*[cols];
		rgb = new DWORD[cols];
		//dwData = new DWORD[cols];
		//memset(dwData,0,sizeof(DWORD)*cols);
		memset(pString,0,sizeof(char*)*cols);
		for(int i=0;i<cols;i++)
		{
			pString[i] = new char[MAX_REPORTTEXT_SIZE];
			rgb[i] = 0xffffffff;
			//*pString[i]=0;
		//	memset(pString[i], 0, sizeof(char)*MAX_REPORTTEXT_SIZE);
		}
		this->cols = cols;
	}
	virtual ~RITEM()
	{
		for(int i=0;i<cols;i++)
			delete [] pString[i];
		delete [] pString;
		delete [] rgb;
		//delete [] dwData;
	}
	
	char ** pString;
	DWORD * rgb;
	//DWORD * dwData;					// ÀÎµ¦½º, Å°¸¦ ³Ö´Â º¯¼ö·Î »ç¿ë
	int cols;
	
};

class cReportItem  
{
public:
	cReportItem():m_Cols(0),m_CurColIdx(0)
	{

		m_pItemList = new cLooseLinkedList<RITEM>;

		pszColumnHeader = NULL;
		pColumnTextColor = NULL;
		pColumnSize = NULL;
	}
	virtual ~cReportItem()
	{
		delete m_pItemList;
		delete [] pColumnTextColor;
		delete [] pColumnSize;
		for(int i=0;i<m_Cols;i++)
			delete [] pszColumnHeader[i];
		delete [] pszColumnHeader;
	}

	void Create(int nMaxCols)
	{
		m_Cols = nMaxCols;
		pszColumnHeader = new char*[m_Cols];
		pColumnTextColor = new DWORD[m_Cols];
		pColumnSize = new DWORD[m_Cols];
		memset(pszColumnHeader, 0, sizeof(char*)*m_Cols);
		memset(pColumnTextColor, 0, sizeof(DWORD)*m_Cols);
		memset(pColumnSize, 0, sizeof(DWORD)*m_Cols);

	}


	LONG GetItemCount()
	{
		return m_pItemList->GetCount();
	}
	void InsertColoumn(WORD nCol, char * szColumnHeading, WORD wColumnSize, DWORD fgcolor=RGB_HALF(255,255,255))
	{
		ASSERT(m_Cols != 0);
		pszColumnHeader[nCol] = new char[wColumnSize];
		pColumnTextColor[nCol] = fgcolor;
		pColumnSize[nCol] = wColumnSize;
		memset(pszColumnHeader[nCol], 0, sizeof(char)*wColumnSize);
		strncpy(pszColumnHeader[nCol], szColumnHeading, strlen(szColumnHeading));
		m_CurColIdx++;

	}
	void InsertItem(WORD nItem, RITEM * ritem)
	{
		/*RITEM * NewRItem = new RITEM(m_Cols);
		for(int i = 0 ; i < m_Cols ;++i)
		{
			strcpy(NewRItem->pString[i], ritem->pString[i]);
			NewRItem->rgb[i] = ritem->rgb[i];
		}

		*/
		
		if(nItem < GetItemCount())
		{
			// ³ªÁß¿¡ °í·Á..
			m_pItemList->InsertpAfter(ritem, nItem);
		}
		else
		{
			m_pItemList->AddTail(ritem);
		}
			
	}
	void SetItemText(WORD nItem/*y*/, WORD nSubItem/*x*/, char * szText, DWORD fgcolor=RGB_HALF(0,0,0))
	{
		if(nItem == 0)
		{
			strcpy(pszColumnHeader[nSubItem], szText);
			pColumnTextColor[nSubItem] = fgcolor;
		}
		else if(0 < nItem && nItem < GetItemCount())
		{
			// ÀÖ´Â°Å °¡Á®¿È
			RITEM * item = m_pItemList->GetAt(nItem);
			strcpy(item->pString[nSubItem], szText);
			item->rgb[nSubItem] = fgcolor;
		}
		else
		{

			//¿¡·¯
		}
	}

	// »ç¿ëÀÚ°¡ Áö¿ö¾ß ÇÔ
	RITEM * DeleteItem(WORD nItem)
	{

		RITEM * item = m_pItemList->DeleteAt(nItem);		// return °ª ÀÌ»óÇØ-_-;
		//if(item)
		//	delete item;
		return item;
	}

	// ¸Þ¸ð¸® ÇÒ´ç¿µ¿ª ±îÁö Áö¿öÁÜ

	void DeleteAllItems()
	{
		//m_pItemList->DeleteAll();

		// 2014-10-23 游戏出错在这里处�?

		RITEM * ritem = NULL;

		while(ritem = m_pItemList->DeleteHead())
		{

			delete ritem;
			ritem = NULL;
		}

		ASSERT(GetItemCount() == 0);
	}
	RITEM * GetRItem(WORD nItem)
	{
		if(nItem < GetItemCount())
			return m_pItemList->GetAt(nItem);
		else
			return NULL;	
	}

protected:
	char ** pszColumnHeader;
	DWORD * pColumnTextColor;
	DWORD * pColumnSize;
	int m_Cols;
	int m_CurColIdx;
	cLooseLinkedList<RITEM> * m_pItemList;
};

#endif // _cREPORTITEM_H_


