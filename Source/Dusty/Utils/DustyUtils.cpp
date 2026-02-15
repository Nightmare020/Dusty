// Fill out your copyright notice in the Description page of Project Settings.


#include "DustyUtils.h"
#include "../Character/DustyCharacter.h"
#include "NavigationSystem.h"
#include "Dusty/AI/Maze/MazeNPC.h"
#include "Components/CapsuleComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Dusty/InteractiveObject/Light/Magnetism/MagneticCube.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "../Character/CharacterStates/BaseState.h"
#include "../Character/CharacterStates/ActionsFilter.h"
#include "../InteractiveObject/Interactable/PoleElevator.h"
#include "DustyConst.h"
#include "../InteractiveObject/Interactable/Bench.h"
#include "../InteractiveObject/Key/KeyController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "CommonInputSubsystem.h"
#include "CommonUITypes.h"
#include "Dusty/UI/HUD/DustyHUD.h"

void DustyUtils::ExtendPole(UPoseableMeshComponent* _Mesh, FName _BoneName, float _Distance)
{
	FVector NewLocation = _Mesh->GetBoneLocationByName(_BoneName, EBoneSpaces::ComponentSpace);
	NewLocation.Z = _Distance;
	_Mesh->SetBoneLocationByName(_BoneName, NewLocation, EBoneSpaces::ComponentSpace);
}

TArray<FVector> DustyUtils::GetAllNormalizedDirections()
{
	TArray<FVector> directions;
	directions.Add(FVector(1, 0, 0));
	directions.Add(FVector(-1, 0, 0));
	directions.Add(FVector(0, 1, 0));
	directions.Add(FVector(0, -1, 0));
	directions.Add(FVector(0, 0, 1));
	directions.Add(FVector(0, 0, -1));
	return directions;	
}


#pragma region Prints
void DustyUtils::PrintMessage(FString Message, FColor Color, float Duration)
{
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message);
#endif
}

void DustyUtils::PrintMessage(FString Message, int value, FColor Color, float Duration)
{
	FString Values = FString::Printf(TEXT(": %i"), value);
	FString NewMessage = Message + Values;

	PrintMessage(NewMessage, Color, Duration);
}

void DustyUtils::PrintMessage(FString Message, float value, FColor Color, float Duration)
{
	FString Values = FString::Printf(TEXT(": %f"), value);
	FString NewMessage = Message + Values;

	PrintMessage(NewMessage, Color, Duration);
}
void DustyUtils::PrintMessage(FString Message, double value, FColor Color, float Duration)
{
	FString Values = FString::Printf(TEXT(": %f"), value);
	FString NewMessage = Message + Values;

	PrintMessage(NewMessage, Color, Duration);
}

void DustyUtils::PrintMessage(FString Message, bool value, FColor Color, float Duration)
{
	FString Values = FString::Printf(TEXT(": %i"), value);
	FString NewMessage = Message + Values;

	PrintMessage(NewMessage, Color, Duration);
}

void DustyUtils::PrintMessage(FString Message, FVector vector, FColor Color, float Duration)
{
	FString Values = FString::Printf(TEXT(": %f, %f, %f"), vector.X, vector.Y, vector.Z);
	FString NewMessage = Message + Values;

	PrintMessage(NewMessage, Color, Duration);
}

void DustyUtils::PrintMessage(FString Message, FRotator rotation, FColor Color, float Duration)
{
	FString Values = FString::Printf(TEXT(": %f, %f, %f"), rotation.Pitch, rotation.Yaw, rotation.Roll);
	FString NewMessage = Message + Values;

	PrintMessage(NewMessage, Color, Duration);
}

void DustyUtils::PrintMessage(FString Message, FVector2D vector, FColor Color, float Duration)
{
	FString Values = FString::Printf(TEXT(": %f, %f"), vector.X, vector.Y);
	FString NewMessage = Message + Values;

	PrintMessage(NewMessage, Color, Duration);
}

void DustyUtils::PrintMessage(FString Message, FString name, FColor Color, float Duration)
{
	FString Values = FString::Printf(TEXT(": %s"), *FString(name));
	FString NewMessage = Message + Values;

	PrintMessage(NewMessage, Color, Duration);
}
#pragma endregion

