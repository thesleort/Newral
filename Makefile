    #Made by Troels Blicher Petersen (trpet15)

EXE = newral.out

TEST = programtest

SRC_DIR = src
INC_DIR = -Iinclude/
OBJ_DIR = build
MOD_DIR = src/modules
INC_ALL = -Iinclude/ $(INC_MOD)

MODULES = $(filter-out $(MOD_DIR)/README.md $(wildcard $(SRC_DIR)/*.cpp), $(wildcard $(MOD_DIR)/* $(SRC_DIR)/*/))

PKG = libconfig++

MAIN_DIR = main
TEST_DIR = tests

INC_MOD = $(addprefix -I, $(wildcard $(MOD_DIR)/*/include/))

SRC = $(filter-out $(wildcard $(SRC_DIR)/tests.c), \
				$(wildcard $(SRC_DIR)/*.cpp \
				$(SRC_DIR)/*/*.cpp \
				$(MOD_DIR)/*/*.cpp ) )
INC = 	$(INC_DIR) \
		$(INC_MOD)
OBJRT 	= $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJOTH  = $(OBJRT:$(SRC_DIR)/*/%.cpp=$(OBJ_DIR)/*/%.o)
OBJMOD 	= $(OBJOTH:$(MOD_DIR)/*/%.cpp=$(OBJ_DIR)/modules/*/%.o)
OBJ = $(OBJMOD)
LIB = $(LDFLAGS)

TEST_SRC = $(filter-out $(wildcard src/main.c) $(MOD_DIR)/README.md, \
				$(wildcard $(SRC_DIR)/*.cpp $(MOD_DIR)/symbol_tree/*.cpp))
TEST_OBJ = $(TEST_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

TC_SRC = $(filter-out $(wildcard src/main.c) $(wildcard $(SRC_DIR)/scan_parse.c \), \
				$(wildcard $(SRC_DIR)/*.c $(MOD_DIR)/symbol_tree/*.c))

CC = g++

CPPFLAGS += $(INC)								# -I is a preprocessor flag, not a compiler flag
CFLAGS += -std=c++14 `pkg-config --cflags $(PKG)` -Wall -Wextra -pedantic -g	# some warnings about bad code

BOOST = -lboost_regex
# -L is a linker flag
LDFLAGS += -Llib
# Left empty if no libs are needed
LDLIBS += -lm -lOpenCL `pkg-config --libs $(PKG)`

all: $(EXE)

###
## Compiler compilation
###
$(EXE): $(OBJ)
	$(CC) $(LIB) $(INC) $^ $(LDLIBS) -o $@ $(BOOST)

###
## Module and object building
### 
$(OBJ_DIR)/modules/*/%.o: $(MOD_DIR)/*/%.cpp
	mkdir -p $(OBJ_DIR)/modules/*
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

###
## Test program compilation
###
$(TEST): $(TEST_OBJ)
	$(CC) $(LIB) $(INC) $^ $(LDLIBS) -o $@

###
## Project files object creation
###
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(addprefix $(OBJ_DIR)/, $(MODULES:$(SRC_DIR)/%=%))
	$(CC) $(INC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR)/*

clean-all:
	rm -rf $(OBJ_DIR)/*
	$(RM) $(EXE)
	$(RM) $(TEST)
