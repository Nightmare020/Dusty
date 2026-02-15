#pragma once

#include "CoreMinimal.h"
#include "KeyController.h"
#include "DiscKeyController.generated.h"

class ACameraFocusToDoor;
class AStaticMeshActor;
class AAudioActor;

UCLASS()
class DUSTY_API ADiscKeyController : public AKeyController
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, Category = "Design | Disc", meta = (ToolTip = "Combination of numbers to solve the puzle. Combination starts from the Inner Disc. Indexes start from 0 up to 7!"))
	TMap<EGlyphs, bool> m_Solution;
	
	UPROPERTY(EditInstanceOnly)
	TArray<ADisc*> m_Discs;
	
	UPROPERTY(EditInstanceOnly)
	AStaticMeshActor* m_base = nullptr;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* m_baseDynamicMaterial;

	UPROPERTY(EditInstanceOnly)
	AGlyphController* m_GlyphController;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
		TArray<FVector> m_DecalPositions;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "DoorCamera")
	ACameraFocusToDoor* m_FocusCamera = nullptr;

	UPROPERTY(EditAnywhere)
	float m_DecalSpeed = 10;

	bool m_Solved = false;
	float m_currentTimeDecalAnimation;
	bool m_EndAnimation = false;


	virtual void LoadActor() override;
	virtual void OnActorSaved_Implementation() override;

	bool CheckSolved();
	void SolvedAniamtion(float DeltaTime);
	virtual void UpdateMaterials() override;

public:
	ADiscKeyController();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	virtual void OnPoleInserted() override;
	virtual void OnPoleRemoved() override;
	virtual void Update(float _DeltaTime, FVector2D _Input) override;

	UPROPERTY(EditAnywhere)
	float TimeDecalAnimation = 3;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DecalActor;

	UPROPERTY(EditAnywhere)
	float MaxDecalSize = 6;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// UAudioComponent* SoundSolved;

	UFUNCTION(BlueprintNativeEvent)
	void PlayMetasound(const FName& _text);

	bool solvedSoundPlayed = false;

	UPROPERTY(EditAnywhere)
	AAudioActor* m_track;
};
