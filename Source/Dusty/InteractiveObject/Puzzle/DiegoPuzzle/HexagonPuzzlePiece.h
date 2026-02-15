#pragma once

#include "CoreMinimal.h"
#include "../PuzzlePiece.h"
#include "HexagonPuzzlePiece.generated.h"


class AHexagonPuzzleController;
class USphereComponent;
class UCylinderComponent;
UCLASS()
class DUSTY_API AHexagonPuzzlePiece : public APuzzlePiece
{

	GENERATED_BODY()
public:
	AHexagonPuzzlePiece();
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Activate(bool _Activating) override;

	void PieceBeginPlaySetter(AHexagonPuzzleController* Controller, float _NewHeightChange, float _BlendSpeed, float _RandomBlendSpeedVariation, float _DescentMultiplier);

	void SetCurrentHeightAsBase();

	void ApplyTexture(UMaterialInterface* Material);

	void ToggleLighted(bool _newValue);


	virtual void LoadActor();
	virtual void OnActorSaved_Implementation();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	bool CorrectPillar = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	int Index = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
	int PillarType = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design")
	UStaticMeshComponent* HexagonTrigger = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design")
	UStaticMeshComponent* Pillar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design")
	USceneComponent* TopScene;



private:
	// speed control
	
	float m_BaseBlendSpeed = 0.f;
	float m_CurrentBlendSpeed = 0.f;
	float m_RandomBlendSpeedVariation = 0.f;
	float m_DescentMultiplier = 0.f;
	
	// height control 
	float m_MinHeight = 0.f;
	float m_HeightChange = 0.f;
	float m_NewHeight = 0.f;
	FVector m_StartingPosition;

	// overall puzzle motion control
	bool m_Updating = false;

	UPROPERTY(Transient)
	AHexagonPuzzleController* m_Controller = nullptr;
	
	UPROPERTY(EditInstanceOnly, Category = "Design | Light", meta = (AllowPrivateAccess = "true"))
	bool m_LightTilt = false;
	UPROPERTY(EditInstanceOnly, Category = "Design | Light", meta = (AllowPrivateAccess = "true"))
	bool m_InitialLight = false;

	UPROPERTY(EditAnywhere, Category = "Art | Light", DisplayName = "MaxLightIntensityRed", meta = (AllowPrivateAccess = "true"))
	float m_MaxLightIntensityRed = 0;

	UPROPERTY(EditAnywhere, Category = "Art | Light", DisplayName = "MaxLightIntensityGreen", meta = (AllowPrivateAccess = "true"))
	float m_MaxLightIntensityGreen = 0;

	UPROPERTY(EditAnywhere, Category = "Art | Light", DisplayName = "MaxLightIntensityBlue", meta = (AllowPrivateAccess = "true"))
	float m_MaxLightIntensityBlue = 8;


	UPROPERTY(EditDefaultsOnly, Category = "Art | Light", meta = (AllowPrivateAccess = "true"))
	float m_Amplitude = 2;
	UPROPERTY(EditDefaultsOnly, Category = "Art | Light", meta = (AllowPrivateAccess = "true"))
	float m_Frequency = 5;

	float m_ColorTimer = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Art | Light", meta = (AllowPrivateAccess = "true"))
	float m_MinRandom = -2.f;
	UPROPERTY(EditDefaultsOnly, Category = "Art | Light", meta = (AllowPrivateAccess = "true"))
	float m_MaxRandom = 2.f;

};
