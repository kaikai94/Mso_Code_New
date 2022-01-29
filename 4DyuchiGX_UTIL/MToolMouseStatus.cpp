#include "MToolMouseStatus.h"
#include "../4dyuchigrx_common/typedef.h"
#include "../4DyuchiGXGFunc/global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MToolMouseStatus::MToolMouseStatus()
{

}

MToolMouseStatus::~MToolMouseStatus()
{

}

BOOL MToolMouseStatus::Initialize(I4DyuchiGXExecutive* pExecutive)
{
	mfMoveHorizonStep		=	3.0f;
	mfMoveVerticalStep		=	20.0f;
	mfMoveStep				=	0.0007f;
	mfZoomStep				=	20.0f;
	mfPickDist				=	0.0f;
//	fRotateStep		=	0.2f;	// Shift�� ���� ���°� �޶����� ȸ��, �̵� �ӵ���.
	mfRotateStep		=	0.13f * PI / 180.0f;	// Shift�� ���� ���°� �޶����� ȸ��, �̵� �ӵ���.
	mpExecutive = pExecutive;
	mpGeometry = pExecutive->GetGeometry();
	mbDoNotRotate		=	FALSE;

	miMouseX	=	-1;
	miMouseY	=	-1;
	miMouseZ	=	-1;

	miOldMouseX	=	-1;
	miOldMouseY	=	-1;

	mbLButtonDown	=	false;
	mbRButtonDown	=	false;
	mbMButtonDown	=	false;
	mbCtrlDown		=	false;
	mbShiftDown		=	false;		// ����Ʈ ��ư�� ��� �̵�.
	mbAltDown		=	false;

	mdwMouseMode	=	MOUSE_NONE;



	return TRUE;
}
bool MToolMouseStatus::MoveMouse(DWORD dwFlag, int iMouseX, int iMouseY, DWORD &viewType)
{
	if( mpGeometry == 0)
	{
		__asm int 3
		// �������� ���� ���� �ȉ��.
	}
	if( miMouseX == -1 || miMouseY == -1)
	{
		miOldMouseX	=	iMouseX;
		miOldMouseY	=	iMouseY;
	}
	miMouseX	=	iMouseX;
	miMouseY	=	iMouseY;

	mbAltDown = false;
	if (GetAsyncKeyState(VK_LMENU)&0x8000)
	{
		mbAltDown	=	 true;
	}
	//	��Ʈ��Ű.
	mbCtrlDown	=	false;
	if( dwFlag & MK_CONTROL)
	{
		mbCtrlDown	=	true;
	}
	//	����ƮŰ.
	mbShiftDown	=	false;
	if( dwFlag & MK_SHIFT)
	{
		mbShiftDown	=	true;
//		fMoveHorizonStep		=	5.0f;
//		fMoveVerticalStep		=	50.0f;
//		fRotateStep		=	0.5f;	// Shift�� ���� ���°� �޶����� ȸ��, �̵� �ӵ���.
	}
	else
	{
//		fMoveHorizonStep		=	1.0f;
//		fMoveVerticalStep		=	10.0f;
//		fRotateStep		=	0.2f;	// Shift�� ���� ���°� �޶����� ȸ��, �̵� �ӵ���.
	}
	//	���콺 ���� ��ư.
	mbLButtonDown	=	false;
	if( dwFlag & MK_LBUTTON)
	{
		mbLButtonDown	=	true;
	}
	//	���콺 �߰� ��ư.
	mbMButtonDown	=	false;
	if( dwFlag & MK_MBUTTON)
	{
		mbMButtonDown	=	true;
	}
	//	���콺 ������ ��ư.
	mbRButtonDown	=	false;
	if( dwFlag & MK_RBUTTON)
	{
		mbRButtonDown	=	true;
	}

	// ��忡 ���� ����ġ, �Լ� ��.
	switch( mdwMouseMode)
	{
		case	MOUSE_NONE:
				miOldMouseX = iMouseX;
				miOldMouseY = iMouseY;
			// ���콺 ������ ��ư�� ���� ���.
			// �̵� ����.
			if( !mbLButtonDown && mbMButtonDown && !mbRButtonDown && !mbCtrlDown && !mbShiftDown && !mbAltDown)	
			{
				CAMERA_DESC Desc;
				mpGeometry->GetCameraDesc( &Desc, 0);

				VECTOR3 vPickPos;
				POINT pt;
				pt.x = iMouseX;
				pt.y = iMouseY;

				if (!mpExecutive->GXMGetHFieldCollisionPointWithScreenCoord(&vPickPos,&mfPickDist,(POINT*)&pt))
				{
					vPickPos.x = 0.0f;
					vPickPos.y = 0.0f;
					vPickPos.z = 0.0f;

					mfPickDist = 10000.0f;
				}

				mdwMouseMode = MOUSE_MOVE_CAM;	
			}
			// ���콺 ������ ��ư�� ��ƮŰ�� ���� ���.
			// ȸ�� ����.
			if( !mbLButtonDown && mbMButtonDown && !mbRButtonDown && !mbCtrlDown && !mbShiftDown && mbAltDown)	
			{
				if (viewType == 0)
				{
					PushCameraAngleAndPos(0);

					viewType = 6;
				}

				mdwMouseMode = MOUSE_ROTATE_CAM;	
			}
			// ���콺 �� ��ư �巡�� -> �ٽ� ������ ������ �ٲ�
			//	���� �̵� ����.
			else if( !mbLButtonDown && mbMButtonDown && !mbRButtonDown && mbCtrlDown && !mbShiftDown && !mbAltDown)	
			{
				mdwMouseMode	=	MOUSE_MOVEHORIZON_CAM;
			}
			// ���콺 ������ ��ư�� ����Ʈ�� ������ ���.
			//	����(����) �̵� ����.
			else if( !mbLButtonDown && mbMButtonDown && !mbRButtonDown && !mbCtrlDown && mbShiftDown && !mbAltDown)	
			{
				mdwMouseMode	=	MOUSE_MOVEVERTICAL_CAM;
			}
			// ���콺 ������ ��ư�� ����Ʈ,��Ʈ���� ���ÿ� ������ ���.
			//	�־��� �Ǻ��� �߽����� ȸ���Ѵ�.
			else if( !mbLButtonDown && mbMButtonDown && !mbRButtonDown && mbCtrlDown && mbShiftDown && !mbAltDown)
			{
				mdwMouseMode	=	MOUSE_ROTATE_WITH_PIVOT_CAM;
			}
			else if( !mbLButtonDown && mbMButtonDown && !mbRButtonDown && mbCtrlDown && !mbShiftDown && mbAltDown)
			{
				mdwMouseMode	=	MOUSE_ZOOM_CAM;
			}

			return this->MouseNone();
			break;
		case	MOUSE_MOVE_CAM:
			if( mbLButtonDown || !mbMButtonDown || mbRButtonDown || mbCtrlDown || mbShiftDown || mbAltDown)	// ��Ե� ���°� �޶����� ��� ����.
			{
				mdwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseMove();
			}

			break;
		case	MOUSE_ROTATE_CAM:
			if( mbLButtonDown || !mbMButtonDown || mbRButtonDown || mbCtrlDown || mbShiftDown || !mbAltDown)	// ��Ե� ���°� �޶����� ��� ����.
			{
				mdwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseRotate();
			}

			break;
		case	MOUSE_MOVEHORIZON_CAM:
			if( mbLButtonDown || !mbMButtonDown || mbRButtonDown || !mbCtrlDown || mbShiftDown || mbAltDown)	// ��Ե� ���°� �޶����� ��� ����.
			{
				mdwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseMoveHorizon();
			}
			break;
		case	MOUSE_MOVEVERTICAL_CAM:
			if( mbLButtonDown || !mbMButtonDown || mbRButtonDown || mbCtrlDown || !mbShiftDown || mbAltDown)	// ��Ե� ���°� �޶����� ��� ����.
			{
				mdwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseMoveVertical();
			}
			break;
		case	MOUSE_ROTATE_WITH_PIVOT_CAM:
			if( mbLButtonDown || !mbMButtonDown || mbRButtonDown || !mbCtrlDown || !mbShiftDown || mbAltDown)	// ��Ե� ���°� �޶����� ��� ����.
			{
				mdwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseRotateWithPivot();
			}
			break;
		case	MOUSE_ZOOM_CAM:
			if( mbLButtonDown || !mbMButtonDown || mbRButtonDown || !mbCtrlDown || mbShiftDown || !mbAltDown)	// ��Ե� ���°� �޶����� ��� ����.
			{
				mdwMouseMode	=	MOUSE_NONE;
			}
			else
			{
				return	this->MouseMoveForward();
			}
			break;
		default:
			break;
	}

return false;
}

