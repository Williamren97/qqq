CC = g++
CFLAGS = -std=c++17 -g -Wno-psabi

# 直接指定路径
ENVIRO_PATH = /usr/local/src/enviro
CHIPMUNK_PATH = /usr/local/src/Chipmunk2D
ENVIRO_INCLUDE = -I$(ENVIRO_PATH)/server/include -I$(ENVIRO_PATH)/client/include -I$(CHIPMUNK_PATH)/include
ENVIRO_LIBS = -L$(ENVIRO_PATH)/server/lib -lenviro -lelma -lpthread -lchipmunk

SOURCES = $(wildcard src/*.cc)
OBJECTS = $(SOURCES:.cc=.o)
HEADERS = $(wildcard src/*.h)

TARGET = bin/urban_traffic_simulator

$(TARGET): $(OBJECTS)
mkdir -p bin
$(CC) $(CFLAGS) -o $@ $^ $(ENVIRO_LIBS)

%.o: %.cc
$(CC) $(CFLAGS) $(ENVIRO_INCLUDE) -c -o $@ $

clean:
rm -f $(OBJECTS) $(TARGET)
