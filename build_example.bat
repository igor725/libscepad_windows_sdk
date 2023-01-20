@echo off

if not exist libScePad.lib (
	call .\create_lib.bat
	if %errorlevel% neq 0 (
		echo Failed to generate libScePad.lib
		exit /b 1
	)
)

cl /Feexample.exe example.cpp libScePad.lib
exit /b %errorlevel%
