@echo off
:: ----------------------------------------------------
:: universal_compiler.bat
:: Compiles all .cpp files in the ./source directory
:: and outputs them to the ./executables folder.
:: Usage examples:
::   universal_compiler.bat
::   universal_compiler.bat myprogram.exe
:: ----------------------------------------------------

:: Change to the directory where this .bat file is located,
:: so relative paths work even if user calls from elsewhere.
cd /D "%~dp0"

:: If no argument given, use a default output name
set "DEFAULT_EXE=main.exe"

if "%~1"=="" (
    echo [INFO] No output name specified, using default: %DEFAULT_EXE%.
    set "OUT_EXE=%DEFAULT_EXE%"
) else (
    set "OUT_EXE=%~1"
)

:: The -Ilibrary means "look in the 'library' folder for headers (like memory_utill.hpp, string_utill.hpp)"
:: Adjust if your headers are inside a different folder.
:: -Wall -Wextra -g3 are recommended flags for warnings + debugging info.
echo [INFO] Compiling all .cpp in ./source to create .\executables\%OUT_EXE% ...

g++ -Wall -Wextra -g3 ^
   -Ilibrary ^
   source\*.cpp ^
   -o executables\%OUT_EXE%

if %ERRORLEVEL%==0 (
    echo [SUCCESS] Compilation succeeded. EXE is at .\executables\%OUT_EXE%.
) else (
    echo [FAIL] Compilation failed!
)
pause
