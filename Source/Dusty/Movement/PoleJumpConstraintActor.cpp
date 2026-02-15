// Fill out your copyright notice in the Description page of Project Settings.


#include "PoleJumpConstraintActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "../Utils/DustyUtils.h"
#include "../Character/DustyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../Character/CharacterStates/ActionsFilter.h"
#include "../DustyGameInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "../Character/DustyMovementComponent.h"
#include "../Utils/DustyConst.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Animations/DustyAnimInstance.h"
#include "Dusty/Audio/AudioSubsytem.h"

APoleJumpConstraintActor::APoleJumpConstraintActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));

	m_Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
	m_Constraint->SetupAttachment(RootComponent);
	m_Constraint->SetDisableCollision(true);

	Pivot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pivot"));
	Pivot->SetupAttachment(RootComponent);
	Pivot->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	Pole = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pole"));
	Pole->SetupAttachment(Pivot);
	Pole->SetRelativeScale3D(FVector(1, 1, 50));
	Pole->SetRelativeLocation(FVector(0, 0, 2600));
	Pole->SetSimulatePhysics(true);

	m_PoleSK = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoleSK"));
	m_PoleSK->SetupAttachment(RootComponent);

	m_TopPolePosition = CreateDefaultSubobject<USceneComponent>(TEXT("TopPolePosition"));
	m_TopPolePosition->SetupAttachment(Pole);

	m_DustyPosition = CreateDefaultSubobject<USceneComponent>(TEXT("DustyPosition"));
	m_DustyPosition->SetupAttachment(m_TopPolePosition);

}

// Called when the game starts or when spawned
void APoleJumpConstraintActor::BeginPlay()
{
	Super::BeginPlay();

	m_DustyCharacter = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Pole->OnComponentHit.AddDynamic(this, &APoleJumpConstraintActor::OnPoleHit);


	m_PoleSK->SetBoneLocationByName(TopPoleBoneName, FVector::UpVector * m_DustyCharacter->InitialTopPolePosition, EBoneSpaces::ComponentSpace);
	m_PoleSK->SetBoneLocationByName(BottomPoleBoneName, FVector::UpVector * m_DustyCharacter->InitialBottomPolePosition, EBoneSpaces::ComponentSpace);


}


void APoleJumpConstraintActor::OnPoleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherComp != nullptr)
	{		
		EndJump();
	}
}
void APoleJumpConstraintActor::EndJump()
{
	if(m_poleSound)
	{
		GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_poleSound);
	}
	m_DustyCharacter->DustyMovementComponent->Activate();
	m_DustyCharacter->DustyMovementComponent->SetVelocity(m_FinalDirection.GetSafeNormal() * m_EndImpulse);

	if (m_DustyCharacter->MovementInput != FVector2D::ZeroVector && m_DustyCharacter->m_DustyPlayerAnimInstance)
	{
		m_DustyCharacter->m_DustyPlayerAnimInstance->SetInputAfterPoleJump(true);
	}

	m_DustyCharacter->ActionsFilterComponent->SetCurrentState(UFallingState::StaticClass());
	m_DustyCharacter->PoleJumpConstraintActor = nullptr;

	FixDustyPosition();	

	m_DustyCharacter->PoleSkeletal->SetHiddenInGame(false);
	m_DustyCharacter->StartRetractingBottomPole();

	
	delete(m_PoleJumpValues); 
	Destroy();
}


void APoleJumpConstraintActor::FixDustyPosition()
{
	/*TArray is the collection that contains all the HitResults*/
	TArray<FHitResult> HitResults;

	/*The Start location of the capsule */
	FVector StartLocation = m_DustyCharacter->GetActorLocation();
	//Bug fix with +0.001
	FVector EndLocation = StartLocation + 0.001f;

	//Capsule values
	float capsuleRadius = m_DustyCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	float capsuleHeight = m_DustyCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	ECollisionChannel ECC = ECollisionChannel::ECC_Visibility;
	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(capsuleRadius, capsuleHeight);
	m_DustyCharacter->GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat::Identity, ECC, CollisionShape, m_DustyCharacter->GetIgnoreCharacterCollisionParams());

	FVector VerticalOffset = FVector::ZeroVector;
	FVector HorizontalOffset = FVector::ZeroVector;

	//If Any object is inside of Dusty Capsule after Jump, due to set actor locations, just set him out
	for (const FHitResult& Hit : HitResults)
	{
		if (FVector::Parallel(Hit.ImpactNormal, FVector::UpVector))
		{
			VerticalOffset = m_DustyCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * Hit.ImpactNormal;
		}
		else
		{
			HorizontalOffset = m_DustyCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius() * Hit.ImpactNormal;
		}
	}

	FVector NewPosition = VerticalOffset + HorizontalOffset;

	m_DustyCharacter->SetActorLocation(m_DustyCharacter->GetActorLocation() + NewPosition);
}

void APoleJumpConstraintActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if(FMath::Abs(m_DustyCharacter->PoleSkeletal->GetComponentRotation().Roll - m_PoleSK->GetComponentRotation().Roll) < m_RotationThreshold)
	{
		m_CanReceiveInput = true;
		m_PoleSK->SetHiddenInGame(false);
		m_DustyCharacter->PoleSkeletal->SetHiddenInGame(true);
	}

	m_FinalDirection = Pole->GetComponentVelocity().GetSafeNormal();

	FVector BottomExtension = m_PoleSK->GetBoneLocationByName(BottomPoleBoneName, EBoneSpaces::ComponentSpace);

	FRotator PoleRotation = Pole->GetComponentRotation();
	PoleRotation.Roll += 180;
	m_PoleSK->SetWorldRotation(PoleRotation);

	FVector PoleDirection = Pole->GetComponentLocation() - Pivot->GetComponentLocation();
	m_PoleSK->SetWorldLocation(Pivot->GetComponentLocation() + PoleDirection.GetSafeNormal() * (BottomExtension.Z - m_DustyCharacter->InitialBottomPolePosition));
		
	FRotator DustyRotation = (PoleDirection.RotateAngleAxis(90, m_DustyCharacter->GetActorRightVector())).Rotation();

	m_TimerToMaxRotation += DeltaTime;
	float RotationSpeed = FMath::Lerp(m_InitialRotationSpeed, m_MaxRotationSpeed, m_TimerToMaxRotation / m_TimeToMaxRotation);

	if (m_TimerToMaxRotation >= m_TimeToMaxRotation)
	{
		m_DustyCharacter->SetActorRotation(DustyRotation);
	}
	else
	{
		m_DustyCharacter->DustyMovementComponent->Deactivate();
		FRotator NewRotation = FMath::RInterpTo(m_DustyCharacter->GetActorRotation(), DustyRotation, DeltaTime, RotationSpeed);
		m_DustyCharacter->SetActorRotation(NewRotation);
	}


	FVector AuxDustyPosition = m_DustyPosition->GetComponentLocation();

	float AngleRotation = DustyUtils::GetAngleBetweenVectors(m_DustyCharacter->GetActorForwardVector(), -Pole->GetRightVector());


	FVector DistanceToPole = AuxDustyPosition - Pole->GetComponentLocation();

	if (Pole->GetComponentRotation().Pitch < -0.1)
	{
		AuxDustyPosition = DistanceToPole.RotateAngleAxis(AngleRotation, Pole->GetUpVector()) + Pole->GetComponentLocation();
	}
	else if (Pole->GetComponentRotation().Pitch > 0.1)
	{
		AuxDustyPosition = DistanceToPole.RotateAngleAxis(-AngleRotation, Pole->GetUpVector()) + Pole->GetComponentLocation();
	}
	
	if (m_PoleJumpValues->PoleMovementType == EPoleMovementType::Jump)
	{
		AuxDustyPosition = AuxDustyPosition - m_DustyCharacter->GetActorForwardVector() * m_DustyForwardJumpOffset + m_DustyCharacter->GetActorRightVector() * m_DustyRightJumpOffset;
	}
	else
	{
		AuxDustyPosition = AuxDustyPosition - m_DustyCharacter->GetActorForwardVector() * m_DustyForwardElevatorOffset + m_DustyCharacter->GetActorRightVector() * m_DustyRightElevatorOffset + m_DustyCharacter->GetActorUpVector() * m_DustyUpElevatorOffset;
	}
	
	m_DustyCharacter->SetActorLocation(AuxDustyPosition);
	

	if (DustyUtils::GetAngleBetweenVectors(FVector::UpVector, PoleDirection) > m_PoleJumpValues->AngleToRetract)
	{
		EndJump();
	}

	if (m_IsExtending)
	{
		m_PoleJumpingExtensionBlend.Update(DeltaTime);

		//Physics constraints doesn't like scaling so

		//We break the constraint
		m_Constraint->BreakConstraint();

		//We set the new scale to the pole
		float NewScale = FMath::Lerp(m_CurrentScale, m_DesiredScale, m_PoleJumpingExtensionBlend.GetBlendedValue());
		Pole->SetRelativeScale3D(FVector(0.1f, 0.1f, NewScale));

		float NewExtension = 0;
		if (m_DesiredScale > m_CurrentScale)
		{
			NewExtension = FMath::Clamp(FMath::Lerp(m_InitialBottomExtension, m_DustyCharacter->BottomPoleExtension + m_DustyCharacter->InitialBottomPolePosition, m_PoleJumpingExtensionBlend.GetBlendedValue()), m_DustyCharacter->InitialBottomPolePosition, m_DustyCharacter->BottomPoleExtension + m_DustyCharacter->InitialBottomPolePosition);

		}
		else
		{
			 NewExtension = FMath::Clamp(FMath::Lerp(m_InitialBottomExtension, m_DesiredScale, m_PoleJumpingExtensionBlend.GetBlendedValue()), m_DustyCharacter->InitialBottomPolePosition, m_DustyCharacter->BottomPoleExtension + m_DustyCharacter->InitialBottomPolePosition);
		}
		
		FVector PreviousExtension = m_PoleSK->GetBoneLocationByName(BottomPoleBoneName, EBoneSpaces::ComponentSpace);
		PreviousExtension.Z = NewExtension;
		m_PoleSK->SetBoneLocationByName(BottomPoleBoneName, PreviousExtension, EBoneSpaces::ComponentSpace);

		//We now move the pole just a little bit so it doesn't enter ground
		FVector Direction = Pivot->GetComponentLocation() - Pole->GetComponentLocation();
		Pole->SetWorldLocation(Pivot->GetComponentLocation() - Direction.GetSafeNormal() * NewScale * 55);

		//We update DustyPosition
		m_DustyPosition->SetWorldLocation(m_TopPolePosition->GetComponentLocation() + Direction.GetSafeNormal() * m_DistanceToTop);

		//We constraint them again
		m_Constraint->SetConstrainedComponents(Pivot, NAME_None, Pole, NAME_None);		

		if (m_PoleJumpingExtensionBlend.IsComplete())
		{
			m_IsExtending = false;
			m_IsExtended = true;
			m_PoleJumpingExtensionBlend.Reset();
		}

	}

	//If Constraint fails and the pole gets stuck in place
	if (m_Impulse != FVector::ZeroVector && m_LastPolePosition == Pole->GetComponentLocation())
	{
		//We break the constraint
		m_Constraint->BreakConstraint();

		Pole->AddImpulse(m_Impulse);

		//We constraint them again
		m_Constraint->SetConstrainedComponents(Pivot, NAME_None, Pole, NAME_None);
	}

}

