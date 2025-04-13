# Snowblower Simulation

https://github.com/user-attachments/assets/a5d7e2c6-15ca-480f-9d34-e1a176cc8551

## OpenGL Project Setup on macOS

To successfully run this project, follow the steps below. This guide assumes you're working on macOS and that you have **Homebrew** installed for package management.

## Step 1: Install Dependencies

First, you need to install the necessary dependencies for OpenGL development. Open the terminal and run the following command:

```bash
brew install glew freeglut glfw3
```

This will install the following libraries:

- **GLEW** (OpenGL Extension Wrangler)
- **FreeGLUT** (OpenGL Utility Toolkit)
- **GLFW3** (Library for creating windows and handling input)

These libraries are equivalent to the following packages on Linux (Ubuntu/Debian):

```bash
sudo apt-get install libx11-dev libglu1-mesa-dev freeglut3-dev libglew2.2 libglew-dev libglm-dev libglu1-mesa libgl1-mesa-glx libgl1-mesa-dev libglfw3-dev libglfw3
```

## Step 2: Compile the Source Code

After installing the required dependencies, you can compile the source code using the `g++` compiler.

Run the following command in your terminal:

```bash
g++ Source.cpp -o source.exe -Wall   -I/opt/homebrew/opt/glew/include   -I/opt/homebrew/opt/freeglut/include   -L/opt/homebrew/opt/glew/lib   -L/opt/homebrew/opt/freeglut/lib   -framework OpenGL -framework GLUT   -lglew -lglut
```

### Explanation:

- `Source.cpp` is the file name we are compiling.
- `-I` flags specify the include directories for the libraries.
- `-L` flags specify the library directories for the libraries.
- `-framework OpenGL` and `-framework GLUT` are used to link OpenGL and GLUT frameworks on macOS.
- `-lglew -lglut` link the GLEW and GLUT libraries.

> **Note**: While an ideal way to compile would be:

```bash
g++ Source.cpp -o source.exe -Wall -lGL -lGLU -lglut -lGLEW
```

The above method requires explicitly specifying the paths as your system setup may not locate the libraries automatically.

## Step 3: Run the Program

Once the compilation is successful, you can run the program by executing the following command in the terminal:

```bash
./source.exe
```

This will launch your OpenGL application.

---

By following these steps, you'll be able to install the necessary dependencies, compile the source code, and run the project on macOS using Homebrew for dependency management.
