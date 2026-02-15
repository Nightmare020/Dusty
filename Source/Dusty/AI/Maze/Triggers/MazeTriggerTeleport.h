#pragma once

#include "CoreMinimal.h"
#include "BaseActors/MazeActorTrigger.h"
#include "MazeTriggerTeleport.generated.h"


class AMazeTriggerWaypoint;
class UNiagaraComponent;

UCLASS()
class DUSTY_API AMazeTriggerTeleport : public AMazeActorTrigger
{
private:
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "Target")
	USceneComponent* m_Target = nullptr;

	// UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "OtherTeleport")
	// AMazeTriggerTeleport* m_OtherTeleport = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "ControlledWaypoint")
	AMazeTriggerWaypoint* m_ControlledWaypoint = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "TeleportDelay")
	float m_TeleportDelay = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "IsActive")
	bool m_IsActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "NiagaraEffect")
	UNiagaraComponent* m_Effect;

	
public:	
	AMazeTriggerTeleport();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void DoBeginOverlap(AActor* OtherActor) override;
	virtual void DoEndOverlap(AActor* OtherActor) override;

	void SetIsActive(bool _isActive);


private:
	void PrepareTeleport(AActor* _targetActor, const USceneComponent* _targetScene);
	
private:
	float m_DelayTimer = 0.f;
	
	FVector m_TargetLocation = FVector();

	UPROPERTY(Transient)
	AActor* m_TargetActor = nullptr;
};
