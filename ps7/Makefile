build:
	@g++ -o ps7 ps7.cpp src/*.cpp -I./include -framework OpenGL -framework GLUT -lglew -std=c++11

build-linux:
	@g++ -o ps7 ps7.cpp src/*.cpp -I./include -lglut -lGLEW -lGL -std=c++11

run:
	-@export MallocStackLogging=1
	@leaks -atExit -- ./ps7 resources/scene1-7 resources/camera1-5

run-linux:
	@./ps7 resources/scene1-7 resources/camera1-5

test_%:
	@g++ -o tmp test/$@.cpp src/*.cpp -I./include -framework OpenGL -framework GLUT -lglew
	-@export MallocStackLogging=1
	-@leaks -atExit -- ./tmp
	@rm tmp

zip:
	zip -r ps7-drasheed.zip * -x *.zip

clean:
	-@rm ps7
	-@rm *.gch

