// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/door.h"

// Sets default values
Adoor::Adoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Adoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Adoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

