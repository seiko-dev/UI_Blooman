// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/NamedSlot.h"
#include "NoOutlineNamedSlot.generated.h"

/**
 * Widget DeisgnerでOutline描かないだけのNamed Slot
 */
UCLASS(Abstract)
class UI_BLOOMAN_API UNoOutlineNamedSlot : public UNamedSlot
{
	GENERATED_BODY()
protected:

#if WITH_EDITOR
    virtual TSharedRef<SWidget> RebuildDesignWidget(TSharedRef<SWidget> Content) override;
#endif
};
