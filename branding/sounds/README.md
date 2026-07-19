# YummyOS Sound Theme

YummyOS uses the freedesktop.org sound naming specification for its sound theme.

## Installation

```bash
# Install sound files to system directory
sudo cp -r sounds/ /usr/share/sounds/YummyOS/

# Set as default sound theme
# Use KDE System Settings > Sound > System Notifications
# or configure in ~/.config/Trolltech.conf
```

## Required Sound Files

All files should be in **OGG Vorbis** format at 44.1kHz / 16-bit.

### System Events

| Filename | freedesktop Name | Description |
|----------|-----------------|-------------|
| `stereo/bell.oga` | `bell` | Terminal bell / system alert |
| `stereo/click.oga` | `button-pressed` | UI click feedback |
| `stereo/error.oga` | `dialog-error` | Error notification |
| `stereo/information.oga` | `dialog-information` | Info notification |
| `stereo/complete.oga` | `completion` | Task completed |
| `stereo/failed.oga` | `completion-fail` | Task failed |

### Session Sounds

| Filename | freedesktop Name | Description |
|----------|-----------------|-------------|
| `stereo/login.oga` | `system-login` | Login / session start |
| `stereo/logout.oga` | `system-logout` | Logout / session end |
| `stereo/shutdown.oga` | `system-shutdown` | System shutdown |
| `stereo/restart.oga` | `system-reboot` | System restart |

### Desktop Events

| Filename | freedesktop Name | Description |
|----------|-----------------|-------------|
| `stereo/notifications.oga` | `message` | Incoming notification |
| `stereo/battery-low.oga` | `battery-low` | Battery running low |
| `stereo/battery-critical.oga` | `battery-critical` | Battery critically low |
| `stereo/power-plug.oga` | `power-plug` | AC adapter connected |
| `stereo/power-unplug.oga` | `power-unplug` | AC adapter disconnected |

### Media

| Filename | freedesktop Name | Description |
|----------|-----------------|-------------|
| `stereo/volume-change.oga` | `volume-change` | Volume adjusted |
| `stereo/audio-volume-up.oga` | `audio-volume-up` | Volume up |
| `stereo/audio-volume-down.oga` | `audio-volume-down` | Volume down |
| `stereo/audio-volume-muted.oga` | `audio-volume-muted` | Mute toggle |

## Directory Structure

```
/usr/share/sounds/YummyOS/
  index.theme          # Theme definition
  stereo/
    bell.oga
    click.oga
    ...
```

## index.theme

```ini
[Sound Theme]
Name=YummyOS
Inherits=freedesktop
Directories=stereo

[stereo]
OutputProfile=stereo
```

## Creating Sounds

Use tools like Audacity, SoX, or FFmpeg to create and convert sounds:

```bash
# Convert from WAV to OGG
ffmpeg -i input.wav -c:a libvorbis -q:a 6 output.oga

# Generate a simple beep with SoX
sox -n bell.oga synth 0.2 sine 880 gain -3 fade t 0 0.2 0.05

# Generate a short click
sox -n click.oga synth 0.05 square 1200 gain -6 fade t 0 0.05 0.02
```

## License

All custom YummyOS sounds are licensed under CC-BY-SA 4.0.
