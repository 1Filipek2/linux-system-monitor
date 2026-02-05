CXX = g++
CXXFLAGS = -std=c++17 -Iinclude
SRC = src/main.cpp src/SystemMonitor.cpp src/ProcessProvider.cpp
OUT = monitor_bin

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)