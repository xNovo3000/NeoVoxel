# NeoVoxel

Simple C++23 desktop game engine

### Macros definitions

- `NV_BUILD_TYPE`: current build type
- `NV_LOG`: logging
- `NV_KEY`: keyboard key
- `NV_MOUSE_BUTTON`: mouse button

### Getting Started

Requirements:
- `cmake`: version `3.20` or higher
- Any C++23 compiler
- Any OpenGL 3.3 Core or higher compatible VGA

### Using VSCode

Insert this properties in `.vscode/settings.json`:
```json
{
    "C_Cpp.files.exclude": {
        "/": true,
        "**/.vscode": true,
        "**/.vs": true,
        "**/.git": true
    },
    "cmake.modifyLists.addNewSourceFiles": "no",
    "cmake.modifyLists.removeDeletedSourceFiles": "no",
    "clangd.arguments": [
        "-header-insertion=never"
    ]
}
```

Install `clangd` extension and, when prompted, disable intellisense.

Insert this properties in `.vscode/c_cpp_properties.json` (choose only your current configuration):
```json
{
    "configurations": [
        {
            "name": "MacOS Apple Silicon",
            "includePath": [
                "${workspaceFolder}/engine/include",
                "${workspaceFolder}/engine/src",
                "${workspaceFolder}/engine/vendor/*/include",
                "${workspaceFolder}/sandbox/include",
                "${workspaceFolder}/sandbox/src"
            ],
            "compilerPath": "/usr/bin/clang",
            "cStandard": "c99",
            "cppStandard": "c++23",
            "intelliSenseMode": "macos-clang-arm64",
            "configurationProvider": "ms-vscode.makefile-tools"
        },
        {
            "name": "MacOS Intel",
            "includePath": [
                "${workspaceFolder}/engine/include",
                "${workspaceFolder}/engine/src",
                "${workspaceFolder}/engine/vendor/*/include",
                "${workspaceFolder}/sandbox/include",
                "${workspaceFolder}/sandbox/src"
            ],
            "compilerPath": "/usr/bin/clang",
            "cStandard": "c99",
            "cppStandard": "c++23",
            "intelliSenseMode": "macos-clang-x64",
            "configurationProvider": "ms-vscode.makefile-tools"
        },
        {
            "name": "Linux Intel",
            "includePath": [
                "${workspaceFolder}/engine/include",
                "${workspaceFolder}/engine/src",
                "${workspaceFolder}/engine/vendor/*/include",
                "${workspaceFolder}/sandbox/include",
                "${workspaceFolder}/sandbox/src"
            ],
            "compilerPath": "/usr/bin/g++",
            "cStandard": "c99",
            "cppStandard": "c++23",
            "intelliSenseMode": "linux-gcc-x64",
            "configurationProvider": "ms-vscode.makefile-tools"
        },
        {
            "name": "Windows Intel",
            "includePath": [
                "${workspaceFolder}/engine/include",
                "${workspaceFolder}/engine/src",
                "${workspaceFolder}/engine/vendor/*/include",
                "${workspaceFolder}/sandbox/include",
                "${workspaceFolder}/sandbox/src"
            ],
            "compilerPath": "TODO",
            "cStandard": "c99",
            "cppStandard": "c++23",
            "intelliSenseMode": "windows-msvc-x64",
            "configurationProvider": "ms-vscode.makefile-tools"
        }
    ],
    "version": 4
}
```