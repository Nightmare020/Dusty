#include "DustyCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DustyMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Components/ChildActorComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Curves/CurveFloat.h"
#include "../DustyGameInstance.h"
#include "..\InteractiveObject\Light\Magnetism\MagneticCube.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "CharacterStates/ActionsFilter.h"
#include "Components/BillboardComponent.h"

#include "Dusty/AI/Ecosystem/Pig/PigNPC.h"

#include "../Utils/DustyUtils.h"
#include "Math/Vector.h"
#include "../InteractiveObject/Light/Magnetism/InfiniteProvider.h"

#include "Kismet/GameplayStatics.h"
#include "../InteractiveObject/ToggleActor.h"
#include "../InteractiveObject/Light/LightActor.h"
#include "../Movement/PoleJumpConstraintActor.h"
#include "../InteractiveObject/Hieroglyphics/GlyphColumn.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "..\InteractiveObject\Key\KeyController.h"
#include "../InteractiveObject/Interactable/Bench.h"
#include "../InteractiveObject/Interactable/PoleElevator.h"
#include "../Camera/DustyCameraActor.h"
#include "Components/PointLightComponent.h"
#include "../Cinematics/CinematicsSubsystem.h"
#include "Dusty/InteractiveObject/Interactable/SittingRock.h"

// UI
#include "Blueprint/UserWidget.h"
#include "../UI/Book/Book2D/BookWidget.h"	
#include "../UI/Book/Book3D/BookActorTextures.h"
#include "../UI/MainMenu/MainMenuWidget.h"
#include "../UI/HUD/HUDSubsystem.h"
#include "../UI/HUD/PlayerHUDWidget.h"
#include "../UI/MainMenu/SettingsWidget.h"
#include "../Settings/SettingsSubsystem.h"
#include "Dusty/UI/HUD/DustyHUD.h"

#include "Dusty/Camera/DustyCameraManager.h"
#include "Dusty/Camera/InGameCamera/DynamicCameras/CameraOverrider.h"

#include "../Animations/DustyAnimInstance.h"
#include "Dusty/Animations/StandUpEndNotify.h"
#include "Dusty/Audio/AudioActor.h"
#include "Dusty/Audio/AudioSubsytem.h"
#include "Sound/SoundClass.h"
DEFINE_LOG_CATEGORY(DustyCharacterLog);

// Sets default values
ADustyCharacter::ADustyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UDustyMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create Dusty Character Movement Component
	DustyMovementComponent = Cast<UDustyMovementComponent>(GetCharacterMovement());

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Crosshair = CreateDefaultSubobject<UStaticMeshComponent>("Crosshair");
	Crosshair->SetupAttachment(GetRootComponent());

	// Create Action Filter Component
	ActionsFilterComponent = CreateDefaultSubobject<UActionsFilter>(TEXT("ActionsFilter"));

	m_BookTargetArrow = CreateDefaultSubobject<UArrowComponent>("BookTargetArrow");
	m_BookTargetArrow->SetupAttachment(RootComponent);
	//Create 3D book
	m_BookChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Custom camera"));
	m_BookChildActorComponent->SetupAttachment(m_BookTargetArrow); //CHANGE top root
	//m_BookChildActorComponent->SetupAttachment(RootComponent);
	m_BookChildActorComponent->SetChildActorClass(ABookActorTextures::StaticClass());

	PoleSkeletal = CreateDefaultSubobject<UPoseableMeshComponent>("PoleSkeletal");
	PoleSkeletal->SetupAttachment(RootComponent);

	capsulePole = CreateDefaultSubobject<UCapsuleComponent>("CapsulePole");
	capsulePole->SetupAttachment(RootComponent);

	//Create Pole Light
	PolePointLight = CreateDefaultSubobject<UPointLightComponent>("PoleLight");
	PolePointLight->SetupAttachment(PoleSkeletal);


	m_bookAudioChildActor = CreateDefaultSubobject<UChildActorComponent>("Book audio");
	m_bookAudioChildActor->SetupAttachment(m_BookChildActorComponent);

	CrosshairHUD = CreateDefaultSubobject<UBillboardComponent>("Crosshair HUD");
	CrosshairHUD->SetupAttachment(RootComponent);
}


void ADustyCharacter::BeginPlay()
{
	Super::BeginPlay();

// so dusty doesnt fall till levels are loaded
	DustyMovementComponent->Deactivate();
	
#if WITH_EDITOR
	m_ExtendiblePoleSkill = SkillsUnlocked;
	m_LightPoleSkill = SkillsUnlocked;
#endif

	SetDustyTattooValue(NoTattooValue, false);
	m_CurrentTattooValue = NoTattooValue;
	m_TargetTattooValue = NoTattooValue;

	SetTickableWhenPaused(true);
	GroundCapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	Crosshair->SetVisibility(false);
	Crosshair->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_BookChildActorComponent->RegisterComponent();
	if (m_BookInstance == nullptr)
	{
		m_BookInstance = Cast<ABookActorTextures>(m_BookChildActorComponent->GetChildActor());
	}

	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (m_SettingsMenuWidget)
	{
		m_SettingsMenuInstance = CreateWidget<USettingsWidget>(GetWorld(), m_SettingsMenuWidget);
		m_SettingsMenuInstance->AddToViewport();
		m_SettingsMenuInstance->CloseSettingsMenu();
	}

	// Setup Player HUD Widget
	if (PlayerHUDWidget)
	{
		m_HUDWidgetInstance = CreateWidget<UPlayerHUDWidget>(GetWorld(), PlayerHUDWidget);
		m_HUDWidgetInstance->AddToViewport();
	}

	PoleSkeletal->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, m_PoleSocketName);


	m_PoleExtensionBlend.SetBlendTime(m_PoleBlendTimer);
	if (UCurveFloat* PoleExtension = Cast<UDustyGameInstance>(GetGameInstance())->AnimationCurves->PoleExtension)
	{
		m_PoleExtensionBlend.SetCustomCurve(PoleExtension);
		m_PoleExtensionBlend.SetBlendOption(EAlphaBlendOption::Custom);
		m_PoleExtensionBlend.Reset();
	}
	else
	{
		UE_LOG(DustyCharacterLog, Error, TEXT("ERROR in DustyCharacter.cpp: Curves Animation for PoleExtension was not set! Please check AnimationCurvesSubsystem and AnimationCurves Data Asset"));
	}

	PoleSkeletal->SetBoneLocationByName(TopPoleBoneName, FVector::UpVector * InitialTopPolePosition, EBoneSpaces::ComponentSpace);
	PoleSkeletal->SetBoneLocationByName(BottomPoleBoneName, FVector::UpVector * InitialBottomPolePosition, EBoneSpaces::ComponentSpace);
	m_CurrentPoleHeight = PoleSkeletal->GetBoneLocationByName(TopPoleBoneName, EBoneSpaces::ComponentSpace).Z;

	PoleSkeletal->SetBoneLocationByName(BallPoleBoneName, m_StaticBallPosition, EBoneSpaces::ComponentSpace);

	if (m_MainMenuWidget && !GetGameInstance()->GetSubsystem<UCinematicsSubsystem>()->IsCinematicPlaying)
	{
		m_MainMenuInstance = CreateWidget<UMainMenuWidget>(GetWorld(), m_MainMenuWidget);
		m_MainMenuInstance->AddToViewport();

#if !WITH_EDITOR
		m_MainMenuInstance->SetIsInMainMenu(true);
#else
		m_MainMenuInstance->SetIsInMainMenu(false);

#endif // !WITH_EDITOR
		m_MainMenuInstance->SetIsInMainMenu(true);

		if (m_MainMenuInstance->GetIsInMainMenu())
		{
			AddInputMappingContext(m_DefaultMappingContext, 0);
			m_MainMenuInstance->OpenMainMenu();
		}
		else
		{
			m_MainMenuInstance->CloseMainMenu();

			

			AddInputMappingContext(m_DefaultMappingContext, 0);

		}

		FTimerHandle AAHandle;
		GetWorld()->GetTimerManager().SetTimer(AAHandle, this, &ADustyCharacter::AAAAAAAAAAAlEVELS, 2.f, false);
		InitializeCharacter();
	}

	PoleDynamicMaterial = UMaterialInstanceDynamic::Create(PoleSkeletal->GetMaterial(0), this);
	PoleDynamicMaterial->SetScalarParameterValue(FName("position"), 1);
	PoleSkeletal->SetMaterial(0, PoleDynamicMaterial);

	if (m_PoleLighted)
	{
		GetPoleMaterial()->SetScalarParameterValue(FName("position"), 0);
	}
	else
	{
		GetPoleMaterial()->SetScalarParameterValue(FName("position"), 1);
	}

	m_polePointLightShouldBeActive = false;
	SetPolePointLightVisibility(false);

	if (!m_DustyPlayerAnimInstance)
	{
		m_DustyPlayerAnimInstance = Cast<UDustyAnimInstance>(GetMesh()->GetAnimInstance());
	}
	
	// APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// playerController->SetAudioListenerOverride(RootComponent, GetActorLocation(), GetActorRotation());


	//SetSoundVolume(AudioClasses::Ambient, 1.0f);
}

void ADustyCharacter::ShowSettingsInMainMenu()
{
	m_DustyInSettingsFromMainMenu = true;
	UBookState* BookState = Cast<UBookState>(*ActionsFilterComponent->m_statesInstancesMap.Find(UBookState::StaticClass()));
	BookState->OriginState = UMainMenuState::StaticClass();
	ActionsFilterComponent->SetCurrentState(UBookState::StaticClass());
}

