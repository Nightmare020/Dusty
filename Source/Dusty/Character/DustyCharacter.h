#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "../Resources/AnimationCurves.h"
#include "AlphaBlend.h"
#include "../InteractiveObject//Interactable/Interactable.h"
#include "Camera/CameraShake.h"


#include "DustyCharacter.generated.h"

#pragma region ForwardDeclarations
class AAudioActor;
class APole;
class UInputAction;
class AFixedCamera;
class UCurveFloat;
class UDustyMovementComponent;
class UBookWidget;
class UInputMappingContext;
class UPointLightComponent;
class ABookActorTextures;
class ULineBatchComponent;
class UMainMenuWidget;
class USettingsWidget;
class APoleJumpConstraintActor;
class ADustyCameraManager;
class UBillboardComponent;
class UPoseableMeshComponent;
class AInfiniteProvider;
class UCapsuleComponent;
class ABench;
class UActionsFilter;
class UPlayerHUDWidget;
class UArrowComponent;

struct PoleJumpValuesStruct;
class UBookPageWidget;
class ABookRenders;

class APoleElevator;
class UDustyAnimInstance;
#pragma endregion


DECLARE_LOG_CATEGORY_EXTERN(DustyCharacterLog, Error, All);

class UNiagaraComponent;
UCLASS()
class DUSTY_API ADustyCharacter : public ACharacter
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "Design", meta = (AllowPrivateAccess = "true"), DisplayName = "BookTargetArrow")
		UArrowComponent* m_BookTargetArrow = nullptr;

	FTransform m_CameraTransform;
	float m_CameraTransitionTime;



public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly)
	bool PlaceholderAlbertoDontShowHud = false;
#endif
	ADustyCharacter(const FObjectInitializer& ObjectInitializer);
	
	void InitializeCharacter();
	void AAAAAAAAAAAlEVELS();
	
	FORCEINLINE UDustyMovementComponent* GetDustyMovementComponent() const { return DustyMovementComponent; }

	FCollisionQueryParams GetIgnoreCharacterCollisionParams() const;

	void OnActorLoaded();
	void SetStartCamera(const FTransform& _CameraTransform, float _CameraTransitionTime);
	void ExitStartCamera();


	UFUNCTION(BlueprintImplementableEvent)
	void UnlockAchievement(FName AchievementId, float Progress = 100.f);

	UPROPERTY(EditAnywhere)
	TArray<FName> AchievementsNames;


	//Placeholder
	UPROPERTY(EditAnywhere, Category = "Design | PoleJump")
		bool JumpTowardsCamera = false;
	UPROPERTY(EditAnywhere, Category = "Design | SaveGame")
		bool SpawnOnSavedData = false;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"), Transient)
		UDustyMovementComponent* DustyMovementComponent = nullptr;
	
	UPROPERTY(Transient)
		ADustyCameraManager* CameraManager = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pole")
		UPoseableMeshComponent* PoleSkeletal = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pole")
		UPointLightComponent* PolePointLight = nullptr;

	UFUNCTION()
		void SetMainMenuAfterCredtis();

	
#pragma region Tattoos
private:
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* m_DustyDynamicMaterial;

	UFUNCTION(BlueprintCallable)
	UMaterialInstanceDynamic* SetDustyDynamicMaterial(UMaterialInterface* DustyMaterial);

	float m_CurrentTattooValue = 0.f;
	float m_TargetTattooValue = 0.f;


public:
	void SetDustyTattooValue(float NewValue, bool IsNew = true);

	UPROPERTY(EditDefaultsOnly, Category = "Art | Tattoo")
		float TattooBlendSpeed = 0.5;
	UPROPERTY(EditDefaultsOnly, Category = "Art | Tattoo")
		float FullTattooValue = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Art | Tattoo")
		float NoTattooValue = 1;
	UPROPERTY(EditDefaultsOnly, Category = "Art | Tattoo")
		float PoleLightedTattooValue = 0.35;
