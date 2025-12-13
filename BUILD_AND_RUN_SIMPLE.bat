@echo off
echo ========================================
echo Smart Parking - Quick Build and Run
echo ========================================
echo.
echo This will open MSYS2 terminal and build automatically.
echo.
pause

REM Create build script
(
echo #!/bin/bash
echo cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"
echo echo "Building..."
echo make clean
echo make
echo if [ -f "smart_parking.exe" ]; then
echo     echo ""
echo     echo "Build successful! Running application..."
echo     ./smart_parking.exe
echo else
echo     echo "Build failed. Check errors above."
echo     read -p "Press Enter to exit..."
echo fi
) > %TEMP%\quick_build.sh

REM Launch MSYS2 and run
start "" "C:\msys64\mingw64.exe" -c "bash %TEMP%\quick_build.sh"

echo.
echo MSYS2 terminal opened. Build will run there.
echo.

