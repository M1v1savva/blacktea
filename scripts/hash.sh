#!/bin/bash

# standalone script to calculate .sha256

sha256sum "$1.tar.gz" > "$1.sha256"