# Compilador 
CXX = g++
CXXFLAGS = -Wall -g

# Executável do compilador
EXEC = compiler

# Source files do compilador
SRCS = principal.cpp parser.cpp scanner.cpp stentry.cpp symboltable.cpp

# Object files do compilador
OBJS = $(SRCS:.cpp=.o)

# Header files do compilador
HDRS = parser.h scanner.h stentry.h symboltable.h token.h

# Default rule - Necessário para o comando make
all: $(EXEC)

# Constroi o executável do compilador
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compila os arquivos para objetos do compilador
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpa os arquivos objetos e o executável
clean:
	rm -f $(OBJS) $(EXEC)

# Indica que os targets clean e all não são arquivos
.PHONY: all clean