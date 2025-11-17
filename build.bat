@echo off
REM Horus Project - Build Script for Windows
REM Author: João Oliveira (@joaoliveira6704)

setlocal enabledelayedexpansion

echo ============================================
echo     Horus Project Build Script (Windows)
echo ============================================
echo.

REM Parse arguments
set BUILD_TYPE=Release
set CLEAN=0
set VERBOSE=0
set GENERATOR=Visual Studio 17 2022

:parse_args
if "%1"=="" goto check_dependencies
if /i "%1"=="-d" set BUILD_TYPE=Debug
if /i "%1"=="--debug" set BUILD_TYPE=Debug
if /i "%1"=="-c" set CLEAN=1
if /i "%1"=="--clean" set CLEAN=1
if /i "%1"=="-v" set VERBOSE=1
if /i "%1"=="--verbose" set VERBOSE=1
if /i "%1"=="-h" goto show_help
if /i "%1"=="--help" goto show_help
if /i "%1"=="-g" (
    set GENERATOR=%2
    shift
)
shift
goto parse_args

:show_help
echo Usage: build.bat [OPTIONS]
echo.
echo Options:
echo   -d, --debug       Build in Debug mode (default: Release)
echo   -c, --clean       Clean build directory before building
echo   -v, --verbose     Verbose output
echo   -g GENERATOR      CMake generator (default: Visual Studio 17 2022)
echo   -h, --help        Show this help message
echo.
echo Example:
echo   build.bat --clean --debug
exit /b 0

:check_dependencies
echo Build Configuration:
echo   Build Type: %BUILD_TYPE%
echo   Clean Build: %CLEAN%
echo   Verbose: %VERBOSE%
echo   Generator: %GENERATOR%
echo.

REM Check for CMake
where cmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake not found! Please install CMake 3.16+
    exit /b 1
)
echo [OK] CMake found

REM Check for Qt
where qmake6 >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Qt 6 found
    goto qt_found
)
where qmake >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [OK] Qt found
    goto qt_found
)
echo [ERROR] Qt not found! Please install Qt 6.x or Qt 5.12+
exit /b 1

:qt_found
echo.

REM Clean if requested
if %CLEAN%==1 (
    echo Cleaning build directory...
    if exist build rmdir /s /q build
    echo.
)

REM Create build directory
echo Creating build directory...
if not exist build mkdir build
cd build

REM Configure with CMake
echo Configuring with CMake...
set CMAKE_ARGS=-G "%GENERATOR%" -A x64 -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
if %VERBOSE%==1 set CMAKE_ARGS=%CMAKE_ARGS% -DCMAKE_VERBOSE_MAKEFILE=ON

cmake .. %CMAKE_ARGS%
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake configuration failed!
    cd ..
    exit /b 1
)
echo [OK] CMake configuration successful
echo.

REM Build
echo Building Horus Project...
cmake --build . --config %BUILD_TYPE% --parallel
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build failed!
    cd ..
    exit /b 1
)
echo [OK] Build successful!
echo.

REM Check for executable
if exist "%BUILD_TYPE%\Horus-Project.exe" (
    echo [OK] Executable created: %BUILD_TYPE%\Horus-Project.exe
    echo.
    echo ============================================
    echo   Build Complete!
    echo ============================================
    echo.
    echo To run the application:
    echo   build\%BUILD_TYPE%\Horus-Project.exe
    echo.
) else (
    echo [ERROR] Executable not found!
    cd ..
    exit /b 1
)

cd ..
exit /b 0
