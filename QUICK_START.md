# Quick Start Guide - Build and Run

## ⚠️ Prerequisites Not Installed

You need to install MSYS2 first to build this project. Follow these steps:

## Step 1: Install MSYS2 (5 minutes)

1. **Download MSYS2**
   - Go to: https://www.msys2.org/
   - Download the installer (msys2-x86_64-*.exe)
   - Run the installer
   - Install to default location: `C:\msys64`

2. **After installation**, close the installer window

## Step 2: Open MSYS2 MinGW 64-bit Terminal

**IMPORTANT**: You MUST use "MSYS2 MinGW 64-bit" terminal, NOT regular MSYS2!

- Press `Windows Key`
- Type: `MSYS2 MinGW 64-bit`
- Click on "MSYS2 MinGW 64-bit" (the one with MinGW in the name)

## Step 3: Update MSYS2

In the MSYS2 MinGW 64-bit terminal, run:

```bash
pacman -Syu
```

- Press Enter when prompted
- Wait for it to complete (may take a few minutes)
- If it asks to close terminal, close and reopen MSYS2 MinGW 64-bit

## Step 4: Install Required Packages

Run these commands one by one (type `Y` and Enter when prompted):

```bash
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-gtk3
pacman -S mingw-w64-x86_64-pkg-config
pacman -S make
```

Or install all at once:
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gtk3 mingw-w64-x86_64-pkg-config make
```

## Step 5: Navigate to Your Project

In the MSYS2 MinGW 64-bit terminal:

```bash
cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"
```

## Step 6: Build the Project

```bash
make
```

## Step 7: Run the Application

```bash
./smart_parking.exe
```

## Troubleshooting

### "pkg-config: command not found"
- Make sure you're in MSYS2 MinGW 64-bit terminal (not regular MSYS2)
- Verify installation: `pacman -S mingw-w64-x86_64-pkg-config`

### "GTK+ not found"
- Install GTK3: `pacman -S mingw-w64-x86_64-gtk3`
- Verify: `pkg-config --modversion gtk+-3.0`

### DLL errors when running
- Run from MSYS2 MinGW 64-bit terminal
- The terminal automatically sets up the PATH

### Can't find project directory
- Use forward slashes: `/c/Users/...`
- Or use: `cd ~/Desktop/Smart\ Parking\ Management\ System` (if Desktop is accessible)

## Alternative: Check if Already Installed

If you think MSYS2 might already be installed, try:

```bash
# In MSYS2 MinGW 64-bit terminal:
which gcc
which pkg-config
pkg-config --modversion gtk+-3.0
```

If these work, you can skip to Step 5!

---

**Once MSYS2 is installed, come back and I'll help you build!**

