#include "DiscKeyController.h"
#include "Disc.h"
#include "../../Character/CharacterStates/ActionsFilter.h"
#include "../Hieroglyphics/GlyphController.h"
#include "../../Utils/DustyConst.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Components/AudioComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Dusty/SaveGame/DustySaveGame.h"
#include "Dusty/DustyGameInstance.h"
#include "Dusty/Audio/AudioActor.h"
#include "Dusty/Camera/InGameCamera/EventCameras/CameraFocusToDoor.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void ADiscKeyController::LoadActor()
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
	Ar << m_Solved;
	Ar << Unlocked;

	if (m_Solved)
	{
		SetActorTickEnabled(false);
		DecalActor->SetRelativeScale3D(FVector(MaxDecalSize, DecalActor->GetRelativeScale3D().Y, DecalActor->GetRelativeScale3D().Z));
	}

	UpdateMaterials();
}

void ADiscKeyController::OnActorSaved_Implementation()
{
	FActorSaveData ActorData;

	ActorData.ActorName = GetFName();
	ActorData.LevelName = GetLevel()->GetOuter()->GetFName();
	ActorData.Transform = GetActorTransform();
	FMemoryWriter MemWriter(ActorData.ByteData);

	FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Ar << m_Solved;
	Ar << Unlocked;

	GetGameInstance<UDustyGameInstance>()->AddActorSaveData(ActorData);
}

bool ADiscKeyController::CheckSolved()
{
	for (TTuple<EGlyphs, bool> discSolution : m_Solution)
	{
		m_Solution[discSolution.Key] = false;
	}

	for (int i = 0; i < m_Discs.Num(); ++i)
	{
		if (!m_Discs[i]->Rotating && m_Solution.Contains(m_Discs[i]->GetCurrentGlyph()))
		{
			m_Solution[m_Discs[i]->GetCurrentGlyph()] = true;
		}
	}

	bool CurrentlySolved = true;
	for (TTuple<EGlyphs, bool> discSolution : m_Solution)
	{
		if (!discSolution.Value)
		{
			CurrentlySolved = false;
			break;
		}
	}
	return CurrentlySolved;
}

void ADiscKeyController::SolvedAniamtion(float DeltaTime)
{
	
	if (m_currentTimeDecalAnimation <= TimeDecalAnimation)
	{
		if(!solvedSoundPlayed && m_track)
		{
			m_track->PlayTrigger("IntroCiv");
			// PlayMetasound("");
			// solvedSoundPlayed = true;
		}
		float sizeX = FMath::Lerp(1, MaxDecalSize, m_currentTimeDecalAnimation / TimeDecalAnimation);
		DecalActor->SetRelativeScale3D(FVector(sizeX, DecalActor->GetRelativeScale3D().Y, DecalActor->GetRelativeScale3D().Z));
		m_currentTimeDecalAnimation += DeltaTime;
	}
	else
	{
		if (!m_EndAnimation)
		{
			// UAudioSubsytem* audioSubsystem = GetGameInstance()->GetSubsystem<UAudioSubsytem>();
			// audioSubsystem->PlayMetasound(SoundSolved, GetActorLocation());
			
			m_GlyphController->SetKeyControllerState(this, m_Solved);
			DustyCharacter->ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
			UpdateMaterials();
			m_EndAnimation = true;
		}
	}
}

void ADiscKeyController::UpdateMaterials()
{
	Super::UpdateMaterials();

	if (m_baseDynamicMaterial == nullptr)
	{
		UMaterialInterface* baseMat = m_base->GetStaticMeshComponent()->GetMaterial(0);
		m_baseDynamicMaterial = UMaterialInstanceDynamic::Create(baseMat, this);
		m_base->GetStaticMeshComponent()->SetMaterial(0, m_baseDynamicMaterial);
	}

	if (Unlocked)
	{
		m_baseDynamicMaterial->SetScalarParameterValue(KeyBaseParam, 0.f); //2 is the value for being turned ON
	}
	else
	{
		m_baseDynamicMaterial->SetScalarParameterValue(KeyBaseParam, 2.f); //2 is the value for being turned OFF
	}
}

ADiscKeyController::ADiscKeyController()
{
	//SoundSolved = CreateDefaultSubobject<UAudioComponent>("Sound solved");
}

void ADiscKeyController::BeginPlay()
{
	if (m_base)
	{
		UMaterialInterface* baseMat = m_base->GetStaticMeshComponent()->GetMaterial(0);
		m_baseDynamicMaterial = UMaterialInstanceDynamic::Create(baseMat, this);
		m_base->GetStaticMeshComponent()->SetMaterial(0, m_baseDynamicMaterial);
		Super::BeginPlay();
		DecalActor = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("DecalScale")));
		LoadActor();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Missing base reference in DiscKeyController Class, please attatch a base!"));
	}


	if (m_Discs.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Disc references in KeyController Class, please attatch a Disc Actor!"));
	}

	if (m_Solution.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Solution in KeyController Class, please attatch a Solution!"));
	}
}


void ADiscKeyController::OnPoleInserted()
{
	Super::OnPoleInserted();

	if (m_FocusCamera && m_FocusCamera->IsActive)
	{
		m_FocusCamera->Init();
	}
	else
	{
		DustyUtils::ShowHud(GetWorld(), EHudCanvas::DiscButtons);
	}
}

void ADiscKeyController::OnPoleRemoved()
{
	Super::OnPoleRemoved();
	DustyUtils::HideHud(GetWorld(), EHudCanvas::DiscButtons);
}

void ADiscKeyController::Update(float _DeltaTime, FVector2D _Input)
{
	if (m_Solved || !DustyCharacter->CanUnnailPole)
	{
		return;
	}
	Super::Update(_DeltaTime, _Input);

	DustyUtils::SetEmissiveMaterialValues(m_Discs[m_CurrentDiscIdx]->DiscMesh, 0.5, 0.5, 0);


	// Rotate discs
	if (_Input.X > m_HorizontalInputThreshold) // Rotating right
	{
		m_Discs[m_CurrentDiscIdx]->RotateDisc(true);
	}
	else if (_Input.X < -m_HorizontalInputThreshold) // Rotating left
	{
		m_Discs[m_CurrentDiscIdx]->RotateDisc(false);
	}	

	DustyUtils::SetEmissiveMaterialValues(m_Discs[m_CurrentDiscIdx]->DiscMesh, 1, 1, 1);
}

void ADiscKeyController::PlayMetasound_Implementation(const FName& _text)
{
}

void ADiscKeyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Unlocked)
	{
		FVector NewDecalPos = FMath::VInterpTo(DecalActor->GetComponentLocation(), GetActorLocation() + m_DecalPositions[m_CurrentDiscIdx + 1], DeltaTime, m_DecalSpeed);
		DecalActor->SetWorldLocation(NewDecalPos);
	}
	else
	{
		DecalActor->SetWorldLocation(GetActorLocation() + m_DecalPositions[0]);
	}
	if (!m_Solved)
	{
		m_Solved = CheckSolved();
	}
	else
	{
		DecalActor->SetWorldLocation(GetActorLocation() + m_DecalPositions[0]);
		Unlocked = false;
		SolvedAniamtion(DeltaTime);
	}
}
