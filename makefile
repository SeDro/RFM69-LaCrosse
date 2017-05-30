CC      = /usr/bin/g++
CFLAGS = -std=c++11
LDFLAGS = -lwiringPi -pthread

OBJ = sensor_supply.o RFM_SENSOR.o Frame.o Registry.o App.o FrameList.o
BIN = sensor_supply
BIN_DIRCETORY = bin
OBJ_DIRECTORY = obj
SRC_DIRECTORY = src
SRCEXT = cpp
OBJEXT = o
SOURCES = $(shell find $(SRC_DIRECTORY) -type f -name *.$(SRCEXT))
OBJECTS = $(patsubst $(SRC_DIRECTORY)/%,$(OBJ_DIRECTORY)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

all: directories $(BIN)

directories:
	@mkdir -p $(BIN_DIRCETORY)
	@mkdir -p $(OBJ_DIRECTORY)

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BIN_DIRCETORY)/$(BIN) $^ $(LDFLAGS)

$(OBJ_DIRECTORY)/%.$(OBJEXT): $(SRC_DIRECTORY)/%.$(SRCEXT)
	$(CC) $(CFLAGS) -c $< -o $@
        
DEPENDFILE = .depend

dep: $(SOURCES)
	$(CC) $(CFLAGS) -MM $(SOURCES) > $(DEPENDFILE)

-include $(DEPENDFILE)

.PHONY: all clean
clean:
	rm -rf $(BIN_DIRCETORY)
	rm -rf $(OBJ_DIRECTORY)