#pragma endregion

#pragma region ConsoleCommands

private:
	UPROPERTY(EditDefaultsOnly, Category = "Console Commands | Locations")
	TArray<FVector> m_SetDustyLocations;

public:
	UFUNCTION(Exec)
		void SetDustyLocation(int LocationIndex);
	UFUNCTION(Exec)
		void UnlockSkills(bool Unlocked);
	UFUNCTION(Exec)
		void TriggerFailState();
	UFUNCTION(Exec)
		void LoadAllLevels();
	UFUNCTION(Exec)
		void LoadLevelsOfArea(int index);
	UFUNCTION(Exec)
		void SetDustySpeed(float NewSpeed);
	UFUNCTION(Exec)
		void UnlockAllPages();
	UFUNCTION(Exec)
		void UnlockAllCollectibles();

	UPROPERTY(EditAnywhere, Category = "Design | Levels")
	TArray<FName> Area1Levels;
	UPROPERTY(EditAnywhere, Category = "Design | Levels")
	TArray<FName> Area2Levels;
	UPROPERTY(EditAnywhere, Category = "Design | Levels")
	TArray<FName> Area3Levels;
	UPROPERTY(EditAnywhere, Category = "Design | Levels")
	TArray<FName> Area4Levels;
	UPROPERTY(EditAnywhere, Category = "Design | Levels")
	TArray<FName> Area5Levels;
	UPROPERTY(EditAnywhere, Category = "Design | Levels")
	TArray<FName> Area6Levels;
	TArray<FName> PlaceholderLevelsToLoad;
	void PlaceholderLoadLevels();


#pragma endregion

