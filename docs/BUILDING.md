# Building YummyOS

This guide covers building the YummyOS ISO image from source, as well as building the individual applications.

## Prerequisites

### System Requirements

- **Operating System:** Arch Linux (or an Arch-based distro)
- **Disk Space:** At least 20 GB free
- **RAM:** 4 GB minimum, 8 GB recommended
- **Internet:** Required for package downloads

### Required Packages

```bash
sudo pacman -S --needed \
    archiso \
    base-devel \
    cmake \
    extra-cmake-modules \
    git \
    qt6-base \
    qt6-declarative \
    qt6-svg \
    qt6-tools \
    extra-cmake-modules \
    pkg-config
```

### KDE Framework 6 Dependencies

```bash
sudo pacman -S --needed \
    kcoreaddons \
    ki18n \
    kconfig \
    kio \
    kiconthemes \
    kdbusaddons \
    kwindowsystem \
    kpackage \
    plasma-framework
```

## Repository Structure

```
YummyOS/
├── apps/                    # Custom applications
│   ├── control-center/      # System settings application
│   ├── welcome/             # First-run welcome wizard
│   └── theme-manager/       # Theme management tool
├── branding/                # Visual branding assets
│   ├── color-schemes/
│   ├── cursors/
│   ├── icons/
│   ├── kvantum/
│   ├── plasma-theme/
│   ├── plymouth/
│   ├── sddm/
│   ├── sounds/
│   └── wallpapers/
├── build/                   # ISO build configuration
│   ├── airootfs/            # Root filesystem overlay
│   ├── efiboot/             # UEFI boot configuration
│   ├── grub/                # GRUB bootloader config
│   ├── packages.x86_64      # Package list for ISO
│   ├── pacman.conf/         # Pacman configuration
│   └── profiledef.sh        # archiso profile definition
├── config/                  # System configuration files
│   ├── etc/                 # /etc configuration
│   └── usr/                 # /usr configuration
├── scripts/                 # Build and utility scripts
├── tests/                   # Test files
└── docs/                    # Documentation
```

## Building the Applications

### Control Center

The control center is the main system settings application built with Qt6 and KDE Frameworks.

```bash
cd apps/control-center

# Configure
cmake -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DQT_MAJOR_VERSION=6

# Build
cmake --build build --parallel $(nproc)

# Install (optional, requires root)
sudo cmake --install build
```

### Welcome App

The welcome app is the first-run wizard shown after installation.

```bash
cd apps/welcome

# Configure
cmake -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DQT_MAJOR_VERSION=6

# Build
cmake --build build --parallel $(nproc)

# Install (optional, requires root)
sudo cmake --install build
```

## Building the ISO

### Quick Build

Use the provided build script for a one-command build:

```bash
./scripts/build-iso.sh
```

The ISO will be output to `out/yummyos-YYYY.MM.DD-x86_64.iso`.

### Manual Build

If you prefer to run archiso directly:

```bash
# Set up the working directory
mkdir -p build/iso/archiso
cd build/iso/archiso

# Copy archiso releng configs
cp /usr/share/archiso/configs/releng/*.sh .
cp /usr/share/archiso/configs/releng/*.conf .
cp -r /usr/share/archiso/configs/releng/airootfs/ .

# Merge project airootfs overlay
cp -r ../../airootfs/. airootfs/

# Copy project files
cp ../../packages.x86_64 .
cp ../../profiledef.sh .
cp ../../pacman.conf/pacman.conf airootfs/etc/pacman.conf

# Build
mkarchiso \
    -w ../work \
    -D ../efiboot \
    -o ../../../out \
    .

cd ../../..
```

### Build Options

The build script accepts these options:

```
Usage: build-iso.sh [OPTIONS]

Options:
    -o DIR      Output directory (default: ./out)
    -c PROFILE  Profile directory (default: ./build)
    -h          Show this help message
```

Example with custom output directory:

```bash
./scripts/build-iso.sh -o /tmp/yummyos-build
```

## Verifying the Build

After building, run the verification script:

```bash
./scripts/verify-build.sh
```

This checks:
- Required files are present
- Package list integrity
- Shell script syntax
- ISO output validity
- Essential packages inclusion

## Cleaning Build Artifacts

To remove all build artifacts and temporary files:

```bash
./scripts/clean-build.sh
```

This removes:
- `build/iso/` - Temporary archiso workspace
- `out/` - Built ISO images
- `build/airootfs/var/` and `build/airootfs/tmp/` - Runtime files
- Generated shadow/gshadow files

## Troubleshooting

### "archiso not found"

Install archiso:
```bash
sudo pacman -S archiso
```

### CMake errors about missing Qt6 or KDE packages

Ensure all dependencies are installed:
```bash
sudo pacman -S --needed qt6-base qt6-declarative extra-cmake-modules \
    kcoreaddons ki18n kconfig kio kiconthemes kdbusaddons kwindowsystem
```

### Build fails with permission errors

The build script needs to create directories and files. Make sure you have write access to the project directory. Do not run the ISO build as root unless necessary.

### ISO is smaller than expected

A full YummyOS ISO should be at least 1 GB. If it's significantly smaller, packages may have failed to install. Check the archiso log output for errors.

### "mkarchiso: command not found"

Ensure archiso is installed and you're running on an Arch Linux system:
```bash
which mkarchiso
# Should output: /usr/bin/mkarchiso
```

### Out of disk space

Building the ISO requires significant temporary disk space. Ensure at least 15 GB is free in the working directory.

## CI/CD

YummyOS uses GitHub Actions for continuous integration. Every push to `main` triggers:

1. **Shell linting** with shellcheck
2. **C++ formatting** with clang-format
3. **Config validation** for XML, INI, and YAML files
4. **Application builds** for control center and welcome app
5. **ISO build** on Arch Linux container (main branch only)

Pull requests trigger all checks except the ISO build to save CI time.
