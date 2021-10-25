# Build:
```bash
make build-linux
```

# Run with my scene file and camera file:
```bash
make run-linux
```

# Run custom scene file/camera_file
```bash
./ps5 scene_file camera_file
```
Note: The camera_file follows this format
```bash
45 1 100 25 25 25 0 0 0 0 0 1
```
It's the same as the demo except it skips the fragment shader and vertex shader file names.
