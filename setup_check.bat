@echo off
echo ========================================
echo Smart Parking System - Setup Checker
echo ========================================
echo.

echo Checking for MSYS2 installation...
if exist "C:\msys64\mingw64\bin\gcc.exe" (
    echo [OK] MSYS2 found at C:\msys64
    echo.
    echo To build this project:
    echo 1. Open "MSYS2 MinGW 64-bit" terminal
    echo 2. Run: cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"
    echo 3. Run: make
    echo 4. Run: ./smart_parking.exe
) else (
    echo [X] MSYS2 not found
    echo.
    echo Please install MSYS2:
    echo 1. Download from: https://www.msys2.org/
    echo 2. Install to default location
    echo 3. Open "MSYS2 MinGW 64-bit" terminal
    echo 4. Run: pacman -Syu
    echo 5. Run: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkg-config make
    echo.
    echo See QUICK_START.md for detailed instructions.
)
echo.
pause

