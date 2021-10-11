build:
	@g++ -o ps4 ps4.cpp src/*.cpp -I./include -framework OpenGL -framework GLUT -lglew

build-linux:
	@g++ -o ps4 ps4.cpp src/*.cpp -I./include -lglut -lGLEW -lGL

run:
	-@export MallocStackLogging=1
	@leaks -atExit -- ./ps4 resources/scene1-3 resources/camera1-3

run-linux:
	@./ps4 resources/scene1-3 resources/camera1-3

test_%:
	-@g++ -o tmp test/$@.cpp src/*.cpp -I./include -framework OpenGL -framework GLUT -lglew
	-@export MallocStackLogging=1
	-@leaks -atExit -- ./tmp
	@rm tmp

zip:
	zip -r ps4-drasheed.zip * -x *.zip

clean:
	-@rm ps4
	-@rm *.gch

