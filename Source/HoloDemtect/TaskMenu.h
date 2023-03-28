#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Task.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"

#include "TaskMenu.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskButtonClickedEvent, FString, button);

// We make the class abstract, as we don't want to create
// instances of this, instead we want to create instances
// of our UMG Blueprint subclass.
UCLASS(Abstract)
class UTaskMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Doing setup in the C++ constructor is not as
	// useful as using NativeConstruct.
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TArray<UButton*> tasks;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* box;

	UPROPERTY(meta = (BindWidget))
	UButton* buttonTemplate;

	UFUNCTION(BlueprintCallable, Category = "HoloDemtect")
	void setTasks(TMap<FString, FTaskInfo> taskInfos);

	UFUNCTION(BlueprintCallable, Category = "HoloDemtect")
	void OnButtonClicked();

	UPROPERTY(BlueprintAssignable, Category = "Custom")
		FTaskButtonClickedEvent OnButtonClickedEvent;
public:

};