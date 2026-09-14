CC := cc

ifeq ($(OS),Windows_NT)
    EXE            := main.exe
    LUA_PLATFORM   :=
    PLATFORM_LDFLAGS := -lopengl32 -lgdi32 -lwinmm -static -static-libgcc
else
    UNAME_S := $(shell uname -s)
    EXE := main
    ifeq ($(UNAME_S),Darwin)
        LUA_PLATFORM     := -DLUA_USE_MACOSX
        PLATFORM_LDFLAGS := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    else
        LUA_PLATFORM     := -DLUA_USE_LINUX
        PLATFORM_LDFLAGS := -lGL -lm -lpthread -ldl -lrt -lX11
    endif
endif

RAYLIB_DIR := vendor/raylib/src
RAYLIB_LIB := $(RAYLIB_DIR)/libraylib.a

LUA_DIR  := vendor/lua
LUA_SRCS := $(wildcard $(LUA_DIR)/*.c)
LUA_OBJS := $(LUA_SRCS:.c=.o)
LUA_LIB  := $(LUA_DIR)/liblua.a

CFLAGS  := -Isrc -I$(RAYLIB_DIR) -I$(LUA_DIR)
LDFLAGS := -L$(RAYLIB_DIR) -L$(LUA_DIR) -lraylib -llua $(PLATFORM_LDFLAGS)

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/$(EXE)

.PHONY: all run clean

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): src/main.c $(RAYLIB_LIB) $(LUA_LIB) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) src/main.c $(LDFLAGS)

$(RAYLIB_LIB):
	$(MAKE) -C $(RAYLIB_DIR) PLATFORM=PLATFORM_DESKTOP

$(LUA_DIR)/%.o: $(LUA_DIR)/%.c
	$(CC) $(LUA_PLATFORM) -O2 -c $< -o $@

$(LUA_LIB): $(LUA_OBJS)
	ar rcs $@ $(LUA_OBJS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(LUA_OBJS) $(LUA_LIB)
	$(MAKE) -C $(RAYLIB_DIR) clean
