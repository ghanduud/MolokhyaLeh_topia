#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Key        UMETA(DisplayName = "Key"),
    Quest      UMETA(DisplayName = "Quest"),
    Consumable UMETA(DisplayName = "Consumable"),
    Ammo       UMETA(DisplayName = "Ammo"),
    Resource   UMETA(DisplayName = "Resource"),
    Other      UMETA(DisplayName = "Other")
};

USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    FName ItemId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    EItemType Type = EItemType::Other;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 Quantity = 0;

    bool operator<(const FInventoryItem& Other) const
    {
        return ItemId.LexicalLess(Other.ItemId);
    }
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MOLOKHYALEH_API UInventory : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventory();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void AddItem(const FName& ItemId, EItemType Type, int32 DeltaQty, FText DisplayName = FText());

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool ConsumeItem(const FName& ItemId, int32 Qty);

    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool HasItem(const FName& ItemId, int32 MinQty = 1) const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool TryGetItem(const FName& ItemId, FInventoryItem& OutItem) const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    const TArray<FInventoryItem>& GetAll() const { return Items; }

private:
    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    TArray<FInventoryItem> Items;

    int32 LowerBound(const FName& ItemId) const;
};
