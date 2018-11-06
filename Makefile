# Output file
OUTPUT_EXEC ?= bin/optparse

# Directory controls
OBJ_DIR ?= obj
SRC_DIRS ?= src
INC_DIRS ?= include

# Compiler configuration
CXX = g++
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
BASE_FLAGS ?= -MMD -MP -m64 -std=c++11
COMPILE_FLAGS ?= $(INC_FLAGS) $(BASE_FLAGS) -g

# Source and object enumerations
SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# Object compilation
$(OBJ_DIR)/%.cpp.o: %.cpp
	@$(MKDIR_P) $(dir $@)
	$(CXX) $(COMPILE_FLAGS) -c $< -o $@

# Output configuration
release: $(OBJS)
	@$(MKDIR_P) $(dir $(OUTPUT_EXEC))
	$(CXX) $(OBJS) -o $(OUTPUT_EXEC) $(LDFLAGS)

# Clean, be careful with this
.PHONY: clean
clean:
	$(RM) -r $(OBJ_DIR)

# Include dependencies
-include $(DEPS)

# Make directory macro
MKDIR_P ?= mkdir -p
