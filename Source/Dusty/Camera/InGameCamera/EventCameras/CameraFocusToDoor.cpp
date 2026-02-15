#include "CameraFocusToDoor.h"

#include "Dusty/Utils/DustyUtils.h"

void ACameraFocusToDoor::Exited()
{
	Super::Exited();
	DustyUtils::ShowHud(GetWorld(), EHudCanvas::DiscButtons);
}
