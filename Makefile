CC = g++
BINARY = main
SOURCES = main.cpp src/ant.cpp src/antColony.cpp src/glad.c src/EBO.cpp src/VBO.cpp src/VAO.cpp src/manageGlBindings.cpp src/environment.cpp
INCLUDES = ./include
LIBRARIES = -lGL -lglfw -lX11 -lpthread -lXrandr -lXi -ldl -lm
OPTIONS = -g -O3 -march=native # -g #-Wall

all:
	$(CC) -o $(BINARY) $(SOURCES) -I$(INCLUDES) $(LIBRARIES) $(OPTIONS)

run:
	./$(BINARY)

clean:
	@rm *.o

