# YummyOS Customization Guide

YummyOS is built for customization. Everything from colors to panels to workflows can be tailored to your preference.

## Themes

### Changing the Global Theme

1. Open **System Settings** > **Appearance** > **Global Theme**
2. Choose from pre-installed themes or download new ones
3. Click **Apply**

### Installing New Themes

**Via System Settings:**

1. System Settings > Appearance > Get New Global Themes
2. Browse or search for themes
3. Click **Install** then **Apply**

**Via Command Line:**

```bash
# Install from KDE Store
kpackagetool6 -t Plasma/LookAndFeel -i /path/to/theme.zip

# Or use the get-hot-stuff tool
get-hot-stuff
```

### Recommended Theme Sources

- [KDE Store](https://store.kde.org) - Official KDE theme repository
- [Pling Store](https://store.kde.org) - Community themes
- [r/unixporn](https://reddit.com/r/unixporn) - Community showcases with dotfiles

## Colors

### Changing Accent Colors

1. System Settings > Appearance > Colors
2. Select a color scheme
3. Or click **Edit Scheme** to customize individual colors

### Creating Custom Color Schemes

Color schemes are stored in `~/.local/share/color-schemes/`. Create a `.colors` file:

```ini
[ColorEffects:Disabled]
Color=56,56,56
ColorAmount=0
ColorEffect=0
ContrastAmount=0.65
ContrastEffect=1
IntensityAmount=0.1
IntensityEffect=2

[ColorEffects:Inactive]
ChangeSelectionColor=true
Color=112,111,110
ColorAmount=0
ColorEffect=1
ContrastAmount=0.1
ContrastEffect=2
IntensityAmount=0
IntensityEffect=0

[General]
ColorScheme=YummyCustom
Name=Yummy Custom
shadeSortColumn=true

[Colors:Button]
BackgroundAlternate=42,42,42
BackgroundNormal=56,56,56
DecorationFocus=61,174,233
DecorationHover=61,174,233
ForegroundActive=61,174,233
ForegroundInactive=161,161,161
ForegroundLink=42,161,154
ForegroundNegative=237,21,21
ForegroundNeutral=201,147,0
ForegroundNormal=252,252,252
ForegroundPositive=17,209,38
ForegroundVisited=140,76,140

[Colors:Selection]
BackgroundAlternate=42,130,213
BackgroundNormal=61,174,233
DecorationFocus=61,174,233
DecorationHover=61,174,233
ForegroundActive=252,252,252
ForegroundInactive=252,252,252
ForegroundLink=42,161,154
ForegroundNegative=237,21,21
ForegroundNeutral=201,147,0
ForegroundNormal=252,252,252
ForegroundPositive=17,209,38
ForegroundVisited=140,76,140

[Colors:Tooltip]
BackgroundAlternate=18,18,18
BackgroundNormal=30,30,30
DecorationFocus=61,174,233
DecorationHover=61,174,233
ForegroundActive=61,174,233
ForegroundInactive=161,161,161
ForegroundLink=42,161,154
ForegroundNegative=237,21,21
ForegroundNeutral=201,147,0
ForegroundNormal=252,252,252
ForegroundPositive=17,209,38
ForegroundVisited=140,76,140

[Colors:View]
BackgroundAlternate=18,18,18
BackgroundNormal=27,27,27
DecorationFocus=61,174,233
DecorationHover=61,174,233
ForegroundActive=61,174,233
ForegroundInactive=161,161,161
ForegroundLink=42,161,154
ForegroundNegative=237,21,21
ForegroundNeutral=201,147,0
ForegroundNormal=252,252,252
ForegroundPositive=17,209,38
ForegroundVisited=140,76,140

[Colors:Window]
BackgroundAlternate=35,35,35
BackgroundNormal=42,42,42
DecorationFocus=61,174,233
DecorationHover=61,174,233
ForegroundActive=61,174,233
ForegroundInactive=161,161,161
ForegroundLink=42,161,154
ForegroundNegative=237,21,21
ForegroundNeutral=201,147,0
ForegroundNormal=252,252,252
ForegroundPositive=17,209,38
ForegroundVisited=140,76,140

[WM]
activeBackground=46,46,46
activeBlend=252,252,252
activeForeground=252,252,252
inactiveBackground=35,35,35
inactiveBlend=252,252,252
inactiveForeground=161,161,161
```

### Applying Accent Color to Terminal

In Konsole:

1. Settings > Edit Current Profile > Appearance
2. Select a color scheme that matches your desktop theme

## Wallpapers

### Changing Wallpapers

**Desktop:**

1. Right-click desktop > Configure Desktop and Wallpaper
2. Choose from pre-installed or add your own
3. Set the mode (Centered, Scaled, Tiled, etc.)

**Lock Screen:**

1. System Settings > Screen Locking > Appearance
2. Choose wallpaper for lock screen

### Wallpaper Locations

Custom wallpapers are stored in:

```
~/.local/share/wallpapers/
```

### Adding Custom Wallpapers

```bash
# Copy wallpaper to the wallpapers directory
cp ~/Pictures/my-wallpaper.png ~/.local/share/wallpapers/MyWallpaper/contents/images/

# Or use a URL
# System Settings supports adding wallpapers from URLs
```

### Recommended Sources

- [Unsplash](https://unsplash.com) - Free high-resolution photos
- [WallpaperAccess](https://wallpaperaccess.com) - Curated collections
- [r/wallpapers](https://reddit.com/r/wallpapers) - Community wallpapers

## Panels

### Customizing the Panel

**Add a Panel:**

1. Right-click desktop > Enter Edit Mode
2. Click **Add Panel** at the top of the screen
3. Choose panel type (Default, Dock, etc.)

**Modify Existing Panel:**

1. Right-click the panel > Enter Edit Mode
2. Drag to resize
3. Add/remove widgets from the widget list
4. Configure panel settings (position, visibility, etc.)

### Panel Settings

- **Screen Edge** - Move panel to any edge
- **Height** - Adjust panel thickness
- **Width** - For vertical panels
- **Visibility** - Always visible, auto-hide, dodge windows, etc.
- **Opacity** - Transparency when idle and when active

### Panel Widget Layout

Popular panel configurations:

**Minimal:**
```
[Application Menu] [Task Manager] [System Tray] [Clock]
```

**macOS-style:**
```
[Global Menu] [Spacer] [Dock] [Spacer] [System Tray] [Clock]
```

**Windows-style:**
```
[Start Menu] [Task Manager] [System Tray] [Clock]
```

## Widgets

### Adding Widgets

1. Right-click desktop > Enter Edit Mode
2. Click **Add Widgets**
3. Browse or search for widgets
4. Click to add to desktop or panel

### Useful Widgets

| Widget | Purpose |
|--------|---------|
| **Application Menu** | Quick app launcher |
| **Clock** | Digital or analog clock |
| **System Tray** | Background service icons |
| **Task Manager** | Open application buttons |
| **System Monitor** | CPU, RAM, network graphs |
| **Notes** | Sticky notes on desktop |
| **Calculator** | Quick calculations |
| **Weather** | Weather information |
| **Audio Volume** | Sound control |
| **Brightness** | Screen brightness slider |

### Installing Widgets from KDE Store

1. Right-click desktop > Enter Edit Mode
2. Click **Get New Widgets**
3. Browse and install from KDE Store

### Custom Widget Locations

Widgets are stored in:

```
~/.local/share/plasma/plasmoids/
~/.local/share/plasma/containments/
```

## Keyboard Shortcuts

### System Settings Shortcuts

System Settings > Shortcuts > Custom Shortcuts

### Default Shortcuts

| Shortcut | Action |
|----------|--------|
| `Meta` | Application launcher |
| `Meta+E` | File manager |
| `Alt+F2` | Run command |
| `Alt+Tab` | Switch applications |
| `Alt+Shift+Tab` | Switch applications (reverse) |
| `Meta+Q` | Close window |
| `Meta+Page Up` | Next virtual desktop |
| `Meta+Page Down` | Previous virtual desktop |
| `Print Screen` | Screenshot |
| `Ctrl+Alt+T` | Terminal |

### Custom Shortcuts

1. System Settings > Shortcuts > Custom Shortcuts
2. Click **Add** > choose type (Command, URL, etc.)
3. Set trigger and action

Example shortcuts:

```bash
# Quick terminal
Command: konsole
Shortcut: Ctrl+Alt+T

# Quick browser
Command: firefox
Shortcut: Ctrl+Alt+B

# Lock screen
Command: qdbus org.kde.screensaver /ScreenSaver Lock
Shortcut: Meta+L
```

## Window Management

### Window Behavior

System Settings > Window Management:

- **Window Behavior** - Focus policy, click-to-focus vs focus-follows-mouse
- **Task Switcher** - Alt+Tab appearance and behavior
- **Virtual Desktops** - Number and layout of desktops
- **Window Rules** - Per-application window positioning

### Window Rules

System Settings > Window Management > Window Rules

Create rules to automatically:

- Position windows on specific screens
- Set window size
- Keep windows above/below others
- Disable window decorations
- Set specific virtual desktop

### KWin Effects

System Settings > Window Management > Desktop Effects:

- **Blur** - Transparent blur for panels and windows
- **Magic Lamp** - Minimize animation
- **Desktop Cube** - 3D virtual desktop switching
- **Wobbly Windows** - Gel-like window movement
- **Zoom** - Screen magnification

## Fonts

### Changing Fonts

System Settings > Fonts:

- **General** - Default system font
- **Fixed Width** - Monospace font (terminals)
- **Small** - Small text
- **Toolbar** - Toolbar buttons
- **Menu** - Menu items
- **Window Title** - Title bar text

### Recommended Fonts

| Font | Best For |
|------|----------|
| **Inter** | UI, clean and modern |
| **JetBrains Mono** | Coding, excellent readability |
| **Noto Sans** | Multi-language support |
| **Fira Code** | Coding with ligatures |
| **Cascadia Code** | Coding, modern look |

### Installing Fonts

```bash
# Copy to user font directory
mkdir -p ~/.local/share/fonts
cp *.ttf ~/.local/share/fonts/

# Refresh font cache
fc-cache -fv

# Or install system-wide
sudo cp *.ttf /usr/share/fonts/
sudo fc-cache -fv
```

## Cursor Themes

### Changing Cursors

System Settings > Appearance > Cursors

### Installing Cursor Themes

```bash
# Extract to cursor directory
mkdir -p ~/.local/share/icons
tar -xf cursor-theme.tar.gz -C ~/.local/share/icons/

# Or install system-wide
sudo tar -xf cursor-theme.tar.gz -C /usr/share/icons/
```

## Kvantum (Qt Theming)

Kvantum provides advanced Qt widget styling.

### Installing Kvantum

```bash
sudo pacman -S kvantum
```

### Applying Kvantum Themes

1. Open **Kvantum Manager** from application menu
2. Select a theme from the list
3. Click **Use this theme**

### Installing Kvantum Themes

1. Download a Kvantum theme (`.kvconfig` + SVG files)
2. Open Kvantum Manager
3. Click **Install Theme**
4. Select the downloaded theme folder

## Desktop Layouts

### Saving Layouts

1. Right-click desktop > Enter Edit Mode
2. Click **More Actions** > **Save as Default** or **Save Layout**

### Resetting Layouts

1. Right-click desktop > Enter Edit Mode
2. Click **More Actions** > **Reset Layout**

### Importing/Exporting Layouts

Layouts are stored in:

```
~/.config/plasma-org.kde.plasma.desktop.*.rc
```

Copy these files between machines to replicate layouts.

## Tips and Tricks

### Quick Theming Script

Create a script to quickly switch between themes:

```bash
#!/bin/bash
# ~/.local/bin/theme-switch

THEME=$1

case $THEME in
    dark)
        plasmapkg2 -t org.kde.plasma.lookandfeel -r "YummyOS Dark"
        plasmapkg2 -t org.kde.plasma.lookandfeel -a "YummyOS Dark"
        ;;
    light)
        plasmapkg2 -t org.kde.plasma.lookandfeel -r "YummyOS Light"
        plasmapkg2 -t org.kde.plasma.lookandfeel -a "YummyOS Light"
        ;;
    *)
        echo "Usage: theme-switch [dark|light]"
        ;;
esac
```

### Reset KDE to Defaults

If your desktop gets messed up:

```bash
# Reset plasma shell
plasmashell --replace &

# Or full reset (removes all customizations)
rm -rf ~/.config/plasma*
rm -rf ~/.local/share/plasma
plasmashell --replace &
```

### Backup Your Customizations

```bash
# Backup KDE config
tar -czf kde-backup.tar.gz \
    ~/.config/plasma* \
    ~/.config/kwin* \
    ~/.config/kcminputrc \
    ~/.local/share/color-schemes \
    ~/.local/share/plasma \
    ~/.local/share/icons \
    ~/.local/share/wallpapers

# Restore
tar -xzf kde-backup.tar.gz -C ~/
```
