// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "Dusty/Utils/DustyConst.h"

class ADustyCharacter;
class AMazeNPC;
class UPoseableMeshComponent;
class UBaseState;
class AInteractable;

class DUSTY_API DustyUtils
{
public:

	/** For throwing a capsule cast from the top of the pole to the desired lenght. Beware the capsule HALF height*/
	static TArray<FHitResult> DustyPoleRaycast(ADustyCharacter* DustyCharacter, float CapsuleHalfHeight, float CapsuleRadius);
	static void SetEmissiveMaterialValues(UStaticMeshComponent* Mesh, float RedValue = 0, float GreenValue = 0, float BlueValue = 0);
	static void SetEmissiveMaterialValues(USkeletalMeshComponent* Mesh, float RedValue = 0, float GreenValue = 0, float BlueValue = 0);
	static void SetEmissiveMaterialValues(UPoseableMeshComponent* Mesh, float RedValue = 0, float GreenValue = 0, float BlueValue = 0);
	
	static void ShowHud(UWorld* World, EHudCanvas CanvasName, int id = 0);
	static void ShowDoublePromptHud(UWorld* World, int FirstId, int SecondId);
	static void ShowDoublePromptHud(UWorld* World);
	static void ShowReadMode(UWorld* World, FText Title, FText Content);
	static void SwapReadModeTexts(UWorld* World, FText Title, FText Content);
	static void HideReadMode(UWorld* World);
	static void HideHud(UWorld* World, EHudCanvas CanvasName);
	static void HideHudWithNoFade(UWorld* World, EHudCanvas CanvasName);
	static void HideDoublePromptHud(UWorld* World);
	static bool IsHudVisible(UWorld* World, EHudCanvas CanvasName);

	/** To transform the Hue, Saturation, and Value from a material to RGB values */
	static FLinearColor HSVtoRGB(float H, float S, float V);

	static void ExtendPole(UPoseableMeshComponent* _Mesh, FName _BoneName, float _Distance);
	
	static TArray<FVector> GetAllNormalizedDirections();
	static FVector Projection(FVector _Vec1, FVector _Vec2);
	static float GetAngleBetweenVectors(FVector _Vec1, FVector _Vec2);
	
	/** All components in a range 0-1*/
	static FVector GetRandomMovement(FVector _InitialLocation, float _Time, float _Amplitude, float _Frequency, int _NoAxisMovement);

	static int ModClamp(int _Value, int _Min, int _Max);
	static float ModClamp(float _Value, float _Min, float _Max);

	static ADustyCharacter* GetDustyCharacter();

	static bool DustyActuallyInsideTrigger(ADustyCharacter* DustyCharacter, AActor* TriggerActor, float VerticalOffset = 0);

#pragma region DustyMoveTo
	static void DustyMoveTo(ADustyCharacter* DustyCharacter, FVector _DesiredLocation, FRotator _DesiredRotation, void (ADustyCharacter::*InCallbackFunction)());
	static void DustyMoveTo(ADustyCharacter* DustyCharacter, FVector _DesiredLocation, FRotator _DesiredRotation, void (AInteractable::*InCallbackFunction)(), AInteractable* _InteractableActor);
#pragma endregion 


#pragma region Prints
	static void PrintMessage(FString Message, FColor Color = FColor::Yellow, float Duration = 1.f);
	static void PrintMessage(FString Message, int value, FColor Color = FColor::Yellow, float Duration = 1.f);
	static void PrintMessage(FString Message, float value, FColor Color = FColor::Yellow, float Duration = 1.f);
	static void PrintMessage(FString Message, double value, FColor Color = FColor::Yellow, float Duration = 1.f);
	static void PrintMessage(FString Message, bool value, FColor Color = FColor::Yellow, float Duration = 1.f);
	static void PrintMessage(FString Message, FVector vector, FColor Color = FColor::Yellow, float Duration = 1.f);
	static void PrintMessage(FString Message, FRotator rotation, FColor Color = FColor::Yellow, float Duration = 1.f);
	static void PrintMessage(FString Message, FVector2D vector, FColor Color = FColor::Yellow, float Duration = 1.f);
	static void PrintMessage(FString Message, FString name, FColor Color = FColor::Yellow, float Duration = 1.f);
#pragma endregion 

#pragma region DrawDebug
	static void DustyDrawDebugString(const UWorld* InWorld, FVector const& TextLocation, const FString& Text, class AActor* TestBaseActor = NULL, FColor const& TextColor = FColor::White, float Duration = -1.000000, bool bDrawShadow = false, float FontScale = 1.f);
	static void DustyDrawDebugLine(const UWorld* InWorld, FVector const& LineStart, FVector const& LineEnd, FColor const& Color, bool bPersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0.f);
	static void DustyDrawDebugCapsule(const UWorld* InWorld, FVector const& Center, float HalfHeight, float Radius, const FQuat& Rotation, FColor const& Color, bool bPersistentLines = false, float LifeTime = -1.f, uint8 DepthPriority = 0, float Thickness = 0);
#pragma endregion

#pragma region AI
	static bool CanSeePlayer(const ACharacter* NPC, const ACharacter* Player, float TraceDistance, float DebugTime = 0.25f);
	
	static float CalculatePathfindingCost(const FVector& CurrentLocation, const FVector& Destination, UWorld* World);

	static void SetAIMeshCOllisions(UMeshComponent* Mesh);
	
#pragma endregion

#pragma region Camera
	static float GetAngularDistance(const FRotator& Rotator1, const FRotator& Rotator2);
	static bool  IsCloseEnough(FVector const& A, FVector const& B, float Tolerance);
	static bool  IsCloseEnough(FRotator const& A, FRotator const& B, float Tolerance);
#pragma endregion

	static ECommonInputType GetCurrentInputType();
};
