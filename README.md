# HyprLauncher

A C++ application launcher built with GTK4 and gtk4-layer-shell, designed for Hyprland.
It features a modern design with rounded corners, transparency, and blur support, behaving similarly to Rofi but with a native GTK feel.

## Requirements

- **Arch Linux** (or any distro with Hyprland)
- `hyprland`
- `gtk4`
- `gtk4-layer-shell`
- `cmake`
- `gcc` or `clang`
- `git`
- `make`

## Installation Guide (Step-by-Step)

Follow these exact steps to install and run the launcher.

### 1. Clone the Repository

Open your terminal and run:

```bash
git clone https://github.com/DuckZ01/HyprLauncher.git
cd HyprLauncher
```

### 2. Build the Application

Create a build directory and compile the source code:

```bash
mkdir build
cd build
cmake ..
make
```

### 3. Test Run

You can run the launcher directly to see if it works:

```bash
./HyprLauncher
```

(Press `Esc` to close it).

### 4. Install (Optional)

You can move the binary to a global location if you prefer, or just keep it here.

```bash
sudo cp ./HyprLauncher /usr/local/bin/hyprlauncher
```

## Hyprland Configuration

To make the window transparent with a nice blur, and to open it with `Alt + Space`, edit your `hyprland.conf`.

### 1. Open Config

```bash
nano ~/.config/hypr/hyprland.conf
```

### 2. Add Window Rules

Add these lines to enable the blur effect for the launcher namespace:

```ini
# Blur and styling for HyprLauncher
layerrule = blur, launcher
layerrule = ignorezero, launcher
layerrule = animation popin 80%, launcher
```

### 3. Add Keybinding

Add this line to bind `Alt + Space` to open the launcher:

```ini
# Replace /path/to/HyprLauncher with the actual path, or just 'hyprlauncher' if installed globally
bind = ALT, SPACE, exec, /path/to/HyprLauncher/build/HyprLauncher
# OR if you installed to /usr/local/bin:
# bind = ALT, SPACE, exec, hyprlauncher
```

### 4. Reload Hyprland

Save the config and reload Hyprland (usually `Super + M` or `hyprctl reload`).

Now press `Alt + Space`, and your launcher should appear in the center of the screen!
