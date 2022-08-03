# Installation

## Requirements

Windows 10,
Visual Studio 2019,
Git,
[vcpkg](https://github.com/microsoft/vcpkg) (or follow these [steps](https://www.sfml-dev.org/tutorials/2.5/start-vc.php))

## Building project using vcpkg
Go to the directory where the tool ``vcpkg`` is installed.

```
cd $(Absolute_Path_To_vcpkg_Directory)$
``` 

Then install the SFML library and integrate it with Visual Studio 2019.

```
.\vcpkg\vcpkg install sfml
.\vcpkg\vcpkg integrate install
```

Then just open the file ``Platformer_game.sln`` in Visual Studio 2019 and build the project.
It is recommended to use x86 platform, because x64 version of SFML may cause problems.
