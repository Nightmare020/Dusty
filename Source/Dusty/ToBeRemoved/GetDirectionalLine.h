// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GetDirectionalLine.generated.h"

class ADirectionalLight;

UCLASS()
class DUSTY_API AGetDirectionalLine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGetDirectionalLine();

	UPROPERTY(EditAnywhere, Category = "DirectionalLight")
		ADirectionalLight* DirectionalLightActor;
	UPROPERTY(EditAnywhere, Category = "DirectionalLight")
		float VerticalOffset = 200;
	UPROPERTY(EditAnywhere, Category = "DirectionalLight")
		float LineLength = 500;

#if WITH_EDITOR 
	virtual bool ShouldTickIfViewportsOnly() const override { return IsSelectedInEditor(); }
#endif
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
