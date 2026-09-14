#!/usr/bin/env bash

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
VENV_DIR="$PROJECT_ROOT/.pio-venv"
PLATFORMIO_VERSION="6.1.19"

echo "=== SmartVent Wokwi Setup ==="
echo "Project root: $PROJECT_ROOT"

echo "Checking Python..."
if ! command -v python3 >/dev/null 2>&1; then
    echo "Error: Python 3 is not installed."
    exit 1
fi

echo "Creating PlatformIO virtual environment..."
if [ ! -d "$VENV_DIR" ]; then
    python3 -m venv "$VENV_DIR"
fi

echo "Activating virtual environment..."
source "$VENV_DIR/bin/activate"

echo "Installing PlatformIO $PLATFORMIO_VERSION..."
python -m pip install --upgrade pip
python -m pip install "platformio==$PLATFORMIO_VERSION"

echo "Building Wokwi ESP32 project..."
cd "$PROJECT_ROOT"
pio run

echo ""
echo "=== Setup complete ==="
echo "PlatformIO version:"
pio --version
echo ""
echo "Wokwi project built successfully."