void ADustyCharacter::ToggleBook()
{
    SetInput(true); // enable again input bcz fade finished
	if (m_BookInstance)
	{
		if (m_BookInstance->GetIsOpen()) // book was open
		{
			if (CameraManager->IsOverrided()) // book is not on wait anymore
			{
				CameraManager->SetCurrentOverriderOnWaitValue(false);

				// set later location and rotation for camera actor
				CameraManager->CameraActor->SetActorLocation(m_LastCameraLocation);
				CameraManager->CameraActor->SetActorRotation(m_LastCameraRotation);
			}
			else // only go back to players camera if it wasn't overrided
			{
				CameraManager->GetOwningPlayerController()->SetViewTargetWithBlend(CameraManager->ControlledCharacter, 0.f, VTBlend_Linear);
			}
		}
		else // book was closed
		{
			// if overrided put on wait
			if (CameraManager->IsOverrided())
			{
				// store current camera location for toggle back in close book
				m_LastCameraLocation = CameraManager->CameraActor->GetActorLocation();
				m_LastCameraRotation = CameraManager->CameraActor->GetActorRotation();
			
				CameraManager->SetCurrentOverriderOnWaitValue(true);
			}
			else // only do set view if i wasn't already in camera actor
			{
				CameraManager->GetOwningPlayerController()->SetViewTargetWithBlend(CameraManager->CameraActor, 0.f, VTBlend_Linear);
			}
		
			// set camera new location to books location
			CameraManager->CameraActor->SetActorLocation(m_BookTargetArrow->GetComponentLocation());
			CameraManager->CameraActor->SetActorRotation(m_BookTargetArrow->GetComponentRotation());
		}

		bool open = m_BookInstance->ToggleBook();

		if (open)
		{
			//DustyMovementComponent->StopMovement();
			CameraManager->SetCameraActorBlur(true);
			GetMesh()->SetVisibility(false);
			PoleSkeletal->SetVisibility(false);
			if (!m_DustyInSettingsFromMainMenu && !m_DustyInFinalQuestion)
			{
				if (FirstTimeOpeningBook)
				{
					DustyUtils::ShowHud(GetWorld(), EHudCanvas::InitialBookButtons);
				}				 
				else
				{
					DustyUtils::ShowHud(GetWorld(), EHudCanvas::BookButtons);
				}
			}
			else if(m_DustyInSettingsFromMainMenu)
			{
				DustyUtils::ShowHud(GetWorld(), EHudCanvas::MainMenuSettingsButtons);
			}
		}
		else
		{
			CameraManager->SetCameraActorBlur(false);
			if (m_DustyInSettingsFromMainMenu)
			{
				DustyUtils::HideHud(GetWorld(), EHudCanvas::MainMenuSettingsButtons); m_MainMenuInstance->SetVisibility(ESlateVisibility::Visible);
				m_DustyInSettingsFromMainMenu = false;

				ActionsFilterComponent->SetCurrentState(UMainMenuState::StaticClass());
				m_MainMenuInstance->SetFocusToSettingsButton();

				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				PlayerController->SetInputMode(FInputModeGameAndUI());
				PlayerController->bShowMouseCursor = true;
			}
			else if (FirstTimeOpeningBook)
			{
				DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::InitialBookButtons);
				FirstTimeOpeningBook = false;
			}


			DustyMovementComponent->SetMovementMode(MOVE_Walking);
			GetMesh()->SetVisibility(true);
			PoleSkeletal->SetVisibility(true);
			DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::BookButtons);
			DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::ReadMode);
			DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::ReadModeButton);

			m_BookInstance->EnableReadMode(false);
		}

		
		BookStartFadeOut();
	}
}

void ADustyCharacter::SetResourcesToBook(UBookPageWidget* _main, UBookPageWidget* _aux, ABookRenders* _render)
{
	if (m_BookInstance == nullptr)
	{
		m_BookInstance = Cast<ABookActorTextures>(m_BookChildActorComponent->GetChildActor());
	}

	m_BookInstance->SetBookResources(_main, _aux, _render);
}

void ADustyCharacter::ToggleReturnButton()
{
	if (m_BookInstance->GetIsOpen())
	{
		FInputActionValue emptyInput;
		Pause(emptyInput);
		//m_BookInstance->GoBack();
	}
	else if (m_MainMenuInstance->GetIsInMainMenu())
	{
		//Go Back in main menu
	}
}

void ADustyCharacter::SetCameraManagerIfNotInit()
{
	// setup camera
	CameraManager = Cast<ADustyCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
}

void ADustyCharacter::InitializeCharacter()
{
	// setup camera
	if (!CameraManager)
	{
		CameraManager = Cast<ADustyCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
	}

	UDustyGameInstance* DustyGameInstance = GetGameInstance<UDustyGameInstance>();
	if (DustyGameInstance)
	{
	#if WITH_EDITOR
			if (SpawnOnSavedData)
			{
				DustyGameInstance->LoadPlayerData(this);
			}
	#else
			DustyGameInstance->LoadPlayerData(this);
	#endif
	}

	// Setup Player Book Widget
	if (m_PlayerBookWidget)
	{
		m_PlayerBookInstance = CreateWidget<UBookWidget>(GetWorld(), m_PlayerBookWidget);
		m_PlayerBookInstance->AddToViewport();
		m_PlayerBookInstance->CloseBook();
	}

	m_currentEyelidsDesiredTime = FMath::FRandRange(EyelidsMinTime, EyelidsMaxTime);


	const TArray<FAnimNotifyEvent> closeBookNotifies = StandUpAnimation->Notifies;

	for (FAnimNotifyEvent eventNotify : closeBookNotifies)
	{
		if (UStandUpEndNotify* CloseBookNotify = Cast<UStandUpEndNotify>(eventNotify.Notify))
		{
			CloseBookNotify->OnNotified.BindUObject(this, &ADustyCharacter::StandUpEnd_Notify);
		}
	}

	ChangeAudioListener();
}

void ADustyCharacter::AAAAAAAAAAAlEVELS()
{
	//In case not all levels dusty had loaded are currently loaded after start
	GetGameInstance<UDustyGameInstance>()->LoadLevels();
}

// Called every frame
void ADustyCharacter::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);

	ULocalPlayer* LocPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetLocalPlayer();
	DustyUtils::PrintMessage(ActionsFilterComponent->GetCurrentBaseState()->GetName(), FColor::Yellow, DeltaTime);
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{	
		//if (m_BookInstance && !m_BookInstance->GetIsOpen() || !m_BookInstance)//Revisit this after Alpha
		{
			if(LocPlayer->ViewportClient->Viewport && LocPlayer->ViewportClient->Viewport->IsForegroundWindow())
			{
				UGameplayStatics::SetGamePaused(GetWorld(), false);
			}
		}
	}
	else
	{
		if (!LocPlayer->ViewportClient->Viewport || !LocPlayer->ViewportClient->Viewport->IsForegroundWindow())
		{
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			return;
			// viewport is either not present or not in the foreground.
		}
		

		TryToAim();


		FHitResult OutHit;
		GetCharacterMovement()->SafeMoveUpdatedComponent(FVector(0.f, 0.f, 0.01f), GetActorRotation(), true, OutHit);
		GetCharacterMovement()->SafeMoveUpdatedComponent(FVector(0.f, 0.f, -0.01f), GetActorRotation(), true, OutHit);
		FVector CapsuleLoc = PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace) + PoleSkeletal->GetBoneLocationByName(BottomPoleBoneName, EBoneSpaces::WorldSpace);
		CapsuleLoc /= 2;

		FVector CapsuleScale = PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace) - PoleSkeletal->GetBoneLocationByName(BottomPoleBoneName, EBoneSpaces::WorldSpace);
		FTransform SkeletalMeshTransform = PoleSkeletal->GetComponentTransform();

		// Set the transform of the CapsuleComponent
		capsulePole->SetWorldLocation(CapsuleLoc);
		capsulePole->SetWorldRotation(SkeletalMeshTransform.GetRotation());
		capsulePole->SetCapsuleHalfHeight(CapsuleScale.Size() * 0.5f);

		if (m_Extending)
		{
			ExtendPoleAnimation(DeltaTime);
		}

		if (m_ExtendingBottom)
		{
			ExtendBottomPole(DeltaTime);
		}

		if (m_PoleLighted && m_BallPlaced)
		{
			BallMovement(DeltaTime);
		}

		if (m_BallMoving)
		{
			PlaceBallMovement(DeltaTime);
		}

		EyelidsBlink(DeltaTime);
	}

	if (CameraManager->IsShakeModifierEnabled() && ActionsFilterComponent->GetCurrentBaseStateClass() == UBookState::StaticClass())
	{
		Pause_CloseBook();
	}
	
	DustyMovementComponent->MaxWalkSpeed = FMath::Lerp(DustyMovementComponent->MaxWalkSpeed, DesiredPlayerSpeed, m_DustyAcceleration * DeltaTime);

	const TSubclassOf<UBaseState> CurrentStateClass = ActionsFilterComponent->GetCurrentBaseStateClass();
	const float CurrentVelocity = DustyMovementComponent->GetLastUpdateVelocity().Z;

	if (CurrentVelocity < 0 && m_CanMoveWithInput && ActionsFilterComponent->GetCurrentBaseStateClass() != UPoleJumpingState::StaticClass())
	{
		ActionsFilterComponent->SetCurrentState(UFallingState::StaticClass());
	}
	else if ((CurrentStateClass == UFallingState::StaticClass() || CurrentStateClass == UJumpingState::StaticClass()) && CurrentVelocity == 0)
	{
		if (m_CurrentJumpingDelay <= 0.f)
		{
			StopJumping();
			ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
			m_CurrentJumpingDelay = m_JumpingDelay;
		}
		else
		{
			m_CurrentJumpingDelay -= DeltaTime;
		}
	}

	// tattoo control
	if(m_CurrentTattooValue != m_TargetTattooValue)
	{
		float const TattooDistance = abs(m_TargetTattooValue-m_CurrentTattooValue);
		float const FAlpha = FMath::Min(DeltaTime * TattooBlendSpeed / TattooDistance, 1.0f);
		m_CurrentTattooValue = FMath::Lerp(m_CurrentTattooValue, m_TargetTattooValue, FAlpha);
		SetDustyTattooValue(m_CurrentTattooValue, false);
	}
}

void ADustyCharacter::SetMainMenuAfterCredtis()
{
	if(m_MainMenuInstance)
	{
		UDustySaveGame* SaveGameInstance = Cast<UDustySaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_SLOT_NAME, 0));
		m_DustyInFinalQuestion = false;
		if (SaveGameInstance)
		{
			SetActorTransform(SaveGameInstance->PlayerSavedData.PlayerTransform);
			SetStartCamera(SaveGameInstance->PlayerSavedData.CameraTransform, SaveGameInstance->PlayerSavedData.CameraTransitionTime);
		}
		SetActorHiddenInGame(false);
		m_MainMenuInstance->OpenMainMenu();
		ActionsFilterComponent->SetCurrentState(UMainMenuState::StaticClass());
	}
}

UMaterialInstanceDynamic* ADustyCharacter::SetDustyDynamicMaterial(UMaterialInterface* DustyMaterial)
{
	m_DustyDynamicMaterial = UMaterialInstanceDynamic::Create(DustyMaterial, this);
	return m_DustyDynamicMaterial;
}

void ADustyCharacter::SetDustyTattooValue(float NewValue, bool IsNew)
{
	if (IsNew)
	{
		m_TargetTattooValue = NewValue;
	}
	else
	{
		m_DustyDynamicMaterial->SetScalarParameterValue(TattooParam, NewValue);
	}
}

void ADustyCharacter::SetDustyLocation(int LocationIndex)
{
	if (LocationIndex >= m_SetDustyLocations.Num())
	{
		return;
	}
	SetActorLocation(m_SetDustyLocations[LocationIndex]);
}

void ADustyCharacter::UnlockSkills(bool Unlocked)
{
	m_ExtendiblePoleSkill = Unlocked;
	m_LightPoleSkill = Unlocked;
}

void ADustyCharacter::TriggerFailState()
{
	StartFadeIn(GetFadeInTime(), &ADustyCharacter::SetBackValidPosition);
}

