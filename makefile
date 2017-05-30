CC      = /usr/bin/g++
CFLAGS = -std=c++11
LDFLAGS = -lwiringPi -pthread

OBJ = sensor_supply.o RFM_SENSOR.o Frame.o Registry.o App.o FrameList.o
BIN = sensor_supply

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<
        
DEPENDFILE = .depend

dep: $(SRC)
	$(CC) $(CFLAGS) -MM $(SRC) > $(DEPENDFILE)

-include $(DEPENDFILE)

.PHONY: clean
clean:
	rm -rf $(BIN) $(OBJ)
