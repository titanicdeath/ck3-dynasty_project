@echo off
:: -------------------------------------------------------------------
:: universal_compiler.bat
:: A universal C++ compiler batch script for Windows.
:: If you don't provide an argument, it defaults to "prototype_version-4_parser.cpp".
::
:: Usage examples:
::    universal_compiler.bat
::    universal_compiler.bat my_other_file.cpp
:: -------------------------------------------------------------------

set "DEFAULT_FILE=prototype_version-4_parser.cpp"

if "%~1"=="" (
    echo [INFO] No .cpp file specified. Defaulting to %DEFAULT_FILE%.
    set "TARGET_FILE=%DEFAULT_FILE%"
) else (
    set "TARGET_FILE=%~1"
)

echo [INFO] Compiling "%TARGET_FILE%" ...
g++ -Wall -Wextra -g3 "%TARGET_FILE%" -o "%~n1.exe"

if %ERRORLEVEL%==0 (
    echo [SUCCESS] Compilation of "%TARGET_FILE%" succeeded!

    :: Uncomment the following lines if you want to auto-push to Git:
    :: git add .
    :: git commit -m "Auto-commit after successful compile of %TARGET_FILE%"
    :: git push origin main

    echo Run the program with: .\%~n1.exe
) else (
    echo [FAIL] Compilation failed.
)
pause
