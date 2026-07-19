# Contributing to YummyOS

Thanks for your interest in contributing to YummyOS! This guide covers everything you need to know to get started.

## Code of Conduct

- Be respectful and constructive
- Focus on what is best for the project
- Show empathy toward other contributors
- No harassment, trolling, or personal attacks

## Getting Started

### 1. Fork and Clone

```bash
git clone https://github.com/your-username/YummyOS.git
cd YummyOS
git remote add upstream https://github.com/yummyfiles/YummyOS.git
```

### 2. Create a Branch

```bash
git checkout -b feature/your-feature-name
```

Branch naming conventions:
- `feature/description` - New features
- `fix/description` - Bug fixes
- `docs/description` - Documentation changes
- `refactor/description` - Code refactoring
- `ci/description` - CI/CD changes

### 3. Make Changes, Commit, Push

```bash
git add .
git commit -m "feat: add your feature description"
git push origin feature/your-feature-name
```

### 4. Open a Pull Request

Go to the repository on GitHub and click "New Pull Request".

## Commit Conventions

We use [Conventional Commits](https://www.conventionalcommits.org/). Format:

```
<type>(<scope>): <description>

[optional body]

[optional footer]
```

### Types

| Type | Description |
|------|-------------|
| `feat` | New feature |
| `fix` | Bug fix |
| `docs` | Documentation only |
| `style` | Formatting, missing semicolons, etc. (no code change) |
| `refactor` | Code change that neither fixes a bug nor adds a feature |
| `perf` | Performance improvement |
| `test` | Adding or updating tests |
| `ci` | CI/CD configuration |
| `build` | Build system or dependency changes |
| `chore` | Other changes that don't modify src or test |

### Scope

Optional scope that provides contextual information:

- `control-center` - Control center app
- `welcome` - Welcome app
- `branding` - Visual assets
- `build` - Build system
- `config` - System configuration
- `scripts` - Build/utility scripts

### Examples

```
feat(control-center): add gaming mode toggle page
fix(welcome): correct network detection on WiFi-only systems
docs: update build instructions for new dependencies
ci(build): add shellcheck linting to pipeline
refactor(control-center): extract config manager to shared library
```

## Code Style

### Shell Scripts

- Use `#!/usr/bin/env bash` as shebang
- Enable strict mode: `set -e -u -o pipefail`
- Use `shellcheck` compatible syntax
- Quote all variable expansions: `"${variable}"`
- Use `${VAR:-default}` for optional variables
- Use functions for repeated logic
- Include a descriptive comment header

```bash
#!/usr/bin/env bash
#
# Script description
#
set -e -u -o pipefail

main() {
    echo "Hello, world!"
}

main "$@"
```

### C++ (Qt6 / KDE Frameworks)

- Standard: C++17 (welcome) or C++20 (control-center)
- Follow KDE coding style
- Use `camelCase` for variables and functions
- Use `PascalCase` for classes
- Use `UPPER_SNAKE_CASE` for constants
- Include header guards using `#pragma once`
- Prefer `auto` when type is obvious
- Use range-based for loops
- Use Qt strings (`QString`, `QLatin1String`)

```cpp
#pragma once

#include <QWidget>

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget() override;

    QString userName() const;
    void setUserName(const QString &name);

private:
    QString m_userName;
};
```

### QML

- Use 2-space indentation
- Property declarations before signals
- Use meaningful id names
- Group related properties

```qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root

    width: 400
    height: 300

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16

        Label {
            text: qsTr("Hello, World!")
            font.pointSize: 16
            font.bold: true
        }
    }
}
```

### CMake

- Use 4-space indentation
- Group related commands
- Use lowercase CMake commands
- Set minimum version explicitly

```cmake
cmake_minimum_required(VERSION 3.20)
project(my-app VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
)
```

### YAML / GitHub Actions

- Use 2-space indentation
- Quote strings with special characters
- Use descriptive job and step names
- Comment complex expressions

## Pull Request Process

### Before Submitting

1. **Test your changes** - Build the affected applications and verify they work
2. **Run linters** - Ensure `shellcheck` and `clang-format` pass
3. **Update documentation** - If you changed behavior, update relevant docs
4. **Check commit messages** - Follow the conventional commits format

### PR Description

Include in your PR:

1. **What** - Brief description of the change
2. **Why** - Reason for the change
3. **How** - Implementation approach (if non-obvious)
4. **Testing** - How you tested the changes
5. **Screenshots** - For UI changes, include before/after

### Review Process

- All PRs require at least one review
- Maintainers may request changes
- Address feedback promptly
- Once approved, a maintainer will merge the PR

### What Makes a Good PR

- Small and focused (one logical change per PR)
- Clear commit messages
- Includes documentation updates
- Doesn't break existing functionality
- Follows the project's code style

## Reporting Issues

### Bug Reports

Use the [bug report template](https://github.com/yummyfiles/YummyOS/issues/new?template=bug_report.md) and include:

- Clear description of the problem
- Steps to reproduce
- Expected vs actual behavior
- System information
- Screenshots if applicable

### Feature Requests

Use the [feature request template](https://github.com/yummyfiles/YummyOS/issues/new?template=feature_request.md) and include:

- Problem you're trying to solve
- Proposed solution
- Alternatives considered
- Additional context

## Development Environment

### Building Apps Locally

```bash
# Install dependencies
sudo pacman -S --needed cmake extra-cmake-modules \
    qt6-base qt6-declarative qt6-tools \
    kcoreaddons ki18n kconfig kio kiconthemes kdbusaddons kwindowsystem

# Build control center
cd apps/control-center
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Build welcome app
cd apps/welcome
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Running Linters

```bash
# Shell scripts
find . -name "*.sh" -not -path "./.git/*" | xargs shellcheck

# C++ formatting
find apps/ -name "*.cpp" -o -name "*.h" | xargs clang-format --dry-run --Werror

# Auto-fix C++ formatting
find apps/ -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

## Areas for Contribution

Looking for ways to help? Here are some areas that need attention:

- **Testing** - Add unit and integration tests
- **Documentation** - Improve guides and inline docs
- **Themes** - Create new color schemes, icon themes, wallpapers
- **Translations** - Internationalize applications
- **Accessibility** - Improve screen reader support, keyboard navigation
- **Performance** - Optimize build times, reduce package count
- **Bug fixes** - Check the issue tracker for reported bugs

## Questions?

Open a [discussion](https://github.com/yummyfiles/YummyOS/discussions) if you have questions about contributing.
