#include "Inventory/Inventory.h"

UInventory::UInventory()
{
    PrimaryComponentTick.bCanEverTick = false;
}

int32 UInventory::LowerBound(const FName& ItemId) const
{
    int32 L = 0, R = Items.Num();
    while (L < R)
    {
        int32 M = (L + R) >> 1;
        if (Items[M].ItemId.LexicalLess(ItemId)) L = M + 1; else R = M;
    }
    return L;
}

bool UInventory::TryGetItem(const FName& ItemId, FInventoryItem& OutItem) const
{
    int32 i = LowerBound(ItemId);
    if (i < Items.Num() && Items[i].ItemId == ItemId)
    {
        OutItem = Items[i];
        return true;
    }
    return false;
}

bool UInventory::HasItem(const FName& ItemId, int32 MinQty) const
{
    FInventoryItem Tmp;
    return TryGetItem(ItemId, Tmp) && Tmp.Quantity >= MinQty;
}

void UInventory::AddItem(const FName& ItemId, EItemType Type, int32 DeltaQty, FText DisplayName)
{
    if (DeltaQty == 0) return;

    int32 i = LowerBound(ItemId);
    if (i < Items.Num() && Items[i].ItemId == ItemId)
    {
        Items[i].Quantity = FMath::Max(0, Items[i].Quantity + DeltaQty);
        if (!DisplayName.IsEmpty()) Items[i].DisplayName = DisplayName;
        if (Type != EItemType::Other) Items[i].Type = Type;

        if (Items[i].Quantity == 0) Items.RemoveAt(i);
    }
    else if (DeltaQty > 0)
    {
        FInventoryItem NewItem;
        NewItem.ItemId = ItemId;
        NewItem.Type = Type;
        NewItem.Quantity = DeltaQty;
        NewItem.DisplayName = DisplayName;
        Items.Insert(MoveTemp(NewItem), i);
    }
}

bool UInventory::ConsumeItem(const FName& ItemId, int32 Qty)
{
    if (Qty <= 0) return true;

    int32 i = LowerBound(ItemId);
    if (i < Items.Num() && Items[i].ItemId == ItemId && Items[i].Quantity >= Qty)
    {
        Items[i].Quantity -= Qty;
        if (Items[i].Quantity == 0) Items.RemoveAt(i);
        return true;
    }
    return false;
}
