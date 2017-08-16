CC      = /usr/bin/g++
CFLAGS = -std=c++11
LDFLAGS = -lmraa -pthread

BIN = sensor_supply
BIN_DIRCETORY = bin
OBJ_DIRECTORY = obj
SRC_DIRECTORY = src
TST_DIRECTORY = test
INC_DIRECTORY = includes
SRCEXT = cpp
OBJEXT = o
SOURCES = $(shell find $(SRC_DIRECTORY) -type f -name *.$(SRCEXT))
OBJECTS = $(patsubst $(SRC_DIRECTORY)/%,$(OBJ_DIRECTORY)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
all: binary tests

tests: test_directories $(OBJECTS) get_values extract_values test_framelist

get_values: $(OBJ_DIRECTORY)/$(TST_DIRECTORY)/get_values.o $(OBJ_DIRECTORY)/RFM_SENSOR.o $(OBJ_DIRECTORY)/Frame.o
	$(CC) $(CFLAGS) -o $(BIN_DIRCETORY)/$(TST_DIRECTORY)/$@ $^ $(LDFLAGS)
	
extract_values: $(OBJ_DIRECTORY)/$(TST_DIRECTORY)/extract_values.o $(OBJ_DIRECTORY)/Frame.o
	$(CC) $(CFLAGS) -o $(BIN_DIRCETORY)/$(TST_DIRECTORY)/$@ $^ $(LDFLAGS)
	@cp $(TST_DIRECTORY)/values* $(BIN_DIRCETORY)/$(TST_DIRECTORY)

test_framelist: $(OBJ_DIRECTORY)/$(TST_DIRECTORY)/test_framelist.o $(OBJ_DIRECTORY)/FrameList.o
	$(CC) $(CFLAGS) -o $(BIN_DIRCETORY)/$(TST_DIRECTORY)/$@ $^ $(LDFLAGS)
	
$(OBJ_DIRECTORY)/$(TST_DIRECTORY)/%.$(OBJEXT): $(TST_DIRECTORY)/%.$(SRCEXT)
	$(CC) $(CFLAGS) -c $< -o $@ -I./$(INC_DIRECTORY)

test_directories: directories
	@mkdir -p $(BIN_DIRCETORY)/$(TST_DIRECTORY)
	@mkdir -p $(OBJ_DIRECTORY)/$(TST_DIRECTORY)

directories:
	@mkdir -p $(BIN_DIRCETORY)
	@mkdir -p $(OBJ_DIRECTORY)

binary: directories $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(BIN_DIRCETORY)/$(BIN) $^ $(LDFLAGS)

$(OBJ_DIRECTORY)/%.$(OBJEXT): $(SRC_DIRECTORY)/%.$(SRCEXT)
	$(CC) $(CFLAGS) -c $< -o $@ -I./$(INC_DIRECTORY)
        
.PHONY: all clean binary tests
clean:
	rm -rf $(BIN_DIRCETORY)
	rm -rf $(OBJ_DIRECTORY)