FVector DustyUtils::Projection(FVector _Vec1, FVector _Vec2)
{
	FVector U = (_Vec1 - _Vec2);

	double dot = FVector::DotProduct(_Vec1, _Vec2);
	dot = dot / (_Vec2.Size() * _Vec2.Size());
	float size = FVector::DotProduct(_Vec1, _Vec2);
	FVector proj  = dot * _Vec2;
	return proj;
}


float DustyUtils::GetAngleBetweenVectors(FVector _Vec1, FVector _Vec2)
{
	return (FMath::Acos(FVector::DotProduct(_Vec1.GetSafeNormal(), _Vec2.GetSafeNormal()))) * (180 / 3.1415926);
}

FVector DustyUtils::GetRandomMovement(FVector _InitialLocation, float _Time, float _Amplitude, float _Frequency, int _NoAxisMovement)
{
	FVector NewLocation = _InitialLocation;
	float sin = FMath::Sin(_Frequency * _Time);
	float cos = FMath::Cos(_Frequency * _Time);

	NewLocation.X += _Amplitude * sin * cos;
	NewLocation.Y += _Amplitude * cos;
	NewLocation.Z += _Amplitude * sin;

	
	switch (_NoAxisMovement)
	{
		case 1:
			NewLocation.X = _InitialLocation.X;
			break;
		case 2:
			NewLocation.Y = _InitialLocation.Y;
			break;
		case 3:
			NewLocation.Z = _InitialLocation.Z;
			break;
		case 4:
			NewLocation.X = _InitialLocation.X;
			NewLocation.Y = _InitialLocation.Y;
			break;

	}

	return NewLocation;
}

int DustyUtils::ModClamp(int _Value, int _Min, int _Max)
{
	if (_Value < _Min)
	{
		return _Max;
	}
	if (_Value > _Max)
	{
		return _Min;
	}
	return _Value;
}

float DustyUtils::ModClamp(float _Value, float _Min, float _Max)
{
	if (_Value < _Min)
	{
		return _Max;
	}
	if (_Value > _Max)
	{
		return _Min;
	}
	return _Value;
}

#pragma region DustyMoveTo

void DustyUtils::DustyMoveTo(ADustyCharacter* DustyCharacter, FVector _DesiredLocation, FRotator _DesiredRotation, void (ADustyCharacter::*InCallbackFunction)())
{
	UAutoMovementState* AutoMovement = Cast<UAutoMovementState>(*DustyCharacter->ActionsFilterComponent->m_statesInstancesMap.Find(UAutoMovementState::StaticClass()));
	AutoMovement->SetAutoMovementValues(_DesiredLocation, _DesiredRotation);
	AutoMovement->CallbackDelegate.BindUObject(DustyCharacter, InCallbackFunction);
	DustyCharacter->ActionsFilterComponent->SetCurrentState(UAutoMovementState::StaticClass());
}

void DustyUtils::DustyMoveTo(ADustyCharacter* DustyCharacter, FVector _DesiredLocation, FRotator _DesiredRotation, void (AInteractable::*InCallbackFunction)(), AInteractable* _InteractableActor)
{
	UAutoMovementState* AutoMovement = Cast<UAutoMovementState>(*DustyCharacter->ActionsFilterComponent->m_statesInstancesMap.Find(UAutoMovementState::StaticClass()));
	AutoMovement->SetAutoMovementValues(_DesiredLocation, _DesiredRotation);
	AutoMovement->CallbackDelegate.BindUObject(_InteractableActor, InCallbackFunction);
	DustyCharacter->ActionsFilterComponent->SetCurrentState(UAutoMovementState::StaticClass());
}

#pragma endregion


ADustyCharacter* DustyUtils::GetDustyCharacter() 
{
	return Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GEngine->GameViewport->GetWorld(), 0));
}

bool DustyUtils::DustyActuallyInsideTrigger(ADustyCharacter* DustyCharacter, AActor* TriggerActor, float VerticalOffset)
{
	if (DustyCharacter && TriggerActor)
	{
		FHitResult Hit;
		FVector Start = DustyCharacter->GetActorLocation() + FVector::UpVector * VerticalOffset;
		FVector End = Start + DustyCharacter->GetActorForwardVector() * 50;

		//DustyUtils::DustyDrawDebugLine(DustyCharacter->GetWorld(), Start, End, FColor::Red,true);
		if (DustyCharacter->GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel3, DustyCharacter->GetIgnoreCharacterCollisionParams()))
		{
			if (Hit.GetActor() == TriggerActor)
			{
				return true;
			}
		}
	}
	return false;
}



