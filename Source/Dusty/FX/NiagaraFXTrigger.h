// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFXTrigger.generated.h"

class ANiagaraActor;
class UNiagaraSystem;
class UBoxComponent;
class UNiagaraComponent;

UCLASS()
class DUSTY_API ANiagaraFXTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANiagaraFXTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Niagara Effect")
	UNiagaraComponent* m_effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* m_trigger;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	bool m_activated;
};
