#include "EventCamera.h"

void AEventCamera::Exit()
{
	if (m_OneUseOnly)
	{
		IsActive = false;
	}
}