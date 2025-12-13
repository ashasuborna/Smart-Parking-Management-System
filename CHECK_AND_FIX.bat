@echo off
echo ========================================
echo Checking MSYS2 Installation
echo ========================================
echo.

REM Check if MSYS2 is installed
if not exist "C:\msys64" (
    echo [ERROR] MSYS2 not found at C:\msys64
    echo Please install MSYS2 first.
    pause
    exit /b 1
)

echo [OK] MSYS2 found at C:\msys64
echo.

REM Check if gcc exists
if exist "C:\msys64\mingw64\bin\gcc.exe" (
    echo [OK] GCC found
) else (
    echo [ERROR] GCC not found!
    echo You need to install: pacman -S mingw-w64-x86_64-gcc
)

REM Check if pkg-config exists
if exist "C:\msys64\mingw64\bin\pkg-config.exe" (
    echo [OK] pkg-config found
) else (
    echo [ERROR] pkg-config not found!
    echo You need to install: pacman -S mingw-w64-x86_64-pkg-config
)

echo.
echo ========================================
echo IMPORTANT: Use the CORRECT Terminal
echo ========================================
echo.
echo You MUST use "MSYS2 MinGW 64-bit" terminal!
echo.
echo Steps:
echo 1. Close any MSYS2 terminal you have open
echo 2. Press Windows Key
echo 3. Type: MSYS2 MinGW 64-bit
echo 4. Click it (NOT regular MSYS2!)
echo 5. Then run your commands
echo.
echo Opening the CORRECT terminal now...
echo.
pause

start "" "C:\msys64\mingw64.exe"

echo.
echo Terminal opened! Now run:
echo   cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"
echo   make
echo.
pause

