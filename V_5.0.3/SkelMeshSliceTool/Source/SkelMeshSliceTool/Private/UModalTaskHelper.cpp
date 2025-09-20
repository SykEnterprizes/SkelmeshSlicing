
#include "UModalTaskHelper.h"
#include "Misc/MessageDialog.h"
#include <Async/Async.h>

////////////////////////////////////////////////////////////
// Modal dialogue popups with confirmation acknowledgements

// runs async, so doesnt block the main thread, allows for a progress bar for long tasks
void UModalTaskHelper::ShowModalWhileRunning(const FString& DialogText, float DelaySeconds)
{
	AsyncTask(ENamedThreads::GameThread, [DialogText, DelaySeconds]()
		{
			FScopedSlowTask SlowTask(1.0f, FText::FromString(DialogText));
			SlowTask.MakeDialog(true);
			SlowTask.EnterProgressFrame(1.0f);

			// Use a timer manager if available, or sleep if safe
			FPlatformProcess::Sleep(DelaySeconds);
		});
}
// a quick pop up error message 
bool UModalTaskHelper::ShowErrorPopup(const FString& Message, bool bAllowCancel)
{
	FText PopupText = FText::FromString(Message);

	if (bAllowCancel)
	{
		EAppReturnType::Type Result = FMessageDialog::Open(EAppMsgType::OkCancel, PopupText);
		return Result == EAppReturnType::Ok;
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, PopupText);
		return true;
	}
}
// blocks the main thread until the user hits the prompt to acknowledge
bool UModalTaskHelper::ShowBlockingConfirmation(const FString& Title, const FString& Message)
{
	TSharedRef<SWindow> DialogWindow = SNew(SWindow)
		.Title(FText::FromString(Title))
		.ClientSize(FVector2D(400, 150))
		.SupportsMinimize(false)
		.SupportsMaximize(false);

	bool bConfirmed = false;

	DialogWindow->SetContent(
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().Padding(10).AutoHeight()
		[
			SNew(STextBlock)
				.Text(FText::FromString(Message))
				.WrapTextAt(380)
		]
		+ SVerticalBox::Slot().Padding(10).AutoHeight().HAlign(HAlign_Right)
		[
			SNew(SButton)
				.Text(FText::FromString(TEXT("OK")))
				.OnClicked_Lambda([&DialogWindow, &bConfirmed]()
					{
						bConfirmed = true;
						DialogWindow->RequestDestroyWindow();
						return FReply::Handled();
					})
		]
	);

	FSlateApplication::Get().AddModalWindow(DialogWindow, nullptr);

	return bConfirmed;
}
// blocks the thread if the condition is met, and continues the code execution after a user hits the acknowledge prompt
void UModalTaskHelper::RunWithConfirmationIf(bool bCondition, const FString& Title, const FString& Message, TFunction<void()> TaskToRun)
{
	bool bUserConfirmed = true;

	if (bCondition)
	{
		FText PopupText = FText::FromString(Message);
		FText TitleText = FText::FromString(Title);
		EAppReturnType::Type Result = FMessageDialog::Open(EAppMsgType::OkCancel, PopupText, &TitleText);
		bUserConfirmed = (Result == EAppReturnType::Ok);
	}

	if (bUserConfirmed && TaskToRun)
	{
		TaskToRun();
	}
}

/////////////////////////////
// Progress reporter section

TSharedPtr<UModalTaskHelper::FProgressReporter> UModalTaskHelper::CreateProgressReporter(int32 TotalSteps, const FString& DialogText)
{
	return MakeShared<FProgressReporter>(TotalSteps, DialogText);
}

UModalTaskHelper::FProgressReporter::FProgressReporter(int32 InTotalSteps, const FString& InDialogText)
	: TotalSteps(InTotalSteps), CurrentProgress(0.0f), SlowTask(1.0f, FText::FromString(InDialogText))
{
	SlowTask.MakeDialog(true); // Show cancel button
}

FProgressCallback UModalTaskHelper::FProgressReporter::GetCallback()
{
	return FProgressCallback::CreateRaw(this, &FProgressReporter::ReportProgress);
}

void UModalTaskHelper::FProgressReporter::ReportProgress(float StepWeight)
{
	CurrentProgress += StepWeight;
	SlowTask.EnterProgressFrame(StepWeight / TotalSteps);
	FSlateApplication::Get().Tick(); // Forces UI update
	//FPlatformProcess::Sleep(0.01f);  // Optional: small delay to allow visual update

}

void UModalTaskHelper::FProgressReporter::ReportProgressWithText(float StepWeight, const FString& StepDescription)
{
	CurrentProgress += StepWeight;
	SlowTask.EnterProgressFrame(StepWeight / TotalSteps, FText::FromString(StepDescription));
	FSlateApplication::Get().Tick(); // Forces UI update
	//FPlatformProcess::Sleep(0.05f);  // Optional: small delay to allow visual update

}