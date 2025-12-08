# Hyprland App Launcher

A C++ application launcher built with GTK4 and gtk4-layer-shell, designed for Hyprland.

## Features

- Search and launch installed applications.
- Aesthetic design: Rounded corners, transparency, and blur support.
- optimized for Hyprland (Layer Shell).

## Prerequisites

- Arch Linux (or other distro) with Hyprland.
- `gtk4`
- `gtk4-layer-shell`
- `cmake`
- `gcc` or `clang`

## Build Instructions

1. Create a build directory:
   ```bash
   mkdir build && cd build
   ```
2. Configure and build:
   ```bash
   cmake ..
   make
   ```
3. Run:
   ```bash
   ./launcher
   ```

## Hyprland Configuration

To enable the blur effect, add this to your `hyprland.conf`:

```ini
layerrule = blur, launcher
layerrule = ignorezero, launcher
```

## Keybindings

You might want to bind a key to open the launcher:

```ini
bind = SUPER, D, exec, /path/to/launcher/build/launcher
```
