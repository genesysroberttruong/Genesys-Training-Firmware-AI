# Attendee prep — do this BEFORE the workshop

You need three things installed and one folder opened in a container. Budget
~20 minutes; the container image download is the slow part, so do it the day
before on a good connection.

## 1. Install the tools (Windows)

1. **Docker Desktop** — https://www.docker.com/products/docker-desktop/
   - After installing, launch it once and let it finish starting (whale icon
     in the system tray stops animating). On Windows it uses the WSL 2 backend;
     accept the prompt to install/update WSL if asked.
2. **Visual Studio Code** — https://code.visualstudio.com/
3. **Dev Containers** extension for VS Code
   - In VS Code: Extensions (Ctrl+Shift+X) → search **"Dev Containers"**
     (publisher: Microsoft) → Install.

## 2. Get the project

Copy the `Training_AI_Demo` folder to your machine (or clone it if it's in a
repo). Remember where you put it.

## 3. Open it in the container (this pre-downloads everything)

1. Open the `Training_AI_Demo` folder in VS Code (File → Open Folder).
2. VS Code will pop up **"Reopen in Container"** (bottom-right). Click it.
   - No popup? Press `F1` → type **"Dev Containers: Reopen in Container"** → Enter.
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
