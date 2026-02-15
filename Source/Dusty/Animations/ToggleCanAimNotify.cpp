// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleCanAimNotify.h"
#include "Dusty/Character/DustyCharacter.h"
#include "Components/BillboardComponent.h"

void UToggleCanAimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);


	if (ADustyCharacter* Player = Cast<ADustyCharacter>(MeshComp->GetOwner()))
	{
		Player->ToggleCanAim(ToggleCanAim);
		if (ToggleCanAim)
		{
			Player->DustyActuallyAiming = true;
			Player->CrosshairHUD->SetHiddenInGame(false);
			Player->CrosshairHUD->SetSprite(Player->CrosshairImages[0]);
		}
	}
}