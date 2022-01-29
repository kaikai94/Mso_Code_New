



#ifndef __NPCSCRIPT_MANAGER__
#define __NPCSCRIPT_MANAGER__


#include "PtrList.h"

#include "../MHFile.h"


#define NPCSCRIPTMGR USINGTON(cNpcScriptManager)


#define MAX_NPC_COUNT 1000 


#define NPC_SCRIPT_PATH		"./Image/InterfaceScript/Npc_Script.bin"
#define NPC_DIALOGUE_PATH	"./Image/Npc_Msg.bin"
#define NPC_HYPERTEXT_PATH	"./Image/Npc_HyperText.bin"


class cPage;
class cDialogueList;
class cHyperTextList;
class CNpc;

class cNpcScriptManager 
{
protected:
	cPtrList			m_NpcScriptList[MAX_NPC_COUNT];
	int					m_nRegistNpcCount;

	cDialogueList*		m_pDialogue;

	cHyperTextList*		m_pHyperText;


	CNpc*				m_pCurSelectedNpc; // ¼±ÅÃ Npc¿¡ ´ëÇÑ Á¤º¸
public:
//	//MAKESINGLETON(cNpcScriptManager);

	cNpcScriptManager();
	virtual ~cNpcScriptManager();
	
	void Init();


	void AddNpcScriptInfo( DWORD dwNpcId, cPage* pPage );
	void DeleteAllScriptInfo();

	// ÆäÀÌÁö Á¤º¸
	cPage* GetMainPage( DWORD dwNpcId );					// ½ÃÀÛ ÆäÀÌÁö¸¦ ¾ò´Â´Ù.
	cPage* GetPage( DWORD dwNpcId, DWORD dwPageId );		// ¸µÅ©µÈ ÆäÀÌÁö¸¦ ¾ò´Â´Ù.

	// 1. NPC Script µ¥ÀÌÅÍ ·Îµù.. 
	void LoadNpcScriptInfo();
	void LoadPageInfo(cPage* pPage, CMHFile* fp);

	cDialogueList* GetDialogueList() { return m_pDialogue; }
	cHyperTextList* GetHyperTextList() { return m_pHyperText; }

	void SetSelectedNpc( CNpc* pNpc ) { m_pCurSelectedNpc = pNpc; }
	CNpc* GetSelectedNpc() { return m_pCurSelectedNpc; }

	// 

	void StartNpcScript( CNpc* pNpc );
	void StartNpcBusiness(DWORD dwData, DWORD dwParam=0);
};
EXTERNGLOBALTON(cNpcScriptManager)
#endif


