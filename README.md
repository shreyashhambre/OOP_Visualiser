# OOP Visualizer

An interactive C++/SDL2 educational simulator for visualizing Object-Oriented Programming concepts.

## Features

- Class & Object
- Encapsulation
- Inheritance
- Polymorphism
- Abstraction
- Constructor
- Destructor
- Virtual Function
- Stack / Heap visualization
- Source-code line highlighting
- RUN / STEP / PAUSE / RESET
- Graphical class/object diagrams
- Continuous animated data-flow dots and pulse effects
- Animated object creation, inheritance, polymorphic dispatch, lifecycle, and memory flow
- Console/output panel
- No `graphics.h`
- Ubuntu + g++ compatible

This is an educational simulation. It uses controlled examples and predefined execution steps rather than compiling arbitrary C++ source code.

## Requirements

Ubuntu/Debian:

```bash
sudo apt update
sudo apt install g++ libsdl2-dev libsdl2-ttf-dev
```

## Build

```bash
g++ -std=c++17 src/main.cpp src/Simulator.cpp src/Renderer.cpp \
    -Iinclude $(sdl2-config --cflags --libs) -lSDL2_ttf \
    -o oop_visualizer
```

Run:

```bash
./oop_visualizer
```

## Controls

- Mouse: select concepts and buttons
- RUN: automatic step execution
- STEP: advance one step
- PAUSE: pause automatic execution
- RESET: reset the selected simulation
- ESC: exit

## Project structure

```text
OOPVisualizer/
├── include/
│   ├── Simulator.h
│   └── Renderer.h
├── src/
│   ├── main.cpp
│   ├── Simulator.cpp
│   └── Renderer.cpp
└── README.md
```

## Design

The application separates:
- simulation state and educational execution steps
- rendering and UI
- SDL event handling

The visualizations are conceptual. For example, the virtual-function/vtable panel is a simplified teaching model, not a claim about one exact compiler implementation.


## Source Code Scrolling
The source-code panel is now larger and supports mouse-wheel scrolling. Move the mouse over the SOURCE CODE panel and use the wheel to view all lines. The scrollbar on the right side of the panel shows the current position.
