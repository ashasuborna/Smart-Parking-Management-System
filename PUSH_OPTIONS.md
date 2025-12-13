# How to Push to GitHub - Multiple Options

## Issue: "Write access not granted" (403 Error)

This means your token doesn't have the right permissions OR needs to be used differently.

## Option 1: Fix Token Permissions (Easiest)

1. Go to: https://github.com/settings/tokens
2. Find your token or create a new one
3. Make sure it has **"repo"** scope checked (full control of private repositories)
4. Save it

Then push using:
```bash
git push -u origin master
```
When asked for password, paste the token.

## Option 2: Use SSH (Recommended for Long-term)

### Step 1: Check if you have SSH key
```bash
ls ~/.ssh/id_rsa.pub
```

### Step 2: If no SSH key, create one:
```bash
ssh-keygen -t ed25519 -C "your_email@example.com"
```
(Press Enter 3 times to use defaults)

### Step 3: Copy your public key:
```bash
cat ~/.ssh/id_rsa.pub
```
Copy the output

### Step 4: Add to GitHub:
1. Go to: https://github.com/settings/keys
2. Click "New SSH key"
3. Paste your key
4. Save

### Step 5: Change remote to SSH:
```bash
git remote set-url origin git@github.com:ashasuborna/Smart-Parking-Management-System.git
```

### Step 6: Push:
```bash
git push -u origin master
```

## Option 3: Use GitHub CLI (gh)

If you have GitHub CLI installed:
```bash
gh auth login
gh repo set-default ashasuborna/Smart-Parking-Management-System
git push -u origin master
```

## Quick Fix: Try This First

Reset remote and use credential helper:
```bash
git remote set-url origin https://github.com/ashasuborna/Smart-Parking-Management-System.git
git config --global credential.helper wincred
git push -u origin master
```
(Use your token as password when prompted)

