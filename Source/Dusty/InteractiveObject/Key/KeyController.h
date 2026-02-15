// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoleKeyActor.h"
#include "../../SaveGame/SaveGameInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Dusty/Utils/DustyConst.h"
#include "KeyController.generated.h"

class ADisc;
class AToggleActor;
class AGlyphController;
class USphereComponent;

UCLASS()
class DUSTY_API AKeyController : public APoleKeyActor
{
	GENERATED_BODY()
protected:	
	bool m_CanReceiveVerticalInput = true;

	UPROPERTY(EditAnywhere)
	int MAX_DISCS = 3;

	UPROPERTY(EditAnywhere, Category = "Design | Disc", meta = (DisplayName = "Vertical Input Cooldown"))
		float m_VerticalInputCooldown = 0.5;
	

	// From Inner to Outer, 0 to 2. This needs to be consistent between interactions.
	int m_CurrentDiscIdx = 0;
	int m_PreviousDiscIdx = 0;

	UPROPERTY(EditAnywhere, Category = "Design | Disc", meta = (DisplayName = "Horizontal Input Threshold"))
		float m_HorizontalInputThreshold = 0.5;

	UPROPERTY(EditAnywhere, Category = "Design | Disc", meta = (DisplayName = "Vertical Input Threshold"))
		float m_VerticalInputThreshold = 0.5;

	UPROPERTY(EditAnywhere, Category = "Design | HUD", meta = (DisplayName = "Show/Hide Tutorial HUD"))
	bool m_TutorialHUDVisibility = true;

	FTimerHandle m_VerticalInputTimerHandle;


public:	
	void EnableVerticalInputCallback();
	// Sets default values for this actor's properties
	AKeyController() {}

public:	
	virtual void Update(float _DeltaTime, FVector2D _Input) override;



};
