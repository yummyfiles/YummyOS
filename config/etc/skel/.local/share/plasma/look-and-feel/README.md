# Look-and-Feel Symlink

To install the YummyOS look-and-feel package for new users:

```bash
# Create symlink for system-wide new user defaults
sudo ln -sf /usr/share/plasma/look-and-feel/YummyOS \
    /etc/skel/.local/share/plasma/look-and-feel/YummyOS
```

This ensures that new users get the YummyOS look-and-feel package in their home directory on first login.

## Manual Installation

If you prefer to copy instead of symlink:

```bash
sudo cp -r /usr/share/plasma/look-and-feel/YummyOS \
    /etc/skel/.local/share/plasma/look-and-feel/
```
