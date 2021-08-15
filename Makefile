CC = g++
BINARY = main
SOURCES = main.cpp src/ant.cpp src/glad.c src/EBO.cpp src/VBO.cpp src/VAO.cpp src/stb_image.cpp
INCLUDES = ./include
LIBRARIES = -lGL -lglfw -lX11 -lpthread -lXrandr -lXi -ldl -lm
OPTIONS = -g -O3 -march=native #-Wall

all:
	$(CC) -o $(BINARY) $(SOURCES) -I$(INCLUDES) $(LIBRARIES) $(OPTIONS)

run:
	./$(BINARY)

clean:
	@rm *.o