bool MToolMouseStatus::MouseNone()
{
	// �ƹ��͵� ���Ѵ�. -_-;
	return false;
}

bool MToolMouseStatus::MouseMove(void)
{
	CAMERA_DESC Desc;
	mpGeometry->GetCameraDesc( &Desc, 0);

	float	x = float(miMouseX - miOldMouseX) * mfPickDist * mfMoveStep;
	float	y = float(miMouseY - miOldMouseY) * mfPickDist * mfMoveStep;

	VECTOR3	vAt, vUp, vRight, vMove;

	vUp = Desc.v3Up;
	vAt = Desc.v3EyeDir;

	Normalize(&vUp, &vUp);
	Normalize(&vAt, &vAt);

	CrossProduct(&vRight, &vUp, &vAt);
	Normalize(&vRight, &vRight);

	vMove = (vUp * y) + (vRight * -x);

	mpGeometry->MoveCamera(&vMove,0);

	miOldMouseX	=	miMouseX;
	miOldMouseY	=	miMouseY;

	return true;
}

bool MToolMouseStatus::MouseRotate()
{
	if( mbDoNotRotate == TRUE)	return true;		// ���콺 ������Ʈ ���� ����ϰ��...

	float	x = float(miMouseX - miOldMouseX) * mfRotateStep;
	float	y = float(miMouseY - miOldMouseY) * mfRotateStep;
	VECTOR3	Rotate;
	Rotate.x = -y;
	Rotate.y = x;
	Rotate.z = 0;


	mpGeometry->RotateCameraRad(&Rotate,0);

	miOldMouseX	=	miMouseX;
	miOldMouseY	=	miMouseY;

	return true;
}

