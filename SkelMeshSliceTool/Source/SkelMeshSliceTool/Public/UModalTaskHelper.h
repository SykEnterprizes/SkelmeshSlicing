#pragma once
#include "CoreMinimal.h"
#include "Misc/ScopedSlowTask.h"

#include "UModalTaskHelper.Generated.h"

DECLARE_DELEGATE_OneParam(FProgressCallback, float);
DECLARE_DELEGATE_TwoParams(FProgressCallbackWithText, float /*Progress*/, FString /*StepDescription*/);

UCLASS()
class SKELMESHSLICETOOL_API UModalTaskHelper : public UObject

{
	GENERATED_BODY()

public:
	/** Cosmetic modal with progress bar while task runs */

	static void ShowModalWhileRunning(const FString& DialogText, float DelaySeconds);

	/** Error popup with optional cancel button */
	static bool ShowErrorPopup(const FString& Message, bool bAllowCancel = false);

	static bool ShowBlockingConfirmation(const FString& Title, const FString& Message);

	static void RunWithConfirmationIf(bool bCondition, const FString& Title, const FString& Message, TFunction<void()> TaskToRun);

	/** Lightweight progress callback wrapper */
	class FProgressReporter
	{
	public:
		FProgressReporter(int32 InTotalSteps, const FString& InDialogText);

		/** Returns a callback you can drop into loops */
		FProgressCallback GetCallback();

		/** Manual progress update with optional weight */
		void ReportProgress(float StepWeight = 1.0f);

		/** Manual progress update with description */
		void ReportProgressWithText(float StepWeight, const FString& StepDescription);

	private:
		int32 TotalSteps;
		float CurrentProgress;
		FScopedSlowTask SlowTask;
	};


	/** Creates a scoped progress reporter with modal dialog */
	static TSharedPtr<FProgressReporter> CreateProgressReporter(int32 TotalSteps, const FString& DialogText);

};