#pragma region InputActions
private:
	void AddInputMappingContext(UInputMappingContext* ContextToAdd, int32 InPriority);
	void RemoveInputMappingContext(UInputMappingContext* ContextToRemove);

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputMappingContext* m_DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_MoveAction;

	/** Walk Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_ShiftAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_ExtendPoleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_RetractPoleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_PoleLightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_BookClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_BookCardinalNavigationAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_BookChangePageAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_BookChangeBookmarkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_SkipCinematicAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_GetBackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* m_ReadModeAction;

#pragma endregion

#pragma region Inputs
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
		FVector2D MovementInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
		FVector2D LookInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
		bool AimInput = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float DefaultInputLookVelocity = 3.f;

	bool ExtendBufferForDoubleJump = false;

	void SetInput(bool _newInput);

	void ResetMovementInput();
	
	bool collision;
#pragma endregion

#pragma region Pole

#pragma region PoleBase
private:

	UPROPERTY(EditAnywhere, Category = Pole)
		TArray<float> m_PoleHeights;

	float m_PoleBaseHeight = 200;
	float m_PoleBaseRadius = 4;


	UPROPERTY(EditAnywhere, Category = Pole, meta = (DisplayName = "PoleSocketName"))
	FName m_PoleSocketName = "R_Palm_PaloSocket";

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* PoleDynamicMaterial;



public:
	bool CanUnnailPole = false;
	bool DustyInPlanetaryKeyController = false;
	UPROPERTY(EditDefaultsOnly)
		float InitialTopPolePosition = -100;

	UPROPERTY(EditDefaultsOnly)
	float InitialBottomPolePosition = -50;
	float GetTotalPoleLength() { return m_PoleBaseHeight + BottomPoleExtension + abs(TopPoleExtension); }
	float GetBottomPoleExtension() { return BottomPoleExtension; }
	float GetTopPoleExtension() { return TopPoleExtension; }
	float GetPoleBaseRadius() { return m_PoleBaseRadius; }
	float GetPoleBaseHeight() { return m_PoleBaseHeight; }
	UMaterialInstanceDynamic* GetPoleMaterial();

#pragma endregion

#pragma region PoleElevator


public:
	APoleElevator* CurrentPoleElevator = nullptr;
	void ExitPoleElevator();
#pragma endregion

#pragma region PoleLight
private:
	void PoleInteract();
	void BallMovement(float _DeltaTime);
	void PlaceBallMovement(float _DeltaTime);

	float m_BallPlacementTimer = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Art | Pole | Ball")
	float m_BallPlacementTime = 0.25;

	UPROPERTY(EditDefaultsOnly, Category = "Art | Pole | Ball")
	FVector m_InitialBallPosition = FVector(0,0,-230);
	UPROPERTY(EditDefaultsOnly, Category = "Art | Pole | Ball")
	FVector m_InitialStaticBallPosition = FVector(0,0,-223);
	FVector m_StaticBallPosition = m_InitialStaticBallPosition;

	bool m_BallPlaced = false;	
	bool m_BallMoving = false;	

	FVector m_AuxBallPosition = m_InitialBallPosition;
	float m_BallMovementTimer = 0;


	UPROPERTY(EditDefaultsOnly, Category = "Art | Pole | Ball")
	float m_Amplitude = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Art | Pole | Ball")
	float m_Frequency = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Art | Light")
	float m_MinRandom = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Art | Light")
	float m_MaxRandom = 1.f;

	UPROPERTY(SaveGame)
	bool m_PoleLighted = false;

	UPROPERTY(EditAnywhere, Category = "Design | Pole | Light", meta = (DisplayName = "Max Light Intensity Red"))
		float m_MaxLightIntensityRed = 150;
	UPROPERTY(EditAnywhere, Category = "Design | Pole | Light", meta = (DisplayName = "Max Light Intensity Green"))
		float m_MaxLightIntensityGreen = 150;
	UPROPERTY(EditAnywhere, Category = "Design | Pole | Light", meta = (DisplayName = "Max Light Intensity Blue"))
		float m_MaxLightIntensityBlue = 0;

	UPROPERTY(EditAnywhere, Category = "Design | Pole | Interaction", meta = (DisplayName = "Interact Extra Radius (Not Used)"))

		float m_InteractExtraRadius = 1.f;

	AInfiniteProvider* m_CurrentProvider = nullptr;

public:
	bool IsPoleLighted() const { return m_PoleLighted; }
	UFUNCTION(Exec)
		void TogglePoleLight(bool Lighted);

	UPROPERTY(EditAnywhere, Category = "Design | Pole | Interaction", meta = (DisplayName = "Interaction Length"))
		float InteractionLength = 200.f;
#pragma endregion

#pragma region PolePointLight
private:
	bool m_polePointLightShouldBeActive = false;

public:
	//Allows the point light to be active if the pole has light
	UFUNCTION()
	void TogglePolePointLight();
	UFUNCTION()
	void SetPolePointLightVisibility(bool _value);
#pragma endregion


#pragma region PoleMovement
private:
	UPROPERTY(EditAnywhere, Category = Pole)
		TSubclassOf<APoleJumpConstraintActor> m_PoleJumpConstraintClass;
	

public:
	void PoleDoubleJump();
	UPROPERTY(Transient)
		APoleJumpConstraintActor* PoleJumpConstraintActor = nullptr;

	void SpawnPoleJumpConstraint(PoleJumpValuesStruct* _PoleJumpValues);
#pragma region PoleJump
	private:
		bool CanDoPoleJump();

		void PoleJump();	

		FVector m_PoleJumpLocation = FVector::ZeroVector;

		FRotator m_CurrentRotation;
		FRotator m_DesiredRotation;

		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "TiltAngle"))
		float m_TiltAngle = 15;

		bool m_Tilt = false;

		FVector m_PolePoint = FVector::ZeroVector;
		UPROPERTY(EditAnywhere, Category = "PoleJump", meta = (DisplayName = "PolePointOffset"))
		FVector m_PolePointOffset = FVector(-13,-20,12.74);

		UPROPERTY(EditAnywhere, Category = "PoleJump", meta = (DisplayName = "HandsOffset"))
		FVector m_HandsOffset = FVector(57,0,100);

		/** Bigger faster, smaller slower */
		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "JumpVelocityFactor"))
		float m_JumpVelocityFactor = 12.f;

		/** How much Dusty brakes when making the jump for the pole jump */
		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "BrakeFactorInitialJump"))
		float m_BrakeFactorInitialJump = 0.7f;

		/** Maximum Angle with Up Vector to consider a valid pole point */
		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "MaxJumpVerticalAngle"))
			float m_MaxJumpVerticalAngle = 45.f;

		/** Minimum Height needed to consider a valid pole point */
		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "MinimumJumpHeight"))
			float m_MinimumJumpHeight = 130.f;


		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "BasePoleJumpDistance"))
			float m_BasePoleJumpDistance = 400;

		/** Speed needed to perform a Pole Jump */
		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "MinPoleJumpSpeed"))
			float m_MinPoleJumpSpeed = 0;

		/** Factor for moving laterally in pole jumps*/
		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "PoleJumpMovementFactor"))
			float m_PoleJumpMovementFactor = 1000.f;

		/** Offset from forward direction to start throwing raycasts for pole jump*/
		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "AngleOffset"))
			float m_AngleOffset = 22.5f;

		/** Angle for throwing raycasts for pole jump*/
		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "PoleJumpRaycastAngle"))
			float m_PoleJumpRaycastAngle = 45.f;

		UPROPERTY(EditAnywhere, Category = "PoleJump")
			float m_MaxOffsetPoleJump = 20.f;

		/** Max angle of pole jump to retract pole. To prevent pole going horizontal*/
		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "PoleJumpAngleBeforeRetract"))
			float m_MaxPoleJumpAngleBeforeRetract = 45.f;

		/** Time To Fully Extend Pole In Pole Jump*/
		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "PoleJumpTimeToExtend"))
			float m_TimeToExtendInPoleJump = 0.5f;
		/** Maximun Floor Slope angle to consider a valid pole point */
		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "MaximumJumpSlopeAngle"))
			float m_MaximumJumpSlopeAngle = 20.f;

		/** Max distance in front of dusty he can try to perform a pole jump*/
		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "ForwardOffsetToDoPoleJump"))
			float m_ForwardOffsetToDoPoleJump = 300.f;
		/** How many tries along the offset will be done. More implies more raycasts but not sure if the precision will imply an upgrade*/
		UPROPERTY(EditAnywhere, Category = "Design | PoleJump", meta = (DisplayName = "ForwardOffsetSubdivisionsToDoPoleJump"))
			float m_ForwardOffsetSubdivisionsToDoPoleJump = 20.f;

