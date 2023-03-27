#include "TaskMenu.h"

void UTaskMenu::NativeConstruct()
{
	Super::NativeConstruct();
	

}


	void UTaskMenu::setTasks(TMap<FString,FTaskInfo> taskInfos){

	box->ClearChildren();

	//for taskInfo TMap iterate and create buttons with text

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
		//TextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(0,0,0,1)));
		TextBlock->SetText(FText::FromString(taskInfo.Value.text ));
		tasks[tasks.Num()-1]->AddChild(TextBlock);

		box->AddChild(tasks[tasks.Num() - 1]);

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