# PowerShell script to download MSYS2 installer
# Run this script, then follow the prompts

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "MSYS2 Installation Helper" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check if MSYS2 already exists
if (Test-Path "C:\msys64") {
    Write-Host "[INFO] MSYS2 directory found at C:\msys64" -ForegroundColor Yellow
    $response = Read-Host "Do you want to reinstall? (y/n)"
    if ($response -ne "y") {
        Write-Host "Skipping download. MSYS2 may already be installed." -ForegroundColor Green
        Write-Host "Try opening 'MSYS2 MinGW 64-bit' from Start Menu." -ForegroundColor Green
        exit
    }
}

Write-Host "This script will download the MSYS2 installer." -ForegroundColor Yellow
Write-Host "After download, you'll need to:" -ForegroundColor Yellow
Write-Host "  1. Run the installer (requires admin rights)" -ForegroundColor Yellow
Write-Host "  2. Install to C:\msys64" -ForegroundColor Yellow
Write-Host "  3. Follow the post-installation steps" -ForegroundColor Yellow
Write-Host ""

$response = Read-Host "Continue with download? (y/n)"
if ($response -ne "y") {
    Write-Host "Cancelled." -ForegroundColor Red
    exit
}

# MSYS2 download URL (update if needed)
$msys2Url = "https://github.com/msys2/msys2-installer/releases/latest/download/msys2-x86_64-latest.exe"
$downloadPath = "$env:USERPROFILE\Downloads\msys2-installer.exe"

Write-Host ""
Write-Host "Downloading MSYS2 installer..." -ForegroundColor Cyan
Write-Host "URL: $msys2Url" -ForegroundColor Gray
Write-Host "Save to: $downloadPath" -ForegroundColor Gray
Write-Host ""

try {
    # Download the installer
    Invoke-WebRequest -Uri $msys2Url -OutFile $downloadPath -UseBasicParsing
    
    Write-Host "[SUCCESS] Download complete!" -ForegroundColor Green
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "Next Steps:" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host ""
    Write-Host "1. Run the installer:" -ForegroundColor Yellow
    Write-Host "   $downloadPath" -ForegroundColor White
    Write-Host ""
    Write-Host "2. During installation:" -ForegroundColor Yellow
    Write-Host "   - Install to: C:\msys64 (default)" -ForegroundColor White
    Write-Host "   - DO NOT check 'Run MSYS2 now' at the end" -ForegroundColor White
    Write-Host ""
    Write-Host "3. After installation:" -ForegroundColor Yellow
    Write-Host "   - Open 'MSYS2 MinGW 64-bit' from Start Menu" -ForegroundColor White
    Write-Host "   - Run: pacman -Syu" -ForegroundColor White
    Write-Host "   - Then run: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkg-config make" -ForegroundColor White
    Write-Host ""
    Write-Host "4. Build the project:" -ForegroundColor Yellow
    Write-Host "   cd '/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System'" -ForegroundColor White
    Write-Host "   make" -ForegroundColor White
    Write-Host "   ./smart_parking.exe" -ForegroundColor White
    Write-Host ""
    
    $open = Read-Host "Open Downloads folder to run installer? (y/n)"
    if ($open -eq "y") {
        Start-Process "explorer.exe" -ArgumentList "/select,`"$downloadPath`""
    }
    
} catch {
    Write-Host "[ERROR] Download failed!" -ForegroundColor Red
    Write-Host $_.Exception.Message -ForegroundColor Red
    Write-Host ""
    Write-Host "Manual download:" -ForegroundColor Yellow
    Write-Host "1. Go to: https://www.msys2.org/" -ForegroundColor White
    Write-Host "2. Download the installer manually" -ForegroundColor White
    Write-Host "3. Run it and install to C:\msys64" -ForegroundColor White
}

