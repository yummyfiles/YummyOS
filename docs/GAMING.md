# YummyOS Gaming Guide

YummyOS comes pre-configured with gaming tools and optimizations. This guide covers everything you need to get gaming.

## What's Included

YummyOS ships with these gaming essentials pre-installed:

| Package | Purpose |
|---------|---------|
| **GameMode** | Automatic CPU/GPU performance optimization for games |
| **MangoHUD** | In-game performance overlay (FPS, temps, usage) |
| **Vulkan drivers** | Intel and AMD Vulkan drivers with 32-bit support |
| **Mesa** | Open-source graphics drivers with 32-bit support |

## Installing Steam

### Via Welcome App

The easiest way is through the YummyOS Welcome app on first boot:

1. Open the Welcome app from the application menu
2. Navigate to the **Gaming** page
3. Click **Install Steam**
4. Follow the on-screen instructions

### Via Command Line

```bash
# Enable multilib repository (required for 32-bit libraries)
sudo sed -i '/\[multilib\]/,/Include/s/^#//' /etc/pacman.conf
sudo pacman -Syu

# Install Steam
sudo pacman -S steam

# For NVIDIA users, install 32-bit drivers
sudo pacman -S lib32-nvidia-utils
```

### Via Flatpak

```bash
flatpak install flathub com.valvesoftware.Steam
```

## GameMode

GameMode automatically applies performance optimizations when a game launches.

### How It Works

- Boosts CPU governor to "performance" mode
- Adjusts I/O priority
- Applies nice values to game processes
- Can be configured via `gamemode.ini`

### Using GameMode

Most games launched through Steam will use GameMode automatically. For manual use:

```bash
# Launch a game with GameMode
gamemoderun ./game-executable

# Check if GameMode is active
gamemoded status

# Run a command and verify GameMode engaged
gamemoderun glxgears
```

### Configuration

Edit `/etc/gamemode.ini` to customize:

```ini
[general]
; Whether to renice game processes
renice=4

; Whether to change I/O priority
ioprio=0

; Whether to apply CPU governor
gpu_io=2
gpu_coreclk=1
gpu_powerctl=2

[scheduler]
; Process priority adjustment
game_aware=1
```

## MangoHUD

MangoHUD provides an in-game overlay showing performance metrics.

### Enable for Steam Games

Add launch options in Steam:

```
mangohud %command%
```

To show the full overlay:

```
MANGOHUD_CONFIG=full mangohud %command%
```

### Standalone Games

```bash
# Basic overlay
mangohud ./game-executable

# With custom config
mangohud --config=/path/to/mangohud.conf ./game-executable
```

### MangoHUD Configuration

Create `~/.config/MangoHUD/MangoHUD.conf`:

```ini
# Layout
position=top-left
round_corners=10.0

# What to show
fps
frametime
gpu_stats
cpu_stats
ram
vram
engine_version
vulkan_driver

# Colors
gpu_color=255,100,100
cpu_color=255,100,100
vram_color=255,100,100
ram_color=255,100,100
fps_color=0,255,100,255

# Appearance
font_size=24
background_alpha=0.5
background_color=020202
```

### Useful MangoHUD Presets

```ini
# Minimal - FPS only
fps
no_display

# Full - Everything
fps
frametime
gpu_stats
cpu_stats
cpu_temp
gpu_temp
ram
vram
io_stats
engine_version
vulkan_driver
procmem

# Streaming - FPS and frame times
fps
frametime
frame_timing
```

## NVIDIA Users

### Driver Installation

```bash
# Install NVIDIA drivers
sudo pacman -S nvidia nvidia-utils lib32-nvidia-utils

# Enable DRM kernel module for Wayland
sudo bash -c 'echo "options nvidia-drm modeset=1" > /etc/modprobe.d/nvidia-drm.conf'

# Rebuild initramfs
sudo mkinitcpio -P
```

### NVIDIA Settings

Access via system tray or:

```bash
nvidia-settings
```

Recommended settings for gaming:
- **PowerMizer**: Prefer Maximum Performance
- **Sync**: V-Sync off in driver, use in-game limiter
- **OpenGL**: Performance mode

## AMD Users

AMD users get the best experience with open-source drivers (included by default):

```bash
# Ensure Mesa and Vulkan drivers are installed
sudo pacman -S mesa lib32-mesa vulkan-radeon lib32-vulkan-radeon
```

### AMD-specific Optimizations

```bash
# Enable AMD-specific kernel parameters
sudo bash -c 'echo "amdgpu.ppfeaturemask=0xffffffff" >> /etc/default/grub'
sudo grub-mkconfig -o /boot/grub/grub.cfg
```

## Controller Support

### Xbox Controllers

Xbox controllers work out of the box via xpad or xone:

```bash
# xpad (included in kernel)
# Just plug in and play

# For Xbox Wireless Controller (newer)
sudo pacman -S xone-dkms
```

### PlayStation Controllers

```bash
# DS4/DualShock 4
sudo pacman -S ds4drv

# DualSense (PS5)
# Works via Bluetooth or USB out of the box
# For advanced features:
sudo pacman -S dualsensectl
```