void ADustyCharacter::LoadAllLevels()
{
	TArray<ULevelStreaming*> AllLevels = GetWorld()->GetStreamingLevels();
	for (ULevelStreaming* lvl : AllLevels) {
		if (lvl->IsLevelLoaded())
		{
			continue;
		}

		FString f = lvl->GetWorldAssetPackageName();
		// f is like "/Game/Maps/Notch_Peak_Master", or in the editor "/Game/Maps/UEDPIE_0_Notch_Peak_Master"
		// Chop at last slash in path if any.
		int i;
		if (f.FindLastChar('/', i)) {
			f = f.RightChop(i + 1);
		}
		// Now f is like "UEDPIE_0_Notch_Peak_Master" or "Notch_Peak_Master"
		// In the editor?
		if (f.StartsWith(TEXT("UEDPIE"))) {
			f.RemoveFromStart(TEXT("UEDPIE"));
			// Like "_2_" being the local simulated player number.
			if (f.RemoveFromStart(TEXT("_")) && f.Len() > 2 && f[1] == '_') {
				f.RemoveFromStart(TEXT("0"));
				f.RemoveFromStart(TEXT("1"));
				f.RemoveFromStart(TEXT("2"));
				f.RemoveFromStart(TEXT("3"));
				f.RemoveFromStart(TEXT("4"));
				f.RemoveFromStart(TEXT("5"));
				f.RemoveFromStart(TEXT("6"));
				f.RemoveFromStart(TEXT("7"));
				f.RemoveFromStart(TEXT("8"));
				f.RemoveFromStart(TEXT("_"));
			}
		}

		if (f.StartsWith(TEXT("LV")))
		{
			continue;
		}
		

		PlaceholderLevelsToLoad.Add(FName(f));


	}

	PlaceholderLoadLevels();
}

void ADustyCharacter::LoadLevelsOfArea(int index)
{
	switch (index)
	{
		case 1:
			PlaceholderLevelsToLoad = Area1Levels;
			break;
		case 2:
			PlaceholderLevelsToLoad = Area2Levels;
			break;
		case 3:
			PlaceholderLevelsToLoad = Area3Levels;
			break;
		case 4:
			PlaceholderLevelsToLoad = Area4Levels;
			break;
		case 5:
			PlaceholderLevelsToLoad = Area5Levels;
			break;
		case 6:
			PlaceholderLevelsToLoad = Area6Levels;
			break;
	}

	PlaceholderLoadLevels();
}

void ADustyCharacter::SetDustySpeed(float NewSpeed)
{
	m_DustyMaxRunSpeed = NewSpeed;
}

void ADustyCharacter::UnlockAllPages()
{
	m_BookInstance->LB_UnlockAllPages();
}

void ADustyCharacter::UnlockAllCollectibles()
{
	m_BookInstance->LB_UnlockAllCollectibles();
}

void ADustyCharacter::PlaceholderLoadLevels()
{
	if (PlaceholderLevelsToLoad.IsEmpty()) return;
	FName LevelName;
	LevelName = PlaceholderLevelsToLoad.Pop();

	DustyUtils::PrintMessage(LevelName.ToString());
	FLatentActionInfo LoadLatentInfo;
	ULevelStreaming* Level = UGameplayStatics::GetStreamingLevel(this, LevelName);

	FTimerHandle ExitStartCamera;
	GetWorld()->GetTimerManager().SetTimer(ExitStartCamera, this, &ADustyCharacter::PlaceholderLoadLevels, 0.25, false);
	UGameplayStatics::LoadStreamLevel(this, LevelName, true, false, LoadLatentInfo);
	
}

void ADustyCharacter::AddInputMappingContext(UInputMappingContext* ContextToAdd, int32 InPriority)
{
	if (!ContextToAdd) return;

	// Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ContextToAdd, InPriority);
		}
	}
}

void ADustyCharacter::RemoveInputMappingContext(UInputMappingContext* ContextToRemove)
{
	if (!ContextToRemove) return;

	// Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(ContextToRemove);
		}
	}
}


void ADustyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Started, this, &ADustyCharacter::EvaluateJump);
		EnhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Completed, this, &ADustyCharacter::StopJumping);


		// Moving
		EnhancedInputComponent->BindAction(m_MoveAction, ETriggerEvent::Triggered, this, &ADustyCharacter::SetMovementInput);
		EnhancedInputComponent->BindAction(m_MoveAction, ETriggerEvent::Completed, this, &ADustyCharacter::SetMovementInput);

		//Walking
		EnhancedInputComponent->BindAction(m_ShiftAction, ETriggerEvent::Triggered, this, &ADustyCharacter::SetWalkSpeed);
		EnhancedInputComponent->BindAction(m_ShiftAction, ETriggerEvent::Completed, this, &ADustyCharacter::SetRunSpeed);


		// Looking
		EnhancedInputComponent->BindAction(m_LookAction, ETriggerEvent::Triggered, this, &ADustyCharacter::SetLookInput);
		EnhancedInputComponent->BindAction(m_LookAction, ETriggerEvent::Completed, this, &ADustyCharacter::SetLookInput);
		EnhancedInputComponent->BindAction(m_LookAction, ETriggerEvent::Started, this, &ADustyCharacter::StartLook);
		EnhancedInputComponent->BindAction(m_LookAction, ETriggerEvent::Completed, this, &ADustyCharacter::StopLook);

		// Aiming
		EnhancedInputComponent->BindAction(m_AimAction, ETriggerEvent::Started, this, &ADustyCharacter::StartAiming);
		EnhancedInputComponent->BindAction(m_AimAction, ETriggerEvent::Completed, this, &ADustyCharacter::StopAiming);

		// Extend and Retract Pole
		EnhancedInputComponent->BindAction(m_ExtendPoleAction, ETriggerEvent::Started, this, &ADustyCharacter::ExtendPole);

		// Interact
		EnhancedInputComponent->BindAction(m_InteractAction, ETriggerEvent::Started, this, &ADustyCharacter::EvaluateInteract);

		// Cancel interact
		EnhancedInputComponent->BindAction(m_SkipCinematicAction, ETriggerEvent::Started, this, &ADustyCharacter::CancelInteract);
		
		// Pause
		EnhancedInputComponent->BindAction(m_PauseAction, ETriggerEvent::Started, this, &ADustyCharacter::Pause);

		//Book Click - Interaction with buttons
		EnhancedInputComponent->BindAction(m_BookClickAction, ETriggerEvent::Started, this, &ADustyCharacter::BookButtonPress);
		EnhancedInputComponent->BindAction(m_BookClickAction, ETriggerEvent::Completed, this, &ADustyCharacter::BookButtonRelease);
		//EnhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Completed, this, &ADustyCharacter::BookButtonRelease);

		//Book Navigation
		EnhancedInputComponent->BindAction(m_BookCardinalNavigationAction, ETriggerEvent::Started, this, &ADustyCharacter::BookCardinalNavigation);

		//Book Navigation
		EnhancedInputComponent->BindAction(m_BookChangePageAction, ETriggerEvent::Started, this, &ADustyCharacter::BookChangePage);
		EnhancedInputComponent->BindAction(m_BookChangeBookmarkAction, ETriggerEvent::Started, this, &ADustyCharacter::BookChangeBookmark);


		// Skip Cinematic
		EnhancedInputComponent->BindAction(m_SkipCinematicAction, ETriggerEvent::Started, this, &ADustyCharacter::OnSkipCinematicPressed);
		EnhancedInputComponent->BindAction(m_SkipCinematicAction, ETriggerEvent::Completed, this, &ADustyCharacter::OnSkipCinematicReleased);


		EnhancedInputComponent->BindAction(m_PoleLightAction, ETriggerEvent::Started, this, &ADustyCharacter::PoleInteract);

		// Get back to previous screen given
		EnhancedInputComponent->BindAction(m_GetBackAction, ETriggerEvent::Started, this, &ADustyCharacter::ToggleReturnButton);

		// Read Mode Book
		EnhancedInputComponent->BindAction(m_ReadModeAction, ETriggerEvent::Started, this, &ADustyCharacter::ToggleReadMode);
	}
}

FCollisionQueryParams ADustyCharacter::GetIgnoreCharacterCollisionParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);

	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}

void ADustyCharacter::OnActorLoaded()
{
	TogglePoleLight(m_PoleLighted);

}

void ADustyCharacter::SetStartCamera(const FTransform& _CameraTransform, float _CameraTransitionTime)
{
	if (CameraManager)
	{
		DustyUtils::PrintMessage("There is a camera manager");
		CameraManager->StartCameraInit(_CameraTransform, _CameraTransitionTime);
	}
	else
	{
		DustyUtils::PrintMessage("There is not a camera manager, we cast a new one");
		CameraManager = Cast<ADustyCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));
		SetStartCamera(_CameraTransform, _CameraTransitionTime);
	}
}

void ADustyCharacter::ExitStartCamera()
{
	if (CameraManager)
	{
		CameraManager->StartCameraExit();
	}
}


void ADustyCharacter::StartAiming()
{
	if (!CameraManager->IsOverrided())
	{
		AimInput = true;
	}
}

void ADustyCharacter::TryToAim()
{
	if (m_ToggleCanAim)
	{
		//This entrance is blocked by a cooldown so the animation doesn't get crazy
		if (AimInput && ActionsFilterComponent->GetCurrentBaseStateClass() == UGroundMovementState::StaticClass() && !m_AimInCooldown)
		{
			ActionsFilterComponent->SetCurrentState(UAimingState::StaticClass()); 
			m_AimInCooldown = true;
		}
		//This exit is blocked by the animation so it always ends
		else if(!AimInput && ActionsFilterComponent->GetCurrentBaseStateClass() == UAimingState::StaticClass())
		{
			ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
		}

	}
}

void ADustyCharacter::HideCrosshair()
{
	CrosshairHUD->SetHiddenInGame(true);
}

void ADustyCharacter::ResetCooldown()
{
	m_AimInCooldown = false;
}

void ADustyCharacter::StopAiming()
{
	if (!CameraManager->IsOverrided())
	{
		AimInput = false;
	}
}

void ADustyCharacter::StandUpEnd_Notify()
{
	ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
}

void ADustyCharacter::UpdateGrassEffect_Implementation(const FLinearColor& _color)
{
}

void ADustyCharacter::ChangeAudioListener_Implementation()
{
	
}



void ADustyCharacter::EyelidsBlink(float _deltaTime)
{
	//time between blinking
	if (m_currentEyelidsDesiredTime > 0)
	{
		m_currentEyelidsDesiredTime -= _deltaTime;

		if (m_currentEyelidsDesiredTime <= 0)
		{
			m_currentEyelidsCloseAnimTime = 0;
			m_currentEyelidsOpenAnimTime = EyelidsAnimTime;
		}
	}
	//blink
	else 
	{		
		//closing eyes - 1/2 of blink
		if (m_currentEyelidsCloseAnimTime < EyelidsAnimTime)
		{
			m_currentEyelidsCloseAnimTime += _deltaTime;
			m_DustyPlayerAnimInstance->SetMorphTarget(TEXT("R_Blink_eye"), m_currentEyelidsCloseAnimTime/ EyelidsAnimTime);
		}
		//opening eyes - 2/2 of blink
		else if (m_currentEyelidsOpenAnimTime > 0)
		{
			m_currentEyelidsOpenAnimTime -= _deltaTime;
			m_DustyPlayerAnimInstance->SetMorphTarget(TEXT("R_Blink_eye"), m_currentEyelidsOpenAnimTime/ EyelidsAnimTime);

			//finished blinking
			if (m_currentEyelidsOpenAnimTime <= 0)
			{
				m_currentEyelidsDesiredTime = FMath::FRandRange(EyelidsMinTime, EyelidsMaxTime);
			}
		}
	}
}

