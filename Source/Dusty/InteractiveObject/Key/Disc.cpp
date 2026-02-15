// Fill out your copyright notice in the Description page of Project Settings.


#include "Disc.h"
#include "Dusty/Utils/DustyUtils.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

// Sets default values
ADisc::ADisc()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	DiscMesh = CreateDefaultSubobject<UStaticMeshComponent>("Disc");
	DiscMesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void ADisc::BeginPlay()
{
	Super::BeginPlay();

	int InitialGlyphDiference = (int)m_InitialGlyph;

	m_GlyphsPosition.Add((0 - InitialGlyphDiference + 8) % 8, EGlyphs::Time);
	m_GlyphsPosition.Add((1 - InitialGlyphDiference + 8) % 8, EGlyphs::Animal);
	m_GlyphsPosition.Add((2 - InitialGlyphDiference + 8) % 8, EGlyphs::Light);
	m_GlyphsPosition.Add((3 - InitialGlyphDiference + 8) % 8, EGlyphs::Road);
	m_GlyphsPosition.Add((4 - InitialGlyphDiference + 8) % 8, EGlyphs::House);
	m_GlyphsPosition.Add((5 - InitialGlyphDiference + 8) % 8, EGlyphs::Person);
	m_GlyphsPosition.Add((6 - InitialGlyphDiference + 8) % 8, EGlyphs::Darkness);
	m_GlyphsPosition.Add((7 - InitialGlyphDiference + 8) % 8, EGlyphs::Stars);
	

	SetActorTickEnabled(true);
	LoadActor();
}

// Called every frame
void ADisc::Tick(float DeltaTime)
{
	if(haveTick)
	{
		Super::Tick(DeltaTime);

		//DustyUtils::PrintMessage("Rotating Disc", GetActorRotation(), FColor::Yellow, DeltaTime);

		if (m_Timer < m_TimeToRotate)
		{
			
			FRotator NewRotation = GetActorRotation();
			m_Timer += DeltaTime;

			NewRotation.Yaw = FMath::Lerp(m_InitialRotation, m_DesiredRotation, m_Timer/m_TimeToRotate);

			SetActorRotation(NewRotation);
		}
		else
		{
			m_Timer = 0;
			FRotator NewRotation = GetActorRotation();
			NewRotation.Yaw = m_DesiredRotation;
			SetActorRotation(NewRotation);

			Rotating = false;
			haveTick = false;
			m_InitialRotation = m_DesiredRotation;
			//SetActorTickEnabled(false);
		}
	}


}

void ADisc::RotateDisc(bool Direction)
{
	if (Rotating)
	{
		return;
	}
	Rotating = true;
	haveTick = true;
	//SetActorTickEnabled(true);
	if (Direction)
	{
		CurrentGlyph = DustyUtils::ModClamp(CurrentGlyph - 1, 0, 8 - 1);
		m_InitialRotation = GetActorRotation().Yaw;
		m_DesiredRotation = m_InitialRotation +	m_rotationAmount;
	}
	else
	{
		CurrentGlyph = DustyUtils::ModClamp(CurrentGlyph + 1, 0, 8 - 1);
		m_InitialRotation = GetActorRotation().Yaw;
		m_DesiredRotation = m_InitialRotation - m_rotationAmount;
	}

	UAudioSubsytem* audioSubsytem = GetGameInstance()->GetSubsystem<UAudioSubsytem>();
	if(audioSubsytem && m_sound)
	{
		audioSubsytem->PlaySound3D(m_sound, GetActorLocation());
	}

}

void ADisc::LoadActor()
{
	FActorSaveData ActorSaveData = GetGameInstance<UDustyGameInstance>()->GetActorSaveData(GetFName(), GetLevel()->GetOuter()->GetFName());

	if (ActorSaveData.Transform.GetLocation() == FVector::ZeroVector)
	{
		return;
	}
	SetActorTransform(ActorSaveData.Transform);

	FMemoryReader MemReader(ActorSaveData.ByteData);
	FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << CurrentGlyph;

}

void ADisc::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << CurrentGlyph;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}

EGlyphs ADisc::GetCurrentGlyph()
{
	return m_GlyphsPosition[CurrentGlyph];
}

