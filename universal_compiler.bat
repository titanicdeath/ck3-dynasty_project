@echo off
:: -------------------------------------------------------------------
:: universal_compiler.bat
:: A universal C++ compiler batch script for Windows.
:: Usage: universal_compiler.bat <filename.cpp>
::
:: Includes optional commented-out lines that auto-commit and push
:: changes to Git if compilation succeeds.
:: -------------------------------------------------------------------

if "%~1"=="" (
    echo [ERROR] No .cpp file specified.
    echo Usage: universal_compiler.bat <filename.cpp>
    pause
    exit /b 1
)

echo [INFO] Compiling "%~1" ...
g++ -Wall -Wextra -g3 "%~1" -o "%~n1.exe"

if %ERRORLEVEL%==0 (
    echo [SUCCESS] Compilation of "%~1" succeeded!

    :: Uncomment the following lines if you want to auto-push to Git:
    :: git add .
    :: git commit -m "Auto-commit after successful compile of %~1"
    :: git push origin main

    echo Run the program with: ".\%~n1.exe"
) else (
    echo [FAIL] Compilation failed.
)
pause