bool MToolMouseStatus::MouseRotateWithPivot()
{
	if( mbDoNotRotate == TRUE)	return true;		// ���콺 ������Ʈ ���� ����ϰ��...
//	����

	mRotationPivot.x	=	50.0f;
	mRotationPivot.y	=	0.0f;
	mRotationPivot.z	=	0.0f;


	float	fRotateYaw		=	float(miMouseX - miOldMouseX) * mfRotateStep;
	float	fRotatePitch	=	-1.0f * float(miMouseY - miOldMouseY) * mfRotateStep;
	VECTOR3	Rotate;
	VECTOR3		NewPos, CurrentPos;
	CAMERA_DESC Desc;
	mpGeometry->GetCameraDesc( &Desc, 0);
	CurrentPos	=	Desc.v3From;

	VECTOR3		CameraAngle;
	mpGeometry->GetCameraAngleRad( &CameraAngle, 0);
/*
	if( CameraAngle.x >= (PI-0.01)/2.0f)	// ī�޶� �Ʒ��� ���� ���� ���.
	{
		_asm int 3;
	}
	else if( CameraAngle.x <= -1.0f * (PI-0.01)/2.0f)	// ī�޶� �Ʒ��� ���� ���� ���.
	{
		_asm int 3;
	}
*/
	// �Ǻ��� �������� ��ġ�� �ű��,
	RotatePositionWithPivot( &NewPos, &mRotationPivot, &CurrentPos, &CameraAngle, fRotateYaw, fRotatePitch);
	mpGeometry->SetCameraPos( &NewPos, 0);

	// ���콺�� ������Ʈ.	(��ġ �ű�°Ͱ��� ������ ���� ����.)
	Rotate.x = fRotatePitch;
	Rotate.y = fRotateYaw;
	Rotate.z = 0;
	mpGeometry->RotateCameraRad(&Rotate,0);

	miOldMouseX	=	miMouseX;
	miOldMouseY	=	miMouseY;

	return true;
}

