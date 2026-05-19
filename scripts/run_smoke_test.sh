#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")/.."

bash scripts/clean_loaded_modules.sh
make clean >/dev/null 2>&1 || true

echo "[1] build all provided modules"
make
echo

echo "[2] hello_module lifecycle"
sudo insmod ./hello_module.ko
lsmod | grep hello_module
sudo rmmod hello_module
sudo dmesg -T | tail -n 8
echo

echo "[3] module_param observation"
sudo insmod ./module_param.ko name=smoke_test debug=1
lsmod | grep module_param
sudo rmmod module_param
sudo dmesg -T | tail -n 8
echo

echo "[4] usage count observation"
sudo insmod ./usage_provider.ko
lsmod | grep usage_
sudo insmod ./usage_consumer.ko
lsmod | grep usage_
if sudo rmmod usage_provider 2>/tmp/usage_provider_rmmod.err; then
    echo "ERROR: usage_provider was removed while usage_consumer was loaded"
    exit 1
else
    cat /tmp/usage_provider_rmmod.err
fi
sudo rmmod usage_consumer
sudo rmmod usage_provider
echo

echo "[5] cleanup check"
bash scripts/clean_loaded_modules.sh
lsmod | grep -E 'hello_module|module_info|module_param|usage_provider|usage_consumer' || true
echo "smoke test complete"

