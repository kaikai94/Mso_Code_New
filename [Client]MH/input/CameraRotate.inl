

void CameraRotate(CKeyboard* Keyboard,CMouse* Mouse)
{
	static BOOL bLastRotating = FALSE;

	if( WINDOWMGR->IsDragWindow() )
		return;

	BOOL bCurRotating = Mouse->RButtonPressed();
	if(bCurRotating)
	{
		short x = Mouse->GetMouseAxisX();
		short y = Mouse->GetMouseAxisY();
		if(x || y)
		{
			CAMERA->MouseRotate(x,y);
		}
	}

	if(bCurRotating != bLastRotating)
	{
		bLastRotating = bCurRotating;
	}
}