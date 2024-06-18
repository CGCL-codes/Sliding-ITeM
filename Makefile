CXX = g++
CXXFLAGS = -g
TARGET = ITeM

SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
DEPS = Function.h hashfunction.h Item.h slidingFunction.h slidingItem.h thread_pool.h

all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $<
clean:
	rm -f $(TARGET) $(OBJECTS)
run: all
	./$(TARGET)
.PHONY: all clean run
