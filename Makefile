CC = g++
BINARY = main

OBJ    = obj/
SRC 	= src/
SRC_IMGUI = extern/imgui/

FILES_IMGUI = imgui imgui_demo imgui_draw imgui_tables imgui_widgets backends/imgui_impl_glfw backends/imgui_impl_opengl3
FILES = main ant antColony environment render/EBO render/VBO render/VAO render/openglBuffersManager render/openglContext utils/constants utils/glad window/UI window/camera 

INCLUDES = ./include
INCLUDES_IMGUI = ./extern/imgui

LIBRARIES = -lGL -lglfw -lX11 -lpthread -lXrandr -lXi -ldl -lm
OPTIONS = -g -O3 -march=native # -g #-Wall

SOURCES=$(patsubst %, ${SRC}%.cpp, ${FILES})
HEADERS=$(patsubst %, ${SRC}%.h, ${FILES})
OBJECTS=$(patsubst %, ${OBJ}%.o, ${FILES})

SOURCES_IMGUI=$(patsubst %, ${SRC_IMGUI}%.cpp, ${FILES_IMGUI})
OBJECTS_IMGUI=$(patsubst %, ${OBJ}%.o, ${FILES_IMGUI})

${OBJ}%.o: ${SRC}%.cpp
	${CC} -c $< -o $@ -I$(INCLUDES) -I$(INCLUDES_IMGUI) $(LIBRARIES) $(OPTIONS)

${OBJ}%.o: ${SRC_IMGUI}%.cpp
	${CC} -c $< -o $@ -I$(INCLUDES_IMGUI) $(LIBRARIES) $(OPTIONS)

all: ${OBJECTS} ${SOURCES} ${OBJECTS_IMGUI} ${SOURCES_IMGUI}
	$(CC) -o $(BINARY) $(OBJECTS) ${OBJECTS_IMGUI} -I$(INCLUDES) $(LIBRARIES) $(OPTIONS)


run:
	./$(BINARY)

clean:
	@rm obj/*.o

