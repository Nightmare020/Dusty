#pragma once

#include "CoreMinimal.h"
#include "Dusty/InteractiveObject/ToggleActor.h"
#include "DissolveActor.generated.h"

class UMaterialInstanceDynamic;

UCLASS()
class DUSTY_API ADissolveActor : public AToggleActor
{
private:
	GENERATED_BODY()

public:
	ADissolveActor();

	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual void Activate(bool _bActivation) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void Init(bool _IsActive, float _Duration);

	virtual void LoadActor();

	virtual void OnActorSaved_Implementation();

	void StartDissolve();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design")
	UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(Transient)
	UMaterialInterface* Material = nullptr;

	UPROPERTY(Transient)
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

	UPROPERTY(EditAnywhere, Category = "Design")
	float MaxDissolveValue = 0.5f;

private:
	bool m_IsActive = true;
	float m_CurrentDissolveValue = 1.f;

	float DissolveTime = 2.f;
	float m_Timer = 0.f;
	
	int m_Direction = 1;
	bool m_startDissolve = false;

	UPROPERTY(EditAnywhere, Category = "Art")
		bool m_FloatingMovement = false;

	FVector m_InitialPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Art")
		float m_Frequency = 0.5;
	float m_FloatingTimer = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Art")
		float m_Amplitude = 20;

};