TArray<FHitResult> DustyUtils::DustyPoleRaycast(ADustyCharacter* DustyCharacter, float CapsuleHalfHeight, float CapsuleRadius)
{
	/*TArray is the collection that contains all the HitResults*/
	TArray<FHitResult> HitResults;

	float capsuleRadius = CapsuleRadius;
	float capsuleHalfHeight = CapsuleHalfHeight;
	/*The Start location of the sphere*/
	FVector StartLocation = DustyCharacter->PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace) - DustyCharacter->PoleSkeletal->GetUpVector() * capsuleHalfHeight;
	FVector EndLocation = StartLocation + 0.01f;

	ECollisionChannel ECC = ECollisionChannel::ECC_Visibility;

	FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(capsuleRadius, capsuleHalfHeight);

	bool bHitSomething = DustyCharacter->GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, DustyCharacter->PoleSkeletal->GetComponentRotation().Quaternion(), ECC, CollisionShape, DustyCharacter->GetIgnoreCharacterCollisionParams());

#if WITH_EDITOR
	//DrawDebugCapsule(DustyCharacter->GetWorld(), StartLocation, CollisionShape.GetCapsuleHalfHeight(), CollisionShape.GetCapsuleRadius(), DustyCharacter->PoleSkeletal->GetComponentRotation().Quaternion(), bHitSomething ? FColor::Green : FColor::Red, false, 5);
#endif
	return HitResults;
}


void DustyUtils::SetEmissiveMaterialValues(UStaticMeshComponent* Mesh, float RedValue, float GreenValue, float BlueValue)
{
	Mesh->SetCustomPrimitiveDataFloat(0, RedValue);
	Mesh->SetCustomPrimitiveDataFloat(1, GreenValue);
	Mesh->SetCustomPrimitiveDataFloat(2, BlueValue);

}
void DustyUtils::SetEmissiveMaterialValues(USkeletalMeshComponent* Mesh, float RedValue, float GreenValue, float BlueValue)
{
	Mesh->SetCustomPrimitiveDataFloat(0, RedValue);
	Mesh->SetCustomPrimitiveDataFloat(1, GreenValue);
	Mesh->SetCustomPrimitiveDataFloat(2, BlueValue);

}
void DustyUtils::SetEmissiveMaterialValues(UPoseableMeshComponent* Mesh, float RedValue, float GreenValue, float BlueValue)
{
	Mesh->SetCustomPrimitiveDataFloat(0, RedValue);
	Mesh->SetCustomPrimitiveDataFloat(1, GreenValue);
	Mesh->SetCustomPrimitiveDataFloat(2, BlueValue);

}

void DustyUtils::ShowHud(UWorld* World, EHudCanvas CanvasName, int id)
{

#if WITH_EDITOR
	if (ADustyCharacter* Character = DustyUtils::GetDustyCharacter())
	{
		if (Character->PlaceholderAlbertoDontShowHud)
		{
			return;
		}
	}
#endif

	Cast<ADustyHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD())->ShowHud(CanvasName, id);	

}

void DustyUtils::ShowDoublePromptHud(UWorld* World, int FirstId, int SecondId)
{
#if WITH_EDITOR
	if (ADustyCharacter* Character = DustyUtils::GetDustyCharacter())
	{
		if (Character->PlaceholderAlbertoDontShowHud)
		{
			return;
		}
	}
#endif
	Cast<ADustyHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD())->ShowDoublePromptHud(FirstId, SecondId);
}

void DustyUtils::ShowDoublePromptHud(UWorld* World)
{
	Cast<ADustyHUD>(UGameplayStatics::GetPlayerController(World, 0)->GetHUD())->ShowDoublePromptHud();
}

void DustyUtils::ShowReadMode(UWorld* World, FText Title, FText Content)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0))
	{
		if (ADustyHUD* DustyHud = Cast<ADustyHUD>(PlayerController->GetHUD()))
		{
			DustyHud->ShowReadMode(Title, Content);
		}
	}
}

