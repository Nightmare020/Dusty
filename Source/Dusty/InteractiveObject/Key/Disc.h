// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../SaveGame/SaveGameInterface.h"
#include "Dusty/Utils/DustyConst.h"
#include "Disc.generated.h"

UCLASS()
class DUSTY_API ADisc : public AActor, public ISaveGameInterface
{
	GENERATED_BODY()

protected:
	float m_InitialRotation = 0;
	float m_DesiredRotation = 0;
	

	UPROPERTY(EditAnywhere, Category = "Design")
	float m_TimeToRotate = 0.5;
	float m_Timer = 0;
	UPROPERTY(EditAnywhere, Category = "Design")
	float m_rotationAmount = 45.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditInstanceOnly)
		EGlyphs m_InitialGlyph;

	TMap<int, EGlyphs> m_GlyphsPosition;
public:	
	// Sets default values for this actor's properties
	ADisc();

	bool Rotating = false;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DiscMesh;

	//UPROPERTY(SaveGame)
	int CurrentGlyph = 0;


	// 0 left, 1 right
	virtual void RotateDisc(bool Direction);

	virtual void LoadActor();
	virtual void OnActorSaved_Implementation();


	bool haveTick = false;

	EGlyphs GetCurrentGlyph();

	UPROPERTY(EditAnywhere)
	USoundBase* m_sound;

};
