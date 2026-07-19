# YummyOS Plymouth Asset Specifications

All assets are for the Plymouth boot splash theme.
Place generated files in `/usr/share/plymouth/themes/YummyOS/`.

## Required Assets

### 1. Logo (`yummyos-logo.png`)

- **Dimensions:** 256×256 px
- **Background:** Fully transparent (RGBA 0,0,0,0)
- **Content:** "YummyOS" text, horizontally and vertically centered
- **Font:** Any clean sans-serif (Inter, Roboto, or similar), bold weight
- **Font size:** ~48px
- **Text color:** `#4CC9F0` (primary accent, RGB 76/201/240)
- **Anti-aliasing:** Enabled, standard subpixel rendering

### 2. Progress Bar Background (`progress-bg.png`)

- **Dimensions:** 400×8 px
- **Shape:** Rounded rectangle, corner radius 4px
- **Color:** `#1a1a2e` (RGB 26/26/46)
- **Border:** None
- **Anti-aliasing:** Enabled on corners

### 3. Progress Bar Fill (`progress-fill.png`)

- **Dimensions:** 400×8 px
- **Shape:** Rounded rectangle, corner radius 4px
- **Color:** `#4CC9F0` (RGB 76/201/240)
- **Border:** None
- **Anti-aliasing:** Enabled on corners

## Optional: Frame-Based Boot Animation

If a frame-based animation is preferred over the script-driven
pulsing effect, generate the following:

### Boot Animation Frames (`boot-animation-NNN.png`)

- **Count:** 30 frames (or 60 for 60fps at 1s cycle)
- **Dimensions:** 256×256 px per frame
- **Content:** "YummyOS" text with varying opacity/size to simulate breathing
- **Cycle:** One full breath = 2 seconds
- **Opacity range:** 150–255 (alpha channel variation)
- **Naming:** `boot-animation-001.png` through `boot-animation-030.png`
- **Frame rate:** 15 fps (each frame shown for ~67ms)

## Color Reference

| Role      | Hex       | RGB              |
|-----------|-----------|------------------|
| Primary   | `#4CC9F0` | 76, 201, 240     |
| Secondary | `#7B2CBF` | 123, 44, 191     |
| Background| `#0a0a1a` | 10, 10, 26       |
| Bar BG    | `#1a1a2e` | 26, 26, 46       |

## Generation Commands (ImageMagick)

```bash
# Logo
convert -size 256x256 xc:transparent \
  -font "Inter-Bold" -pointsize 48 \
  -fill "#4CC9F0" -gravity center \
  -annotate +0+0 "YummyOS" \
  yummyos-logo.png

# Progress bar background
convert -size 400x8 xc:"#1a1a2e" \
  \( +clone -alpha extract \
     -draw "fill black polygon 0,0 4,0 0,4 fill white circle 4,4 4,0" \
     -channel A -evaluate multiply 1 +channel \
     -compose CopyOpacity -composite \) \
  -gravity center -composite \
  progress-bg.png

# Progress bar fill (same shape, different color)
convert -size 400x8 xc:"#4CC9F0" \
  \( +clone -alpha extract \
     -draw "fill black polygon 0,0 4,0 0,4 fill white circle 4,4 4,0" \
     -channel A -evaluate multiply 1 +channel \
     -compose CopyOpacity -composite \) \
  -gravity center -composite \
  progress-fill.png
```
