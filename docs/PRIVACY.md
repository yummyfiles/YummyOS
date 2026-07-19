# YummyOS Privacy Policy

**Last updated:** January 2026

YummyOS is built on a simple principle: your computer is your computer. This document explains what we do and don't collect.

## The Short Version

YummyOS collects **zero telemetry**. There are **no ads**, **no tracking**, **no analytics**, and **no phone-home** functionality. Your usage data never leaves your machine.

## What We DON'T Collect

- **No telemetry** - We don't collect usage statistics, app launch counts, or performance data
- **No tracking** - We don't track which websites you visit, apps you use, or files you access
- **No ads** - There are no advertisements, sponsored content, or ad networks
- **No analytics** - We don't use analytics services (Google Analytics, Mixpanel, etc.)
- **No fingerprints** - We don't collect hardware fingerprints or device identifiers
- **No location data** - We don't access or transmit your geographic location
- **No personal data** - We don't collect names, emails, or any personally identifiable information
- **No crash reports** - We don't automatically send crash reports or diagnostics to external servers
- **No keylogging** - We don't record keystrokes or input data
- **No file scanning** - We don't scan or index your personal files

## What IS On Your System

The following data exists on your system but **never leaves your machine**:

### Package Manager

- `pacman` maintains a local database of installed packages
- This data is stored in `/var/lib/pacman/local/` and is never transmitted to us
- Mirror synchronization uses standard Arch Linux mirrors (not controlled by YummyOS)

### System Logs

- Standard systemd journal logs are stored locally
- These are used for local debugging only
- They are never transmitted to external servers

### KDE Configuration

- KDE Plasma stores your preferences locally in `~/.config/`
- Wallet stores encrypted credentials locally in `~/.local/share/kwallet/`
- No KDE telemetry features are enabled

### Network

- NetworkManager stores WiFi passwords locally for your convenience
- DNS resolution goes through your configured resolver
- No DNS queries are logged or intercepted by YummyOS

## Optional Third-Party Services

Some pre-installed applications may have their own privacy policies:

| Application | Privacy Implications |
|------------|---------------------|
| Firefox | Mozilla's privacy policy applies. You can configure telemetry in `about:preferences#privacy` |
| Flatpak | Flathub is used for package downloads. No YummyOS data is shared |
| KDE Connect | Local network only. Requires explicit pairing with your devices |

These services are not controlled by YummyOS and have their own data practices.

## Build and CI/CD

- GitHub Actions CI runs build checks on code contributions
- CI logs contain build output only (no user data)
- CI is triggered by git pushes, not by end-user activity

## Updates

- YummyOS uses standard Arch Linux repositories for package updates
- Update checks go to official Arch mirrors, not YummyOS servers
- No update telemetry is sent

## What We CAN See

The only metrics we have access to are **public GitHub repository statistics**:

- Number of stars, forks, and watchers
- Number of open issues
- Pull request activity
- Clone counts (if enabled)

These are standard GitHub features and don't contain any personal data.

## Network Activity from YummyOS

The only network activity initiated by YummyOS itself is:

1. **Package downloads** - Via pacman from Arch Linux mirrors
2. **Time synchronization** - Standard NTP via systemd-timesyncd
3. **DNS resolution** - Via your configured DNS resolver

No other outbound connections are made by YummyOS system components.

## If You Want Maximum Privacy

For users who want additional privacy measures:

1. **Disable KDE telemetry** - System Settings > About This System > Privacy
2. **Use a VPN** - Route traffic through a trusted VPN
3. **Configure DNS** - Use a privacy-focused DNS resolver (e.g., `1.1.1.1` or `9.9.9.9`)
4. **Review installed packages** - Remove any applications you don't use
5. **Enable firewall** - Use `ufw` or `firewalld` to control network access

## Philosophy

Privacy is not a feature. It's a default.

Every operating system should respect user privacy by default, not as an opt-in setting. YummyOS was created because we believe users deserve an OS that doesn't spy on them.

If you find any component of YummyOS that collects data without explicit user consent, please [open an issue](https://github.com/yummyfiles/YummyOS/issues) immediately.

## Changes to This Policy

Any changes to this privacy policy will be documented in the repository's commit history. We will never add telemetry, tracking, or advertising to YummyOS without explicit community consent.

## Contact

If you have questions about this privacy policy, open an issue or discussion on the [YummyOS GitHub repository](https://github.com/yummyfiles/YummyOS).
