# Attendee prep — do this BEFORE the workshop

You need a few things installed and one folder opened in a container. Budget
~20 minutes; the container image download is the slow part, so do it the day
before on a good connection.

## 1. Install the tools (Windows)

1. **WSL 2** — Docker Desktop runs on top of this, so get it working first
   rather than mid-install.
   - Open **PowerShell as Administrator** and run:

     ```powershell
     wsl --install
     ```

     This enables the required Windows features, installs WSL 2, and adds a
     default Linux distro. **Restart when prompted.** (Requires Windows 10
     version 2004+ or Windows 11. If the command isn't found, run
     `winver` to check your version and update Windows first.)
   - **On first launch the Linux distro opens a terminal and asks you to create
     a UNIX username and password.** Enter any values you like — they're local
     to that distro, separate from your Windows login. Remember the password:
     you'll need it for `sudo` commands inside Linux. (The password field stays
     blank as you type — that's normal.)
   - Already have WSL installed from something else? Make sure it's up to
     date and version 2 is the default:

     ```powershell
     wsl --update
     wsl --set-default-version 2
     ```
   - Verify it worked:

     ```powershell
     wsl --status
     ```

     Should report **Default Version: 2**. If it says 1, run
     `wsl --set-default-version 2` and check again.
2. **Docker Desktop** — https://www.docker.com/products/docker-desktop/
   - After installing, launch it once and let it finish starting (whale icon
     in the system tray stops animating).
   - Settings → General → confirm **"Use the WSL 2 based engine"** is
     checked (it's on by default on a fresh install, but worth a glance).
   - Docker adds its own internal `docker-desktop` distro. **It must not be your
     default distro.** Check with `wsl --list` — if it shows
     **`docker-desktop (Default)`**, point the default back at your real Linux
     distro, or Dev Containers will fail with "unsupported WSL distribution":

     ```powershell
     wsl --set-default Ubuntu
     ```
3. **Visual Studio Code** — https://code.visualstudio.com/
4. **Dev Containers** extension for VS Code
   - In VS Code: Extensions (Ctrl+Shift+X) → search **"Dev Containers"**
     (publisher: Microsoft) → Install.

## 2. Get the project

Clone it from GitHub. You'll need **Git** — most machines have it; if not, grab
it from https://git-scm.com/download/win. In a terminal (PowerShell or Git
Bash):

```bash
git clone https://github.com/genesysroberttruong/Genesys-Training-Firmware-AI.git
cd Genesys-Training-Firmware-AI
```

Remember where you cloned it — that's the folder you'll open in VS Code.

## 3. Open it in the container (this pre-downloads everything)

1. Open the `Genesys-Training-Firmware-AI` folder in VS Code (File → Open Folder).
2. VS Code will pop up **"Reopen in Container"** (bottom-right). Click it.
   - No popup? Press **Ctrl+Shift+P** to open the Command Palette → type
     **"Dev Containers: Reopen in Container"** → Enter.
3. Wait for the image to build. **The first build downloads ~1–2 GB** and can
   take 10–20 minutes. Later starts are seconds.
4. When it finishes, open a terminal in VS Code (`` Ctrl+` ``) and confirm the
   toolchain is present:

   ```bash
   make run
   ```

   You should see the banner and a stream of `[t=...] raw=...` telemetry lines.
   Quit QEMU with **Ctrl-A then X**.

## 4. Sign in to Claude Code

The **Claude Code** extension is installed automatically inside the container.
Open it from the VS Code activity bar (or run `claude` in the terminal) and sign
in **before** the session so we don't lose time on auth.

## You're ready

If `make run` printed telemetry and Claude Code is signed in, you're set. If
anything failed, message the facilitator ahead of time — not on the day.
