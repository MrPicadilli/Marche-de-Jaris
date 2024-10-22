CXX=g++
CXXFLAGS=-g -Wall -MMD -std=c++17
SRC_DIR=src
OBJ_DIR=obj

# List all source files in the SRC_DIR
SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
# Replace .cpp with .o for object files
OBJECTS=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
# List dependency files for automatic tracking
DEPS=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.d,$(SOURCES))

# Target for the final executable
hull: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o hull

# Ensure the object directory exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Pattern rule for compiling object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) hull
