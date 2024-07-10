CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = index
SRCS = index.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: run
run:
	./$(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)
