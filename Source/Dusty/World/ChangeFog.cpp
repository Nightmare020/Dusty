// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeFog.h"	
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/BoxComponent.h"
#include "Dusty/Character/DustyCharacter.h"

template <typename T>
T InterpolateTo(T Current, T Target, float DeltaTime, float InterpSpeed)
{
	return FMath::FInterpTo(Current, Target, DeltaTime, InterpSpeed);
}

// Specialization for FLinearColor
template <>
FLinearColor InterpolateTo<FLinearColor>(FLinearColor Current, FLinearColor Target, float DeltaTime, float InterpSpeed)
{
	return FMath::CInterpTo(Current, Target, DeltaTime, InterpSpeed);
}

// Specialization for FColor
template <>
FColor InterpolateTo<FColor>(FColor Current, FColor Target, float DeltaTime, float InterpSpeed)
{
	FLinearColor CurrentLinear = Current.ReinterpretAsLinear();
	FLinearColor TargetLinear = Target.ReinterpretAsLinear();
	FLinearColor InterpolatedLinear = FMath::CInterpTo(CurrentLinear, TargetLinear, DeltaTime, InterpSpeed);
	return InterpolatedLinear.ToFColor(true);
}

void AChangeFog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!MapExponentialHeightFog)
	{
		//DustyUtils::PrintMessage("MISSING MAP EXPONENTIAL HEIGHT FOG", FColor::Red, DeltaTime);
		return;
	}
	if (ChangingFog)
	{
		Timer += DeltaTime;

		if (UExponentialHeightFogComponent* Fog = MapExponentialHeightFog->GetComponent())
		{
			UpdateFogComponent(Fog, DeltaTime, ChangeVelocity);
		}
	}
}

void AChangeFog::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (ADustyCharacter* Character = Cast<ADustyCharacter>(OtherActor))
	{
		ChangingFog = true;
	}
}

// Sets default values
AChangeFog::AChangeFog()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));

	Trigger = CreateDefaultSubobject<UBoxComponent>("Trigger");
	Trigger->SetupAttachment(RootComponent);

}

void AChangeFog::UpdateFogComponent(UExponentialHeightFogComponent* Fog, float DeltaTime, float InterpSpeed)
{
	if (!Fog) return;

	// Interpolate and set each property
	Fog->FogDensity = InterpolateTo(Fog->FogDensity, FogDensity, DeltaTime, InterpSpeed);
	Fog->FogHeightFalloff = InterpolateTo(Fog->FogHeightFalloff, FogHeightFalloff, DeltaTime, InterpSpeed);
	Fog->SecondFogData.FogDensity = InterpolateTo(Fog->SecondFogData.FogDensity, SecondFogDensity, DeltaTime, InterpSpeed);
	Fog->SecondFogData.FogHeightFalloff = InterpolateTo(Fog->SecondFogData.FogHeightFalloff, SecondFogHeightFalloff, DeltaTime, InterpSpeed);
	Fog->SecondFogData.FogHeightOffset = InterpolateTo(Fog->SecondFogData.FogHeightOffset, SecondFogHeightOffset, DeltaTime, InterpSpeed);
	Fog->FogInscatteringLuminance = InterpolateTo(Fog->FogInscatteringLuminance, FogInscatteringColor, DeltaTime, InterpSpeed);
	Fog->FogMaxOpacity = InterpolateTo(Fog->FogMaxOpacity, FogMaxOpacity, DeltaTime, InterpSpeed);
	Fog->StartDistance = InterpolateTo(Fog->StartDistance, StartDistance, DeltaTime, InterpSpeed);
	Fog->FogCutoffDistance = InterpolateTo(Fog->FogCutoffDistance, FogCutoffDistance, DeltaTime, InterpSpeed);

	Fog->bEnableVolumetricFog = VolumetricFog;
	Fog->VolumetricFogScatteringDistribution = InterpolateTo(Fog->VolumetricFogScatteringDistribution, ScatteringDistribution, DeltaTime, InterpSpeed);
	Fog->VolumetricFogAlbedo = InterpolateTo(Fog->VolumetricFogAlbedo, Albedo, DeltaTime, InterpSpeed);
	Fog->VolumetricFogEmissive = InterpolateTo(Fog->VolumetricFogEmissive, Emissive, DeltaTime, InterpSpeed);
	Fog->VolumetricFogExtinctionScale = InterpolateTo(Fog->VolumetricFogExtinctionScale, ExtinctionScale, DeltaTime, InterpSpeed);
	Fog->VolumetricFogDistance = InterpolateTo(Fog->VolumetricFogDistance, ViewDistance, DeltaTime, InterpSpeed);


	Fog->MarkRenderStateDirty();
	
}

