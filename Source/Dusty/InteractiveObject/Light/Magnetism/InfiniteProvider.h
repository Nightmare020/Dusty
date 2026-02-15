#pragma once

#include "CoreMinimal.h"
#include "../../ToggleActor.h"
#include "InfiniteProvider.generated.h"

class UNiagaraComponent;
class USphereComponent;
class ALightActor;
class ADustyCharacter;
class ALightParticle;
class UNiagaraSystem;

UCLASS()
class DUSTY_API AInfiniteProvider : public AToggleActor
{
private:
	GENERATED_BODY()

private:
	TArray<ALightActor*> m_AffectedActors;
	
	UPROPERTY(Transient)
	ADustyCharacter* m_Player = nullptr;

	UPROPERTY(EditAnywhere)
	USphereComponent* m_Trigger;
	
	UPROPERTY(EditInstanceOnly, Category = "Design | Provider")
	TArray<ALightActor*> m_StartingActors;

	UPROPERTY(EditDefaultsOnly, Category = "Art | Meshes", meta = (DisplayName = "ProviderMesh"))
		UStaticMesh* m_ProviderMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Art | Meshes", meta = (DisplayName = "DrainerMesh"))
		UStaticMesh* m_DrainerMesh;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* m_Mesh;
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* m_BallMesh;
	UPROPERTY(EditDefaultsOnly)
		USceneComponent* m_BallPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Art | BallColor | Provider", meta = (DisplayName = "RedBallColor"))
		float BallRColor = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Art | BallColor | Provider", meta = (DisplayName = "GreenBallColor"))
		float BallGColor = 4.894998f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Art | BallColor | Provider", meta = (DisplayName = "BlueBallColor"))
		float BallBColor = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Art | BallColor | Drainer", meta = (DisplayName = "RedBallColor"))
		float BallDrainerRColor = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Art | BallColor | Drainer", meta = (DisplayName = "GreenBallColor"))
		float BallDrainerGColor = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Art | BallColor | Drainer", meta = (DisplayName = "BlueBallColor"))
		float BallDrainerBColor = 8;

	UPROPERTY(EditInstanceOnly, SaveGame)
		bool m_Activated = true;


	bool m_PlayerInsideTrigger = false;

	UNiagaraComponent* m_niagaraEffect = nullptr;
	float particleLifetime = 0.0f;

	UPROPERTY(EditAnywhere)
	USoundBase* m_poleChargedSound;
	UPROPERTY(EditAnywhere)
	USoundBase* m_poleDischargeSound;

	virtual void OnConstruction(const FTransform& Transform) override;
	void ToggleBallLight(bool _Lighted, float _redValue, float _greenValue, float _blueValue);

	UPROPERTY(EditDefaultsOnly, Category = "Art | Ball")
	float m_Frequency = 2;
	float m_BallTimer = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Art | Ball")
	float m_Amplitude = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Art | Light")
	float m_MinRandom = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Art | Light")
	float m_MaxRandom = 1.f;

	FVector m_InitialBallPosition;
	FVector m_DesiredBallPosition;
	
	UPROPERTY(EditAnywhere)
	float m_maxBallHeight;
	UPROPERTY(EditAnywhere)
	float m_lightForce;

	void BallMovement(float _DeltaTime, float _distance);
public:	
	AInfiniteProvider();
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	void SetPlayer(ADustyCharacter* Character);
	void RemovePlayer();

	virtual void Activate() override;

	UStaticMeshComponent* GetBallMesh() const;
	USceneComponent* GetBallComponent() const;



	UPROPERTY(EditAnywhere)
	TSubclassOf<ALightParticle> LightParticle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Design | Provider")
	bool IsDrainer;
	
	UPROPERTY(EditAnywhere, Transient)
	UNiagaraSystem* m_effect = nullptr;

	UFUNCTION()
	void OnNiagaraSystemFinished(UNiagaraComponent* FinishedComponent);
};
