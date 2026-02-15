#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "MazeNPC.generated.h"


class AMazeNPC;
class ALightActor;
class AMazeTriggerWaypoint;
class AMazeAIController;

UENUM(BlueprintType)
enum class AIType : uint8 {
	Guide			    UMETA(DisplayName = "Guide"),
	Interacting			UMETA(DisplayName = "Interacting"),
	PuzzleSolver        UMETA(DisplayName = "PuzzleSolver")
};

UENUM(BlueprintType)
enum class MazeLocomotion : uint8 {
	Move			    UMETA(DisplayName = "Move"),
	Stop			UMETA(DisplayName = "Stop"),
	Interacting        UMETA(DisplayName = "Interacting")
};

UCLASS()
class DUSTY_API AMazeNPC : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	ALightActor* m_TargetActor = nullptr;
	
	UPROPERTY(Transient)
	AMazeAIController* m_AIMazeController = nullptr;;
	
	int m_CurrentPathPointIndex = 0;

	// used to align can move with animations
	bool m_IsWaitingForStop = false;

public:
	AMazeNPC();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region Getters / Setters
	
	AMazeTriggerWaypoint* GetNextWaypoint(bool Advance);
	
	ALightActor* GetTargetActor() const;
	void SetTargetActor(ALightActor* _newTarget);

	void SetOverrideWaypoint(AMazeTriggerWaypoint* _overrideWaypoint);
	void SetCurrentPathPoint(int _newCurrent, bool _toggle = false);
	int GetCurrentPathPointIndex() const;
	AMazeTriggerWaypoint* GetCurrentTarget() const;
	AMazeTriggerWaypoint* GetCurrentPathPoint() const;

	void SetNeedsToSeePlayer(bool _needsToSeePlayer);
	bool GetNeedsToSeePlayer() const;

	void SetCanMove(bool _canSeePlayer);
	bool GetCanMove() const;
	
	void CheckCanMove();
	void CheckCanStop();

	void SetNPCTargetLocation(FVector _targetLocation);
	FVector GetNPCTargetLocation() const;
	
	void AddPathPoint(AMazeTriggerWaypoint* _newPathPoint);
	void ClearPath();

	void SetCurrentState(AIType _newState);
	AIType GetCurrentState() const;

	void SetCurrentLocomotionState(MazeLocomotion _newState);
	MazeLocomotion GetCurrentLocomotionState() const;

	void SetCanSee(bool _newCanSee);
	bool GetCanSee() const;

	bool GetLighting() const;
	void SetLighting(bool _newLighting, bool _IsInstant);
	UMaterialInstanceDynamic* GetMaterial(UMaterialInterface* _material);
	
	void PlayMontage(UAnimMontage* AnimMontage);

	
	
#pragma endregion

#pragma region Light

private:
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* DynamicMaterial = nullptr;

	// determines if the NPC has light
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "IsLighted")
	bool m_IsLighted = false;
	
	float m_LightValueTarget = 1.f;
	float m_CurrentLightValue = 1.f;

	bool m_IsCorrectlyLighted = false;

private:
	void UpdateLighting(float DeltaTime);
	
#pragma endregion
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	UBehaviorTree* Tree = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	TArray<AMazeTriggerWaypoint*> PathPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	float ValidDistance = 750.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	MazeLocomotion MazeNPCState = MazeLocomotion::Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	TArray<UAnimMontage*> Montages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	float TimeForIdleAnimation = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	float RandomVariationForIdleAnimation = 2.f;

	// -0.47 de blend
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	float LightBlendSpeed = 0.37f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	float LightMinValue = -0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	float LightMaxValue = 0.25f;

public:
	UPROPERTY(Transient)
	AMazeTriggerWaypoint* OverridedWaypoint = nullptr;

	UPROPERTY(Transient)
	AMazeTriggerWaypoint* StoppedBy = nullptr;

	// allows to know how many controllers have responsibility withing this NPC
	int BlockedBy = 0;
	
};