public:
	void EndInitialJumpInPoleJump();

	void StartExtendingBottomPole();
	void StartRetractingBottomPole();

#pragma endregion
	
#pragma endregion

#pragma region PoleHit
public:
	UPROPERTY(EditAnywhere)
		UCapsuleComponent* capsulePole;

	bool CanPlayHitMontage = true;
private:

	UPROPERTY(EditAnywhere, Category = Pole, meta = (DisplayName = "PoleHitMontage"))
		UAnimMontage* m_PoleHitMontage = nullptr;
	UPROPERTY(EditAnywhere, Category = Pole, meta = (DisplayName = "PoleHitCameraShake"))
		TSubclassOf<ULegacyCameraShake> m_PoleHitCameraShake;
	UPROPERTY(EditAnywhere, Category = Pole, meta = (DisplayName = "PoleHitSound"))
		USoundBase*  m_PoleHitSound;
	UPROPERTY(EditAnywhere, Category = Pole, meta = (DisplayName = "PoleExtendSound"))
	USoundBase*  m_extendPoleSound;

	UPROPERTY(EditAnywhere, Category = Pole, meta = (DisplayName = "PoleRetractSound"))
	USoundBase*  m_retractPoleSound;
	bool m_extendPoleSoundPlayed = false;
	bool m_secondExtendPoleSoundPlayed = false;

	UPROPERTY(EditAnywhere, Category = Pole, meta = (DisplayName = "BookPageSound"))
	USoundBase*  m_bookPageSound;

	UPROPERTY(EditAnywhere, Category = "Design | PoleHit", meta = (DisplayName = "Extend Push Force"))
		float m_ExtendPushForce = 150;
	UPROPERTY(EditAnywhere, Category = "Design | PoleHit", meta = (DisplayName = "Push Force"))
		float m_PushForce = 200;
	UPROPERTY(EditAnywhere, Category = "Design | PoleHit", meta = (DisplayName = "Push velocity"))
	float m_PushVelocity = 200;

	bool m_HitMontageInCooldown = false;

	UPROPERTY(EditAnywhere, Category = "Design | PoleHit", meta = (DisplayName = "HitMontageCooldownTime"))
		float m_HitMontageCooldownTime = 0.5f;

	FTimerHandle m_HitMontageCooldownHandle;

	void ResetHitMontageCooldown();
	void HitWall();
