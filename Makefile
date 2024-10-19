$(info ---------------------------------)
CFLAGS= -Wall -Wextra 
# -ggdb

$(info CFLAGS => $(CFLAGS))
$(info ---------------------------------)

# linux
CC=clang
RAYLIB_DIR=./raylib-5.0_linux_amd64
RAYLIB_CFLAGS= -l:libraylib.a -lm -I$(RAYLIB_DIR)/include/ -L$(RAYLIB_DIR)/lib/ 

# windows
CC_W=x86_64-w64-mingw32-gcc
RAYLIB_DIR_W=./raylib-5.0_win64_mingw-w64
RAYLIB_CFLAGS_W= -l:libraylib.a -lm -I$(RAYLIB_DIR_W)/include/ -L$(RAYLIB_DIR_W)/lib/ 

$(info CC => $(CC))
$(info RAYLIB_CFLAGS => $(RAYLIB_CFLAGS))
$(info ---------------------------------)
$(info CC_W => $(CC_W))
$(info RAYLIB_CFLAGS_W => $(RAYLIB_CFLAGS_W))
$(info ---------------------------------)

CONTAINER_DIR=./container
CONTAINER_CFLAGS= -I$(CONTAINER_DIR)/include/ -L$(CONTAINER_DIR)/lib/ -lcontainer

CONTAINER_SRC=$(wildcard ./container/src/*)
CONTAINER_OBJ=$(patsubst $(CONTAINER_DIR)/src/%.c, $(CONTAINER_DIR)/obj/%.o, $(CONTAINER_SRC))

SRC_DIR=./src
BUILD_DIR=./build

SOURCES=$(wildcard $(SRC_DIR)/*)

$(info CONTAINER_CFLAGS => $(CONTAINER_CFLAGS))
$(info ---------------------------------)
$(info SOURCES => $(SOURCES) $(CONTAINER_SRC))
$(info ---------------------------------)

$(shell test -d $(BUILD_DIR)         || mkdir -p $(BUILD_DIR))
$(shell test -d $(CONTAINER_DIR)/obj || mkdir -p $(CONTAINER_DIR)/obj)
$(shell test -d $(CONTAINER_DIR)/lib || mkdir -p $(CONTAINER_DIR)/lib)

default: main

main: $(SRC_DIR)/main.c libcontainer.a
	$(CC) -o $(BUILD_DIR)/$@ $< $(RAYLIB_CFLAGS) $(CONTAINER_CFLAGS) $(CFLAGS)

win: $(SRC_DIR)/main.c $(CONTAINER_SRC)
	$(CC_W) -o $(BUILD_DIR)/$@.exe $^ $(RAYLIB_CFLAGS_W) -I$(CONTAINER_DIR)/include/ $(CFLAGS) -static -lwinmm -lgdi32 

libcontainer.a: $(CONTAINER_OBJ)
	ar -crv $(CONTAINER_DIR)/lib/$@ $(CONTAINER_OBJ)

$(CONTAINER_DIR)/obj/%.o: $(CONTAINER_DIR)/src/%.c
	$(CC) $^ -c -o $@ -I$(RAYLIB_DIR)/include/ -I$(CONTAINER_DIR)/include $(CFLAGS)

.PHONY: clean
clean:
	rm -fr $(CONTAINER_DIR)/lib/ 
	rm -fr $(CONTAINER_DIR)/obj/
	rm -fr $(BUILD_DIR)
