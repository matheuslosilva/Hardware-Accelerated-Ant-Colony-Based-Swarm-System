CC = g++
BINARY = main

OBJ    = obj/
SRC 	= src/
SRC_IMGUI = include/extern/imgui/

FILES_IMGUI = imgui imgui_demo imgui_draw imgui_tables imgui_widgets backends/imgui_impl_glfw backends/imgui_impl_opengl3
FILES = main opengl/window/openglContext opengl/window/UI opengl/window/camera 
FILES += swarmEnvironment/foodSource swarmEnvironment/anthill swarmEnvironment/ant swarmEnvironment/antSensor swarmEnvironment/environment swarmEnvironment/parameterAssigner
FILES += opengl/render/EBO opengl/render/VBO opengl/render/VAO opengl/render/shader 
FILES += opengl/render/bufferManagers/openglBuffersManager
FILES += opengl/render/bufferManagers/antBufferManager opengl/render/bufferManagers/anthillBufferManager opengl/render/bufferManagers/foodBufferManager 
FILES += opengl/utils/glad opengl/utils/constants 

INCLUDES = ./include
INCLUDES_IMGUI = ./include/extern/imgui

LIBRARIES = -lGL -lglfw -lX11 -lpthread -lXrandr -ldl -lm #-lXi
OPTIONS = -g -O3 -march=native -Wall -pg

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
	@rm obj/opengl/render/*.o
	@rm obj/opengl/utils/*.o
	@rm obj/opengl/window/*.o
	@rm obj/swarmEnvironment/*.o







