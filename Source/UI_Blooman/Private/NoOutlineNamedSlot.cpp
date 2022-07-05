// Fill out your copyright notice in the Description page of Project Settings.


#include "NoOutlineNamedSlot.h"

TSharedRef<SWidget> UNoOutlineNamedSlot::RebuildDesignWidget(TSharedRef<SWidget> Content)
{
    // ああああ
    return SNew(SOverlay)

        + SOverlay::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [
            Content
        ];
}
