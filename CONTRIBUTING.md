# Contributing to Horus Project

First off, thank you for considering contributing to the Horus Project! It's people like you that make this UAV ground control station a great tool for the community.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [How Can I Contribute?](#how-can-i-contribute)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)
- [Testing](#testing)
- [Documentation](#documentation)
- [Community](#community)

## Code of Conduct

This project and everyone participating in it is governed by our [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. Please report unacceptable behavior to the project maintainers.

## Getting Started

### Prerequisites

Before you begin, ensure you have the following installed:

- **C++ Compiler**: GCC 7+, Clang 5+, or MSVC 2017+
- **CMake**: Version 3.16 or higher
- **Qt Framework**: Qt 6.x (Qt 5.12+ also supported)
- **Git**: For version control

### Hardware (Optional for testing)

- ESP32 development board
- MPU6050 IMU sensor
- Connecting wires

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check existing issues to avoid duplicates. When you create a bug report, include as many details as possible:

- **Use a clear and descriptive title**
- **Describe the exact steps to reproduce the problem**
- **Provide specific examples** (code snippets, screenshots)
- **Describe the behavior you observed** and what you expected
- **Include your environment details** (OS, Qt version, compiler)
- **Include relevant logs** from console output

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When creating an enhancement suggestion:

- **Use a clear and descriptive title**
- **Provide a detailed description** of the proposed functionality
- **Explain why this enhancement would be useful**
- **List examples** of how it would be used
- **Mention if you're willing to implement it**

### Your First Code Contribution

Unsure where to begin? Look for issues labeled:

- `good first issue` - Simple issues for newcomers
- `help wanted` - Issues where we need community help
- `documentation` - Documentation improvements
- `bug` - Bug fixes needed

### Pull Requests

We actively welcome your pull requests:

1. Fork the repository and create your branch from `main`
2. Make your changes following our coding standards
3. Test your changes thoroughly
4. Update documentation if needed
5. Ensure your code compiles without warnings
6. Submit your pull request!

## Development Setup

### 1. Clone the Repository

```bash
git clone https://github.com/joaoliveira6704/Horus-Project.git
cd Horus-Project
```

### 2. Build the Project

**Linux/macOS:**
```bash
mkdir build && cd build
cmake ..
make
```

**Windows (Visual Studio):**
```bash
mkdir build && cd build
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
```

**Windows (MinGW):**
```bash
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
```

### 3. Run the Application

```bash
./Horus-Project  # Linux/macOS
Horus-Project.exe  # Windows
```

## Coding Standards

### C++ Style Guidelines

- **C++ Standard**: Use C++17 features
- **Naming Conventions**:
  - Classes: `PascalCase` (e.g., `AttitudeIndicator`)
  - Functions: `camelCase` (e.g., `updatePitchRoll`)
  - Variables: `camelCase` (e.g., `pitchAngle`)
  - Constants: `UPPER_SNAKE_CASE` (e.g., `MAX_PITCH_ANGLE`)
  - Private members: prefix with `m_` (e.g., `m_pitchAngle`)

- **Code Formatting**:
  - Use `.clang-format` configuration (included in repo)
  - Indent with 4 spaces (no tabs)
  - Max line length: 100 characters
  - Always use braces for control structures

- **Comments**:
  - Use `//` for single-line comments
  - Use `/** */` for documentation comments
  - Document all public functions and classes
  - Explain *why*, not *what* in comments

### Example

```cpp
/**
 * @brief Updates the pitch and roll angles for the attitude indicator
 * @param pitch Pitch angle in degrees (-90 to +90)
 * @param roll Roll angle in degrees (-180 to +180)
 */
void AttitudeIndicator::updatePitchRoll(double pitch, double roll) {
    // Clamp pitch to valid range to prevent visual artifacts
    m_pitch = qBound(-90.0, pitch, 90.0);
    m_roll = qBound(-180.0, roll, 180.0);

    update(); // Trigger repaint
}
```

### Qt-Specific Guidelines

- Use Qt naming conventions for slots/signals
- Prefer `nullptr` over `NULL` or `0`
- Use Qt containers when interfacing with Qt APIs
- Use `const` references for parameters when appropriate
- Follow RAII principles for resource management

### Arduino/ESP32 Firmware

- Follow Arduino coding style
- Keep firmware lightweight and efficient
- Document I2C addresses and baud rates
- Include error handling for sensor failures
- Comment hardware-specific configurations

## Commit Guidelines

We follow the [Conventional Commits](https://www.conventionalcommits.org/) specification:

### Commit Message Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

### Types

- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, no code change)
- `refactor`: Code refactoring
- `perf`: Performance improvements
- `test`: Adding or updating tests
- `chore`: Maintenance tasks (dependencies, build)
- `ci`: CI/CD changes

### Examples

```
feat(pfd): add vertical speed indicator

Implement vertical speed tape on the PFD with ±2000 fpm range.
Includes smoothing filter to reduce noise.

Closes #42
```

```
fix(serial): handle ESP32 disconnection gracefully

Application now detects when ESP32 is disconnected and switches
to simulation mode automatically.

Fixes #38
```

```
docs(readme): update hardware wiring diagram

Added clearer ESP32-MPU6050 connection diagram with pin labels.
```

## Pull Request Process

### Before Submitting

1. **Update your branch** with the latest `main`:
   ```bash
   git checkout main
   git pull upstream main
   git checkout your-branch
   git rebase main
   ```

2. **Build and test**:
   ```bash
   cd build
   cmake ..
   make
   ./Horus-Project  # Verify it runs
   ```

3. **Check code formatting**:
   ```bash
   clang-format -i *.cpp *.h
   ```

4. **Update documentation** if you:
   - Add new features
   - Change existing behavior
   - Modify configuration options

### PR Checklist

- [ ] Code compiles without errors or warnings
- [ ] Code follows project style guidelines
- [ ] Commit messages follow conventional commits format
- [ ] Documentation is updated (README, docs/, code comments)
- [ ] No debugging code or commented-out code left behind
- [ ] Changes are tested on at least one platform
- [ ] PR description clearly explains the changes

### PR Description Template

```markdown
## Description
Brief description of what this PR does

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Documentation update
- [ ] Refactoring
- [ ] Performance improvement

## Testing
How has this been tested?

## Screenshots (if applicable)
Add screenshots for UI changes

## Checklist
- [ ] My code follows the project style guidelines
- [ ] I have commented my code where necessary
- [ ] I have updated the documentation
- [ ] My changes generate no new warnings
- [ ] I have tested this on: [ ] Windows [ ] Linux [ ] macOS
```

### Review Process

1. At least one maintainer review is required
2. All CI checks must pass (when implemented)
3. Maintainer may request changes or improvements
4. Once approved, maintainer will merge the PR

## Testing

Currently, the project uses manual testing. We welcome contributions to add automated testing!

### Manual Testing Checklist

When testing your changes:

- [ ] Application launches successfully
- [ ] No crashes or freezes during operation
- [ ] Artificial horizon responds correctly to pitch/roll
- [ ] Serial communication works (if testing with ESP32)
- [ ] Simulation mode works without hardware
- [ ] UI elements render correctly at different window sizes
- [ ] Performance remains at 60+ FPS
- [ ] No console errors or warnings

### Future: Automated Testing

We plan to add:
- Unit tests with Google Test or Qt Test
- Integration tests for serial communication
- Performance benchmarks
- Cross-platform CI/CD testing

**Want to help?** Setting up a testing framework would be a great contribution!

## Documentation

### Types of Documentation

1. **Code Documentation**: Comment complex logic, public APIs
2. **User Documentation**: Update README.md for user-facing changes
3. **Technical Documentation**: Update docs/ folder for technical details
4. **Examples**: Add examples for new features

### Writing Good Documentation

- **Be clear and concise**
- **Include code examples** when relevant
- **Add screenshots** for UI features
- **Explain the "why"** not just the "how"
- **Keep it up to date** with code changes

### Documentation To-Do

Help us complete these documentation files:

- `docs/ESP32_SETUP.md` - Detailed ESP32 setup guide
- `docs/MPU6050_CALIBRATION.md` - Sensor calibration procedures
- `docs/SERIAL_PROTOCOL.md` - Serial data format specification
- `docs/SENSOR_EXPANSION.md` - Adding new sensors guide
- `docs/ARCHITECTURE.md` - Technical architecture overview

## Community

### Communication Channels

- **GitHub Issues**: Bug reports, feature requests, discussions
- **GitHub Discussions**: General questions, ideas, showcase
- **Pull Requests**: Code contributions and reviews

### Getting Help

- Check the [README](README.md) for setup and usage instructions
- Search existing issues for similar problems
- Ask questions in GitHub Discussions
- Tag maintainers in issues if urgent

### Recognition

Contributors are recognized in several ways:

- Listed in [AUTHORS](AUTHORS.md) file
- Mentioned in [CHANGELOG](CHANGELOG.md) for significant contributions
- GitHub's automatic contributor tracking

## License

By contributing to Horus Project, you agree that your contributions will be licensed under the [MIT License](LICENSE).

---

## Questions?

Don't hesitate to ask! We're here to help:

- **Open an issue** with the `question` label
- **Start a discussion** in GitHub Discussions
- **Contact maintainers** through GitHub

## Thank You!

Your contributions make Horus Project better for everyone. We appreciate your time and effort! 🚁✨

---

**Project Maintainer**: [@joaoliveira6704](https://github.com/joaoliveira6704)