#pragma endregion

#pragma region PoleExtend
private:
	bool CanExtendPole();
	void ExtendPole();

	float m_NewPoleHeight;
	float m_CurrentPoleHeight;
	float m_DesiredExtension;

	float m_CurrentBottomPoleHeight;
	float m_DesiredBottomExtension;

	float m_BottomExtensionTimer = 0.f;
	UPROPERTY(EditAnywhere, Category = "Design | Pole | Extension", meta = (DisplayName = "BottomExtensionTime"))
	float m_BottomExtensionTime = 0.25f;

	bool m_PoleExtended = false;

	UPROPERTY(EditAnywhere, Category = "Design | Pole | Extension", meta = (DisplayName = "Extension Extra Radius"))

		float m_ExtendExtraRadius = 1.f;

	UPROPERTY(EditAnywhere, Category = Blend)
		FAlphaBlend m_PoleExtensionBlend;
	UPROPERTY(EditAnywhere, Category = "Design | Pole | Extension", meta = (DisplayName = "PoleBlendTimer"))
		float m_PoleBlendTimer = 0.5;

public:
	UPROPERTY(EditAnywhere, Category = "Design | Pole | Extension", meta = (DisplayName = "BottomPoleExtension"))
		float BottomPoleExtension = 400;

	UPROPERTY(EditAnywhere, Category = "Design | Pole | Extension", meta = (DisplayName = "TopPoleExtension"))
		float TopPoleExtension = -150;

	void ExtendPoleAnimation(double _DeltaTime);
	void ExtendBottomPole(double _DeltaTime);
	float GetPoleCurrentHeight() { return m_CurrentPoleHeight; }

#pragma endregion

#pragma endregion

#pragma region Jump
private:
	void EvaluateJump();
	void ReleasedJumpInput();
	void NormalJump();

	float m_JumpingDelay = 0.1f;
	float m_CurrentJumpingDelay = m_JumpingDelay;
#pragma endregion

#pragma region StatesPole

private:
	bool m_Extending = false;
	bool m_ExtendingBottom = false;

#pragma endregion

#pragma region PlayerProgress
	/* El progreso es el arte de caminar hacia uno mismo, descubriendo en cada paso la infinita profundidad de nuestra propia esencia */
public: 
	UPROPERTY(EditAnywhere, Category = "Design | Skills")
	bool SkillsUnlocked = true;

private:
	UPROPERTY(SaveGame)
	bool m_ExtendiblePoleSkill = false;

	UPROPERTY(SaveGame)
	bool m_LightPoleSkill = false;

