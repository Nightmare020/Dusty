#pragma once

#include "CoreMinimal.h"
#include "Dusty/InteractiveObject/Light/LightActor.h"
#include "CaterpillarNPC.generated.h"


class ACaterpillarSpline;

UCLASS()
class DUSTY_API ACaterpillarNPC : public ALightActor
{
private:
	GENERATED_BODY()

public:
	ACaterpillarNPC();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Activate(bool _bActivation) override;

	void SetLighting(bool _newLighting, bool _IsInstant);

private:
	FRotator GetNewRotation() const;
	void UpdateLighting(float DeltaTime);
	UMaterialInstanceDynamic* GetMaterial(UMaterialInterface* _material);


	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "SkeletalMesh")
	USkeletalMeshComponent* m_SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "CaterpillarSpline")
	ACaterpillarSpline* m_Spline = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "BaseSpeed")
	float m_BaseSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "SplineStartingPoint")
	float m_SplineStartingPoint= 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "MaxLightValue")
	float m_MaxLightValue= 4.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "MinLightValue")
	float m_MinLightValue= 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "BlendLightSpeed")
	float m_BlendLightSpeed = 2.f;
	

private:
	float RayDistance = 500.f;

	float m_LightValueTarget = 2.f;
	float m_CurrentLightValue = 1.f;
	
	bool m_IsCorrectlyLighted = false;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* DynamicMaterial = nullptr;
	
};