void ADustyCharacter::SetSoundVolume(int _audioClass, float _value)
{
	if(m_soundClasses.Num() > 0)
	{
		if (_value < 0.1f)
		{
			_value = 0.001f;
		}
		if(_audioClass == AudioClasses::Music)
		{
			m_soundClasses[0]->Properties.Volume = _value;
		}
		else if(_audioClass == AudioClasses::Effects)
		{
			m_soundClasses[1]->Properties.Volume = _value;
		}
		
	}
	
}

void ADustyCharacter::SetAllSoundVolume(float _value)
{
	for(USoundClass* soundClass : m_soundClasses)
	{
		soundClass->Properties.Volume = _value;
	}
}

void ADustyCharacter::SetCurrentSound(AAudioActor* _audio)
{
	m_currentPlayingSound = _audio;
}

AAudioActor* ADustyCharacter::GetCurrentAudio()
{
	return m_currentPlayingSound;
}

void ADustyCharacter::EndInitialJumpInPoleJump()
{		

	FVector JumpForwardDirection;
	FVector JumpRightDirection;
	if (JumpTowardsCamera)
	{
		JumpForwardDirection = FollowCamera->GetForwardVector();
		JumpForwardDirection.Z = 0;
		JumpRightDirection = FollowCamera->GetRightVector();
		JumpRightDirection.Z = 0;
	}
	else
	{
		JumpForwardDirection = GetActorForwardVector();
		JumpRightDirection = GetActorRightVector();

	}



	//The final direction
	FVector DirectionPoint = m_PolePoint - (GetActorLocation() + GetActorForwardVector() * m_HandsOffset.X + GetActorUpVector() * m_HandsOffset.Z);
	float DesiredPoleHeight = DirectionPoint.Size();

	//The direction we apply the impulse
	FVector PoleJumpDirection = FVector::CrossProduct(DirectionPoint.GetSafeNormal(), JumpRightDirection);
	PoleJumpDirection.Normalize();

	float InitialSpeed = FMath::Sqrt(2 * FMath::Abs(DustyMovementComponent->GetGravityZ()) * DesiredPoleHeight);
	
	FVector InitialVelocity = InitialSpeed * PoleJumpDirection * m_JumpVelocityFactor;

	// We set the values for the incoming jump
	PoleJumpValuesStruct* PoleJumpValues = new PoleJumpValuesStruct(m_PolePoint, InitialVelocity, DirectionPoint, m_TimeToExtendInPoleJump, m_MaxPoleJumpAngleBeforeRetract, m_PoleJumpMovementFactor, DirectionPoint.Size() / m_BasePoleJumpDistance, false, ((GetBottomPoleExtension() + GetPoleBaseHeight()) / 100));

	SpawnPoleJumpConstraint(PoleJumpValues);


}


void ADustyCharacter::StartExtendingBottomPole()
{
	m_CurrentBottomPoleHeight = InitialBottomPolePosition;
	const float DesiredScale = ((m_PolePoint - (GetActorLocation() + GetActorForwardVector() * m_HandsOffset.X + GetActorUpVector() * m_HandsOffset.Z)).Size() / 100);
	
	m_DesiredBottomExtension = (BottomPoleExtension + InitialBottomPolePosition) * (DesiredScale - 2) / 5;
	m_ExtendingBottom = true;
	m_Tilt = false;

	m_CurrentRotation = GetActorRotation();
	m_DesiredRotation = m_CurrentRotation;
	m_DesiredRotation.Pitch += m_TiltAngle;
	

}

void ADustyCharacter::StartRetractingBottomPole()
{
	m_CurrentBottomPoleHeight = PoleSkeletal->GetBoneLocationByName(BottomPoleBoneName, EBoneSpaces::ComponentSpace).Z;
	m_DesiredBottomExtension = InitialBottomPolePosition;
	m_ExtendingBottom = true;
	m_Tilt = true;


	m_CurrentRotation = GetActorRotation();
	m_DesiredRotation = m_CurrentRotation;
	m_DesiredRotation.Pitch = 0;
}


void ADustyCharacter::ResetHitMontageCooldown()
{
	m_HitMontageInCooldown = false;
	CameraManager->DisableShakeModifier();
	CanMoveWithInput(true);
}

void ADustyCharacter::HitWall()
{
	if (!CanPlayHitMontage)
	{
		return;
	}

	PlayAnimMontage(m_PoleHitMontage);
	//Lo pondria para que lo parametrice dise�o, pero no lo van a hacer
	GetCharacterMovement()->AddImpulse(GetActorForwardVector() * -550 + GetActorUpVector() * 80, true);
	CameraManager->StartCameraShake(m_PoleHitCameraShake, 1);
	CameraManager->EnableShakeModifier();
	GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_PoleHitSound);
	m_HitMontageInCooldown = true;
	CanMoveWithInput(false);
	GetWorld()->GetTimerManager().SetTimer(m_HitMontageCooldownHandle, this, &ADustyCharacter::ResetHitMontageCooldown, m_HitMontageCooldownTime, false);
}

void ADustyCharacter::ExtendPole()
{
	if (!GetExtendiblePoleSkill()) //Unlocked skill?
	{
		return;
	}

	if (ActionsFilterComponent->GetCurrentBaseStateClass() == UAimingState::StaticClass())
	{
		if(m_extendPoleSound && !m_extendPoleSoundPlayed && m_retractPoleSound)
		{
			if(m_PoleExtended)
			{
				GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_retractPoleSound, GetActorLocation());
				m_extendPoleSoundPlayed = true;
			}

			else
			{
				GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_extendPoleSound, GetActorLocation());
				m_extendPoleSoundPlayed = true;
			}
			
		}
		if (!m_PoleExtended)
		{
			if ((m_PoleExtensionBlend.GetAlpha() <= 0.01f))
			{
				m_PoleExtended = true;
				m_Extending = true;
				m_DesiredExtension = m_CurrentPoleHeight + TopPoleExtension;
			}

		}
		else if(m_PoleExtended)
		{
			if ((m_PoleExtensionBlend.GetAlpha() <= 0.01f))
			{
				m_PoleExtended = false;
				m_Extending = true;
				m_DesiredExtension = InitialTopPolePosition;
			}
		}
	}

}

void ADustyCharacter::ExtendPoleAnimation(double _DeltaTime)
{
	m_PoleExtensionBlend.Update(_DeltaTime);
	// if(m_extendPoleSound && !m_extendPoleSoundPlayed)
	// {
	// 	GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_extendPoleSound, GetActorLocation());
	// 	m_extendPoleSoundPlayed = true;
	// }
	if (m_PoleExtensionBlend.IsComplete())
	{
		m_Extending = false;
		m_extendPoleSoundPlayed = false;
		m_secondExtendPoleSoundPlayed = false;
		m_CurrentPoleHeight = m_NewPoleHeight;
		m_PoleExtensionBlend.Reset();
		return;
	}

	m_NewPoleHeight = FMath::Lerp(m_CurrentPoleHeight, m_DesiredExtension, m_PoleExtensionBlend.GetBlendedValue());

	m_AuxBallPosition.Z = m_InitialBallPosition.Z + (m_NewPoleHeight - InitialTopPolePosition);
	m_StaticBallPosition.Z = m_InitialStaticBallPosition.Z + (m_NewPoleHeight - InitialTopPolePosition);

	DustyUtils::ExtendPole(PoleSkeletal, TopPoleBoneName, m_NewPoleHeight);



	if (m_CurrentPoleHeight > m_DesiredExtension)
	{
		//If we hit during extension
		FHitResult Hit;

		FVector Start = PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace);
		FVector End = Start + GetActorForwardVector() * 10;
		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, GetIgnoreCharacterCollisionParams());

		AMagneticCube* MagnetCube = Cast<AMagneticCube>(Hit.GetActor());
		if (MagnetCube)
		{
			FVector distance = MagnetCube->GetActorLocation() - GetActorLocation();
			float direction = FVector::DotProduct(MagnetCube->GetActorUpVector().GetSafeNormal(), distance.GetSafeNormal());

			bool haveMagnets = false;
			TArray<AActor*> attachedMagnets;
			MagnetCube->GetAttachedActors(attachedMagnets); //Check if the magnet is attached

			if (attachedMagnets.Num() > 0 || MagnetCube->m_parent)
			{
				haveMagnets = true;
				return;
			}

			if (!haveMagnets)
			{
				distance = MagnetCube->GetActorLocation() - GetActorLocation();
				direction = FVector::DotProduct(GetActorForwardVector().GetSafeNormal(), distance.GetSafeNormal());

				MagnetCube->m_velocity += m_ExtendPushForce * -Hit.ImpactNormal * direction;
				MagnetCube->m_pushed = true;
					
				//PoleSkeletal->SetBoneLocationByName(TopPoleBoneName, Hit.Location, EBoneSpaces::WorldSpace);
				m_DustyPlayerAnimInstance->AddAlphaStickCube(3 * GetWorld()->DeltaTimeSeconds);
			}
		}
		else if (Hit.bBlockingHit && !m_HitMontageInCooldown)
		{
			HitWall();
		}
	}
}

void ADustyCharacter::ExtendBottomPole(double _DeltaTime)
{
	m_BottomExtensionTimer += _DeltaTime;
	if(m_extendPoleSound && !m_extendPoleSoundPlayed)
	{
		GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_extendPoleSound, GetActorLocation());
		m_extendPoleSoundPlayed = true;
	}
	if (m_BottomExtensionTimer >= m_BottomExtensionTime)
	{
		m_ExtendingBottom = false;
		m_extendPoleSoundPlayed = false;
		m_secondExtendPoleSoundPlayed = false;
		m_BottomExtensionTimer = 0;

		if (m_Tilt)
		{
			SetActorRotation(m_DesiredRotation);
		}

		DustyUtils::ExtendPole(PoleSkeletal, BottomPoleBoneName, m_DesiredBottomExtension);
		return;
	}
	m_NewPoleHeight = FMath::Lerp(m_CurrentBottomPoleHeight, m_DesiredBottomExtension, m_BottomExtensionTimer/m_BottomExtensionTime);
	
	if (m_Tilt)
	{
		SetActorRotation(FMath::Lerp(m_CurrentRotation, m_DesiredRotation, m_BottomExtensionTimer / m_BottomExtensionTime));
	}

	DustyUtils::ExtendPole(PoleSkeletal, BottomPoleBoneName, m_NewPoleHeight);
}


void ADustyCharacter::CheckFailState()
{
	if (UFallingState* CurrentState = Cast<UFallingState>(ActionsFilterComponent->GetCurrentBaseState()))
	{
		float InitialHeight = CurrentState->InitialHeight;
		if (!m_Fading && GetActorLocation().Z <= InitialHeight - m_MaxFallingDistance)
		{
			StartFadeIn(m_FadeDuration, &ADustyCharacter::SetBackValidPosition);
		}
	}

}

float ADustyCharacter::GetFadeInTime() const
{
	return m_FadeDuration;
}

void ADustyCharacter::StartFadeIn(float _fadeDuration, void(ADustyCharacter::* InCallbackFunction)())
{
	m_Fading = true;
	CameraManager->StartCameraFade(0, 1, _fadeDuration, FLinearColor::Black, false, true);
	GetWorld()->GetTimerManager().SetTimer(FadeTimeHandle, this, InCallbackFunction, _fadeDuration, false);
}

