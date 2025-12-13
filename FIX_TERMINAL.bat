@echo off
echo ========================================
echo Fixing MSYS2 Terminal Issue
echo ========================================
echo.
echo The error means you're in the WRONG terminal.
echo.
echo SOLUTION:
echo.
echo 1. CLOSE the current terminal window
echo.
echo 2. Open the CORRECT terminal:
echo    - Press Windows Key
echo    - Type: MSYS2 MinGW 64-bit
echo    - Click on "MSYS2 MinGW 64-bit" (NOT regular MSYS2!)
echo.
echo 3. In the NEW terminal, run:
echo    cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"
echo    make
echo.
echo ========================================
echo.
echo Opening the CORRECT terminal for you now...
echo.
pause

REM Open the correct terminal
start "" "C:\msys64\mingw64.exe"

echo.
echo The correct terminal should have opened.
echo Now try the commands again!
echo.
pause