void DustyUtils::SwapReadModeTexts(UWorld* World, FText Title, FText Content)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0))
	{
		if (ADustyHUD* DustyHud = Cast<ADustyHUD>(PlayerController->GetHUD()))
		{
			DustyHud->SwapReadModeTexts(Title, Content);
		}
	}
}

void DustyUtils::HideReadMode(UWorld* World)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0))
	{
		if (ADustyHUD* DustyHud = Cast<ADustyHUD>(PlayerController->GetHUD()))
		{
			DustyHud->HideHud(EHudCanvas::ReadMode);
		}
	}
}


void DustyUtils::HideHud(UWorld* World, EHudCanvas CanvasName)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0))
	{
		if (ADustyHUD* DustyHud = Cast<ADustyHUD>(PlayerController->GetHUD()))
		{
			DustyHud->HideHud(CanvasName);
		}
	}
}

void DustyUtils::HideDoublePromptHud(UWorld* World)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0))
	{
		if (ADustyHUD* DustyHud = Cast<ADustyHUD>(PlayerController->GetHUD()))
		{
			DustyHud->HideDoublePromptHud();
		}
	}
}

bool DustyUtils::IsHudVisible(UWorld* World, EHudCanvas CanvasName)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0))
	{
		if (ADustyHUD* DustyHud = Cast<ADustyHUD>(PlayerController->GetHUD()))
		{
			return DustyHud->IsHudVisible(CanvasName);
		}
	}
	return false;
}

void DustyUtils::HideHudWithNoFade(UWorld* World, EHudCanvas CanvasName)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0))
	{
		if (ADustyHUD* DustyHud = Cast<ADustyHUD>(PlayerController->GetHUD()))
		{
			DustyHud->HideHudWithNoFade(CanvasName);
		}
	}
}

FLinearColor DustyUtils::HSVtoRGB(float H, float S, float V)
{
	float R = 0.f, G = 0.f, B = 0.f;

	int i = FMath::FloorToInt(H * 6);
	float f = H * 6 - i;
	float p = V * (1 - S);
	float q = V * (1 - f * S);
	float t = V * (1 - (1 - f) * S);

	switch (i % 6)
	{
	case 0: R = V, G = t, B = p; break;
	case 1: R = q, G = V, B = p; break;
	case 2: R = p, G = V, B = t; break;
	case 3: R = p, G = q, B = V; break;
	case 4: R = t, G = p, B = V; break;
	case 5: R = V, G = p, B = q; break;
	}

	return FLinearColor(R, G, B, 1.0f);
}

#pragma region DrawDebug

void DustyUtils::DustyDrawDebugString(const UWorld* InWorld, FVector const& TextLocation, const FString& Text, AActor* TestBaseActor, FColor const& TextColor, float Duration, bool bDrawShadow, float FontScale)
{
#if WITH_EDITOR
	DrawDebugString(InWorld, TextLocation, Text, TestBaseActor, TextColor, Duration, bDrawShadow, FontScale);
#endif
}

