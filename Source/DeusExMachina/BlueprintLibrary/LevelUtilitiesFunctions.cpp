#include "LevelUtilitiesFunctions.h"
#include <Kismet/GameplayStatics.h>
#include "EngineUtils.h"

AActor* ULevelUtilitiesFunctions::GetActorOfClassInSublevel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Sublevel, const TSubclassOf<AActor> ActorClass)
{
	// Return nothing if no sublevel provided
	if (!Sublevel.IsValid()) return nullptr;

	// Not sure of what this is doing but this is in the implementation of Unreal's GetActorOfClass
	// I think this has an utility in Unreal's profiling system
	QUICK_SCOPE_CYCLE_COUNTER(ULevelUtilitiesFunctions_GetActorOfClassInSublevel);

	// Return nothing if no class provided
	if (!ActorClass) return nullptr;

	// Get the ULevel from the SoftPtr UWorld
	const ULevel* Level = Sublevel.Get()->GetCurrentLevel();


	// Retrieve the world that contains all currently loaded actors from all currently loaded sublevels
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// Check all actors that inherit the provided class
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;
			
			// Check if the found actor is part of the provided sublevel
			if (IsActorPartOfSublevel(Actor, Level))
			{
				// Return the first actor that meets all conditions
				return Actor;
			}
		}
	}

	// Return nothing if no actor of the provided class has been found in the provided sublevel
	return nullptr;
}

void ULevelUtilitiesFunctions::GetAllActorsOfClassInSublevel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Sublevel, const TSubclassOf<AActor> ActorClass, TArray<AActor*>& OutActors)
{
	// Return nothing if no sublevel provided
	if (!Sublevel.IsValid()) return;

	// Not sure of what this is doing but this is in the implementation of Unreal's GetActorOfClass
	// I think this has an utility in Unreal's profiling system
	QUICK_SCOPE_CYCLE_COUNTER(ULevelUtilitiesFunctions_GetAllActorsOfClassInSublevel);

	// Return nothing if no class provided
	if (!ActorClass) return;

	// Get the ULevel from the SoftPtr UWorld
	const ULevel* Level = Sublevel.Get()->GetCurrentLevel();


	// Retrieve the world that contains all currently loaded actors from all currently loaded sublevels
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// Check all actors that inherit the provided class
		for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
		{
			AActor* Actor = *It;

			// Check if the found actor is part of the provided sublevel
			if (IsActorPartOfSublevel(Actor, Level))
			{
				// Add to the OutActors list every actors that meets all conditions
				OutActors.Add(Actor);
			}
		}
	}
}


bool ULevelUtilitiesFunctions::IsActorPartOfSublevel(const AActor* Actor, const TSoftObjectPtr<UWorld> Sublevel)
{
	// Get the ULevel from the SoftPtr UWorld
	const ULevel* Level = Sublevel.Get()->GetCurrentLevel();

	return IsActorPartOfSublevel(Actor, Level);
}

bool ULevelUtilitiesFunctions::IsActorPartOfSublevel(const AActor* Actor, const ULevel* Sublevel)
{
	// Get the ULevel where the Actor has been created
	const ULevel* ActorLevel = Actor->GetLevel();

	return ActorLevel == Sublevel;
}