public:
	bool GetExtendiblePoleSkill() { return m_ExtendiblePoleSkill; };
	bool GetLightPoleSkill() { return m_LightPoleSkill; };

	void SetExtendiblePoleSkill(bool unlocked = true) { m_ExtendiblePoleSkill = unlocked; UnlockAchievement(TEXT("PaloExt_ACH")); };
	void SetLightPoleSkill(bool unlocked = true) { m_LightPoleSkill = unlocked; UnlockAchievement(TEXT("PaloLuz_ACH"));};
#pragma endregion

#pragma region FailState
private:

	UPROPERTY(EditAnywhere, Category = "Design | FailState", meta = (DisplayName = "Max Falling Distance"))
		float m_MaxFallingDistance = 600;
	UPROPERTY(EditAnywhere, Category = "Design | FailState", meta = (DisplayName = "Fade In Duration"))
		float m_FadeDuration = 0.25;
	UPROPERTY(EditAnywhere, Category = "Design | FailState", meta = (DisplayName = "Fade Out Duration"))
		float m_DurationBetweenFades = 0.125;

	UPROPERTY(EditAnywhere, Category = "FailState", meta = (DisplayName = "Floor Detection Offset"))
		float m_FloorDetectionOffset = 100;

	UPROPERTY(EditAnywhere, Category = "Design | FailState", meta = (DisplayName = "TimeBetweenSaves"))
		float m_TimeBetweenSaves = 3.f;

	float m_FailStateSaveTimer = m_TimeBetweenSaves;

	FTimerHandle m_FailStateTimerHandle;

	void StartFadeOut(float _fadeDuration);
	
	void SetBackMovementAfterFail();

	void SafeLastValidPosition();

	bool m_Fading = false;

public:
	FTimerHandle FadeTimeHandle;
	bool FixedSafePosition = false;
	FVector LastSafePosition = FVector::ZeroVector;
	void SetBackValidPosition();
	float GetFadeInTime() const;
	void StartFadeIn(float _fadeDuration, void (ADustyCharacter::* InCallbackFunction)());
	void CheckFailState();

#pragma endregion

#pragma region UI
private:
	bool m_DustyInSettingsFromMainMenu = false;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UBookWidget> m_PlayerBookWidget;

	UPROPERTY(EditAnywhere, Category = "UI", meta = (DisplayName = "BookChildActorComponent"))
		UChildActorComponent* m_BookChildActorComponent;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UMainMenuWidget> m_MainMenuWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<USettingsWidget> m_SettingsMenuWidget;

	UPROPERTY(Transient)
		UBookWidget* m_PlayerBookInstance;
	UPROPERTY(EditAnywhere, Transient)
		ABookActorTextures* m_BookInstance;

	UPROPERTY(Transient)
		USettingsWidget* m_SettingsMenuInstance;

	void BookStartFadeOut();


	UPROPERTY(EditDefaultsOnly, Category = "Design | Credits")
	TSubclassOf<UUserWidget> CreditsWidget;

public:
	UPROPERTY(Transient)
	UMainMenuWidget* m_MainMenuInstance;
	bool m_DustyInFinalQuestion = false;
	UPROPERTY(SaveGame)
	bool FirstTimeOpeningBook = true;

	void ShowSettingsInMainMenu();
	bool GetIsInSettingsMainMenu() { return m_DustyInSettingsFromMainMenu; }
	bool GetIsInFinalQuestion() { return m_DustyInFinalQuestion; }

	UPROPERTY(EditDefaultsOnly, Category = "Design | Book")
		float BookFadeDuration = 0.2;

	void ToggleBook();
	UFUNCTION(BlueprintCallable)
	void ShowFinalQuestion();
	UFUNCTION(BlueprintCallable)
	void ShowCredits();

	UMainMenuWidget& GetMainMenuWidget() { return *m_MainMenuInstance; }
	USettingsWidget& GetSettingsMenuWidget() { return *m_SettingsMenuInstance; }
	UBookWidget& GetBookWidget() { return *m_PlayerBookInstance; }
	ABookActorTextures& GetBookActor() { return *m_BookInstance; }

	void SetResourcesToBook(UBookPageWidget* _main, UBookPageWidget* _aux, ABookRenders* _render);

	void ToggleReturnButton();

	bool GetIsDustyInFinalQuestion() { return m_DustyInFinalQuestion; }