### Nintendo Switch Pro Controller

Works via Bluetooth. For better compatibility:

```bash
sudo pacman -S joycond
sudo systemctl enable --now joycond
```

### Generic Controllers

```bash
# Install SDL2 for broad controller support
sudo pacman -S sdl2

# For Xbox-like mapping on generic controllers
sudo pacman -S xpad
```

### Controller Configuration

Use Steam's built-in controller configuration:

1. Open Steam Settings > Controller
2. Enable your controller type
3. Configure per-game layouts

For non-Steam games, use `antimicrox` for keyboard-to-gamepad mapping:

```bash
sudo pacman -S antimicrox
antimicrox
```

## Performance Tips

### System Optimizations

```bash
# Set CPU governor to performance
sudo pacman -S cpupower
sudo cpupower frequency-set -g performance

# Disable unnecessary services
sudo systemctl disable bluetooth  # if not using BT controllers
sudo systemctl disable ModemManager  # if not using mobile broadband

# Enable TRIM for SSDs
sudo systemctl enable fstrim.timer
```

### Kernel Parameters

Add to `/etc/default/grub` on the `GRUB_CMDLINE_LINUX_DEFAULT` line:

```
# Reduce latency
mitigations=off  # SECURITY RISK - only for single-user gaming systems
threadirqs
preempt=full

# NVIDIA specific
nvidia-drm.modeset=1

# AMD specific
amdgpu.ppfeaturemask=0xffffffff
```

After editing:

```bash
sudo grub-mkconfig -o /boot/grub/grub.cfg
```

### Memory

```bash
# Check current swap
swapon --show

# Create swap file (if needed)
sudo fallocate -l 4G /swapfile
sudo chmod 600 /swapfile
sudo mkswap /swapfile
sudo swapon /swapfile
echo '/swapfile none swap sw 0 0' | sudo tee -a /etc/fstab

# Optimize swappiness for gaming
echo 'vm.swappiness=10' | sudo tee -a /etc/sysctl.d/99-gaming.conf
sudo sysctl --system
```

### Storage

```bash
# Ensure games are on fast storage
# Steam library locations can be set in Steam Settings > Storage

# Use ext4 or Btrfs for game drives
# NTFS can cause performance issues with Linux games

# Check disk health
sudo pacman -S smartmontools
sudo smartctl -a /dev/sdX
```

## Steam Proton / Compatibility

### Enable Proton

Steam Play is enabled by default. To configure:

1. Steam Settings > Compatibility
2. Enable "Enable Steam Play for all other titles"
3. Select Proton Experimental or latest stable version

### Proton Versions

| Version | Best For |
|---------|----------|
| Proton Experimental | Latest games, cutting-edge fixes |
| Proton 8.x | Stable, wide compatibility |
| Proton GE (custom) | Modified builds with extra patches |

### Install Proton-GE (Custom)

```bash
# Using AUR helper
yay -S proton-ge-custom-bin

# Or manually download from:
# https://github.com/PlagueVivala/Proton-GE-Custom/releases
```

## Troubleshooting

### Game Won't Launch

1. Verify game files in Steam
2. Check Proton version (try switching versions)
3. Check `~/.steam/steam/steamapps/common/<game>/steam_logs/`
4. Run from terminal to see errors:
   ```bash
   cd ~/.steam/steam/steamapps/common/<Game>/
   ./game-executable
   ```

### Low FPS

1. Ensure GameMode is active: `gamemoded status`
2. Check MangoHUD for bottleneck (GPU vs CPU)
3. Verify correct GPU is being used:
   ```bash
   # For NVIDIA
   nvidia-smi
   
   # For AMD
  radeontop
   ```
4. Close unnecessary background applications

### Controller Not Detected

1. Check connection: `lsinput` or `evtest`
2. Install correct driver (xone for Xbox Wireless)
3. Restart Steam after connecting controller
4. Check Steam Settings > Controller > Detected Controllers

### Audio Issues

```bash
# Ensure PipeWire is running
systemctl --user status pipewire pipewire-pulse

# Restart audio
systemctl --user restart pipewire pipewire-pulse wireplumber

# Check audio devices
pactl list sinks
```

## Game Launch Options Reference

Common Steam launch options:

```
# Performance
mangohud %command%                    # MangoHUD overlay
gamemoderun %command%                 # GameMode optimization
__GL_THREADED_OPTIMIZATIONS=1 %command%  # NVIDIA threaded optimization

# Compatibility
PROTON_USE_WINED3D=1 %command%        # Use OpenGL instead of Vulkan
PROTON_NO_ESYNC=1 %command%           # Disable esync
PROTON_NO_FSYNC=1 %command%           # Disable fsync

# Resolution
WINEDLLOVERRIDES="dxgi=n,b" %command% # Force DXVK
MANGOHUD_CONFIG="fps_no_target" %command%  # No FPS target

# Debug
PROTON_LOG=1 %command%                # Create log file in home directory
```
