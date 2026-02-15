// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Interactable/DissolveActor.h"
#include "Camera/CameraShake.h"
#include "DissolveDoor.generated.h"

/**
 * 
 */
class ADustyCameraManager;
UCLASS()
class DUSTY_API ADissolveDoor : public ADissolveActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void OpenDoor();

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool PreviewAnimationDebug = false;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool IsDissolveDoor = false;

	UPROPERTY(EditAnywhere, Category = "Animation values")
	float TimeStartOpenOffset = 0;

	UPROPERTY(EditAnywhere, Category = "Animation values")
	float TimeStartOpen = 4;

	UPROPERTY(EditAnywhere, Category = "Animation values")
	float VelocityStartOpen = 20;

	UPROPERTY(EditAnywhere, Category = "Animation values")
	float TimeBlockDoor = 1;

	UPROPERTY(EditAnywhere, Category = "Animation values")
	float VelocityBlockDoor = 25;

	UPROPERTY(EditAnywhere, Category = "Animation values")
	float TimeStaticDoor = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Animation values")
	float TimeReOpen = 3;

	UPROPERTY(EditAnywhere, Category = "Animation values")
	float VelocityReOpen = 30;

	UPROPERTY(EditAnywhere, Category = "Animation values")
	float TimeDissolve = 3;

	UPROPERTY(EditAnywhere, Category = "Animation values")
	TSubclassOf<ULegacyCameraShake> CameraShake;

	virtual void Activate(bool _bActivation) override;

	virtual void LoadActor() override;
	virtual void OnActorSaved_Implementation() override;

private:
	void OpenDoorAnimation(float DeltaTime);
	float easeOutElastic(float x);
	float easeInCubic(float x);
	float easeOutCubic(float x);

	bool m_OpenWhenLoaded = false;

	float m_currentTimeStartOffset;
	float m_currentTimeStartOpen;
	float m_currentTimeBlockDoor;
	float m_currentTimeStaticDoor;
	float m_currentTimeReOpenDoor;
	float m_currentTimeDissolve;

	bool m_openDoor = false;
	float m_lastBlockValue = 0;
	bool m_shakeOnce = false;

	UPROPERTY(Transient)
	ADustyCameraManager* m_cameraManager;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* m_sound;

	bool soundPlayed = false;
};