void APoleJumpConstraintActor::Jump()
{
	m_CanReceiveInput = false;
	FVector Direction = (m_TopPolePosition->GetRelativeLocation() - m_DustyPosition->GetRelativeLocation());

	

	m_DistanceToTop = Direction.Size();
	m_DustyPosition->SetWorldLocation(m_TopPolePosition->GetComponentLocation() + m_PoleJumpValues->PoleDirection.GetSafeNormal() * m_DistanceToTop);

	m_TimeToMaxRotation = m_BaseTimeToMaxRotation * (m_PoleJumpValues->PoleDirection.Size() / m_MaxJumpDistance);

	m_PoleJumpingExtensionBlend.SetBlendTime(m_PoleJumpValues->TimeToExtend / m_PoleJumpValues->ScaleFactor);
	m_PoleJumpingExtensionBlend.Reset();
	m_PoleJumpingExtensionBlend.SetBlendOption(EAlphaBlendOption::Sinusoidal);
	m_Constraint->SetConstrainedComponents(Pivot, NAME_None, Pole, NAME_None);
	Pole->AddImpulseAtLocation(m_PoleJumpValues->PoleImpulse, Pole->GetComponentLocation() + Pole->GetUpVector() * 50 * Pole->GetRelativeScale3D().Z);

	m_TimerToMaxRotation = 0;
}

