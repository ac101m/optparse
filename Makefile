# Output executables & libraries
OUTPUT_TEST ?= bin/tests/test
OUTPUT_LIB_STATIC ?= bin/install/liboptparse.a
OUTPUT_LIB_SHARED ?= bin/install/liboptparse.so

# Directory controls
OBJ_DIR ?= build
SRC_DIRS ?= src
INC_DIRS ?= include

# Compiler configuration
AR = ar
CXX = g++
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
BASE_FLAGS ?= -MMD -MP -m64 -std=c++11
COMPILE_FLAGS ?= $(INC_FLAGS) $(BASE_FLAGS) -g

# Sources that define mains
MAIN_SRCS := $(shell find $(SRC_DIRS) -maxdepth 1 -name *.cpp)
MAIN_OBJS := $(MAIN_SRCS:%=$(OBJ_DIR)/%.o)
MAIN_DEPS := $(MAIN_OBJS:.o-.d)

# Sources that don't define mains
SUB_SRCS := $(shell find $(SRC_DIRS) -mindepth 2 -name *.cpp)
SUB_OBJS := $(SUB_SRCS:%=$(OBJ_DIR)/%.o)
SUB_DEPS := $(SUB_OBJS:.o-.d)

# Object compilation
$(OBJ_DIR)/%.cpp.o: %.cpp
	@$(MKDIR_P) $(dir $@)
	$(CXX) $(COMPILE_FLAGS) -c $< -o $@

# Static library
LIB_STATIC_OBJS := $(SUB_OBJS)
lib_static: copy_headers $(LIB_STATIC_OBJS)
	@$(MKDIR_P) $(dir $(OUTPUT_LIB_STATIC))
	rm -f $(OUTPUT_LIB_STATIC)
	$(AR) rcs $(OUTPUT_LIB_STATIC) $(LIB_STATIC_OBJS)

# Test configuration
TEST_OBJS := $(SUB_OBJS) $(OBJ_DIR)/src/main.cpp.o
test: $(TEST_OBJS)
	@$(MKDIR_P) $(dir $(OUTPUT_TEST))
	$(CXX) $(TEST_OBJS) -o $(OUTPUT_TEST) $(LDFLAGS)

# Move library headers to apropriate place
copy_headers: include/optparse.hpp
	@$(MKDIR_P) $(dir $(OUTPUT_LIB_STATIC))
	cp include/optparse.hpp bin/install

all: lib_static test

# Clean, be careful with this
.PHONY: clean
clean:
	@$(RM) -rv $(OBJ_DIR)

# Include dependencies
-include $(SUB_DEPS) $(MAIN_DEPS)

# Make directory macro
MKDIR_P ?= mkdir -p
