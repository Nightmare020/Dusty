// Fill out your copyright notice in the Description page of Project Settings.


#include "CinematicsSubsystem.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieScene.h"
#include "LevelSequenceActor.h"
#include "Kismet/GameplayStatics.h"
#include "MovieScene.h"
#include "Tracks/MovieSceneCameraCutTrack.h"
#include "Sections/MovieSceneCameraCutSection.h"
#include "../Character/DustyCharacter.h"
#include "CineCameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Tracks/MovieScene3DTransformTrack.h"
#include "Sections/MovieScene3DTransformSection.h"
#include "../Character/CharacterStates/ActionsFilter.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"
#include "../UI/HUD/PlayerHUDWidget.h"
#include "../Utils/DustyUtils.h"

void UCinematicsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Warning, TEXT("Cinematic Subsystem Initialized"));
}

void UCinematicsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UCinematicsSubsystem::PlayCinematic(FString CinematicName, bool PlayerRelevant, bool InGame, bool ShotFragmented)
{
	// Initialize player
	m_PlayerCharacter = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Change character state to cinematic state before anything else
	m_PlayerCharacter->ActionsFilterComponent->SetCurrentState(UCinematicState::StaticClass());

	ULevelSequence* Cinematic = GetCinematic(CinematicName);

	if (!Cinematic || !m_PlayerCharacter)
	{
		//DustyUtils::PrintMessage("Cinematic or Player is null");
		return;
	}
	else
	{
		// Save the original camera transform
		m_PlayerSpringArm = m_PlayerCharacter->FindComponentByClass<USpringArmComponent>();

		if (m_PlayerSpringArm)
		{
			m_OriginalCameraLocation = m_PlayerSpringArm->GetRelativeLocation();
			m_OriginalCameraRotation = m_PlayerSpringArm->GetRelativeRotation();
		}

		// Create the Level Sequence Actor
		m_LevelSequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>();

		if (m_LevelSequenceActor)
		{
			// Assign the cinematic sequence to the actor
			m_LevelSequenceActor->SetSequence(Cinematic);

			// Create Level Sequence Player
			m_CinematicPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Cinematic, FMovieSceneSequencePlaybackSettings(), m_LevelSequenceActor);

			if (m_CinematicPlayer)
			{
				// Set the cinematic variables
				SetCinematicFlags(PlayerRelevant, InGame, ShotFragmented);

				// Bind the player's character only if it's not shot fragmented or if it's the last shot
				if(!ShotFragmented || (ShotFragmented && Cinematic == m_CinematicInfo->LastShotSequence))
				{ 
					BindPlayerToCinematicActor(Cinematic);
				}

				/** We temporize the scene */

				// Schedule the end of the cinematic by calculating the cinematic end time
				UMovieScene* MovieScene = Cinematic->GetMovieScene();
				FFrameRate TickResolution = MovieScene->GetTickResolution();

				// We calculate teh end time dividing the last cinematic frame by the tick resolution (Frames / Frames per seconds = Seconds time)
				float CinematicEndTime = MovieScene->GetPlaybackRange().GetUpperBoundValue() / TickResolution;
				GetWorld()->GetTimerManager().SetTimer(m_CinematicTimerHandle, this, &UCinematicsSubsystem::EndCinematic, CinematicEndTime, false);

				// Set the playing cinematic to true
				IsCinematicPlaying = true;

				// Broadcast delegate event to notify the start of the cinematic
				OnStartCinematic.Broadcast();

				// Play the cinematic
				m_CinematicPlayer->Play();
			}
			else
			{
				//DustyUtils::PrintMessage("Unable to create level sequence player");
			}
		}
	}
}


