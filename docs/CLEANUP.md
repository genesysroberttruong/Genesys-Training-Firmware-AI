# Cleanup & reset

Three levels, depending on what you're doing:

- **Between demo runs / sessions** → [reset the code](#1-reset-the-code-to-the-baseline).
- **Reclaim build space** → [remove build artifacts](#2-remove-build-artifacts).
- **After the workshop** → [reclaim Docker / WSL disk](#3-reclaim-docker--wsl-disk-after-the-workshop),
  or a [full teardown](#4-full-teardown).

---

## 1. Reset the code to the baseline

During the demos the AI edits files live (fixes the bug, adds `filter.c`, etc.).
Reset to the committed baseline before the next run.

```bash
git status                 # see what changed / what the AI created
git reset --hard HEAD      # discard edits to tracked files
git clean -fdx             # remove untracked + ignored files (build/, new modules)
git status                 # should say: "nothing to commit, working tree clean"
```

> ⚠️ `git reset --hard` and `git clean -fdx` **permanently discard** the demo
> work and everything not committed. That's the point here — but see below if you
> want to keep a run.

**Preview before deleting** (safer): `git clean -ndx` lists what *would* be
removed without touching anything.

**Want to keep a demo run?** Save it to a branch first, then return to baseline:

```bash
git switch -c demo-run-1 && git add -A && git commit -m "demo run 1"
git switch master
```

---

## 2. Remove build artifacts

Just the compiled output, leaving your source edits intact:

```bash
make clean                 # removes build/firmware.elf, .bin, tests, *.o
```

(`git clean -fdx` in step 1 already covers this, since `build/` is git-ignored.)

---

## 3. Reclaim Docker / WSL disk (after the workshop)

The dev container image is ~1–2 GB. To get that space back:

**a. Leave the container.** In VS Code: **Ctrl+Shift+P → "Dev Containers: Reopen
Folder Locally"**, or just close the window.

**b. Remove the container and image.** Easiest via **Docker Desktop** → the
**Containers** and **Images** tabs → delete the workshop entries (named
`vsc-genesys-training-firmware-ai-…`). Or from a terminal:

```bash
docker container prune           # remove stopped containers
docker images                    # find the vsc-... workshop image
docker rmi <image-id>            # remove it
```

**c. Reclaim everything Docker isn't using** (⚠️ affects *all* your Docker
projects, not just this one):

```bash
docker system prune -a
```

**d. Shrink the WSL virtual disk (optional).** Docker's data lives in a WSL
`.vhdx` that grows but doesn't auto-shrink. After pruning:

```powershell
wsl --shutdown
```

Then Docker Desktop → **Settings → Resources → Advanced → "Clean / Purge data"**,
or compact the disk with `Optimize-VHD` (needs Hyper-V) / `diskpart`.

---

## 4. Full teardown

Only if you're completely done and want the machine back as it was:

1. Delete the cloned `Genesys-Training-Firmware-AI` folder.
2. Docker Desktop → uninstall (if it was installed just for this).
3. Remove the WSL distro if it was added for this workshop:

   ```powershell
   wsl --list --verbose
   wsl --unregister Ubuntu          # ⚠️ permanently deletes that distro's data
   ```

Leave WSL 2 itself enabled — it's a Windows feature other tools use, and turning
it off buys you nothing.

---

## Quick reference

| Goal | Command |
|---|---|
| See what the AI changed | `git status` |
| Reset code to baseline | `git reset --hard HEAD && git clean -fdx` |
| Preview what clean removes | `git clean -ndx` |
| Save a run before resetting | `git switch -c demo-run-N && git add -A && git commit -m "…"` |
| Remove build output only | `make clean` |
| Free Docker space | `docker system prune -a` |
| Stop WSL (before compacting) | `wsl --shutdown` |
