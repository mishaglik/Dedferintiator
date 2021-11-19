LIB_DIR = lib/
BIN_DIR = bin/
SRC_DIR = src/

MAJOR_VERSION = 1
MINOR_VERSION = 0
# BUILD_VERSION = `cat bld_version`
# TODO: Auto increment version

SDL_LIB   = SDL2 SDL2main SDL2_image
LIBRARIES = Logger Stack File $(SDL_LIB)

CXXFLAGS = `cat $(LIB_DIR)Cflags`
SANFLAGS = `cat $(LIB_DIR)SanitizeFlags`
LXXFLAGS = -L$(LIB_DIR) $(addprefix -l, $(LIBRARIES))

CXXFLAGS += -DMAJOR_VERSION=$(MAJOR_VERSION)
CXXFLAGS += -DMINOR_VERSION=$(MINOR_VERSION)
# CXXFLAGS += -DBUILD_VERSION=$(BUILD_VERSION)

# CXXFLAGS += $(SANFLAGS)

SOURCES_Tree = Tree.cpp

SOURCES_StringBuffer = StringBuffer.cpp

SOURCES_AkkWindow = Window.cpp Application.cpp

SUBDIRS = ${shell find $(SRC_DIR) -type d -printf '%P '}

SOURCES = Akkinator.cpp Akkinator_Verbose.cpp $(foreach dir, $(SUBDIRS), $(addprefix $(dir)/, $(SOURCES_$(dir))))

EXECUTABLE  = mainG.cpp #main.cpp 

SRC = $(SOURCES) $(EXECUTABLE)

OBJ = $(SRC:.cpp=.o)

DEP = $(SRC:.cpp=.d)

TARGETS = main

init:
	mkdir -p $(addprefix $(BIN_DIR), $(SUBDIRS))

all: $(TARGETS)
	# ./increaseVersion.sh bld_version

main: $(addprefix $(BIN_DIR), $(OBJ))
	g++ $(CXXFLAGS) $^ $(LXXFLAGS) -o $@

$(BIN_DIR)%.o : $(SRC_DIR)%.cpp
	g++ -c $(CXXFLAGS) $(LXXFLAGS) -o $@ $<

.PHONY: deps
deps: $(addprefix $(BIN_DIR), $(DEP))
	echo "Deps builded"
	
$(BIN_DIR)%.d : $(SRC_DIR)%.cpp
	g++ -MM -MT $(@:.d=.o) $< -o $@

-include $(addprefix $(BIN_DIR), $(DEP))

.PHONY: clean
clean:
	rm -f $(addprefix $(BIN_DIR), $(OBJ) $(DEP))
	rm $(TARGETS)
