CC=clang
CFLAGS= -Wall -Wextra 
# -ggdb

# $(info CC is $(CC))
# $(info CFLAGS is $(CFLAGS))

RAYLIB_DIR=./raylib-5.0_linux_amd64
RAYLIB_CFLAGS= -l:libraylib.a -lm -I$(RAYLIB_DIR)/include/ -L$(RAYLIB_DIR)/lib/ 

# $(info RAYLIB_DIR is $(RAYLIB_DIR))
# $(info RAYLIB_CFLAGS is $(RAYLIB_CFLAGS))

CONTAINER_DIR=./container
CONTAINER_CFLAGS= -lcontainer  -I$(CONTAINER_DIR)/include/ -L$(CONTAINER_DIR)/lib/ 

CONTAINER_SRC=$(wildcard ./container/src/*)
CONTAINER_OBJ=$(patsubst $(CONTAINER_DIR)/src/%.c, $(CONTAINER_DIR)/obj/%.o, $(CONTAINER_SRC))

# $(info CONTAINER_DIR is $(CONTAINER_DIR))
# $(info CONTAINER_CFLAGS is $(CONTAINER_CFLAGS))
# $(info CONTAINER_SRC is $(CONTAINER_SRC))
# $(info CONTAINER_OBJ is $(CONTAINER_OBJ))

SRC_DIR=./src
BUILD_DIR=./build
$(shell test -d $(BUILD_DIR)         || mkdir -p $(BUILD_DIR))
$(shell test -d $(CONTAINER_DIR)/obj || mkdir -p $(CONTAINER_DIR)/obj)
$(shell test -d $(CONTAINER_DIR)/lib || mkdir -p $(CONTAINER_DIR)/lib)

default: main

main: $(SRC_DIR)/main.c libcontainer.a
	$(CC) -o $(BUILD_DIR)/$@ $< $(RAYLIB_CFLAGS) $(CONTAINER_CFLAGS) $(CFLAGS)

libcontainer.a: $(CONTAINER_OBJ)
	ar -crv $(CONTAINER_DIR)/lib/$@ $(CONTAINER_OBJ)

$(CONTAINER_DIR)/obj/%.o: $(CONTAINER_DIR)/src/%.c
	$(CC) $^ -c -o $@ -I$(RAYLIB_DIR)/include/ -I$(CONTAINER_DIR)/include $(CFLAGS)

.PHONY: clean
clean:
	rm -fr $(CONTAINER_DIR)/lib/ 
	rm -fr $(CONTAINER_DIR)/obj/
	rm -fr $(BUILD_DIR)
