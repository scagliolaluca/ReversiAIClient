#-------------------------------------------------------------------------------------------------------------------------------
# SETTINGS
#-------------------------------------------------------------------------------------------------------------------------------

## Executable name
TARGET_NAME ?= client03
DEBUG_NAME ?= debug.app
## Directorys
SRC_DIR := src
INC_DIR := include
#LIB_DIR := dependencies\lib
BIN_DIR := bin
DEBUG_DIR := $(BIN_DIR)
OBJ_DIR := src/obj
DEP_DIR := src/dep

## Compiler
CXX := g++
CC := gcc
AS := as

## VPATH
vpath %.cpp $(SRC_DIR)
vpath %.c $(SRC_DIR)
vpath %.s $(SRC_DIR)
vpath %.o $(OBJ_DIR)
vpath %.d $(DEP_DIR)

## Flags
### Default mode - debug
DEBUG_FLAG =
INC_FLAGS = -I$(INC_DIR)
LINK_FLAGS = #-L$(LIB_DIR)
LINK_FILES = 
GENERAL_FLAGS = -Wall -Wextra
CPP_FLAGS = -std=c++11
C_FLAGS =
ASM_FLAGS = 
### Compiler flags for dependencies
DEPFLAGS += -MT $@ -MMD -MP -MF

SUFFIXES += .d

#-------------------------------------------------------------------------------------------------------------------------------
# Scraper
#-------------------------------------------------------------------------------------------------------------------------------

## Sources
SRCS_PATH := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*.s)
SRCS := $(SRCS_PATH:src/%=%)
## Objectfiles
OBJS := $(SRCS:%=%.o)
OBJS_PATH := $(addprefix $(OBJ_DIR)/, $(OBJS))
DEPS := $(OBJS:%.o=$(DEP_DIR)/%.d)

#-------------------------------------------------------------------------------------------------------------------------------
# Build
#-------------------------------------------------------------------------------------------------------------------------------

## Default mode - release (best practise: clean before build)
.PHONY: all
all: $(BIN_DIR)/$(TARGET_NAME)


## Debug mode
.PHONY: debug 
debug: DEBUG_FLAG += -g
debug: $(DEBUG_DIR)/$(DEBUG_NAME)

## profiler flag set
.PHONY: prof
prof: GENERAL_FLAGS += -pg
prof: $(BIN_DIR)/$(TARGET_NAME)


## Debug build
$(DEBUG_DIR)/$(DEBUG_NAME): $(OBJS)
	${shell mkdir -p $(DEBUG_DIR)}
	$(CXX) $(OBJS_PATH) $(GENERAL_FLAGS) $(LINK_FLAGS) $(LINK_FILES) $(DEBUG_FLAG) -o $@

## Release build
$(BIN_DIR)/$(TARGET_NAME): $(OBJS)
	${shell mkdir -p $(BIN_DIR)}
	$(CXX) $(OBJS_PATH) $(GENERAL_FLAGS) $(LINK_FLAGS) $(LINK_FILES) $(DEBUG_FLAG) -o $@

## Compile .cpp files
%.cpp.o: %.cpp %.cpp.d
	${shell mkdir -p $(OBJ_DIR)}
	${shell mkdir -p $(DEP_DIR)}
	$(CXX) $(DEPFLAGS) $(DEP_DIR)/$*.cpp.d $< -c $(DEBUG_FLAG) $(INC_FLAGS) $(CPP_FLAGS) $(GENERAL_FLAGS) -o $(OBJ_DIR)/$@

## Compile .c files
%.c.o: %.c %.c.d
	${shell mkdir -p $(OBJ_DIR)}
	${shell mkdir -p $(DEP_DIR)}
	$(CC) $(DEPFLAGS) $(DEP_DIR)/$*.c.d $< -c $(DEBUG_FLAG) $(INC_FLAGS) $(C_FLAGS) $(GENERAL_FLAGS) -o $(OBJ_DIR)/$@

## Compile .s files
%.s.o: %.s %.s.d
	${shell mkdir -p $(OBJ_DIR)}
	${shell mkdir -p $(DEP_DIR)}
	$(AS) $(DEPFLAGS) $(DEP_DIR)/$*.s.d $< -c $(DEBUG_FLAG) $(INC_FLAGS) $(ASM_FLAGS) $(GENERAL_FLAGS) -o $(OBJ_DIR)/$@

## Empty build for dependencies
%.d: ;

## include dependencies
-include $(DEPS)

#-------------------------------------------------------------------------------------------------------------------------------
# Clean / Delete
#-------------------------------------------------------------------------------------------------------------------------------

.PHONY: clean
clean:
	${shell rm -r $(OBJ_DIR)}
	${shell rm -r $(DEP_DIR)}
	${shell rm -r $(BIN_DIR)}