void UCinematicsSubsystem::PlayBenchCinematic(FString CinematicName, bool PlayerRelevant, bool InGame, bool ShotFragmented)
{
	// Initialize player
	m_PlayerCharacter = Cast<ADustyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	ULevelSequence* Cinematic = GetCinematic(CinematicName);

	if (!Cinematic || !m_PlayerCharacter)
	{
		//DustyUtils::PrintMessage("Cinematic or Player is null");
		return;
	}
	else
	{
		// Save the original camera transform
		m_PlayerSpringArm = m_PlayerCharacter->FindComponentByClass<USpringArmComponent>();

		if (m_PlayerSpringArm)
		{
			m_OriginalCameraLocation = m_PlayerSpringArm->GetRelativeLocation();
			m_OriginalCameraRotation = m_PlayerSpringArm->GetRelativeRotation();
		}

		// Create the Level Sequence Actor
		m_LevelSequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>();

		if (m_LevelSequenceActor)
		{
			// Assign the cinematic sequence to the actor
			m_LevelSequenceActor->SetSequence(Cinematic);

			// Create Level Sequence Player
			m_CinematicPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Cinematic, FMovieSceneSequencePlaybackSettings(), m_LevelSequenceActor);

			if (m_CinematicPlayer)
			{
				// Set the cinematic variables
				SetCinematicFlags(PlayerRelevant, InGame, ShotFragmented);

				// Bind the player's character only if it's not shot fragmented or if it's the last shot
				if (!ShotFragmented || (ShotFragmented && Cinematic == m_CinematicInfo->LastShotSequence))
				{
					BindPlayerToCinematicActor(Cinematic);
				}

				// Play the bench cinematic in loop until we skip the scene
				if (!m_PlayerCharacter->SkipButtonPressed)
				{
					// Set the playing cinematic to true
					IsCinematicPlaying = true;

					// Broadcast delegate event to notify the start of the cinematic
					OnStartCinematic.Broadcast();

					// Play the cinematic
					m_CinematicPlayer->PlayLooping();
				}
			}
			else
			{
				//DustyUtils::PrintMessage("Unable to create level sequence player");
			}
		}
	}
}

void UCinematicsSubsystem::SetCinematicFlags(bool PlayerRelevant, bool InGame, bool ShotFragmented)
{
	m_PlayerRelevant = PlayerRelevant;
	m_InGame = InGame;
	m_ShotFragmented = ShotFragmented;
}

void UCinematicsSubsystem::BindPlayerToCinematicActor(ULevelSequence* Cinematic)
{
	// Bind the player's character to the sequence by tag if relevant
	if (m_PlayerRelevant)
	{
		FMovieSceneObjectBindingID BindingID = Cinematic->FindBindingByTag("Player");

		if (BindingID.IsValid())
		{
			m_LevelSequenceActor->SetBindingByTag("Player", { m_PlayerCharacter });
		}
	}
}

void UCinematicsSubsystem::EndCinematic()
{
	FTransform FinalTransform = FTransform::Identity;

	if (m_PlayerRelevant)
	{
		if (m_InGame)
		{
			// Set the final transform using the sequence 
			FinalTransform = GetFinalCharacterTransform(m_CurrentSequence);
		}
		else
		{
			// Obtain the transform from the Cinematic Trigger
			FinalTransform = m_AfterCinematicTransform;
		}
	}

	if (m_PlayerCharacter)
	{
		// Reset the player's camera transform to the original location and rotation
		if (m_PlayerSpringArm)
		{
			m_PlayerSpringArm->SetRelativeLocation(m_OriginalCameraLocation);
			m_PlayerSpringArm->SetRelativeRotation(m_OriginalCameraRotation);
		}

		// Set the state to ground movement again
		m_PlayerCharacter->ActionsFilterComponent->SetCurrentState(UGroundMovementState::StaticClass());
	}

	// Stop the cinematic sequence player actor
	if (m_CinematicPlayer)
	{
		m_CinematicPlayer->Stop();
		m_CinematicPlayer = nullptr;
	}

	// Destroy the Level Sequence Actor
	if (m_LevelSequenceActor)
	{
		m_LevelSequenceActor->Destroy();
		m_LevelSequenceActor = nullptr;
	}

	// Set the final transform of the player character after destroying the Level Sequence Actor
	if (FinalTransform.IsValid())
	{
		if (!m_CurrentCinematicName.Equals("LS_Intro_Master"))
		{
			m_PlayerCharacter->SetActorLocationAndRotation(FinalTransform.GetLocation(), FinalTransform.GetRotation().Rotator());
		}
	}

	// Clear the timer
	GetWorld()->GetTimerManager().ClearTimer(m_CinematicTimerHandle);

	// Set the is cinematic playing to false again
	IsCinematicPlaying = false;

	// Broadcast delegate event to notify the start of the cinematic
	OnEndCinematic.Broadcast();
}

