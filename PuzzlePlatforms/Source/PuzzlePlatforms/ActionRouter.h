#pragma once

#include "Delegates/DelegateCombinations.h"
#include "Containers/UnrealString.h"

DECLARE_DELEGATE(FToggleEvent);
DECLARE_DELEGATE_OneParam(FJoinEvent, const FString& Address);
DECLARE_DELEGATE_OneParam(FJoinOSSEvent, int32 index);

namespace ActionRouter
{
	extern FToggleEvent OnHost;
	extern FJoinEvent OnJoin;
	extern FToggleEvent OnLeave;
	extern FToggleEvent OnQuit;

	extern FToggleEvent OnHostOSS;
	extern FJoinOSSEvent OnJoinOSS;

	extern FToggleEvent OnWidgetConstruct;
	extern FToggleEvent OnWidgetDestruct;
}