# Output file
OUTPUT_EXEC ?= bin/optparse

# Directory controls
OBJ_DIR ?= obj
SRC_DIR ?= src
INC_DIR ?= include

# Compiler configuration
INC_FLAGS := $(addprefix -I,$(INC_DIR))
BASE_FLAGS ?= -std=c++11 -m64 -MMD -MP
COMPILE_FLAGS ?= $(INC_FLAGS) $(BASE_FLAGS)

# Source and object enumerations
SRCS := $(shell find $(SRC_DIR) -name *.cpp)
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)
DEPS := $(OBJS)

# Object compilation
$(OBJ_DIR)/%.cpp.o: %.cpp
	@$(MKDIR_P) $(dir $@)
	$(CXX) $(COMPILE_FLAGS) -c $< -o $@

# Linking
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
