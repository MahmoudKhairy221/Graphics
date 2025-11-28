# Chrono Heist - OpenGL Game

A 3D adventure game where a time-traveling agent retrieves temporal artifacts across two scenes: a Neo-Tokyo research facility and an ancient Egyptian temple.

## Requirements

- CMake 3.15+
- OpenGL 3.3+ compatible graphics card
- C++17 compatible compiler
- GLFW3
- Assimp
- GLAD (included in external/)
- GLM (included in external/)

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Running

```bash
./ChronoHeist
```

Or on WSL:
```bash
wsl -d Ubuntu -e bash -c "cd /mnt/c/Users/mahmo/Desktop/Graphics-1/build && ./ChronoHeist"
```

## Controls

- **WASD / Arrow Keys**: Move
- **Space**: Jump
- **Mouse**: Look around
- **C**: Toggle between first-person and third-person camera
- **Mouse Click / E**: Interact with objects (console, etc.)

## Gameplay

### Neo-Tokyo Research Facility
- Collect 6 Temporal Crystals (500 points each)
- Avoid Laser Grids, Motion Detectors, and Security Cameras
- Activate the Control Console to unlock the Exit Portal
- Reach the Exit Portal to win

### Ancient Egyptian Temple
- Collect 6 Golden Scarabs (500 points each)
- Avoid Pressure Plates, Moving Platforms, and Guardian Statues
- Collect at least 4 Scarabs to energize the Exit Portal
- Reach the Exit Portal to win

## Project Structure

- `src/`: Source code
- `assets/`: 3D models, textures, sounds
- `external/`: Third-party libraries (GLAD, GLM, stb_image)

