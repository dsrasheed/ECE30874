build:
	@g++ -o ps6 ps6.cpp src/*.cpp -I./include -framework OpenGL -framework GLUT -lglew -std=c++11

build-linux:
	@g++ -o ps6 ps6.cpp src/*.cpp -I./include -lglut -lGLEW -lGL -std=c++11

run:
	-@export MallocStackLogging=1
	@leaks -atExit -- ./ps6 resources/scene1-5 resources/camera1-5

run-linux:
	@./ps6 resources/scene1-5 resources/camera1-5

test_%:
	-@g++ -o tmp test/$@.cpp src/*.cpp -I./include -framework OpenGL -framework GLUT -lglew
	-@export MallocStackLogging=1
	-@leaks -atExit -- ./tmp
	@rm tmp

zip:
	zip -r ps6-drasheed.zip * -x *.zip

clean:
	-@rm ps6
	-@rm *.gch

