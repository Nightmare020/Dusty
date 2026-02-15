#pragma once

#include "CoreMinimal.h"
#include "../DustyCharacter.h"
#include "BaseState.generated.h"

class UBaseStateCamera;
struct PoleJumpValuesStruct;
class APoleElevator;
class APoleKeyActor;
class ASittingRock;

/*Me gustaría declararle mi amor, pero solo se declarar variables*/

#pragma region BaseState
UCLASS()
class DUSTY_API UBaseState : public UObject
{
	GENERATED_BODY()
public:
	UBaseState();
	virtual ~UBaseState();

	virtual void Init() {};
	virtual void Update(float _DeltaTime);
	virtual void Exit() {};

	ADustyCharacter* GetOwner() { return DustyCharacter; };
	void SetOwner(ADustyCharacter* _DustyCharacter) { DustyCharacter = _DustyCharacter; }

	bool CanUpdateTick = false;

protected:
	UPROPERTY(Transient)
	ADustyCharacter* DustyCharacter = nullptr;
};

#pragma endregion

#pragma region GroundMovement
UCLASS()
class DUSTY_API UGroundMovementState : public UBaseState
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;
};

#pragma endregion

#pragma region JumpingState
UCLASS()
class DUSTY_API UJumpingState : public UBaseState
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;
};

#pragma endregion

#pragma region AimingState
UCLASS()
class DUSTY_API UAimingState : public UBaseState
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;
};

#pragma endregion

#pragma region ClimbingState
UCLASS()
class DUSTY_API UPoleElevatorState : public UBaseState
{
	GENERATED_BODY()
public:
	bool Extended = false;

	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;
};

#pragma endregion

#pragma region PoleJumpingState
UCLASS()
class DUSTY_API UPoleJumpingState : public UBaseState
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;

	void SetPoleJumpValues(PoleJumpValuesStruct* _PoleJumpValues);

private:
	PoleJumpValuesStruct* m_PoleJumpValues;
};

#pragma endregion

#pragma region FallingState

UCLASS()
class DUSTY_API UFallingState : public UBaseState
{
	GENERATED_BODY()

public:
	float InitialHeight;
	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;
};

#pragma endregion

#pragma region PoleKeyState

UCLASS()
class DUSTY_API UPoleKeyState : public UBaseState
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;

	void SetupPoleKeyState(APoleKeyActor* _Interactable = nullptr);

	void PoleKeyInserted();
	void PoleKeyRemoved();

private:
	APoleKeyActor* m_Interactable = nullptr;
};

#pragma endregion

#pragma region AutoMovementState

DECLARE_DELEGATE(FCallbackDelegate)

UCLASS()
class DUSTY_API UAutoMovementState : public UBaseState
{
	GENERATED_BODY()
	FVector m_DesiredLocation;
	
	FRotator m_DesiredRotation;
	FRotator m_InitialRotation;

	float m_LastDistanceDifference = 0;

	float m_Timer = 0;
	float m_BaseTimeToRotate = 0.25;
	float m_MaxTimeToRotate = 1;
	float m_ActualTimeToRotate = 0.25;

	bool m_AutoRotation = false;

public:
	FCallbackDelegate CallbackDelegate;

	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;

	void SetAutoMovementValues(FVector _DesiredLocation, FRotator _DesiredRotation);	
};

#pragma endregion

#pragma region BookState

UCLASS()
class DUSTY_API UBookState : public UBaseState
{
	GENERATED_BODY()
public:
	bool SingleButtonPromptWasShown = false;
	bool DoubleButtonPromptWasShown = false;

	TSubclassOf<UBaseState> OriginState;

	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;
};

#pragma endregion

#pragma endregion

#pragma region FailState

UCLASS()
class DUSTY_API UFailState : public UBaseState
{
	GENERATED_BODY()
public:
	//virtual void Init() override;
	//virtual void Update(float _DeltaTime) override;
	//virtual void Exit() override;
};

#pragma endregion

#pragma region CinematicState

UCLASS()
class DUSTY_API UCinematicState : public UBaseState
{
	GENERATED_BODY()

public:

	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;

private:
	APlayerController* m_DustyController;
};

#pragma endregion

#pragma region CameraAnimation

UCLASS()
class DUSTY_API UCameraAnimationState : public UBaseState
{
	GENERATED_BODY()

public:

	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;
};

#pragma endregion

#pragma region MainMenuState

UCLASS()
class DUSTY_API UMainMenuState : public UBaseState
{
	GENERATED_BODY()

public:

	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;

private:
	APlayerController* m_DustyController;
};

#pragma endregion

#pragma region BenchState

UCLASS()
class DUSTY_API UBenchState : public UBaseState
{
	GENERATED_BODY()

public:

	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;

private:
	APlayerController* m_DustyController;
};

#pragma endregion

#pragma region OpeningState

UCLASS()
class DUSTY_API UOpeningState : public UBaseState
{
	GENERATED_BODY()

public:

	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;

private:
	APlayerController* m_DustyController;
};

#pragma endregion

#pragma region SittingState

UCLASS()
class DUSTY_API USittingState : public UBaseState
{
	GENERATED_BODY()

public:

	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
	virtual void Exit() override;

private:
	APlayerController* m_DustyController = nullptr;
	bool m_standingUp = false;
};

#pragma endregion