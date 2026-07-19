# YummyOS Wallpaper Generation

## Prerequisites

- **ImageMagick 7+** (`convert` or `magick` command)

```bash
# Debian/Ubuntu
sudo apt install imagemagick

# Fedora
sudo dnf install ImageMagick

# Arch
sudo pacman -S imagemagick
```

## Generating Wallpapers

```bash
./generate-wallpapers.sh
```

Output goes to `config/usr/share/wallpapers/YummyOS/contents/images/` by default.

To specify a custom output directory:

```bash
./generate-wallpapers.sh /path/to/output
```

## Generated Wallpapers

| File | Resolution | Description |
|------|-----------|-------------|
| `yummyos-dark-1920x1080.png` | 1920×1080 | Dark gradient with geometric shapes |
| `yummyos-dark-2560x1440.png` | 2560×1440 | Dark gradient with geometric shapes |
| `yummyos-dark-3840x2160.png` | 3840×2160 | Dark gradient with geometric shapes |
| `yummyos-light-1920x1080.png` | 1920×1080 | Light variant |
| `yummyos-minimal-1920x1080.png` | 1920×1080 | Minimal dark with accent gradient |

## Installation

Wallpapers are installed system-wide to `/usr/share/wallpapers/YummyOS/contents/images/`. They appear automatically in KDE Plasma wallpaper settings.

## Theme Colors

- Primary accent: `#4CC9F0`
- Secondary accent: `#7B2CBF`
- Dark start: `#0a0a1a`
- Dark end: `#1a1a2e`
- Light background: `#f0f4f8`
