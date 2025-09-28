#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnlockKey.generated.h"

UCLASS()
class MOLOKHYALEH_API AUnlockKey : public AActor
{
	GENERATED_BODY()
	
public:	
	AUnlockKey();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
