CXX = g++
CXXFLAGS = -Wall -g
TARGET = index
SRCS = $(TARGET).cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	objdump -d -t -x -r $(TARGET) -Mintel > objdump.out

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: run
run: $(TARGET)
	./$(TARGET)

.PHONY: debug
debug: $(TARGET)
	gdb -q -tui -ex "b _main" -ex "lay src" -ex "run" ./$(TARGET)

.PHONY: time
time: $(TARGET)
	bash -c "time ./$(TARGET)"

.PHONY: clean
clean:
	rm -rf $(TARGET) objdump.out $(OBJS) core
