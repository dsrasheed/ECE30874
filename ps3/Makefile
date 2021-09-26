build:
	@g++ -o ps3 ps3.cpp src/*.cpp -I./include -framework OpenGL -framework GLUT -lglew

build-linux:
	@g++ -o ps3 ps3.c src/*.c src/*.cpp -I./include -lglut -lGLEW -lGL

run:
	@leaks -atExit -- ./ps3

test_%:
	-@g++ -o tmp test/$@.cpp src/*.cpp -I./include -framework OpenGL -framework GLUT -lglew
	-@export MallocStackLogging=1
	-@leaks -atExit -- ./tmp
	@rm tmp

clean:
	-@rm ps3
	-@rm *.gch

