#include "Inventory/Inventory.h"

UInventory::UInventory()
{
    PrimaryComponentTick.bCanEverTick = false;
}

int32 UInventory::LowerBound(const FName& Key) const
{
    int32 L = 0, R = Items.Num();
    while (L < R)
    {
        int32 M = (L + R) >> 1;
        if (Items[M].Key.LexicalLess(Key)) L = M + 1; else R = M;
    }
    return L;
}

bool UInventory::TryGetItem(const FName& Key, FInventoryItem& OutItem) const
{
    int32 i = LowerBound(Key);
    if (i < Items.Num() && Items[i].Key == Key)
    {
        OutItem = Items[i];
        return true;
    }
    return false;
}

bool UInventory::HasItem(const FName& Key, int32 MinQty) const
{
    FInventoryItem Tmp;
    return TryGetItem(Key, Tmp) && Tmp.Quantity >= MinQty;
}

void UInventory::AddItem(const FName& Key, EItemType Type, int32 DeltaQty, FText DisplayName)
{
    if (DeltaQty == 0) return;

    int32 i = LowerBound(Key);
    if (i < Items.Num() && Items[i].Key == Key)
    {
        Items[i].Quantity = FMath::Max(0, Items[i].Quantity + DeltaQty);
        if (!DisplayName.IsEmpty()) Items[i].DisplayName = DisplayName;
        if (Type != EItemType::Other) Items[i].Type = Type;

        if (Items[i].Quantity == 0) Items.RemoveAt(i);
    }
    else if (DeltaQty > 0)
    {
        FInventoryItem NewItem;
        NewItem.Key = Key;
        NewItem.Type = Type;
        NewItem.Quantity = DeltaQty;
        NewItem.DisplayName = DisplayName;
        Items.Insert(MoveTemp(NewItem), i);
    }
}

bool UInventory::ConsumeItem(const FName& Key, int32 Qty)
{
    if (Qty <= 0) return true;

    int32 i = LowerBound(Key);
    if (i < Items.Num() && Items[i].Key == Key && Items[i].Quantity >= Qty)
    {
        Items[i].Quantity -= Qty;
        if (Items[i].Quantity == 0) Items.RemoveAt(i);
        return true;
    }
    return false;
}