void ADustyCharacter::SetBackValidPosition()
{
	ActionsFilterComponent->SetCurrentState(UFailState::StaticClass());
	SetActorLocation(LastSafePosition);
	
	FTimerDelegate const TimerDelegate = FTimerDelegate::CreateUObject(this, &ADustyCharacter::StartFadeOut, m_FadeDuration);
	GetWorld()->GetTimerManager().SetTimer(FadeTimeHandle, TimerDelegate, m_DurationBetweenFades, false);
}

void ADustyCharacter::StartFadeOut(float _fadeDuration)
{
	GetWorld()->GetTimerManager().SetTimer(FadeTimeHandle, this, &ADustyCharacter::SetBackMovementAfterFail, _fadeDuration, false);
	CameraManager->StartCameraFade(1, 0, _fadeDuration, FLinearColor::Black, false, true);
}

void ADustyCharacter::BookStartFadeOut()
{
	CameraManager->StartCameraFade(1, 0, BookFadeDuration * 2, FLinearColor::Black, false, true);
}

void ADustyCharacter::ShowFinalQuestion()
{
	//Prevent dusty from switching pages on last question
	m_DustyInFinalQuestion = true;
	UBookState* BookState = Cast<UBookState>(*ActionsFilterComponent->m_statesInstancesMap.Find(UBookState::StaticClass()));
	BookState->OriginState = UGroundMovementState::StaticClass();
	ActionsFilterComponent->SetCurrentState(UBookState::StaticClass());


}

void ADustyCharacter::ShowCredits()
{
	UCinematicsSubsystem* cinematicSubsystem = GetGameInstance()->GetSubsystem<UCinematicsSubsystem>();
	if (cinematicSubsystem)
	{
		cinematicSubsystem->OnEndCinematic.Clear();

		cinematicSubsystem->OnEndCinematic.AddDynamic(this, &ADustyCharacter::SetMainMenuAfterCredtis);
		cinematicSubsystem->PlayCinematic(cinematicSubsystem->CreditsSequence);
	}
}

void ADustyCharacter::SetBackMovementAfterFail()
{
	m_Fading = false;
	ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
}


void ADustyCharacter::SafeLastValidPosition()
{
	//We safe last valid point for fail state
	FHitResult Hit;

	//We throw a raycast at bottom of dusty
	const FVector Start = GetActorLocation();

	TArray<FVector> Directions = { GetActorForwardVector(), -GetActorForwardVector(), GetActorRightVector(), -GetActorRightVector() };

	for (const FVector& Direction : Directions)
	{
		const FVector End = Start + Direction * GetCapsuleComponent()->GetUnscaledCapsuleRadius() - GetActorUpVector() * (GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() + m_FloorDetectionOffset);

		//DrawDebugLine(GetWorld(), Start, End, FColor::Black, true);
		//If it hits something and is walkable, not big slope, that's a safe spot
		if (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, GetIgnoreCharacterCollisionParams()))
		{
			return;
		}
		
	}

	if (DustyUtils::GetAngleBetweenVectors(Hit.ImpactNormal, FVector::UpVector) < DustyMovementComponent->GetWalkableFloorAngle())
	{
		LastSafePosition = GetActorLocation();
	}


	
}


void ADustyCharacter::SetInput(bool _newInput)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (_newInput)
		{
			EnableInput(PlayerController);
		}
		else
		{
			DisableInput(PlayerController);
			MovementInput = FVector2D::ZeroVector;
			LookInput = FVector2D::ZeroVector;
			AimInput = false;
		}
	}
}

void ADustyCharacter::ResetMovementInput()
{
	m_currentMovementInput = FVector2D::ZeroVector;
}

UMaterialInstanceDynamic* ADustyCharacter::GetPoleMaterial()
{
	return PoleDynamicMaterial;
}

void ADustyCharacter::ExitPoleElevator()
{
	CurrentPoleElevator->ClearTimers();
	CurrentPoleElevator = nullptr;
	UPoleElevatorState* PoleJumpState = Cast<UPoleElevatorState>(*ActionsFilterComponent->m_statesInstancesMap.Find(UPoleElevatorState::StaticClass()));
	PoleJumpState->Extended = false;
}

void ADustyCharacter::PoleInteract()
{
	
	TArray<FHitResult> HitResults = DustyUtils::DustyPoleRaycast(this, InteractionLength/2, m_PoleBaseRadius + m_InteractExtraRadius);

	for (const FHitResult& Hit : HitResults)
	{
		if (ALightActor* LightActor = Cast<ALightActor>(Hit.GetActor()))
		{
			if (!LightActor->CanBeActivatedByPlayer || !m_LightPoleSkill)
			{
				continue;
			}
			FHitResult VisibilityHit;
			
			FVector Start = PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace);
			//FVector End = Hit.ImpactPoint;
			FVector End = Start - PoleSkeletal->GetUpVector() * InteractionLength;
			GetWorld()->LineTraceSingleByChannel(VisibilityHit, Start, End, ECC_Visibility, GetIgnoreCharacterCollisionParams());

			if (VisibilityHit.bBlockingHit && VisibilityHit.GetActor() == LightActor)
			{
				if (IsPoleLighted() && !LightActor->GetLightStatus())
				{
					LightActor->Activate(true, this);
					GetPoleMaterial()->SetScalarParameterValue(FName("position"), 1);
					TogglePoleLight(false);
					break;
				}
				if (!IsPoleLighted() && LightActor->GetLightStatus())
				{
					LightActor->Activate(false, this);
					GetPoleMaterial()->SetScalarParameterValue(FName("position"), 0);
					TogglePoleLight(true);
					break;
				}

			}

		}
		else if (AToggleActor* ToggleActor = Cast<AToggleActor>(Hit.GetActor()))
		{

			if (ToggleActor->CanBeActivatedByPlayer)
			{
				FHitResult VisibilityHit;

				FVector Start = PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace);
				//FVector End = Hit.ImpactPoint;
				FVector End = Start - PoleSkeletal->GetUpVector() * InteractionLength;
				GetWorld()->LineTraceSingleByChannel(VisibilityHit, Start, End, ECC_Visibility, GetIgnoreCharacterCollisionParams());

				if (VisibilityHit.bBlockingHit && VisibilityHit.GetActor() == ToggleActor)
				{
					ToggleActor->Activate();
					break;
				}
			}
		}
	}
}

void ADustyCharacter::BallMovement(float _DeltaTime)
{
	m_BallMovementTimer += _DeltaTime * FMath::FRandRange(m_MinRandom, m_MaxRandom);
	PoleSkeletal->SetBoneLocationByName(BallPoleBoneName,DustyUtils::GetRandomMovement(m_AuxBallPosition, m_BallMovementTimer, m_Amplitude, m_Frequency, 2), EBoneSpaces::ComponentSpace);
}

void ADustyCharacter::PlaceBallMovement(float _DeltaTime)
{
	FVector Initial;
	FVector Desired;

	m_BallPlacementTimer += _DeltaTime;


	if (m_PoleLighted)
	{
		Initial = m_StaticBallPosition;
		Desired = m_AuxBallPosition;
	}
	else
	{
		Initial = m_AuxBallPosition;
		Desired = m_StaticBallPosition;
	}
	if (m_BallPlacementTimer >= m_BallPlacementTime)
	{
		m_BallPlacementTimer = 0;
		m_BallMoving = false;
		m_BallPlaced = true;
		PoleSkeletal->SetBoneLocationByName(BallPoleBoneName, Desired, EBoneSpaces::ComponentSpace);
		return;
	}

	DustyUtils::PrintMessage("Initial", Initial, FColor::Yellow, _DeltaTime);
	DustyUtils::PrintMessage("Desired", Desired, FColor::Yellow, _DeltaTime);

	FVector NewVector = FMath::Lerp(Initial, Desired, m_BallPlacementTimer/ m_BallPlacementTime);
	DustyUtils::PrintMessage("NewVector", NewVector, FColor::Yellow, _DeltaTime);
	PoleSkeletal->SetBoneLocationByName(BallPoleBoneName, NewVector, EBoneSpaces::ComponentSpace);



}

