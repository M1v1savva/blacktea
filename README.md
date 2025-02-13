
# Hello, World!... in BlackTea

Package that prints "Hello, world!".

## Install cmake on Ubuntu

```bash
sudo apt purge --auto-remove cmake
```
```bash
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
```
```bash
echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ noble main' | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null
```
```bash
sudo apt update
sudo apt install cmake
```

## Build

```bash
mkdir build && cd build
cmake ..
make