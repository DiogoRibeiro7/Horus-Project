# Changelog

All notable changes to the Horus Project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Unit test framework with Qt Test
- Automated CI/CD pipeline
- Cross-platform build testing
- GPS integration for position display
- Additional sensor support (airspeed, compass)
- Flight data recording and playback
- Configuration GUI panel

## [1.0.0] - 2025-01-17

### Added
- ESP32 value reading from MPU6050 sensor for pitch and roll
- ESP32 firmware code for IMU data acquisition
- Real-time attitude sensing with 50Hz update rate
- Complementary filter (98% gyro, 2% accel) for stable attitude data
- Automatic I2C address detection (0x68/0x69)
- Serial communication protocol (CSV format: pitch,roll)
- Simulation mode for testing without hardware
- Battery percentage indicator
- Complete Primary Flight Display with all instruments
- High-performance rendering at 60+ FPS
- Cross-platform support (Windows, macOS, Linux)
- Professional aviation-grade instrument styling
- MIT License for open-source distribution

### Changed
- Refactored CMakeLists.txt for better project organization
- Updated README.md with comprehensive documentation
- Improved HUD preview image to show latest build
- Updated simTime to make fake data simulation more realistic
- Refined aircraft dimensions for W-shape configuration

### Fixed
- Battery percentage calculation accuracy
- W-shape aircraft dimensions visualization
- Serial port handling and error detection
- IMU data parsing and validation

### Documentation
- Added hardware wiring diagram (ESP32-MPU6050)
- Added installation instructions for all platforms
- Added troubleshooting section
- Added configuration guide
- Added roadmap for future versions
- Added contributing guidelines
- Added code of conduct

## [0.9.0] - 2025-01-10

### Added
- Initial Primary Flight Display implementation
- Artificial Horizon with 6x zoom
- Pitch Ladder (±10° visible with 5° increments)
- Roll Indicator with scale marks
- Altitude Tape (100 ft increments, 500 ft labels)
- Speed Indicator (vertical tape)
- Heading Indicator with NSWE cardinal directions
- Battery Voltage Display
- RPM Gauges for up to 4 motors
- QNH Display (hPa and inHg)
- Flight Mode Display
- Clock/Time Display
- OAT (Outside Air Temperature) Display
- Barometric altitude calculation with ISA model
- Qt-based GUI framework
- CMake build system
- Font resources (Nimbus Mono, Armarurgt)

### Changed
- Optimized rendering pipeline for 60+ FPS performance
- Improved instrument readability and contrast
- Enhanced anti-aliasing for smooth graphics

## [0.5.0] - 2025-01-05

### Added
- Project initialization
- Basic Qt application structure
- AttitudeIndicator widget implementation
- QPainter-based rendering
- Initial CMake configuration
- README with project overview
- MIT License
- .gitignore for C++/Qt projects

---

## Version History Summary

- **v1.0.0** (2025-01-17): First stable release with ESP32 integration
- **v0.9.0** (2025-01-10): Complete PFD implementation
- **v0.5.0** (2025-01-05): Initial project setup

---

## Notes

### Semantic Versioning

This project uses [Semantic Versioning](https://semver.org/):

- **MAJOR** version: Incompatible API changes
- **MINOR** version: New functionality (backwards-compatible)
- **PATCH** version: Bug fixes (backwards-compatible)

### Change Categories

- **Added**: New features
- **Changed**: Changes to existing functionality
- **Deprecated**: Soon-to-be removed features
- **Removed**: Removed features
- **Fixed**: Bug fixes
- **Security**: Vulnerability fixes

### Unreleased Changes

Development work and upcoming features are tracked in the [Unreleased] section.
See the [project roadmap](README.md#roadmap) for planned features.

---

**Maintained by**: [@joaoliveira6704](https://github.com/joaoliveira6704)

[Unreleased]: https://github.com/joaoliveira6704/Horus-Project/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/joaoliveira6704/Horus-Project/releases/tag/v1.0.0
[0.9.0]: https://github.com/joaoliveira6704/Horus-Project/releases/tag/v0.9.0
[0.5.0]: https://github.com/joaoliveira6704/Horus-Project/releases/tag/v0.5.0
