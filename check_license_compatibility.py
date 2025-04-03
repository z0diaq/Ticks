#!/usr/bin/env python3
"""
Script to check license compatibility with LGPLv3
"""

import sys
import os

# Define known compatible and incompatible licenses
compatible_licenses = [
    "LGPL-3.0", "LGPL-2.1", "GPL-3.0", "GPL-2.0", 
    "MIT", "BSD-2-Clause", "BSD-3-Clause", "Apache-2.0", 
    "MPL-2.0", "Zlib", "ISC", "Unlicense", "CC0-1.0",
    "wxWindows", "WTFPL"  # wxWidgets uses a custom wxWindows license
]

incompatible_licenses = [
    "GPL-1.0", "AGPL-1.0", "CDDL-1.0", "EPL-1.0", "EPL-2.0"
]

# Dependencies used in the project
dependencies = {
    "wxWidgets": {"license": "wxWindows", "version": "3.2.0", "usage": "dynamic linking"},
    "yaml-cpp": {"license": "MIT", "version": "0.7.0", "usage": "dynamic linking"},
    "googletest": {"license": "BSD-3-Clause", "version": "1.12.0", "usage": "static linking, testing only"}
}

def check_compatibility():
    """Check if all dependencies are compatible with LGPLv3"""
    incompatible_deps = []
    
    for dep_name, dep_info in dependencies.items():
        license_type = dep_info["license"]
        if license_type in incompatible_licenses:
            incompatible_deps.append({
                "name": dep_name,
                "license": license_type,
                "reason": "Incompatible license"
            })
        elif license_type not in compatible_licenses:
            incompatible_deps.append({
                "name": dep_name,
                "license": license_type,
                "reason": "Unknown license compatibility"
            })
    
    if incompatible_deps:
        print("Warning: Incompatible licenses detected!")
        for dep in incompatible_deps:
            print(f"  - {dep['name']} ({dep['license']}): {dep['reason']}")
        return False
    else:
        print("All dependencies are compatible with LGPLv3")
        return True

if __name__ == "__main__":
    print("Checking license compatibility with LGPLv3...")
    result = check_compatibility()
    
    print("\nDependency summary:")
    for dep_name, dep_info in dependencies.items():
        print(f"  - {dep_name} ({dep_info['version']}): {dep_info['license']}")
        print(f"    Usage: {dep_info['usage']}")
    
    if not result:
        sys.exit(1)