CC = g++
OBJ    = obj/
SRC 	= src/
BINARY = main
FILES = main ant antColony glad EBO VBO VAO manageGlBindings glfwenvironment constants
INCLUDES = ./include
LIBRARIES = -lGL -lglfw -lX11 -lpthread -lXrandr -lXi -ldl -lm
OPTIONS = -g -O3 -march=native # -g #-Wall

SOURCES=$(patsubst %, ${SRC}%.cpp, ${FILES})
HEADERS=$(patsubst %, ${SRC}%.h, ${FILES})
OBJECTS=$(patsubst %, ${OBJ}%.o, ${FILES})

${OBJ}%.o: ${SRC}%.cpp
	${CC} -c $< -o $@ -I$(INCLUDES) $(LIBRARIES) $(OPTIONS)

all: ${OBJECTS} ${SOURCES}
	$(CC) -o $(BINARY) $(OBJECTS) -I$(INCLUDES) $(LIBRARIES) $(OPTIONS)


run:
	./$(BINARY)

clean:
	@rm obj/*.o

