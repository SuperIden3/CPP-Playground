CXX = g++
CXXFLAGS = -Wall -g
TARGET = index
SRCS = $(TARGET).cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	objdump -d $(TARGET) -Mintel > objdump.out

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: run clean

.PHONY: run
run:
	./$(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS) core