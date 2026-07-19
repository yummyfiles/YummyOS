# YummyOS Architecture

This document describes the architecture of YummyOS, how the components fit together, and the data flow from build to boot.

## Overview

YummyOS is an Arch Linux-based distribution built with archiso. It packages a KDE Plasma 6 desktop with custom branding, configuration, and first-run applications into a bootable live ISO.

The architecture has three layers:

1. **Build System** - Archiso profile and scripts that assemble the ISO
2. **System Configuration** - Config files and overlays applied to the root filesystem
3. **Applications** - Custom Qt6/KDE apps for user interaction

## Directory Structure

```
YummyOS/
├── .github/                    # GitHub CI/CD and templates
│   ├── workflows/              # GitHub Actions
│   │   ├── build.yml           # Main CI pipeline
│   │   ├── release.yml         # Release automation
│   │   └── codeql.yml          # Security analysis
│   ├── ISSUE_TEMPLATE/         # Bug/feature templates
│   └── PULL_REQUEST_TEMPLATE.md
├── apps/                       # Custom applications
│   ├── control-center/         # System settings GUI
│   │   ├── CMakeLists.txt
│   │   └── src/
│   │       ├── main.cpp        # Entry point
│   │       ├── mainwindow.*    # Main window (tabbed UI)
│   │       ├── sidebarwidget.* # Navigation sidebar
│   │       ├── pages/          # Individual settings pages
│   │       │   ├── appearancepage.*
│   │       │   ├── gamingpage.*
│   │       │   ├── networkpage.*
│   │       │   ├── performancepage.*
│   │       │   ├── privacypage.*
│   │       │   └── ... (15 pages total)
│   │       ├── utils/          # Utility classes
│   │       │   ├── configmanager.*
│   │       │   └── systemutils.*
│   │       └── qml/            # QML UI components
│   │           └── theme.qml
│   ├── welcome/                # First-run wizard
│   │   ├── CMakeLists.txt
│   │   └── src/
│   │       ├── main.cpp
│   │       ├── backend/        # Backend logic
│   │       └── qml/            # QML pages
│   │           ├── main.qml
│   │           └── components/ # Page components
│   └── theme-manager/          # Theme management (planned)
├── branding/                   # Visual identity assets
│   ├── color-schemes/          # KDE color schemes
│   ├── cursors/                # Cursor themes
│   ├── gtk/                    # GTK theme integration
│   ├── icons/                  # Icon packs
│   ├── kvantum/                # Qt theme engine configs
│   ├── plasma-theme/           # Plasma shell theme
│   ├── plymouth/               # Boot splash animation
│   ├── sddm/                   # Login screen theme
│   ├── sounds/                 # System sounds
│   ├── splash/                 # Splash screens
│   └── wallpapers/             # Desktop wallpapers
├── build/                      # archiso build profile
│   ├── airootfs/               # Root filesystem overlay
│   │   ├── etc/                # System config files
│   │   └── root/               # Build-time scripts
│   │       └── customize_airootfs.sh
│   ├── efiboot/                # UEFI boot entries
│   │   └── entries/yummyos.conf
│   ├── grub/                   # BIOS boot config
│   ├── packages.x86_64         # Package manifest
│   ├── pacman.conf/            # Pacman mirrorlist
│   └── profiledef.sh           # archiso profile definition
├── config/                     # System configuration
│   ├── etc/                    # /etc files
│   │   ├── hostname, hosts
│   │   ├── locale.conf, locale.gen
│   │   ├── pacman.conf
│   │   ├── NetworkManager/
│   │   ├── sddm/               # Login manager config
│   │   ├── skel/               # Default user dotfiles
│   │   ├── systemd/            # Systemd overrides
│   │   └── xdg/                # XDG autostart entries
│   └── usr/                    # /usr files
│       ├── bin/                # Custom scripts
│       ├── lib/                # Libraries
│       └── share/              # Shared data
├── packages/                   # Custom package sources (AUR)
├── scripts/                    # Build utilities
│   ├── build-iso.sh            # Main build script
│   ├── clean-build.sh          # Cleanup script
│   └── verify-build.sh         # Build verification
├── tests/                      # Test files
├── docs/                       # Documentation
│   ├── BUILDING.md
│   ├── ARCHITECTURE.md
│   ├── CUSTOMIZATION.md
│   ├── GAMING.md
│   └── PRIVACY.md
├── .editorconfig               # Editor configuration
├── .gitignore
├── CONTRIBUTING.md
└── LICENSE                     # MIT License
```

## Component Details

### Control Center (`apps/control-center/`)

The control center is a Qt6/KDE Frameworks 6 application that provides a unified interface for system configuration. It is structured as:

- **MainWindow** - Top-level window with sidebar navigation and stacked page widget
- **SidebarWidget** - Category navigation panel with icons
- **Pages** - Individual settings pages, each a QWidget subclass:
  - Appearance - Theme, icons, cursors, fonts
  - Performance - CPU governor, swap, kernel parameters
  - Gaming - GameMode, MangoHUD, controller setup
  - Privacy - Telemetry controls, firewall settings
  - System Info - Hardware details, OS version
  - Audio - PipeWire/PulseAudio device management
  - Display - Resolution, refresh rate, multi-monitor
  - Power - Sleep, hibernate, lid actions
  - Startup Apps - Autostart application management
  - Packages - Software center integration
  - Network - Connection management
  - Updates - System update checking
  - Themes - Theme browser and installer
  - Wallpapers - Wallpaper manager
  - Backup - System backup configuration
  - Bluetooth - Device pairing and management
  - Drivers - GPU and hardware driver management
  - User Accounts - User creation and management
