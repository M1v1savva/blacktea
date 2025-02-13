#!/bin/bash

PACKAGE_NAME="hello_package_v0.1"
ARCHIVE_PATH="./../$PACKAGE_NAME.tar.gz"

if [[ $EUID -ne 0 ]]; then
    echo "Please run as root to install $PACKAGE_NAME."
    exit 1
fi

if [ -f "$INSTALL_DIR/$PACKAGE_NAME" ]; then
    echo "Removing old version..."
    rm -f "$INSTALL_DIR/$PACKAGE_NAME"
fi

# Extract and move the new binary
if [ -f "$ARCHIVE_PATH" ]; then
    tar -xzf "$ARCHIVE_PATH" -C "$INSTALL_DIR"
    mv "$INSTALL_DIR/$PACKAGE_NAME/$PACKAGE_NAME" "$INSTALL_DIR/$PACKAGE_NAME"
    chmod +x "$INSTALL_DIR/$PACKAGE_NAME"
    echo "$PACKAGE_NAME installed successfully in $INSTALL_DIR."
else
    echo "Error: Archive not found at $ARCHIVE_PATH."
    exit 1
fi