void APoleJumpConstraintActor::Move(FVector _MovementVector)
{
	// If not (Needs to be extended but isn't)
	// Condition for Pole Elevator that needs to be fully extended
	if (!(m_PoleJumpValues->NeedsToBeExtendedForMovement && !m_IsExtended) && m_CanReceiveInput)
	{
		//If the player started moving towards a direction, prevent moving to opposite ones
		if (m_StartingMovementDirection == FVector::ZeroVector && _MovementVector != FVector::ZeroVector)
		{
			m_StartingMovementDirection = _MovementVector;	

			if (m_PoleJumpValues->PoleMovementType == EPoleMovementType::Elevator)
			{
				if (DustyUtils::GetAngleBetweenVectors(m_StartingMovementDirection, m_DustyCharacter->GetActorForwardVector()) < m_ForwardAngleThreshold)
				{
					m_DustyCharacter->m_DustyPlayerAnimInstance->SetForwardPoleElevator(true);
				}
				else
				{
					m_DustyCharacter->m_DustyPlayerAnimInstance->SetForwardPoleElevator(false);					
				}

				m_DustyCharacter->ActionsFilterComponent->SetCurrentState(UPoleJumpingState::StaticClass());
			}
		}
		else if(DustyUtils::GetAngleBetweenVectors(_MovementVector, m_StartingMovementDirection) > 90)
		{
			return;
		}


		//Project the movement to the Direction of the jump so it doesn't get more speed
		FVector InitialPoleJumpDirection = m_PoleJumpValues->PoleDirection;
		InitialPoleJumpDirection.Z = 0;

		_MovementVector = _MovementVector.VectorPlaneProject(_MovementVector, InitialPoleJumpDirection.GetSafeNormal());

		m_Impulse = _MovementVector * m_PoleJumpValues->MovementFactor;
		m_LastPolePosition = Pole->GetComponentLocation();
		Pole->AddImpulse(m_Impulse);
	}
}

void APoleJumpConstraintActor::Extend(bool _NewExtension)
{
	if (m_DustyCharacter)
	{
		m_DustyCharacter->ExtendBufferForDoubleJump = false;
	}

	if (!m_IsExtending)
	{

		m_CurrentScale = Pole->GetRelativeScale3D().Z;
		m_IsExtending = true;
		m_InitialBottomExtension = m_PoleSK->GetBoneLocationByName(BottomPoleBoneName, EBoneSpaces::ComponentSpace).Z;

		if (_NewExtension)
		{
			if (m_PoleJumpValues->ExtendScale < m_CurrentScale || m_IsExtended)
			{
				m_IsExtending = false;
			}

			m_CanReceiveInput = true;
			m_PoleSK->SetHiddenInGame(false);
			m_DustyCharacter->PoleSkeletal->SetHiddenInGame(true);
			m_DesiredScale = m_PoleJumpValues->ExtendScale;
			m_DesiredExtension = m_InitialBottomExtension + m_DustyCharacter->BottomPoleExtension;
		}
		else
		{
			m_DesiredExtension = m_DustyCharacter->InitialBottomPolePosition;
			m_DesiredScale = 2;
		}
	}
}

void APoleJumpConstraintActor::SetPoleJumpValues(PoleJumpValuesStruct* _PoleJumpValues)
{
	m_PoleJumpValues = _PoleJumpValues;
}

void APoleJumpConstraintActor::ExtendBottom(float _Percentage)
{
	FVector PreviousExtension = m_PoleSK->GetBoneLocationByName(BottomPoleBoneName, EBoneSpaces::ComponentSpace);
	PreviousExtension.Z = (m_DustyCharacter->BottomPoleExtension + m_DustyCharacter->InitialBottomPolePosition) * _Percentage - m_ExtendOffset * (1 - (m_PoleJumpValues->PoleDirection.Size() / m_MaxJumpDistance));
	m_PoleSK->SetBoneLocationByName(BottomPoleBoneName, PreviousExtension, EBoneSpaces::ComponentSpace);
}

float APoleJumpConstraintActor::GetExtensionTime()
{
	return m_PoleJumpingExtensionBlend.GetBlendTime();
}

void APoleJumpConstraintActor::ActivatePoleLight()
{
	UMaterialInstanceDynamic* PoleDynamicMaterial = UMaterialInstanceDynamic::Create(m_PoleSK->GetMaterial(0), this);
	PoleDynamicMaterial->SetScalarParameterValue(FName("position"), 1);
	m_PoleSK->SetMaterial(0, PoleDynamicMaterial);
	
	PoleDynamicMaterial->SetScalarParameterValue(FName("position"), 0);
	
}

FVector APoleJumpConstraintActor::GetOffsetFromDustyPosition()
{
	FVector Offset = (m_DustyPosition->GetRelativeLocation() + m_TopPolePosition->GetRelativeLocation()) / (Pole->GetRelativeScale3D().Z) - m_DustyCharacter->GetActorForwardVector() * m_DustyForwardJumpOffset + m_DustyCharacter->GetActorRightVector() * m_DustyRightJumpOffset;
	DustyUtils::PrintMessage("Offset", Offset, FColor::Yellow, 100);
	return Offset;

}