bool MToolMouseStatus::MouseMoveForward()
{
	float	y	=	float(miMouseY - miOldMouseY) * -mfZoomStep;

	CAMERA_DESC Desc;
	mpGeometry->GetCameraDesc( &Desc, 0);

	VECTOR3	vAt = Desc.v3EyeDir;

	Normalize(&vAt, &vAt);

	vAt = vAt * y;
	mpGeometry->MoveCamera( &vAt,0);

	miOldMouseX	=	miMouseX;
	miOldMouseY	=	miMouseY;

	return true;
}

bool MToolMouseStatus::MouseMoveHorizon()
{
	float	x	=	float(miMouseX - miOldMouseX) * mfMoveHorizonStep * -1.0f;
	float	y	=	float(miMouseY - miOldMouseY) * mfMoveHorizonStep * -1.0f;
	
	VECTOR3	Rot;
	mpGeometry->GetCameraAngleRad(&Rot,0);

	VECTOR3	To;
	To.x	=	(-1 * x * (float)cosf(Rot.y) *10.0f + y * (float)sinf(Rot.y)*10.0f);
	To.y	=	0.0f;
	To.z	=	x * (float)sinf(Rot.y) *10.0f + y * (float)cosf(Rot.y)*10.0f;

	mpGeometry->MoveCamera( &To,0);

	miOldMouseX	=	miMouseX;
	miOldMouseY	=	miMouseY;
						
	return true;
}

bool MToolMouseStatus::MouseMoveVertical()
{
	VECTOR3		To;
	To.x	=	0;
	To.y	=	float(miMouseY - miOldMouseY) * mfMoveVerticalStep * -1;
	To.z	=	0;
	mpGeometry->MoveCamera(&To,0);

	miOldMouseX	=	miMouseX;
	miOldMouseY	=	miMouseY;

	return true;
}


void	MToolMouseStatus::DoNotRotate(void)
{
	mbDoNotRotate	=	TRUE;
}


void	MToolMouseStatus::AllowRotate(void)
{
	mbDoNotRotate	=	FALSE;
}

void	MToolMouseStatus::PushCameraAngleAndPos(DWORD dwStackNum)
{
	CAMERA_DESC		Desc;
	mpGeometry->GetCameraDesc(&Desc,0);
	mpCameraStackPos[dwStackNum]		=	Desc.v3From;
	mpfCameraStackFromTo[dwStackNum]	=	CalcDistance( &(Desc.v3From), &(Desc.v3To));
	mpGeometry->GetCameraAngleRad(&mpCameraStackAngle[dwStackNum],0);
}

void	MToolMouseStatus::PopCameraAngleAndPos(DWORD dwStackNum)
{
	CAMERA_DESC		Desc;
	mpGeometry->GetCameraDesc(&Desc,0);

	mpCameraStackPos[dwStackNum]		=	Desc.v3From;

	VECTOR3	To	=	mpCameraStackPos[dwStackNum];
	To.z	=	To.z + mpfCameraStackFromTo[dwStackNum];
	VECTOR3 Up;
	Up.x = Up.z = 0;
	Up.y = 1;

	if (dwStackNum == 0 /*VIEW_TYPE_TOP*/)
	{
		mpCameraStackAngle[dwStackNum].x	=	-90.0f * (PI / 180.0f);
		mpCameraStackAngle[dwStackNum].y	=	0.0f;
		mpCameraStackAngle[dwStackNum].z	=	0.0f;
	}

	float fNear = DEFAULT_NEAR;
	float fFar = DEFAULT_FAR;

	mpGeometry->ResetCamera( &(mpCameraStackPos[dwStackNum]),fNear,fFar,DEFAULT_FOV,0);
//	mpGeometry->ResetCamera( &(mpCameraStackPos[dwStackNum]),fNear,5000.0f,DEFAULT_FOV,0);

	mpGeometry->SetCameraAngleRad( &(mpCameraStackAngle[dwStackNum]),0 );
}

