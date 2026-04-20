CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -g

SRC_DIR  = src
BIN      = compiler

# Coleta recursiva de fontes sem depender de find
rwildcard = $(foreach d,$(wildcard $(1)/*),$(call rwildcard,$(d),$(2)) $(filter $(subst *,%,$(2)),$(d)))
SRCS     = $(call rwildcard,$(SRC_DIR),*.cpp)
OBJS     = $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(BIN)

$(BIN): $(OBJS)
ifeq ($(strip $(OBJS)),)
	$(error Nenhum arquivo .cpp encontrado em $(SRC_DIR)/. Execute o bootstrap da Etapa 1 antes de compilar)
endif
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(BIN)

# Uso: make run FILE=tests/input/teste1.lang
run: $(BIN)
	./$(BIN) $(FILE)