void DustyUtils::DustyDrawDebugLine(const UWorld* InWorld, FVector const& LineStart, FVector const& LineEnd, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
#if WITH_EDITOR
	DrawDebugLine(InWorld, LineStart, LineEnd, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
#endif
}

void DustyUtils::DustyDrawDebugCapsule(const UWorld* InWorld, FVector const& Center, float HalfHeight, float Radius, const FQuat& Rotation, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness)
{
	DrawDebugCapsule(InWorld, Center, HalfHeight, Radius, Rotation, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
}

#pragma endregion

#pragma region AI_Utils

bool DustyUtils::CanSeePlayer(const ACharacter* NPC, const ACharacter* Player, float TraceDistance, float DebugTime)
{
	if (FVector::Dist(NPC->GetActorLocation(), Player->GetActorLocation()) > TraceDistance)
	{
		return false;
	}
	
	// this method launches two simultaneous line traces
	FHitResult LeftOutHit;
	FHitResult RightOutHit;
	if (NPC && Player)
	{
		// get players visibility
		FVector TargetLocation = Player->GetActorLocation();
		
		FVector TraceBaseStart = TargetLocation;
		TraceBaseStart.Z += NPC->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2;
		
		FVector TraceDirection = TargetLocation - NPC->GetActorLocation();
		TraceDirection.Normalize();

		// use perpendicular vector to get both trace starts
		FVector PerpendicularVector = FVector::CrossProduct(TraceDirection, FVector::ZAxisVector);
		PerpendicularVector.Normalize();

		FVector const TraceLeftStart = (PerpendicularVector * NPC->GetCapsuleComponent()->GetScaledCapsuleRadius()) + NPC->GetActorLocation();
		FVector const TraceRightStart = (-1 * PerpendicularVector * NPC->GetCapsuleComponent()->GetScaledCapsuleRadius()) + NPC->GetActorLocation();

		// ignore self
		FCollisionQueryParams TraceParams(FName(TEXT("Visibility")), true, NPC);
		Player->GetWorld()->LineTraceSingleByChannel(LeftOutHit, TraceLeftStart, TargetLocation, ECollisionChannel::ECC_Camera, TraceParams);
		Player->GetWorld()->LineTraceSingleByChannel(RightOutHit, TraceRightStart, TargetLocation, ECollisionChannel::ECC_Camera, TraceParams);

#if WITH_EDITOR
		// DrawDebugLine(Player->GetWorld(), TraceLeftStart, TargetLocation, FColor::Red, false, DebugTime, 0, 1);
		// DrawDebugLine(Player->GetWorld(), TraceRightStart, TargetLocation, FColor::Red, false, DebugTime, 0, 1);

#endif
	}
	bool const CanSeeLeftPlayer = LeftOutHit.bBlockingHit && Cast<ADustyCharacter>(LeftOutHit.GetActor());
	bool const CanSeeRightPlayer = RightOutHit.bBlockingHit && Cast<ADustyCharacter>(RightOutHit.GetActor());
	
	return CanSeeLeftPlayer && CanSeeRightPlayer;
}

float DustyUtils::CalculatePathfindingCost(const FVector& CurrentLocation, const FVector& Destination, UWorld* World)
{
	// Use UE5's Navigation System to calculate path
	if (const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(World))
	{
		// Get the navigation path cost
		FVector::FReal PathCost;
		if (NavSys->GetPathCost(CurrentLocation, Destination, PathCost) == ENavigationQueryResult::Success)
		{
			return PathCost;
		}
	}
	// Return a large value if path calculation fails
	return MAX_FLT;
}

void DustyUtils::SetAIMeshCOllisions(UMeshComponent* Mesh)
{
	// base collisions
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);

	// trace channels
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // IgnoreTriggerOverlap
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Block); // BlockTrigger

	// Object responses
	Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECC_Destructible, ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap); //Enemy NPC
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Overlap); //Auxiliary PoleJump
}

#pragma endregion


#pragma region CameraUtils

float DustyUtils::GetAngularDistance(const FRotator& Rotator1, const FRotator& Rotator2)
{
	FRotator DeltaRotator = Rotator1 - Rotator2;
	DeltaRotator.Normalize();

	float AngularDistance = FMath::Sqrt(
		FMath::Square(DeltaRotator.Pitch) +
		FMath::Square(DeltaRotator.Yaw) +
		FMath::Square(DeltaRotator.Roll)
	);
	return AngularDistance;
}

bool  DustyUtils::IsCloseEnough(FVector const& A, FVector const& B, float Tolerance)
{
	return FVector::DistSquared(A, B) < FMath::Square(Tolerance);
}

bool  DustyUtils::IsCloseEnough(FRotator const& A, FRotator const& B, float Tolerance)
{
	return FMath::Abs(A.Pitch - B.Pitch) < Tolerance && FMath::Abs(A.Yaw - B.Yaw) < Tolerance && FMath::Abs(A.Roll - B.Roll) < Tolerance;
}

#pragma endregion

ECommonInputType DustyUtils::GetCurrentInputType()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GEngine->GameViewport->GetWorld(), 0);

	if (PlayerController)
	{
		const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

		if (LocalPlayer)
		{
			UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(LocalPlayer);

			if (InputSubsystem)
			{
				return InputSubsystem->GetCurrentInputType();
			}
		}
	}

	// Default to Keyboard if no input is detected
	return ECommonInputType::MouseAndKeyboard;
}