bool ADustyCharacter::CanDoPoleJump()
{
	// Some checks to save raycasts
	if (*ActionsFilterComponent->GetCurrentBaseStateClass() != UGroundMovementState::StaticClass())
	{
		return false;
	}
	if (DustyMovementComponent->GetLastUpdateVelocity().Size() < m_MinPoleJumpSpeed)
	{
		return false;
	}

	FHitResult OutHit;

	FVector JumpForwardDirection;
	FVector JumpRightDirection;
	if (JumpTowardsCamera)
	{
		JumpForwardDirection = FollowCamera->GetForwardVector();
		JumpForwardDirection.Z = 0;
		JumpRightDirection = FollowCamera->GetRightVector();
		JumpRightDirection.Z = 0;
	}
	else
	{
		JumpForwardDirection = GetActorForwardVector(); 
		JumpRightDirection = GetActorRightVector();

	}

	//DustyPosition for calcs
	FVector InitialDustyPosition = GetActorLocation() + JumpForwardDirection * GetCapsuleComponent()->GetScaledCapsuleRadius();

	
	//We will give dusty some distance to make the pole jump
	bool ThereIsNoEdge = true;
	for (int i = 0; i < m_ForwardOffsetSubdivisionsToDoPoleJump; i++)
	{
		FVector Start = InitialDustyPosition + JumpForwardDirection * (m_ForwardOffsetToDoPoleJump / m_ForwardOffsetSubdivisionsToDoPoleJump) * i;
		
		//We will see if any position in across the distance is in mid air, meaning we have reached an edge		
		FVector End = Start - GetActorUpVector() * (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 5.f);
		//DrawDebugLine(GetWorld(), Start, End, FColor::Black, true);

		GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_GameTraceChannel1, GetIgnoreCharacterCollisionParams());
		//If there is a edge, we save that location for the pole jump
		if (!OutHit.bBlockingHit)
		{
			ThereIsNoEdge = false;
			m_PoleJumpLocation = Start;
			break;
		}
		else
		{
			//DustyUtils::PrintMessage(OutHit.GetActor()->GetName());
		}
	}
	//If across all the distance we are still in the floor, we can't jump
	if (ThereIsNoEdge)
	{
		return false;
	}
	
	//We will check if we can reach the pole jump location
	FVector DustyLocation = GetActorLocation();
	FHitResult ReachableHit;
	FCollisionShape Capsule = FCollisionShape::MakeCapsule(GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.75, (GetActorLocation() - m_PoleJumpLocation).Size() / 2);
	FVector Direction = (GetActorLocation() - m_PoleJumpLocation).GetSafeNormal();

	Direction = Direction.RotateAngleAxis(90, GetActorRightVector());
	Direction = Direction.RotateAngleAxis(90, GetActorUpVector());

	FRotator ReachCapsuleRotation = Direction.Rotation();

	FVector CapsuleStart = (GetActorLocation() + m_PoleJumpLocation) / 2;
	GetWorld()->SweepSingleByChannel(ReachableHit, CapsuleStart, CapsuleStart+0.001, ReachCapsuleRotation.Quaternion(), ECC_Visibility, Capsule, GetIgnoreCharacterCollisionParams());
	//DustyUtils::DustyDrawDebugCapsule(GetWorld(), CapsuleStart, Capsule.GetCapsuleHalfHeight(), Capsule.GetCapsuleRadius(), ReachCapsuleRotation.Quaternion(), FColor::Red, false, 1.0f);

	if (ReachableHit.bBlockingHit)
	{
		//DustyUtils::PrintMessage(ReachableHit.GetActor()->GetName());
		return false;
	}

	//Valid Points for calculations
	FVector FirstHit = FVector::ZeroVector;
	FVector LastHit = FVector::ZeroVector;

	// The sum of base pole and bottom extension
	float RaycastDistance = m_PoleBaseHeight + BottomPoleExtension;

	// Throw rays from character to the floor in an arc shape
	for (int i = 0; i < m_PoleJumpRaycastAngle; i++)
	{
		// Forward Ray
		FVector Ray = JumpForwardDirection * RaycastDistance;

		// Rotated Ray
		Ray = Ray.RotateAngleAxis(m_AngleOffset + i, JumpRightDirection);

		FVector End = m_PoleJumpLocation + Ray;

		GetWorld()->LineTraceSingleByChannel(OutHit, m_PoleJumpLocation, End, ECollisionChannel::ECC_GameTraceChannel1, GetIgnoreCharacterCollisionParams());
		if (OutHit.bBlockingHit)
		{
			FVector HitDirection = m_PoleJumpLocation - OutHit.ImpactPoint;

			//The slope of the floor, mustn't exceed MaximumJumpSlopeAngle
			float FloorAngle = DustyUtils::GetAngleBetweenVectors(FVector::UpVector, OutHit.ImpactNormal);

			//The angle from character to the hit point, mustn't exceed MaxJumpVerticalAngle
			float JumpAngle = DustyUtils::GetAngleBetweenVectors(FVector::UpVector, HitDirection.GetSafeNormal());

			// Check if player is at minimum valid jump height
			if (HitDirection.Z >= m_MinimumJumpHeight + GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() && FloorAngle < m_MaximumJumpSlopeAngle && JumpAngle < m_MaxJumpVerticalAngle)
			{
				//DustyUtils::DustyDrawDebugLine(GetWorld(), m_PoleJumpLocation, End, FColor::Green, true, 1.f, 0, 1.f);

				//If not points hit yet
				if (FirstHit == FVector::ZeroVector)
				{
					FirstHit = OutHit.ImpactPoint;
				}
				//If new point hit is higher we have to reset the points. 
				//When calculating middle point, if first is different, the pole would clip the floor and don't do pole jump
				else if (abs(OutHit.ImpactPoint.Z - FirstHit.Z) > 5)
				{
					FirstHit = OutHit.ImpactPoint;
				}
				LastHit = OutHit.ImpactPoint;
			}
			else
			{
				//This is debug
				/*if (!(HitDirection.Z >= m_MinimumJumpHeight + GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()))
				{
					DustyUtils::PrintMessage("No altura", HitDirection.Z);
				}
				if (!(FloorAngle < m_MaximumJumpSlopeAngle))
				{
					DustyUtils::PrintMessage("suelo demasiado inclinado", FloorAngle);
				}
				if (!(JumpAngle < m_MaxJumpVerticalAngle))
				{
					DustyUtils::PrintMessage("max jump vertical angle", JumpAngle);
				}*/
				//DustyUtils::PrintMessage(OutHit.Component->GetName());
				//DustyUtils::DustyDrawDebugLine(GetWorld(), m_PoleJumpLocation, End, FColor::Blue, true, 1.f, 0, 1.f);
			}
		}
		else
		{
			//DustyUtils::DustyDrawDebugLine(GetWorld(), m_PoleJumpLocation, End, FColor::Red, false, 1.f, 0, 1.f);
		}
	}

	if (FirstHit == FVector::ZeroVector)
	{
		return false;
	}

	//Now we calculate middle point of all points for the incoming jump
	m_PolePoint = (LastHit + FirstHit) / 2;
	return true;	
}
void ADustyCharacter::EvaluateJump()
{
	if (m_ExtendiblePoleSkill)
	{
		if (ActionsFilterComponent->GetCurrentBaseStateClass() == UPoleJumpingState::StaticClass())
		{
			PoleDoubleJump();
		}
		else if (CanDoPoleJump())
		{
			FRotator ForwardRotation;
			if (JumpTowardsCamera)
			{
				ForwardRotation = FollowCamera->GetForwardVector().Rotation();
			}
			else
			{
				ForwardRotation = GetActorForwardVector().Rotation();
			}
			DustyUtils::DustyMoveTo(this, m_PoleJumpLocation, ForwardRotation, &ADustyCharacter::PoleJump);
		}
		else if (ActionsFilterComponent->GetCurrentBaseStateClass() == UAutoMovementState::StaticClass())
		{
			ExtendBufferForDoubleJump = true;
		}
		else
		{
			NormalJump();
		}
	}
	else
	{
		NormalJump();
	}
}

void ADustyCharacter::ReleasedJumpInput()
{
	StopJumping();

	if (ActionsFilterComponent->GetCurrentBaseStateClass() == UBookState::StaticClass())
	{
		BookButtonRelease();
		//m_BookInstance->ReleaseBookButton();
	}
}

void ADustyCharacter::NormalJump()
{
	ActionsFilterComponent->SetCurrentState(UJumpingState::StaticClass());
}

void ADustyCharacter::PoleJump()
{
	ActionsFilterComponent->SetCurrentState(UPoleJumpingState::StaticClass());
	DustyMovementComponent->Velocity *= m_BrakeFactorInitialJump;
	/*
	if(m_trail2)
	{
		m_trail2->Activate();
	}*/
	Jump();
}

void ADustyCharacter::PoleDoubleJump()
{
	if (!GetExtendiblePoleSkill()) //Unlocked skill?
	{
		return;
	}
	if (PoleJumpConstraintActor)
	{
		PoleJumpConstraintActor->Extend(true);
	}

	//Play Pole extend Sound
	if(m_extendPoleSound && !m_secondExtendPoleSoundPlayed)
	{
		GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound3D(m_extendPoleSound, GetActorLocation());
		m_secondExtendPoleSoundPlayed = true;
	}
}

void ADustyCharacter::SpawnPoleJumpConstraint(PoleJumpValuesStruct* _PoleJumpValues)
{
	if (_PoleJumpValues == nullptr)
	{
		//DustyUtils::PrintMessage("Jeje, no crash");
		return;
	}
	FRotator DirectionRotation = _PoleJumpValues->PoleDirection.Rotation();
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += 90;
	if (_PoleJumpValues->PoleMovementType == EPoleMovementType::Jump)
	{
		//It works fine when character yaw is 90
		float CharacterYaw = GetActorRotation().Yaw;

		//So let's rotate that
		m_PolePointOffset = m_PolePointOffset.RotateAngleAxis(CharacterYaw - 90, FVector::UpVector);
		_PoleJumpValues->PolePoint += m_PolePointOffset; 
	}
	PoleJumpConstraintActor = GetWorld()->SpawnActor<APoleJumpConstraintActor>(m_PoleJumpConstraintClass, _PoleJumpValues->PolePoint, Rotation);

	const float DesiredScale = (_PoleJumpValues->PoleDirection.Size() / 100);
	
	PoleJumpConstraintActor->SetPoleJumpValues(_PoleJumpValues);

	if (m_PoleLighted)
	{
		PoleJumpConstraintActor->ActivatePoleLight();
	}


	if (*ActionsFilterComponent->GetCurrentBaseStateClass() == UPoleJumpingState::StaticClass())
	{
		PoleJumpConstraintActor->ExtendBottom((DesiredScale-2)/5);
	}

	PoleJumpConstraintActor->Pole->SetRelativeScale3D(FVector(0.1f, 0.1f, DesiredScale));

	FRotator PoleRotation = PoleJumpConstraintActor->Pole->GetRelativeRotation();
	PoleRotation.Roll = _PoleJumpValues->PoleImpulse.Rotation().Pitch;
	PoleJumpConstraintActor->Pole->SetRelativeRotation(PoleRotation);

	//DustyUtils::DustyDrawDebugLine(GetWorld(), PoleJumpConstraintActor->Pivot->GetComponentLocation(), PoleJumpConstraintActor->Pivot->GetComponentLocation() - _PoleJumpValues->PoleDirection.GetSafeNormal() * DesiredScale * 55, FColor::Blue, true);

	PoleJumpConstraintActor->Pole->SetWorldLocation(PoleJumpConstraintActor->Pivot->GetComponentLocation() - _PoleJumpValues->PoleDirection.GetSafeNormal() * DesiredScale * 55);

	PoleJumpConstraintActor->Jump();

	if (_PoleJumpValues->PoleMovementType == EPoleMovementType::Jump && PoleJumpConstraintActor && ExtendBufferForDoubleJump)
	{
		PoleJumpConstraintActor->Extend(true);
	}

}

void ADustyCharacter::TogglePoleLight(bool Lighted)
{
	if (!GetLightPoleSkill()) //Unlocked skill?
	{
		return;
	}

	if (m_PoleLighted != Lighted)
	{
		m_BallMoving = true;
		m_BallPlaced = false;
		DustyUtils::PrintMessage("BallMoving", m_BallMoving);
	}

	m_PoleLighted = Lighted;


	if (m_PoleLighted)
	{
		SetDustyTattooValue(PoleLightedTattooValue);
		DustyUtils::SetEmissiveMaterialValues(PoleSkeletal, m_MaxLightIntensityRed, m_MaxLightIntensityGreen, m_MaxLightIntensityBlue);

		if (m_polePointLightShouldBeActive)
		{
			SetPolePointLightVisibility(true);
		}
	}
	else
	{
		SetDustyTattooValue(NoTattooValue);
		m_BallMovementTimer = 0;
		DustyUtils::SetEmissiveMaterialValues(PoleSkeletal, 0, 0, 0);

		SetPolePointLightVisibility(false);
	}
}

void ADustyCharacter::TogglePolePointLight()
{
	m_polePointLightShouldBeActive = !m_polePointLightShouldBeActive;
	if (m_polePointLightShouldBeActive && m_PoleLighted)
	{
		SetPolePointLightVisibility(true);
	}
	else
	{
		SetPolePointLightVisibility(false);
	}
}

void ADustyCharacter::SetPolePointLightVisibility(bool _value)
{
	if (_value)
	{
		PolePointLight->Activate();
		PolePointLight->SetVisibility(true);
	}
	else
	{
		PolePointLight->Deactivate();
		PolePointLight->SetVisibility(false);
	}
}


void ADustyCharacter::ToggleReadMode(const FInputActionValue& Value)
{
	if (ActionsFilterComponent->GetCurrentBaseStateClass() == UBookState::StaticClass())
	{
		m_BookInstance->ToggleReadMode();
	}
}

void ADustyCharacter::Pause(const FInputActionValue& Value)
{
	if (m_DustyInFinalQuestion || !m_BookInstance->NotAnimating() || DustyInPlanetaryKeyController || ActionsFilterComponent->GetCurrentBaseStateClass() == UMainMenuState::StaticClass() || CameraManager->IsShakeModifierEnabled())
	{
		return;
	}
	
	if (ActionsFilterComponent->GetCurrentBaseStateClass() == UBookState::StaticClass())
	{
		Pause_CloseBook();
	}
	else
	{
		Pause_OpenBook();
	}
}

