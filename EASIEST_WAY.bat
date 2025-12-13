@echo off
cls
echo ========================================
echo   SMART PARKING - EASIEST WAY TO RUN
echo ========================================
echo.
echo This will open MSYS2 terminal and do everything automatically.
echo.
echo Just follow these steps:
echo.
echo 1. A terminal window will open
echo 2. It will build the project automatically
echo 3. Then it will run the application
echo.
echo ========================================
echo.
pause

REM Create the build and run script
(
echo #!/bin/bash
echo clear
echo echo "========================================"
echo echo "Building Smart Parking Management System"
echo echo "========================================"
echo echo ""
echo cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"
echo echo "Current directory:"
echo pwd
echo echo ""
echo echo "Cleaning old build..."
echo make clean 2^>^&1
echo echo ""
echo echo "Building project (this may take 30 seconds)..."
echo make
echo echo ""
echo if [ -f "smart_parking.exe" ]; then
echo     echo "========================================"
echo     echo "BUILD SUCCESSFUL!"
echo     echo "========================================"
echo     echo ""
echo     echo "Starting application..."
echo     echo ""
echo     ./smart_parking.exe
echo else
echo     echo "========================================"
echo     echo "BUILD FAILED"
echo     echo "========================================"
echo     echo ""
echo     echo "Please check the errors above."
echo     echo ""
echo     read -p "Press Enter to exit..."
echo fi
) > %TEMP%\auto_build_run.sh

echo Opening MSYS2 terminal...
echo.
start "" "C:\msys64\mingw64.exe" -c "bash %TEMP%\auto_build_run.sh"

echo.
echo ========================================
echo Terminal opened! Watch it build and run.
echo ========================================
echo.
echo If the application opens, login with:
echo   Username: admin
echo   Password: admin123
echo.
pause