FTransform UCinematicsSubsystem::GetFinalCharacterTransform(ULevelSequence* Cinematic)
{

	if (!Cinematic)
	{
		return FTransform::Identity;
	}

	UMovieScene* MovieScene = Cinematic->GetMovieScene();
	if (!MovieScene)
	{
		return FTransform::Identity;
	}

	FTransform FinalTransform = FTransform::Identity;

	// Get the player transform track
	for (const FMovieSceneBinding& Binding : MovieScene->GetBindings())
	{
		for (UMovieSceneTrack* Track : Binding.GetTracks())
		{
			if (UMovieScene3DTransformTrack* TransformTrack = Cast<UMovieScene3DTransformTrack>(Track))
			{
				// Check if the binding name is the Dusty Character blueprint
				if (Binding.GetName() == CinematicPlayerCharacterName)
				{
					// Get all the transform sections i the track
					const TArray<UMovieSceneSection*>& Sections = TransformTrack->GetAllSections();

					if (Sections.Num() > 0)
					{
						// Find the section with the maximum range
						UMovieScene3DTransformSection* LastSection = nullptr;
						FFrameNumber EndFrame = MovieScene->GetPlaybackRange().GetUpperBoundValue();

						// Find the section which covers the last sequence frame
						for (UMovieSceneSection* Section : Sections)
						{
							if (Section->GetRange().Contains(EndFrame))
							{
								// Handle open upper bound by using the end time of the sequence
								LastSection = Cast<UMovieScene3DTransformSection>(Section);
								break;
							}
						}

						// Evaluate the transform at the last frame of the last section
						if (LastSection)
						{
							return EvaluateTransformAtFrame(LastSection, EndFrame);
						}
					}
				}
			}
			;
		}
	}

	return FinalTransform;
}

FTransform UCinematicsSubsystem::EvaluateTransformAtFrame(UMovieScene3DTransformSection* TransformSection, FFrameNumber Frame)
{
	FTransform Transform = FTransform::Identity;
	FVector Location(0.f, 0.f, 0.f);
	FRotator Rotation(0.f, 0.f, 0.f);
	FVector Scale(1.f, 1.f, 1.f);
	double Value;

	// Evaluate Translation
	if (TransformSection->GetChannelProxy().GetChannel<FMovieSceneDoubleChannel>(0)->Evaluate(Frame, Value))
	{
		Location.X = Value;
	}
	if (TransformSection->GetChannelProxy().GetChannel<FMovieSceneDoubleChannel>(1)->Evaluate(Frame, Value))
	{
		Location.Y = Value;
	}
	if (TransformSection->GetChannelProxy().GetChannel<FMovieSceneDoubleChannel>(2)->Evaluate(Frame, Value))
	{
		Location.Z = Value;
	}

	// Evaluate Rotation
	if (TransformSection->GetChannelProxy().GetChannel<FMovieSceneDoubleChannel>(3)->Evaluate(Frame, Value))
	{
		Rotation.Roll = Value;
	}
	if (TransformSection->GetChannelProxy().GetChannel<FMovieSceneDoubleChannel>(4)->Evaluate(Frame, Value))
	{
		Rotation.Pitch = Value;
	}
	if (TransformSection->GetChannelProxy().GetChannel<FMovieSceneDoubleChannel>(5)->Evaluate(Frame, Value))
	{
		Rotation.Yaw = Value;
	}

	Transform.SetLocation(Location);
	Transform.SetRotation(FQuat(Rotation));

	return Transform;
}

ULevelSequence* UCinematicsSubsystem::GetCinematic(FString CinematicName)
{
	m_CurrentCinematicName = CinematicName;

	if (CinematicDataAsset)
	{

		m_CinematicInfo = CinematicDataAsset->CinematicData.Find(CinematicName);

		if (!m_CinematicInfo)
		{
			//DustyUtils::PrintMessage("Cinematic info not found");
			return nullptr;
		}

		if (m_ShotFragmented)
		{
			// The level sequence is recovered from the last shot level sequence from the cinematic info of the data asset
			m_CurrentSequence = m_CinematicInfo->LastShotSequence;
		}
		else
		{
			// The level sequence is a simple level sequence from the cinematic info of the data asset
			m_CurrentSequence = m_CinematicInfo->CurrentSequence;
		}

		return m_CurrentSequence;
	}
	return nullptr;
}