# Build and Run Instructions

## Step 1: Verify Installation

In MSYS2 MinGW 64-bit terminal, run:
```bash
gcc --version
pkg-config --modversion gtk+-3.0
```

If these show version numbers, you're good to go!

## Step 2: Navigate to Project

```bash
cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"
```

## Step 3: Build the Project

```bash
make
```

This will compile all source files. You should see output like:
```
gcc -Wall -Wextra -std=c11 ... -c src/main.c -o src/main.o
...
gcc src/main.o ... -o smart_parking.exe
```

## Step 4: Run the Application

```bash
./smart_parking.exe
```

The GUI should open!

## Login Credentials

- Username: `admin`
- Password: `admin123`

## Troubleshooting

If you get errors:
- Make sure you're in MSYS2 MinGW 64-bit terminal
- Check you're in the correct directory: `pwd`
- Try: `make clean` then `make` again

