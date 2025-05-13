# opengl-glsl-renderer
A c++ project with various shader implementations in glsl and a graphical pipeline implementation in OpenGL.

## Requirements
You need to have glew, freeglut, and g++

## Running
Just use the provided script
```
./run.sh
```

## Config
There is a very rudimentary config system in `defaults.h`. You can change the scenes here.

## Keybinds

### Camera Movement (Relative to View Direction)
| Key       | Action           |
|-----------|------------------|
| `WASD`    | Move around      |
| `Space`   | Move upward      |
| `C`       | Move downward    |

### Camera Rotation (Local Axes)
| Key       | Action                   |
|-----------|--------------------------|
| `R`       | Pitch up                 |
| `F`       | Pitch down               |
| `T`       | Yaw right                |
| `G`       | Yaw left                 |
| `Q`       | Roll clockwise           |
| `E`       | Roll counter-clockwise   |

### World-Axis Movement (Absolute Positioning)
| Key        | Action           |
|------------|------------------|
| `XYZ`        | Move along +X, +Y, +Z|
| `XYZ+Shift`  | Move along -X, -Y, -Z|


### View & Shader Utilities
| Key       | Action                                      |
|-----------|---------------------------------------------|
| `M`       | Cycle polygon mode: Point → Line → Fill     |
| `1`       | Switch to perspective projection            |
| `2`       | Switch to orthogonal projection             |
| `B`       | Cycle through available shader programs     |

### Exit
`Shift+Q`

## TODOs
The project structure is quite messy, it should be fixed sometime.
