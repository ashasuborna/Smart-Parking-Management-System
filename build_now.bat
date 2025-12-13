@echo off
echo ========================================
echo Building Smart Parking Management System
echo ========================================
echo.

cd /d "%~dp0"

REM Check if MSYS2 is installed
if not exist "C:\msys64\mingw64.exe" (
    echo [ERROR] MSYS2 not found at C:\msys64
    echo Please install MSYS2 first.
    pause
    exit /b 1
)

echo [INFO] Using MSYS2 to build...
echo.

REM Use MSYS2 to run make
C:\msys64\usr\bin\bash.exe -lc "cd '/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System' && make"

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo Build successful!
    echo ========================================
    echo.
    echo To run the application:
    echo 1. Open "MSYS2 MinGW 64-bit" terminal
    echo 2. Run: cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"
    echo 3. Run: ./smart_parking.exe
    echo.
) else (
    echo.
    echo [ERROR] Build failed!
    echo.
    echo Please build manually in MSYS2 MinGW 64-bit terminal:
    echo   cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"
    echo   make
    echo.
)

pause

