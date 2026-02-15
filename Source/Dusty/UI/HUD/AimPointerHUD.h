// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AimPointerHUD.generated.h"

UCLASS()
class DUSTY_API AAimPointerHUD : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAimPointerHUD();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Mesh component for pointer sphere when it's not hitting interactable object */
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Aim Pointer Sphere Mesh")
		UStaticMeshComponent* InactiveMeshSphere;

	/** Mesh component for pointer sphere when it's hitting interactable object */
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Aim Pointer Sphere Mesh")
		UStaticMeshComponent* ActiveMeshSphere;

};