#pragma endregion

#pragma region NewGameStart
private:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design | NewGameStart" meta = (AllowPrivateAccess = "true"))

public:
	UFUNCTION()
	void NewGameCinematicEnd();
#pragma endregion


#pragma region Cinematics
public:
	bool SkipButtonPressed = false;

	// Skip cinematic functions
	void OnSkipCinematicPressed();
	void OnSkipCinematicReleased();
#pragma endregion

#pragma region Camera
public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	void SetCameraManagerIfNotInit();

private:
	FVector m_LastCameraLocation = FVector::ZeroVector;
	FRotator m_LastCameraRotation = FRotator::ZeroRotator;


#pragma endregion

#pragma region PlayerActions
private:
	void EvaluateInteract(const FInputActionValue& Value);
	void CancelInteract(const FInputActionValue& Value);
	AInteractable* GetNearInteractables();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UActionsFilter* ActionsFilterComponent;

	/** Called for pole movement input */
		//
	void UpdatePoleMovement();
#pragma endregion


#pragma region Movement
public:
	void CanMoveWithInput(bool canMove = true);

private:
	void SetMovementInput(const FInputActionValue& Value);
	/** Called for looking input */
	void SetLookInput(const FInputActionValue& Value);
	void StartLook();
	void StopLook();
	void SetWalkSpeed();

	UPROPERTY(EditDefaultsOnly, Category = "Design | Movement", meta = (DisplayName = "DustyMaxRunSpeed"))
	float m_DustyMaxRunSpeed = 425.f;
	UPROPERTY(EditDefaultsOnly, Category = "Design | Movement", meta = (DisplayName = "DustyMaxAimSpeedRun"))
	float m_DustyMaxAimSpeedRun = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Design | Movement", meta = (DisplayName = "DustyMaxAimSpeedWalk"))
	float m_DustyMaxAimSpeedWalk = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Design | Movement", meta = (DisplayName = "DustyMaxWalkSpeed"))
	float m_DustyMaxWalkSpeed = 210.f;
	UPROPERTY(EditDefaultsOnly, Category = "Design | Movement", meta = (DisplayName = "DustyAcceleration"))
	float m_DustyAcceleration = 2.0;
	UPROPERTY(EditDefaultsOnly, Category = "Design | Movement", meta = (DisplayName = "DustyInputAcceleration"))
	float m_DustyInputAcceleration = 4.0;

	FVector2D m_currentMovementInput;
	bool m_CanMoveWithInput = true;
public:
float GetDustyMaxWalkSpeed() { return m_DustyMaxWalkSpeed; };


public:
	float DesiredPlayerSpeed = 0;
	FVector AutoMovingDirection = FVector::ZeroVector;
	float GroundCapsuleHalfHeight = 96.f;
	/** Called for movement input */
	void UpdateMovement(float _DeltaTime);
	void UpdateLookCamera();
	void SetRunSpeed();

	virtual void NotifyHit(UPrimitiveComponent* MyComp,
	AActor* Other,
	UPrimitiveComponent* OtherComp,
	bool bSelfMoved,
	FVector HitLocation,
	FVector HitNormal,
	FVector NormalImpulse,
	const FHitResult& Hit
	) override;
	
#pragma endregion

#pragma region BookControls
private:
	void ToggleReadMode(const FInputActionValue& Value);

	void Pause(const FInputActionValue& Value);
	void Pause_OpenBook();
	void Pause_CloseBook();
	void BookButtonPress(const FInputActionValue& Value);
	void BookButtonRelease();
	void BookCardinalNavigation(const FInputActionValue& Value);
	void BookChangePage(const FInputActionValue& Value);
	void BookChangeBookmark(const FInputActionValue& Value);
#pragma endregion

