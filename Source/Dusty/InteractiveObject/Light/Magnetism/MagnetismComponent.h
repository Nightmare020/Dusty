// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MagnetismComponent.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

USTRUCT(BlueprintType, Blueprintable)

struct FMagnetDirections
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FVector direction = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* system = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUSTY_API UMagnetismComponent : public UActorComponent
{
	GENERATED_BODY()
private:
public:	
	// Sets default values for this component's properties
	UMagnetismComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#pragma region directions
private:
	UPROPERTY(EditAnywhere)
	bool right;
	UPROPERTY(EditAnywhere)
	bool left;
	UPROPERTY(EditAnywhere)
	bool top;
	UPROPERTY(EditAnywhere)
	bool bottom;
	UPROPERTY(EditAnywhere)
	bool front;
	UPROPERTY(EditAnywhere)
	bool back;

#pragma endregion

	UPROPERTY(EditAnywhere)
	float m_magnetDistance;
	
	TArray<FVector> m_directions;

	UFUNCTION(BlueprintCallable)
	void Raycast(FVector _direction);

	UPROPERTY(EditAnywhere)
	float m_magnetAcceleration;

	UPROPERTY(EditAnywhere)
	bool m_lighted;

	UPROPERTY(EditAnywhere)
	float m_springConstant;

	UPROPERTY(EditAnywhere)
	float m_damperConstant;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* m_attachEffect;
	
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* m_attractEffect;
	UNiagaraComponent* m_attractComponent;

	UPROPERTY(EditAnywhere)
	float m_effectTimer;
	float m_timer;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* m_repulseEffect;
	UNiagaraComponent* m_repulseComponent;


	//Audio
	UPROPERTY(EditAnywhere)
	USoundBase* m_repulseSound;
	UPROPERTY(EditAnywhere)
	USoundBase* m_attractSound;

	// UPROPERTY(EditAnywhere)
	// TArray<FMagnetDirections> directions;


public:
	UFUNCTION()
	void ChangeMagnetism(bool _value);

	UFUNCTION()
	void SeparateMagnet(AActor* _magnet);
	bool GetLightStatus() const;

	UFUNCTION()
	void ExecuteMagnetism(float _deltaTime);

	UPROPERTY(EditAnywhere)
	bool m_repulsed;
};
