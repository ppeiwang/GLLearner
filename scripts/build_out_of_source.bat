cd ..
if exist build == FALSE
	md build

cd build && cmake -G"Visual Studio 16" ..

if NOT ["%errorlevel%"]==["0"] GOTO HANG_ON

@REM succ condition
@REM check/open solution
@IF EXIST .vs\openGL_show_case\v16\Browse.VC.opendb GOTO SUCC

start openGL_show_case.sln

@GOTO SUCC

:HANG_ON
@pause

:SUCC
@timeout /T 2
