#include "DustyCameraActor.h"

#include "Camera/CameraComponent.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Kismet/GameplayStatics.h"

void ADustyCameraActor::BeginPlay()
{
	Super::BeginPlay();
	GetCameraComponent()->SetConstraintAspectRatio(false);
	m_Player = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (m_Player)
	{
		GetCameraComponent()->FieldOfView = m_Player->FollowCamera->FieldOfView;
		GetCameraComponent()->AspectRatio = m_Player->FollowCamera->AspectRatio;
	}
}

void ADustyCameraActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ADustyCameraActor::SetFov(float newFov)
{
	if (!std::isnan(newFov))
	{
		GetCameraComponent()->SetFieldOfView(newFov);
	}
}