void MToolMouseStatus::SetRotationPivot(VECTOR3 *pPivot)
{
	mRotationPivot	=	*pPivot;
}


/// Ű����� ī�޶� �����̱� ���� �κ�
// ī�޶��� �������� �̵��Ѵ�
void MToolMouseStatus::MoveCamera_Forward( float p_Value )
{
	CAMERA_DESC Desc;
	mpGeometry->GetCameraDesc( &Desc, 0);

	VECTOR3	vAt = Desc.v3EyeDir;

	Normalize(&vAt, &vAt);

	vAt = vAt * p_Value * 5.0f;

	mpGeometry->MoveCamera( &vAt,0);
}

void MToolMouseStatus::MoveCamera_Backward( float p_Value )
{
	CAMERA_DESC Desc;
	mpGeometry->GetCameraDesc( &Desc, 0);

	VECTOR3	vAt = Desc.v3EyeDir;

	Normalize(&vAt, &vAt);

	vAt = vAt * -p_Value * 5.0f;

	mpGeometry->MoveCamera( &vAt,0);
}

void MToolMouseStatus::MoveCamera_LeftSideStep( float p_Value )
{
	// ȸ�������κ��� ���溤�͸� ��´�
	VECTOR3	Rot;
	mpGeometry->GetCameraAngleRad(&Rot,0);

	VECTOR3	t_Front;
	t_Front.x	=	p_Value * (float)sinf(Rot.y);
	t_Front.y	=	p_Value * (float)sinf(Rot.x);
	t_Front.z	=	p_Value * (float)cosf(Rot.y);

	// ���溤�Ϳ� (0,1,0) ���͸� ����, ���̵庤�͸� ��´�
	VECTOR3 t_UpVec = { 0.0f, 1.0f, 0.0f };
	VECTOR3 t_SideVec;
	CrossProduct( &t_SideVec, &t_Front, &t_UpVec );

	mpGeometry->MoveCamera( &t_SideVec,0);
}

void MToolMouseStatus::MoveCamera_RightSideStep( float p_Value )
{
	// ȸ�������κ��� ���溤�͸� ��´�
	VECTOR3	Rot;
	mpGeometry->GetCameraAngleRad(&Rot,0);

	VECTOR3	t_Front;
	t_Front.x	=	p_Value * (float)sinf(Rot.y);
	t_Front.y	=	p_Value * (float)sinf(Rot.x);
	t_Front.z	=	p_Value * (float)cosf(Rot.y);

	// ���溤�Ϳ� (0,1,0) ���͸� ����, ���̵庤�͸� ��´�
	VECTOR3 t_UpVec = { 0.0f, 1.0f, 0.0f };
	VECTOR3 t_SideVec;
	CrossProduct( &t_SideVec, &t_UpVec, &t_Front );	// �������̴ϱ� �ݴ��

	mpGeometry->MoveCamera( &t_SideVec,0);
}

void MToolMouseStatus::MoveCamera_Up( float p_Value )
{
	// �׳� ����
	VECTOR3 t_Vec = { 0.0f, p_Value, 0.0f };

	mpGeometry->MoveCamera( &t_Vec,0);
}

void MToolMouseStatus::MoveCamera_Down( float p_Value )
{
	// �׳� �Ʒ���
	VECTOR3 t_Vec = { 0.0f, -1 * p_Value, 0.0f };

	mpGeometry->MoveCamera( &t_Vec,0);
}