#include "Styling/SlateColor.h"
#include "TaskMenu.h"

void UTaskMenu::NativeConstruct()
{
	Super::NativeConstruct();
	

}


	void UTaskMenu::setTasks(TMap<FString,FTaskInfo> taskInfos){

	box->ClearChildren();



	for (auto& taskInfo : taskInfos){

		FString text = taskInfo.Key;
		UButton* Task = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(),(TEXT("%s"),*text));
		//constructor copy of buttonTemplate instance into var task
		//UButton* Task = new UButton(buttonTemplate);
		Task->BackgroundColor = buttonTemplate->BackgroundColor;
		Task->SetStyle(buttonTemplate->WidgetStyle);
		Task->OnPressed.AddDynamic(this, &UTaskMenu::OnButtonClicked);


		tasks.Add(Task);
		
		tasks[tasks.Num()-1]->SetVisibility(ESlateVisibility::Visible);
		tasks[tasks.Num()-1]->SetIsEnabled(true);
		
		//Set width of UButon
		//tasks[tasks.Num()- 1]->SetMinDesiredWidth(200.0f);

		
		// Set text color an size
		//tasks[tasks.Num() 1]->SetBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));

		//tasks[i]->SetButtonText(FText::FromString(FString::Printf(TEXT("Botón %d"), i + 1)));
		//tasks[tasks.Num() - 1]->OnClicked.AddDynamic(this, &UTaskMenu::OnButtonClicked);
		//tasks[i]->SetText(FText::FromString(StringArray[i]));
		
		UTextBlock* TextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		//set text color to black
		TextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(0,0,0,1)));
		TextBlock->SetText(FText::FromString(taskInfo.Value.text ));
		tasks[tasks.Num()-1]->AddChild(TextBlock);

		box->AddChild(tasks[tasks.Num() - 1]);

	}


	/*// Recorrer los slots y establecer el tamaño de los botones
	for (UCanvasPanelSlot* Slot : Slots)
	{
		// Verificar si el widget del slot es un botón
		if (UButton* Button = Cast<UButton>(Slot->Content))
		{
			// Obtener el objeto FAnchorData del slot
			FAnchorData AnchorData = Slot->GetLayout();

			// Establecer el tamaño en X y Y del botón a 100 unidades
			AnchorData.Size = FSlateLayoutSize(ESlateSizeRule::Fixed, 100.f, 100.f);

			// Actualizar el objeto FAnchorData del slot
			Slot->SetLayout(AnchorData);
		}
	}*/
	
	TArray<UWidget*> ScrollBoxWidgets = box->GetAllChildren();
	// Iterar sobre los widgets y establecer el Padding en el slot correspondiente
	for (UWidget* Widget : ScrollBoxWidgets){
		if (UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(Widget->Slot)){
			// Establecer el Padding en el slot
			ScrollBoxSlot->SetPadding(FMargin(50.f, ScrollBoxSlot->Padding.Top, 50.f, ScrollBoxSlot->Padding.Bottom));
		}
	}

}

void UTaskMenu::OnButtonClicked() {

	for (int i = 0; i < tasks.Num(); i++) {
		if (tasks[i]->IsPressed()) {
			UE_LOG(LogTemp, Display, TEXT("Clicked button %d"), i);
			//Get name of the button that was clicked
			FString name = tasks[i]->GetName();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("Button %s Clicked!"),*name));
			OnButtonClickedEvent.Broadcast(name);
			break;
		}
	}

}