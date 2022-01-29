// MToolMouseStatus.h: interface for the CToolMouseStatus class.
//
//////////////////////////////////////////////////////////////////////
/*
	���������� ���ư� �� �ִ� Ŭ������ �ƴϴ�.
*/
#if !defined(AFX_TOOLMOUSESTATUS_H__D1FE7C12_4295_41B2_90A3_5EC282CCA92A__INCLUDED_)
#define AFX_TOOLMOUSESTATUS_H__D1FE7C12_4295_41B2_90A3_5EC282CCA92A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGRX_common/IExecutive.h"
#include "../4DyuchiGRX_common/IGeometry.h"

//	���콺 �̺�Ʈ.
enum eMouseMode
{
	MOUSE_NONE,
	MOUSE_MOVE_CAM,
	MOUSE_ROTATE_CAM,
	MOUSE_MOVEHORIZON_CAM,
	MOUSE_MOVEVERTICAL_CAM,
	MOUSE_ROTATE_WITH_PIVOT_CAM,
	MOUSE_ZOOM_CAM,
};

class MToolMouseStatus  
{
protected:		// ������.
	int		miOldMouseX;
	int		miOldMouseY;
	int		miMouseX;
	int		miMouseY;
	int		miMouseZ;		// ��.

	float	mfMoveHorizonStep;
	float	mfMoveVerticalStep;
	float	mfMoveStep;
	float	mfZoomStep;
	float	mfRotateStep;	// Shift�� ���� ���°� �޶����� ȸ��, �̵� �ӵ���.

	float	mfPickDist;

	bool	mbLButtonDown;
	bool	mbRButtonDown;
	bool	mbMButtonDown;
	bool	mbCtrlDown;
	bool	mbShiftDown;		// ����Ʈ ��ư�� ��� �̵�.
	bool	mbAltDown;

	DWORD	mdwMouseMode;

	I4DyuchiGXExecutive*		mpExecutive;
	I4DyuchiGXGeometry*			mpGeometry;

	BOOL	mbDoNotRotate;
public:

	// �ʱ�ȭ �Ҷ� �� �־�����Ѵ�.
	BOOL	Initialize(I4DyuchiGXExecutive* pExecutive);

	// ������Ʈ ��尡 WithPivot�϶� ������.
	void SetRotationPivot( VECTOR3* pPivot);
	// WM_MOUSEMOVE �޽����� �޾����� ȣ���� �Լ�. dwFlag�� ��ư ����.
	bool MoveMouse( DWORD dwFlag, int iMouseX, int iMouseY, DWORD& viewType);

	void	DoNotRotate(void);
	void	AllowRotate(void);

	void	PushCameraAngleAndPos(DWORD	dwStackNum);
	void	PopCameraAngleAndPos(DWORD	dwStackNum);
	DWORD	GetMouseMode() {return mdwMouseMode;}
	MToolMouseStatus();
	virtual ~MToolMouseStatus();
	void MoveCamera_Forward( float p_Value );
	void MoveCamera_Backward( float p_Value );
	void MoveCamera_LeftSideStep( float p_Value );
	void MoveCamera_RightSideStep( float p_Value );
	void MoveCamera_Up( float p_Value );
	void MoveCamera_Down( float p_Value );


	VECTOR3		mpCameraStackPos[10];
	float		mpfCameraStackFromTo[10];
	VECTOR3		mpCameraStackAngle[10];

	VECTOR3		mRotationPivot;

protected:		// �Լ���.
	bool MouseNone(void);
	bool MouseMove(void);
	bool MouseRotate(void);
	bool MouseMoveVertical(void);
	bool MouseMoveHorizon(void);
	bool MouseRotateWithPivot(void);
	bool MouseMoveForward(void);
};

#endif // !defined(AFX_TOOLMOUSESTATUS_H__D1FE7C12_4295_41B2_90A3_5EC282CCA92A__INCLUDED_)