- **Utils** - ConfigManager (reads/writes config files), SystemUtils (hardware detection)

### Welcome App (`apps/welcome/`)

The welcome app is a Qt6/QML first-run wizard that guides users through initial setup. Pages include:

- Network - Connect to WiFi/Ethernet
- Drivers - Install proprietary drivers
- Updates - Check for system updates
- Gaming - Install Steam and gaming tools
- Developer - Install development tools
- Appearance - Choose initial theme
- Import - Migrate settings from other systems
- Done - Completion summary

The backend (C++) handles system operations while QML provides the UI.

### Branding (`branding/`)

Contains all visual identity assets that differentiate YummyOS from stock KDE Plasma:

- **Color Schemes** - Pre-defined KDE color palettes
- **Cursors** - Custom cursor theme
- **GTK Theme** - GTK application theme matching KDE
- **Icons** - Custom icon set
- **Kvantum** - Qt theme engine configurations
- **Plasma Theme** - Plasma shell visual theme
- **Plymouth** - Boot splash animation
- **SDDM Theme** - Login screen styling
- **Sounds** - System notification sounds
- **Wallpapers** - Default desktop backgrounds

### Build System (`build/`)

The build system uses archiso to assemble the ISO:

1. **profiledef.sh** defines ISO metadata (name, label, publisher, boot modes)
2. **packages.x86_64** lists all packages to install in the ISO
3. **airootfs/** is overlaid onto the base Arch Linux root filesystem
4. **customize_airootfs.sh** runs post-install configuration
5. **efiboot/** contains UEFI bootloader entries
6. **pacman.conf/** configures package repositories

### Configuration (`config/`)

System configuration files that are applied to the installed system:

- **Hostname/Hosts** - Network identity
- **Locale** - Language and region settings
- **Pacman** - Package manager configuration
- **NetworkManager** - Network configuration
- **SDDM** - Display manager settings
- **Skel** - Default shell profiles (.bashrc, .zshrc)
- **Systemd** - Service overrides and defaults
- **XDG** - Desktop autostart entries

## Data Flow

### Build Pipeline

```
Source Code
    │
    ▼
┌─────────────────────┐
│  CI/CD (GitHub)      │
│  - Lint shell/C++    │
│  - Validate configs  │
│  - Build apps        │
│  - Build ISO         │
└─────────┬───────────┘
          │
          ▼
┌─────────────────────┐
│  archiso             │
│  - Install packages  │
│  - Apply overlay     │
│  - Run customize.sh  │
│  - Generate squashfs │
│  - Create ISO        │
└─────────┬───────────┘
          │
          ▼
┌─────────────────────┐
│  ISO Image           │
│  - EFI boot          │
│  - Live filesystem   │
│  - Installer         │
└─────────────────────┘
```

### Boot Sequence

```
UEFI Firmware
    │
    ▼
systemd-boot (EFI)
    │
    ▼
Linux Kernel + initramfs
    │
    ▼
systemd (PID 1)
    │
    ▼
SDDM (Display Manager)
    │
    ▼
KDE Plasma (Desktop)
    │
    ├──► Welcome App (first boot only)
    └──► Control Center (user-initiated)
```

### System Configuration Flow

```
Control Center / CLI
        │
        ▼
┌───────────────────┐
│ ConfigManager      │
│ - Read config file │
│ - Modify values    │
│ - Write config     │
└───────┬───────────┘
        │
        ▼
┌───────────────────┐
│ System Config      │
│ /etc/*.conf        │
│ ~/.config/*        │
│ ~/.local/share/*   │
└───────┬───────────┘
        │
        ▼
┌───────────────────┐
│ KDE / Systemd      │
│ - Reload config    │
│ - Apply changes    │
│ - Restart services │
└───────────────────┘
```

## Technology Stack

| Component | Technology |
|-----------|-----------|
| Base System | Arch Linux |
| ISO Build | archiso |
| Desktop | KDE Plasma 6 |
| Display Server | Wayland (X11 fallback) |
| Display Manager | SDDM |
| Audio | PipeWire |
| Networking | NetworkManager |
| Boot Loader | systemd-boot |
| Applications | Qt6, KDE Frameworks 6 |
| UI Markup | QML |
| Build System | CMake |
| CI/CD | GitHub Actions |
| Security Scanning | CodeQL |

## Package Categories

The ISO includes these package groups:

1. **Base System** - Linux kernel, firmware, systemd
2. **Boot** - systemd-boot, EFI tools
3. **Desktop** - KDE Plasma, SDDM, KWin, Dolphin, Konsole
4. **Display/Graphics** - Xorg, Mesa, Vulkan drivers (Intel + AMD)
5. **Audio** - PipeWire stack, PulseAudio compatibility
6. **Network** - NetworkManager, Firefox, SSH, curl
7. **Applications** - LibreOffice, VLC, KDE Connect, Gwenview
8. **System Utilities** - htop, btop, git, neovim, flatpak
9. **Gaming** - GameMode, MangoHUD (with 32-bit libraries)
10. **Development** - GCC, CMake, Python, Node.js, Rust, Docker
11. **Themes** - Kvantum, Breeze icons, adwaita
12. **Boot Splash** - Plymouth
