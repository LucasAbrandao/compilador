CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -g -Isrc

SRC_DIR  = src
BIN      = compiler

rwildcard = $(foreach d,$(wildcard $(1)/*),$(call rwildcard,$(d),$(2)) $(filter $(subst *,%,$(2)),$(d)))
SRCS     = $(call rwildcard,$(SRC_DIR),*.cpp)
OBJS     = $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(BIN)

$(BIN): $(OBJS)
	$(if $(strip $(OBJS)),,$(error Nenhum arquivo .cpp encontrado em $(SRC_DIR)/. Execute o bootstrap da Etapa 1 antes de compilar))
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(BIN)

run: $(BIN)
	./$(BIN) $(FILE)
