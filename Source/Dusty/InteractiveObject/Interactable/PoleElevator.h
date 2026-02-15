// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "PoleElevator.generated.h"

class USphereComponent;
class APoleJumpConstraintActor;

UCLASS()
class DUSTY_API APoleElevator : public AInteractable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly)
		USphereComponent* m_ElevatorTrigger;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* m_ElevatorMesh;


	UPROPERTY(EditDefaultsOnly, Category = Pole)
		TSubclassOf<APoleJumpConstraintActor> m_PoleJumpConstraintClass;

	UPROPERTY(Transient)
		APoleJumpConstraintActor* m_PoleJumpConstraintActor = nullptr;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
		FVector m_DustyPosition;

	UPROPERTY(EditAnywhere)
	USoundBase* m_poleSound;


	/** Factor for moving laterally in pole jumps*/
	UPROPERTY(EditAnywhere, Category = "Design | PoleElevator", meta = (DisplayName = "PoleElevatorMovementFactor"))
		float m_PoleElevatorMovementFactor = 1000.f;
	/** Max angle of pole jump to retract pole. To prevent pole going horizontal*/
	UPROPERTY(EditAnywhere, Category = "Design | PoleElevator", meta = (DisplayName = "PoleElevatorAngleBeforeRetract"))
		float m_MaxPoleElevatorAngleBeforeRetract = 45.f;
	/** Time To Fully Extend Pole In Pole Elevator*/
	UPROPERTY(EditAnywhere, Category = "Design | PoleElevator", meta = (DisplayName = "PoleElevatorTimeToExtend"))
		float m_TimeToExtendInPoleElevator = 0.5f;
	/** Time To Automatically Exit Pole Elevator*/
	UPROPERTY(EditAnywhere, Category = "Design | PoleElevator", meta = (DisplayName = "TimeToAutoRetract"))
		float m_TimeToRetract = 5.f;
	/** Time To Automatically Play Pole Sound*/
	UPROPERTY(EditAnywhere, Category = "Design | PoleElevator", meta = (DisplayName = "TimePoleSound"))
	float m_timeToPoleSound = 5.f;

	float m_DistanceToCenter = 0.f;

	//Placeholder
	FTimerHandle m_TimeToEndAnimationHandle;
	FTimerHandle m_RetractHandle;
	FTimerHandle m_poleSoundTimer;


	void SetExtendedInState(bool _Extended);

public:
	void SpawnPoleElevator();
	virtual void BeginPlay() override;


	APoleElevator();
	virtual void Interact() override;
	void ExtendElevator();
	void ExitElevator();
	void ClearTimers();
	void PlayPoleSound();

	bool Extended = false;

	void Retract();

	virtual void OnFinishedAutoMovement() override;

	UPROPERTY(EditAnywhere)
		USceneComponent* PolePoint;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	
	UPROPERTY(EditAnywhere)
	int m_HudButtonId = 4;
};
