@echo off
rem Drift for OS/2 - Open Watcom build script
rem Run from the project directory on ArcaOS

set LOGFILE=compile-wat.log
echo. > %LOGFILE%
echo === Drift for OS/2 - OpenWatcom build === >> %LOGFILE%
echo Started: %DATE% %TIME% >> %LOGFILE%

rem Locate Open Watcom
if exist c:\watcom\binp\wcc386.exe set WATCOM=c:\watcom
if exist c:\watcom2\binp\wcc386.exe set WATCOM=c:\watcom2

if "%WATCOM%" == "" (
    echo ERROR: Open Watcom not found. Set WATCOM env var.
    echo ERROR: Open Watcom not found >> %LOGFILE%
    exit /b 1
)

rem Locate OS/2 Toolkit
if "%OS2TK%" == "" set OS2TK=c:\os2tk45

echo Using WATCOM=%WATCOM% >> %LOGFILE%
echo Using OS2TK=%OS2TK%   >> %LOGFILE%

rem Add Watcom to PATH
set PATH=%WATCOM%\binp;%WATCOM%\binw;%PATH%
set INCLUDE=%WATCOM%\h;%OS2TK%\h

rem Clean previous build
echo --- Clean --- >> %LOGFILE%
wmake -f makefile.wat clean >> %LOGFILE% 2>&1

rem Build
echo --- Build --- >> %LOGFILE%
wmake -f makefile.wat all >> %LOGFILE% 2>&1

if errorlevel 1 (
    echo BUILD FAILED - check %LOGFILE%
    echo BUILD FAILED >> %LOGFILE%
    exit /b 1
)

echo BUILD OK >> %LOGFILE%
echo BUILD OK - driftos2.exe ready.
echo Check %LOGFILE% for details.
