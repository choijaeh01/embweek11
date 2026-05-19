#!/usr/bin/env bash
set -euo pipefail

for module in usage_consumer usage_provider module_param module_info hello_module; do
    if lsmod | awk '{print $1}' | grep -qx "$module"; then
        echo "Removing $module"
        sudo rmmod "$module"
    fi
done

echo "No target practice modules remain loaded."
