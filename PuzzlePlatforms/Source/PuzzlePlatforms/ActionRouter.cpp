#include "ActionRouter.h"

FToggleEvent ActionRouter::OnHost;
FJoinEvent ActionRouter::OnJoin;
FToggleEvent ActionRouter::OnLeave;
FToggleEvent ActionRouter::OnQuit;

FToggleEvent ActionRouter::OnHostOSS;
FJoinOSSEvent ActionRouter::OnJoinOSS;

FToggleEvent ActionRouter::RefreshServersToJoin;

FToggleEvent ActionRouter::OnStartGame;

FToggleEvent ActionRouter::OnWidgetConstruct;
FToggleEvent ActionRouter::OnWidgetDestruct;