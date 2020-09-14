@echo off
set back=%cd%
for /d %%i in (.\*) do (
	call .\tools\run_example.bat %%i
)
cd %back%