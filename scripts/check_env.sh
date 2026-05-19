#!/usr/bin/env bash
set -u

kernel="$(uname -r)"
build="/lib/modules/${kernel}/build"
status=0

echo "[kernel]"
uname -a
echo

echo "[architecture]"
uname -m
dpkg --print-architecture 2>/dev/null || true
echo

echo "[compiler]"
if command -v gcc >/dev/null 2>&1; then
    gcc --version | sed -n '1p'
else
    echo "MISSING: gcc"
    status=1
fi

if command -v make >/dev/null 2>&1; then
    make --version | sed -n '1p'
else
    echo "MISSING: make"
    status=1
fi
echo

echo "[kernel build tree]"
ls -ld "$build" 2>&1 || true
if [ -e "$build/Makefile" ]; then
    echo "OK: $build is ready."
else
    echo "MISSING: $build"
    echo "Install kernel headers that match the running kernel."
    status=1
fi
echo

echo "[header packages]"
dpkg -l 2>/dev/null | grep -E 'linux-headers|raspberrypi-kernel-headers' || true
echo

if [ "$status" -eq 0 ]; then
    echo "Result: ready for Chapter 9 kernel module practice."
else
    echo "Result: setup is incomplete."
fi

exit "$status"

