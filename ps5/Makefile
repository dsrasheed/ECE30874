build:
	@g++ -o ps5 ps5.cpp src/*.cpp -I./include -framework OpenGL -framework GLUT -lglew

build-linux:
	@g++ -o ps5 ps5.cpp src/*.cpp -I./include -lglut -lGLEW -lGL

run:
	-@export MallocStackLogging=1
	@leaks -atExit -- ./ps5 resources/scene1-5 resources/camera1-5

run-linux:
	@./ps5 resources/scene1-5 resources/camera1-5

test_%:
	-@g++ -o tmp test/$@.cpp src/*.cpp -I./include -framework OpenGL -framework GLUT -lglew
	-@export MallocStackLogging=1
	-@leaks -atExit -- ./tmp
	@rm tmp

zip:
	zip -r ps5-drasheed.zip * -x *.zip

clean:
	-@rm ps5
	-@rm *.gch

