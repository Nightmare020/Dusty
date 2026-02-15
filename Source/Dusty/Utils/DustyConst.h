// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**-
 *
 */

const FName DissolveDoorParam = TEXT("dissolve");
const FName KeyBaseParam = TEXT("Position");
const FName DissolveKeyControllerParam = TEXT("Emissive Intensity");
const FName RootParam = TEXT("Offset");
const FName TattooParam = TEXT("position");
const FName SpriteParamHUD = TEXT("SpriteHUD");
const FName AlphaParamHUD = TEXT("AlphaHUD");

const FName TopPoleBoneName = TEXT("Hueso_Top");
const FName BottomPoleBoneName = TEXT("Hueso_End");
const FName BallPoleBoneName = TEXT("Hueso_Bola");

const FString SAVE_SLOT_NAME = TEXT("SaveSlot");

UENUM(BlueprintType)
enum class PlayerStates : uint8 {
	None UMETA(DisplayName = "None"),
	Neutral UMETA(DisplayName = "Neutral"),
	Walking UMETA(DisplayName = "Walking"),
	Jogging UMETA(DisplayName = "Jogging"),
	Jumping UMETA(DisplayName = "Jumping"),
	Aiming UMETA(DisplayName = "Aiming"),
	PoleElevator UMETA(DisplayName = "PoleElevator"),
	PoleKey UMETA(DisplayName = "PoleKey"),
	PoleJumping UMETA(DisplayName = "PoleJumping"),
	Falling UMETA(DisplayName = "Falling"),
	Cinematic UMETA(DisplayName = "Cinematic"),
	MainMenu UMETA(DisplayName = "MainMenu"),
	Bench UMETA(DisplayName = "Bench"),
	Opening UMETA(DisplayName = "Opening"),
	Sitting UMETA(DisplayName = "Sitting"),
	Book UMETA(DisplayName = "Book"),
};

UENUM(BlueprintType)
enum class EGlyphs : uint8
{
	Time UMETA(DisplayName = "Time"),
	Animal UMETA(DisplayName = "Animal"),
	Light UMETA(DisplayName = "Light"),
	Road UMETA(DisplayName = "Road"),
	House UMETA(DisplayName = "House"),
	Person UMETA(DisplayName = "Person"),
	Darkness UMETA(DisplayName = "Darkness"),
	Stars UMETA(DisplayName = "Stars"),
};

UENUM(BlueprintType)
enum class EVocabulary : uint8
{
	City UMETA(DisplayName = "City"),
	Sky UMETA(DisplayName = "Sky"),
	Guide UMETA(DisplayName = "Guide"),
	Memory UMETA(DisplayName = "Memory"),
	Sacrifice UMETA(DisplayName = "Sacrifice")
};

UENUM(BlueprintType)
enum class EHudCanvas : uint8
{
	Crosshair UMETA(DisplayName = "Crosshair"),
	NewPoleSkill UMETA(DisplayName = "NewPoleSkill"),
	DiaryNotification UMETA(DisplayName = "DiaryNotification"),
	SingleButtonPrompt UMETA(DisplayName = "SingleButtonPrompt"),
	DoubleButtonPrompt UMETA(DisplayName = "DoubleButtonPrompt"),
	SaveGame UMETA(DisplayName = "SaveGame"),
	InitialBookButtons UMETA(DisplayName = "InitialBookButtons"),
	MainMenuSettingsButtons UMETA(DisplayName = "MainMenuSettingsButtons"),
	BookButtons UMETA(DisplayName = "BookButtons"),
	DiscButtons UMETA(DisplayName = "DiscButtons"),
	PlanetsButtons UMETA(DisplayName = "PlanetsButtons"),
	ReadMode UMETA(DisplayName = "ReadMode"),
	ReadModeButton UMETA(DisplayName = "ReadModeButton"),
	Tooltip UMETA(DisplayName = "Tooltip"),
};

UENUM()
enum class LG_Bookmarks
{
	LogBook = 0,
	Glyphs = 1,
	Collectibles = 2,
	Settings = 3,
	FinalQuestion = 4
};

UENUM()
enum class EPoleMovementType
{
	Jump,
	Elevator
};