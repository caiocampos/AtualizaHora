@echo off
set retryCount=0

:SyncStart
if %retryCount == 20 goto SyncEnd
set /A retryCount=retryCount + 1

net start w32time
w32tm /resync /force
if errorlevel 1 goto SyncDelay
if errorlevel 0 goto SyncEnd

:SyncDelay
timeout 15 > NUL
goto SyncStart

:SyncEnd
net stop w32time
exit