void ADustyCharacter::Pause_OpenBook()
{
	UBookState* BookState = Cast<UBookState>(*ActionsFilterComponent->m_statesInstancesMap.Find(UBookState::StaticClass()));
	if (ActionsFilterComponent->GetCurrentBaseStateClass() == UPoleKeyState::StaticClass())
	{
		m_BookInstance->OpeningBookmark = LG_Bookmarks::Glyphs;
	}

	BookState->OriginState = ActionsFilterComponent->GetCurrentBaseStateClass();
	ActionsFilterComponent->SetCurrentState(UBookState::StaticClass());
	AAudioActor* audio = Cast<AAudioActor>(m_bookAudioChildActor->GetChildActor());
	if (audio && BookState->OriginState == UGroundMovementState::StaticClass())
	{
		audio->PlaySound();
	}
	if (m_currentPlayingSound && BookState->OriginState == UGroundMovementState::StaticClass())
	{
		m_currentPlayingSound->PauseSound();
	}
}

void ADustyCharacter::Pause_CloseBook()
{
	UBookState* BookState = Cast<UBookState>(*ActionsFilterComponent->m_statesInstancesMap.Find(UBookState::StaticClass()));
	ActionsFilterComponent->SetCurrentState(BookState->OriginState);
	AAudioActor* audio = Cast<AAudioActor>(m_bookAudioChildActor->GetChildActor());
	if (audio)
	{
		audio->StopSound();

	}

	if (m_currentPlayingSound)
	{
		m_currentPlayingSound->PlaySound();
	}
}

void ADustyCharacter::BookButtonPress(const FInputActionValue& Value)
{
	if (ActionsFilterComponent->GetCurrentBaseStateClass() == UBookState::StaticClass())
	{
		m_BookInstance->PressBookButton();
	}
}

void ADustyCharacter::BookCardinalNavigation(const FInputActionValue& Value)
{
	if (ActionsFilterComponent->GetCurrentBaseStateClass() == UBookState::StaticClass())
	{
		m_BookInstance->NavigateButtons(Value.Get<FVector2D>());
	}
}

void ADustyCharacter::BookChangePage(const FInputActionValue& Value)
{
	if (!m_DustyInFinalQuestion && !m_DustyInSettingsFromMainMenu && m_BookInstance->GetCurrentBookmark() == LG_Bookmarks::LogBook)
	{

		if (ActionsFilterComponent->GetCurrentBaseStateClass() == UBookState::StaticClass())
		{
			if (Value.Get<float>() > 0)
			{
				m_BookInstance->LB_NextPage_Start();
			}
			else
			{
				m_BookInstance->LB_PreviousPage_Start();
			}

			if(m_bookPageSound)
			{
				GetGameInstance()->GetSubsystem<UAudioSubsytem>()->PlaySound2D(m_bookPageSound);
			}
		}
	}
}

void ADustyCharacter::BookChangeBookmark(const FInputActionValue& Value)
{
	if (!m_DustyInFinalQuestion && !m_DustyInSettingsFromMainMenu && !FirstTimeOpeningBook)
	{
		if (ActionsFilterComponent->GetCurrentBaseStateClass() == UBookState::StaticClass())
		{

			DustyUtils::HideHudWithNoFade(GetWorld(), EHudCanvas::ReadMode);
			if (Value.Get<float>() > 0)
			{
				m_BookInstance->NextBookmark();
			}
			else
			{
				m_BookInstance->PreviousBookmark();
			}
		}
	}
}

void ADustyCharacter::BookButtonRelease()
{
	if (ActionsFilterComponent->GetCurrentBaseStateClass() == UBookState::StaticClass())
	{
		m_BookInstance->ReleaseBookButton();
	}
}

void ADustyCharacter::OnSkipCinematicPressed()
{
	// Get Cinematic Subsystem
	UCinematicsSubsystem* cinematicSubsystem = GetGameInstance()->GetSubsystem<UCinematicsSubsystem>();

	if (cinematicSubsystem->IsCinematicPlaying)
	{
		SkipButtonPressed = true;
	}
}

void ADustyCharacter::OnSkipCinematicReleased()
{
	// Get Cinematic Subsystem
	UCinematicsSubsystem* cinematicSubsystem = GetGameInstance()->GetSubsystem<UCinematicsSubsystem>();

	if (cinematicSubsystem->IsCinematicPlaying)
	{
		SkipButtonPressed = false;
	}
}

void ADustyCharacter::NewGameCinematicEnd()
{
	GetMesh()->SetVisibility(true);
	PoleSkeletal->SetVisibility(true);

	// Initialize character
	//InitializeCharacter();
	//UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(this, 0); //En principio esto no hace falta, pq abrimos el libro y el libro tiene su propia camara

	//TArray<AActor*> rocks;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASittingRock::StaticClass(), rocks);
	//for (int i = 0; i < rocks.Num(); i++)
	//{
	//	ASittingRock* rock = Cast<ASittingRock>(rocks[i]);
	//	if (rock->IsNewGameRock)
	//	{
	//		rock->SitDusty(this);
	//		break;
	//	}
	//}

	// Open Book
	UBookState* BookState = Cast<UBookState>(*ActionsFilterComponent->m_statesInstancesMap.Find(UBookState::StaticClass()));
	BookState->OriginState = UGroundMovementState::StaticClass();
	ActionsFilterComponent->SetCurrentState(UBookState::StaticClass());

	//Remove itself from the event
	GetGameInstance()->GetSubsystem<UCinematicsSubsystem>()->OnEndCinematic.Clear();
}

void ADustyCharacter::CanMoveWithInput(bool canMove)
{
	m_CanMoveWithInput = canMove;
}

void ADustyCharacter::SetMovementInput(const FInputActionValue& Value)
{
	MovementInput = Value.Get<FVector2D>();
	UpdateGrassEffect(UE::Geometry::ToLinearColor(GetCapsuleComponent()->GetComponentLocation()));
}
void ADustyCharacter::UpdateMovement(float _DeltaTime)
{
	if (!DustyMovementComponent || !m_CanMoveWithInput) return;

	m_FailStateSaveTimer += _DeltaTime;
	if (m_FailStateSaveTimer >= m_TimeBetweenSaves)
	{
		//Restart timer
		m_FailStateSaveTimer = 0;
		if (!m_Fading && !FixedSafePosition)
		{
			SafeLastValidPosition();
		}
	}
	if (Controller != nullptr)
	{
		if (ActionsFilterComponent->GetCurrentBaseStateClass() == UAutoMovementState::StaticClass())
		{
			// add movement 
			AddMovementInput(AutoMovingDirection.GetSafeNormal(), 1);
			return;
		}
		FRotator Rotation = Controller->GetControlRotation();
		if (CameraManager && CameraManager->IsOverrided())
		{
			Rotation = CameraManager->CameraOverrider->GetInputVector();
		}
		else
		{
			Rotation = Controller->GetControlRotation();
		}
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		// add movement 
		m_currentMovementInput.X = FMath::Lerp(m_currentMovementInput.X, MovementInput.X, m_DustyAcceleration * m_DustyInputAcceleration * _DeltaTime);
		m_currentMovementInput.Y = FMath::Lerp(m_currentMovementInput.Y, MovementInput.Y, m_DustyAcceleration * m_DustyInputAcceleration * _DeltaTime);


		//m_currentMovementInput.X = MovementInput.X;
		//m_currentMovementInput.Y = MovementInput.Y;

		if (MovementInput.IsNearlyZero() && m_currentMovementInput.IsNearlyZero(0.1))
		{
			m_currentMovementInput = FVector2D::ZeroVector;
		}

		AddMovementInput(ForwardDirection, m_currentMovementInput.Y);
		AddMovementInput(RightDirection, m_currentMovementInput.X);

		
	}
}


void ADustyCharacter::SetWalkSpeed()
{
	if (*ActionsFilterComponent->GetCurrentBaseStateClass() == UGroundMovementState::StaticClass())
	{
		DesiredPlayerSpeed = m_DustyMaxWalkSpeed;
	}
	else if (*ActionsFilterComponent->GetCurrentBaseStateClass() == UAimingState::StaticClass())
	{
		DesiredPlayerSpeed = m_DustyMaxAimSpeedWalk;
	}
}

void ADustyCharacter::SetRunSpeed()
{
	if (*ActionsFilterComponent->GetCurrentBaseStateClass() == UGroundMovementState::StaticClass())
	{
		DesiredPlayerSpeed = m_DustyMaxRunSpeed;
	}
	else if (*ActionsFilterComponent->GetCurrentBaseStateClass() == UAimingState::StaticClass())
	{
		DesiredPlayerSpeed = m_DustyMaxAimSpeedRun;
	}
}

void ADustyCharacter::SetLookInput(const FInputActionValue& Value)
{
	LookInput = Value.Get<FVector2D>();
	
//	if (DustyActuallyAiming)
//	{
//		LookInput.X = FMath::Clamp(LookInput.X, -1, 1);
//		LookInput.Y = FMath::Clamp(LookInput.Y, -1, 1);
//	}
}

void ADustyCharacter::CancelInteract(const FInputActionValue& Value)
{
	if (ActionsFilterComponent->GetCurrentBaseStateClass() == UPoleKeyState::StaticClass() && CanUnnailPole)
	{
		ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
	}
	else if (ActionsFilterComponent->GetCurrentBaseStateClass() == UPoleElevatorState::StaticClass())
	{
		CurrentPoleElevator->Retract();
	}
}

void ADustyCharacter::UpdateLookCamera()
{
	// Get the settings subsystem
	USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

	if (Controller != nullptr && CameraManager && !CameraManager->IsOverrided())
	{
		AddControllerYawInput(LookInput.X * settingsSubsystem->GetCameraSensitivity() /** DefaultInputLookVelocity*/);
		AddControllerPitchInput(LookInput.Y * settingsSubsystem->GetCameraSensitivity() /** DefaultInputLookVelocity*/);
	}
}

void ADustyCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);



	if (*ActionsFilterComponent->GetCurrentBaseStateClass() == UAimingState::StaticClass())
	{
		AMagneticCube* MagnetCube = Cast<AMagneticCube>(Other);

		FVector groundVelocity;
		groundVelocity.X = FVector::DotProduct(GetActorRightVector(), GetVelocity());
		groundVelocity.Y = FVector::DotProduct(GetActorForwardVector(), GetVelocity());
		float groundVelocityMagnitude = groundVelocity.Length();
		float offsetGroundVelocity = 25.f;
		if (MagnetCube)
		{
			FVector distance = MagnetCube->GetActorLocation() - GetActorLocation();
			float direction = FVector::DotProduct(MagnetCube->GetActorUpVector().GetSafeNormal(), distance.GetSafeNormal());
		// 	if (direction < -0.85f)
		// 	{
		// 		MagnetCube->AddForce(-MagnetCube->GetActorUpVector() * 35000);
		// 	}


			bool haveMagnets = false;
			TArray<AActor*> attachedMagnets;
			MagnetCube->GetAttachedActors(attachedMagnets); //Check if the magnet is attached


			if (attachedMagnets.Num() > 0 || MagnetCube->m_parent)
			{
				haveMagnets = true;
				return;
			}

			if (!haveMagnets)
			{
				distance = MagnetCube->GetActorLocation() - GetActorLocation();
				direction = FVector::DotProduct(GetActorForwardVector().GetSafeNormal(), distance.GetSafeNormal());

				if (GetVelocity() != FVector::ZeroVector && direction >= 0.5f)
				{
					if (0.5f < abs(FVector::DotProduct(GetLastMovementInputVector().GetSafeNormal(), GetActorForwardVector().GetSafeNormal())))
					{
						MagnetCube->m_velocity += m_PushForce * -Hit.ImpactNormal * direction;
						//MagnetCube->AddForce(m_PushForce * -Hit.ImpactNormal * direction);
						MagnetCube->m_pushed = true;
						GetCharacterMovement()->Velocity = GetLastMovementInputVector().Size() * GetActorForwardVector() * m_PushVelocity;
						//MagnetCube->m_velocity += 50 * -HitResult.ImpactNormal * direction;
					}
				}
				m_DustyPlayerAnimInstance->AddAlphaStickCube(3 * GetWorld()->DeltaTimeSeconds);
			}
		}			
	

		else if (m_PoleHitMontage && !m_HitMontageInCooldown && groundVelocityMagnitude > m_DustyMaxAimSpeedWalk + offsetGroundVelocity)
		{
			FVector HitDirection = HitLocation - GetActorLocation();

			if (DustyUtils::GetAngleBetweenVectors(HitDirection, GetActorForwardVector()) < 70)
			{
				HitWall();

				if (Other && Other->IsA(APigNPC::StaticClass()))
				{
					UnlockAchievement(TEXT("PigAttack_ACH"));
				}
			}
		}
		
	}
	
	if (Hit.PhysMaterial != nullptr)
	{
		if (Hit.PhysMaterial->SurfaceType)
		{
			//DustyUtils::PrintMessage("PhysicalMaterial", FColor::Green, 5);
			UpdateGrassEffect(UE::Geometry::ToLinearColor(GetCapsuleComponent()->GetComponentLocation()));
		}
	}


	else
	{
		//DustyUtils::PrintMessage("Not Physical Material", FColor::Red, 5);
	}

}

void ADustyCharacter::UpdateAimingCamera()
{

	if (Controller != nullptr)
	{
		USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

		AddControllerYawInput(LookInput.X * settingsSubsystem->GetAimCameraSensitivity() /** DefaultInputLookVelocity / 2*/);
		AddControllerPitchInput(LookInput.Y * settingsSubsystem->GetAimCameraSensitivity() /** DefaultInputLookVelocity / 2*/);
	}

	return;


	TArray<FHitResult> OutHits;
	FVector StartLocation = capsulePole->GetComponentLocation();
	FVector EndLocation = StartLocation;
	FRotator Rotation = capsulePole->GetComponentRotation();


	float CapsuleRadius = capsulePole->GetScaledCapsuleRadius() + 10;
	float CapsuleHalfHeight = capsulePole->GetScaledCapsuleHalfHeight() + 10;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(capsulePole->GetCollisionObjectType());


	bool bHit = GetWorld()->SweepMultiByChannel(
		OutHits,
		StartLocation,
		EndLocation,
		Rotation.Quaternion(),
		ECC_WorldStatic,
		FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight),
		QueryParams
	);

	DrawDebugCapsule(
		GetWorld(),
		EndLocation,
		CapsuleHalfHeight,
		CapsuleRadius,
		Rotation.Quaternion(),
		FColor::Blue,
		false,
		GetWorld()->GetDeltaSeconds()
	);

	if (bHit)
	{
		for (FHitResult& OutHit :  OutHits)
		{
			FVector CameraForward = Controller->GetControlRotation().Vector();
			FVector CameraRight = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);

			FVector LocalHitToCapsuleDir;
			LocalHitToCapsuleDir.X = FVector::DotProduct(CameraRight, OutHit.Normal);
			LocalHitToCapsuleDir.Y = FVector::DotProduct(CameraForward, OutHit.Normal);
			LocalHitToCapsuleDir.Z = OutHit.Normal.Z;  

			if ((LookInput.X > 0 && -LocalHitToCapsuleDir.Y > 0.1f) || (LookInput.X < 0 &&	-LocalHitToCapsuleDir.Y < -0.1f))
			{
				LookInput.X = 0;
			}

			if ((LookInput.Y > 0 && LocalHitToCapsuleDir.Z > 0.1f) || (LookInput.Y < 0 && LocalHitToCapsuleDir.Z < -0.1f))
			{
				LookInput.Y = 0;
			}

			if ((m_currentMovementInput.X > 0 && -LocalHitToCapsuleDir.Y > 0.1f) || (m_currentMovementInput.X < 0 && -LocalHitToCapsuleDir.Y < -0.1f))
			{
				m_currentMovementInput.X = 0;
				MovementInput.X = 0;
			}

			if ((m_currentMovementInput.Y > 0 && LocalHitToCapsuleDir.Z > 0.1f) || (MovementInput.Y < 0 && LocalHitToCapsuleDir.Z < -0.1f))
			{
				m_currentMovementInput.Y = 0;
				MovementInput.Y = 0;
			}

			collision = true;

			DustyUtils::PrintMessage("X: %f", LocalHitToCapsuleDir.X, FColor::Cyan, GetWorld()->GetDeltaSeconds());
			DustyUtils::PrintMessage("Y: %f", LocalHitToCapsuleDir.Y, FColor::Cyan, GetWorld()->GetDeltaSeconds());
			DustyUtils::PrintMessage("Z: %f", LocalHitToCapsuleDir.Z, FColor::Cyan, GetWorld()->GetDeltaSeconds());
		}
	}
	else
	{
		collision = false;
	}

	if (Controller != nullptr)
	{
		USettingsSubsystem* settingsSubsystem = GetGameInstance()->GetSubsystem<USettingsSubsystem>();

		AddControllerYawInput(LookInput.X * settingsSubsystem->GetAimCameraSensitivity() /** DefaultInputLookVelocity / 2*/);
		AddControllerPitchInput(LookInput.Y * settingsSubsystem->GetAimCameraSensitivity() /** DefaultInputLookVelocity / 2*/);
	}
}

void ADustyCharacter::UpdatePoleMovement()
{
	if (Controller != nullptr && PoleJumpConstraintActor)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		FVector PoleMovement = ForwardDirection * MovementInput.Y + RightDirection * MovementInput.X;

		PoleJumpConstraintActor->Move(PoleMovement);
	}
}

void ADustyCharacter::ToggleCanAim(bool _ToggleCanAim)
{
	m_ToggleCanAim = _ToggleCanAim;
}

void ADustyCharacter::InitAiming()
{
	//Movement mode
	GetDustyMovementComponent()->bUseControllerDesiredRotation = true;
	GetDustyMovementComponent()->bOrientRotationToMovement = false;

	GetCapsuleComponent()->SetSimulatePhysics(true);
	capsulePole->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetSimulatePhysics(false);

	DesiredPlayerSpeed = m_DustyMaxAimSpeedRun;
	

	m_PoleExtensionBlend.Reset();
}

void ADustyCharacter::UpdateAiming(float _DeltaTime)
{
	if (DustyActuallyAiming)
	{
		FVector Start = PoleSkeletal->GetBoneLocationByName(BallPoleBoneName, EBoneSpaces::WorldSpace);
		FVector HitEnd = Start - PoleSkeletal->GetUpVector() * InteractionLength;

		FHitResult HitResult;

		CrosshairHUD->SetWorldLocation(HitEnd);

		// Hitting collision detection
		bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, HitEnd, ECollisionChannel::ECC_Visibility, GetIgnoreCharacterCollisionParams());
		
		if (ALightActor* LightActorHitObject = Cast<ALightActor>(HitResult.GetActor()))
		{
			if (LightActorHitObject->Lighted != IsPoleLighted() && m_LightPoleSkill)
			{
				CrosshairHUD->SetSprite(CrosshairImages[1]);
			}
			else
			{
				CrosshairHUD->SetSprite(CrosshairImages[0]);
			}
		}
		else
		{
			CrosshairHUD->SetSprite(CrosshairImages[0]);
		}

		if (Hit)
		{
			CrosshairHUD->SetWorldLocation(HitResult.ImpactPoint);
		}
	}
}

void ADustyCharacter::ExitAiming()
{
	//Movement mode
	GetDustyMovementComponent()->bUseControllerDesiredRotation = false;
	GetDustyMovementComponent()->bOrientRotationToMovement = true;

	GetCapsuleComponent()->SetSimulatePhysics(true);
	capsulePole->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetSimulatePhysics(false);

	DesiredPlayerSpeed = m_DustyMaxRunSpeed;

	HideCrosshair();

	m_PoleExtensionBlend.Reset();
	m_PoleExtended = false;
	m_Extending = true;
	m_DesiredExtension = InitialTopPolePosition;

	Crosshair->SetVisibility(false);
	m_DustyPlayerAnimInstance->AddAlphaStickCube(-1);

	if (m_CurrentProvider)
	{
		m_CurrentProvider->RemovePlayer();
		m_CurrentProvider = nullptr;
	}
	DustyActuallyAiming = false;
}

void ADustyCharacter::StartLook()
{
	if (CameraManager != nullptr)
	{
		CameraManager->SetModifier(false);
	}
}

void ADustyCharacter::StopLook()
{
	if (CameraManager != nullptr)
	{
		CameraManager->SetModifier(true);
	}
}


//Evaluates the different actions associated with the interact input. For now they are interact and climb
void ADustyCharacter::EvaluateInteract(const FInputActionValue& Value)
{	
	if (ActionsFilterComponent->GetCurrentBaseStateClass() == UAutoMovementState::StaticClass())
	{
		return;
	}

	if (ActionsFilterComponent->GetCurrentBaseStateClass() == UPoleKeyState::StaticClass() && CanUnnailPole)
	{
		ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
	}
	else if (ActionsFilterComponent->GetCurrentBaseStateClass() == UPoleElevatorState::StaticClass())
	{
		CurrentPoleElevator->Retract();
	}
	else if (AInteractable* interactable = GetNearInteractables())
	{
		interactable->DustyCharacter = this;
		interactable->Interact();
	}
}

AInteractable* ADustyCharacter::GetNearInteractables()
{
	TArray<FHitResult> HitResult;

	const FVector StartLocation = GetActorLocation();
	const FVector EndLocation = GetActorLocation() + (GetActorForwardVector() * 20);

	bool Hit = GetWorld()->SweepMultiByChannel(HitResult, StartLocation, EndLocation, GetActorRotation().Quaternion(), ECC_Visibility, FCollisionShape::MakeCapsule(100, 100), GetIgnoreCharacterCollisionParams());

	for (const FHitResult result : HitResult)
	{
		AInteractable* Interactable = Cast<AInteractable>(result.GetActor());
		if (Interactable)
		{
			return Interactable;
		}
		//DustyUtils::DustyDrawDebugCapsule(GetWorld(), StartLocation, FCollisionShape::MakeCapsule(100, 200).GetCapsuleHalfHeight(), FCollisionShape::MakeCapsule(100, 200).GetCapsuleRadius(), GetActorRotation().Quaternion(), Interactable ? FColor::Green : FColor::Red, false, 1.0f);
	}
	return nullptr;
}