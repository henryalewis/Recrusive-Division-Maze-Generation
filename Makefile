CXX      = g++
CXXFLAGS = -Wall -Werror -std=c++17 -g -O

BIN_DIR = bin
OBJ_DIR = obj

GEN_BIN  = $(BIN_DIR)/maze-gen

GEN_OBJS = $(OBJ_DIR)/main-gen.o $(OBJ_DIR)/maze.o

all: directories $(GEN_BIN)

directories:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)

$(GEN_BIN): directories $(GEN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(GEN_OBJS)

$(OBJ_DIR)/%.o: %.cpp | directories
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Smoke test: generate a maze and confirm --validate reports it fully connected.
# Uses only maze-gen, so it needs no external fixture files.
test: $(GEN_BIN)
	@echo "=== generate + validate ==="; \
	$(GEN_BIN) 11 11 99 | $(GEN_BIN) --validate 2>&1 | grep -qx VALID \
		&& echo "OK" || echo "FAIL"

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

.PHONY: all clean directories test