#pragma region Aiming
private:
	/** Called for starting aim input */
	void StartAiming();
	void TryToAim();





	bool m_ToggleCanAim = true;
	bool m_AimInCooldown = false;


public:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	UBillboardComponent* CrosshairHUD;

	void HideCrosshair();

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
		TArray<UTexture2D*> CrosshairImages;

	UPROPERTY(EditAnywhere, Category = "Design | PoleAim")
		float AimCooldown = 0.5f;
	FTimerHandle AimCooldownHandle;

	void ResetCooldown();

	bool DustyActuallyAiming = false;
	void ToggleCanAim(bool _ToggleCanAim = true);
	void ResetCanAim() { m_ToggleCanAim = true; }
	/** Called for aiming looking input */
	void InitAiming();
	void UpdateAiming(float _DeltaTime);
	void ExitAiming();
	void UpdateAimingCamera();
	/** Called for stopping aim input */
	void StopAiming();
#pragma endregion

#pragma region effects
	UFUNCTION(BlueprintNativeEvent)
	void UpdateGrassEffect(const FLinearColor& _color);

#pragma endregion

#pragma region Bench
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* SittedAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* StandUpAnimation;

#pragma endregion

#pragma region SittingRock
private:
	UFUNCTION()
	void StandUpEnd_Notify();
#pragma endregion

#pragma region HUD
public:
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UPlayerHUDWidget> PlayerHUDWidget;

	UPROPERTY(EditAnywhere, Category = "UI| Crosshair", meta = (DisplayName = "Crosshair Mesh"))
		UStaticMeshComponent* Crosshair;

	UPROPERTY(EditAnywhere, Category = "UI| Crosshair", meta = (DisplayName = "Crosshair Material No Aim"))
		UMaterialInterface* CrosshairMaterialNoAim;

	UPROPERTY(EditAnywhere, Category = "UI| Crosshair", meta = (DisplayName = "Crosshair Material Aim"))
		UMaterialInterface* CrosshairMaterialAim;

	UPlayerHUDWidget* GetHUDWidget() { return m_HUDWidgetInstance; }


private:

	// Player HUD widget instance
	UPROPERTY(Transient)
		UPlayerHUDWidget* m_HUDWidgetInstance;

#pragma endregion

#pragma region Anim
public:
		UPROPERTY(Transient)
		UDustyAnimInstance* m_DustyPlayerAnimInstance;
#pragma endregion

#pragma region MorphTargets
		public:
		//Average human time is 5 seconds
		UPROPERTY(EditDefaultsOnly, Category = "Mesh")
			float EyelidsMinTime = 3.5f;
		UPROPERTY(EditDefaultsOnly, Category = "Mesh")
			float EyelidsMaxTime = 6.5f;
		UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		float EyelidsAnimTime = 0.25f;

		private:
		float m_currentEyelidsCloseAnimTime = 0;
		float m_currentEyelidsOpenAnimTime = 0;
		float m_currentEyelidsDesiredTime = 0;
		void EyelidsBlink(float _deltaTime);
#pragma endregion

#pragma region audio
	UPROPERTY(EditAnywhere)
	TArray<USoundClass*> m_soundClasses;

	UPROPERTY(EditAnywhere)
	AAudioActor* m_bookSound;
	UPROPERTY(EditAnywhere)
	AAudioActor* m_currentPlayingSound;

	UPROPERTY(EditAnywhere, Category = "Audio", meta = (DisplayName = "Book Audio Child Actor"))
	UChildActorComponent* m_bookAudioChildActor;
	
public:
	UFUNCTION()
	void SetSoundVolume(int _audioClass, float _value);
	void SetAllSoundVolume(float _value);
	void SetCurrentSound(AAudioActor* _audio);
	AAudioActor* GetCurrentAudio();

	
	UFUNCTION(BlueprintNativeEvent)
	void ChangeAudioListener();
#pragma endregion



	
};
