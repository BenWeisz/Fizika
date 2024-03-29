# Fizika

## Description

Fizika [ˈfizikɒ] is the Hungarian word for physics

This program is meant as a basis for physics based simulations / animations. The renderer is implemented in C++ using Opengl and GLSL 4.5
If you would like to take a look at some of the physics simulations, take a look at the branches.

Currently the following simulations are implemented:
|          Simulation          |                                               Branch                                               |
| :--------------------------: | :------------------------------------------------------------------------------------------------: |
|        Bouncing Balls        |           [bouncing-particle](https://github.com/BenWeisz/Fizika/tree/bouncing-particle)           |
| Bouncing Balls w/ Collisions | [bouncing-particle-collision](https://github.com/BenWeisz/Fizika/tree/bouncing-particle-collision) |
|  Linear Spring-Mass System   |                 [mass-spring](https://github.com/BenWeisz/Fizika/tree/mass-spring)                 |

## Required Setup

The following libraries / implementations are required. Download the following and place them in a single folder. I've place mine in a folder in the root folder. My file structure:

```
~/vendors
├── glad-core-4.5
├── glfwkeys
│   ├── glfw_keycodes_to_string.hpp
├── glm
├── imgui
├── implot
├── stb
└── tinyxml
```

You'll also need to install Eigen3 and GLFW system wide. You may also need to install OpenCL if you wish to rebuild the model files, but the model binaries will be provided for each branch.

Download them here:
- [Glad](https://glad.dav1d.de/)
- [GLFW keys](https://gist.github.com/0xD34DC0DE/910855d41786b962127ae401da2a3441)
- [glm](https://github.com/g-truc/glm)
- [imgui](https://github.com/ocornut/imgui)
- [implot](https://github.com/epezent/implot)
- [stb](https://github.com/nothings/stb)
- [tinyxml](https://github.com/leethomason/tinyxml2)
- [GLFW](https://www.glfw.org/download)
- [Eigen3](https://robots.uc3m.es/installation-guides/install-eigen.html)

Make sure to modify the `VENDOR_DIR` in the `CMakeLists.txt` file

Next create a build directory in the main project folder and generate the make file with cmake. Finally run cmake
```sh
mkdir build
cd build
cmake ..
make Fizika
./Fizika
```

Additionally if you wish to run the DataBuild Pipeline you'll need to build it as well.

```sh
make DataBuild
./DataBuild ../res/databuild/databuild_sample.xml
```

![Demo Image](https://raw.githubusercontent.com/BenWeisz/Fizika/main/images/demo.png)
