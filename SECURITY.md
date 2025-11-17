# Security Policy

## Supported Versions

The following versions of Horus Project are currently supported with security updates:

| Version | Supported          |
| ------- | ------------------ |
| 1.0.x   | :white_check_mark: |
| < 1.0   | :x:                |

We recommend always using the latest stable release to benefit from security fixes and improvements.

## Reporting a Vulnerability

The Horus Project team takes security vulnerabilities seriously. We appreciate your efforts to responsibly disclose your findings.

### How to Report

**Please do NOT report security vulnerabilities through public GitHub issues.**

Instead, please report security vulnerabilities by:

1. **Opening a GitHub Security Advisory** (preferred method):
   - Go to the [Security tab](https://github.com/joaoliveira6704/Horus-Project/security)
   - Click "Report a vulnerability"
   - Fill out the advisory form with details

2. **Direct contact** (for sensitive issues):
   - Contact the project maintainer: [@joaoliveira6704](https://github.com/joaoliveira6704)
   - Use GitHub's private vulnerability reporting feature
   - Include detailed information (see below)

### What to Include

To help us understand and resolve the issue quickly, please include:

- **Type of vulnerability** (e.g., buffer overflow, code injection, privilege escalation)
- **Full description** of the vulnerability
- **Steps to reproduce** the issue
- **Potential impact** of the vulnerability
- **Affected version(s)** of the software
- **Any proof-of-concept or exploit code** (if available)
- **Suggested fix** (if you have one)
- **Your contact information** for follow-up questions

### Response Timeline

We aim to respond to security reports according to the following timeline:

- **Initial Response**: Within 48 hours
- **Triage and Assessment**: Within 1 week
- **Fix Development**: Depends on severity and complexity
- **Coordinated Disclosure**: After fix is ready and tested

### What to Expect

After you submit a report:

1. **Acknowledgment**: We'll acknowledge receipt of your report within 48 hours
2. **Assessment**: We'll assess the vulnerability and determine severity
3. **Updates**: We'll keep you informed of our progress
4. **Credit**: With your permission, we'll credit you in the security advisory
5. **Disclosure**: We'll coordinate public disclosure after the fix is released

## Security Considerations

### Serial Communication

The Horus Project uses serial communication (115200 baud) to receive data from ESP32. Consider:

- **Data Validation**: All incoming serial data is parsed and validated
- **Buffer Limits**: Serial buffers have defined size limits
- **Error Handling**: Malformed data is rejected gracefully
- **No Authentication**: Serial protocol has no authentication (physical access required)

**Security Tip**: Only connect trusted ESP32 devices to your computer.

### Qt Framework Security

We use Qt 6.x which includes security improvements over Qt 5:

- Regular security updates from Qt maintainers
- Memory-safe string handling with QString
- Protected against common vulnerabilities (buffer overflows, format string bugs)

**Recommendation**: Keep Qt updated to the latest stable version.

### Build Security

- **CMake**: We use CMake 3.16+ with modern best practices
- **Compiler Warnings**: Build with `-Wall -Wextra` to catch potential issues
- **No Untrusted Input**: The application doesn't process untrusted external input
- **Code Review**: All changes go through pull request review

## Known Security Considerations

### Not Security-Sensitive

This application is designed for:

- **Local Use**: Ground control station on your own computer
- **Development/Testing**: UAV development and testing
- **Simulation**: Flight display simulation

It is **NOT** designed for:

- Network communication (no network sockets)
- Multi-user access control
- Processing untrusted data
- Mission-critical flight operations

### Physical Security

- The application assumes physical access to your computer is trusted
- ESP32 connection requires physical USB access
- No authentication is implemented for local use

### Data Privacy

- **No Data Collection**: The application does not collect or transmit user data
- **Local Only**: All data stays on your local machine
- **No Telemetry**: No usage statistics or crash reports are sent

## Best Practices for Users

### For Developers

- Keep Qt framework updated to latest stable version
- Use latest compiler with security features enabled
- Review third-party dependencies regularly
- Enable Address Sanitizer during development:
  ```bash
  cmake -DCMAKE_CXX_FLAGS="-fsanitize=address" ..
  ```

### For End Users

- Download releases from official GitHub repository only
- Verify release signatures (when implemented)
- Keep your operating system updated
- Only connect trusted ESP32 devices
- Review source code before building (it's open source!)

## Dependency Security

### Current Dependencies

- **Qt 6.x**: Maintained by The Qt Company with regular security updates
- **ESP32 Arduino Core**: Maintained by Espressif Systems
- **MPU6050 Library**: Community-maintained, widely used

### Updating Dependencies

We monitor security advisories for:

- Qt Framework vulnerabilities
- ESP32 firmware security issues
- Arduino library updates

Users are notified of critical dependency updates through:

- GitHub Security Advisories
- CHANGELOG.md notes
- Release notes

## Security Updates

Security updates are released as:

- **Patch versions** (e.g., 1.0.1) for minor security fixes
- **Minor versions** (e.g., 1.1.0) for moderate security improvements
- **Major versions** (e.g., 2.0.0) for significant security architecture changes

All security updates are documented in:

- [CHANGELOG.md](CHANGELOG.md)
- GitHub Security Advisories
- Release notes

## Security Hardening (Future)

We are considering these security enhancements for future versions:

- [ ] Code signing for release binaries
- [ ] Checksum verification for downloads
- [ ] Serial protocol authentication (optional)
- [ ] Encrypted serial communication (optional)
- [ ] Static analysis integration in CI/CD
- [ ] Fuzzing tests for serial data parser
- [ ] Dependency vulnerability scanning

## Responsible Disclosure

We believe in responsible disclosure:

- We'll work with you to understand and fix the vulnerability
- We'll coordinate public disclosure after a fix is available
- We'll credit researchers who report vulnerabilities (with permission)
- We aim to fix critical vulnerabilities within 30 days

## Attribution

We thank security researchers who help make Horus Project more secure:

- [List will be maintained here as vulnerabilities are reported and fixed]

## Questions?

If you have questions about this security policy, please:

- Open a public GitHub Discussion (for general security questions)
- Contact the maintainer privately (for specific vulnerability concerns)

---

**Security Contact**: [@joaoliveira6704](https://github.com/joaoliveira6704)

Thank you for helping keep Horus Project and its users safe! 🛡️
