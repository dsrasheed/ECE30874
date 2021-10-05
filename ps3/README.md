# Build:
make build-linux

# Run with my scene file and camera file:
make run-linux

# Run custom scene file/camera_file
./ps3 scene_file camera_file
Note: The camera_file follows the same format as the demo except it 
skips the vertex and fragment shader file names and starts at the fov.
See resources/camera1-3 for an example