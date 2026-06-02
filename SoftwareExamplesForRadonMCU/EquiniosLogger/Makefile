LIB_NAME := libEquiniosLogger.a
API_DIR := api
INCLUDE_DIR := include
SRC_DIR := src
BUILD_DIR := build

SRCS := $(SRC_DIR)/equinios.c $(SRC_DIR)/EquiniosLogger.c $(SRC_DIR)/RingBuffer.c $(SRC_DIR)/TimestampProvider.c $(SRC_DIR)/EquiniosLock.c
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

CROSS_COMPILE ?= nios2-elf-
CC := $(CROSS_COMPILE)gcc
AR := $(CROSS_COMPILE)ar
RM ?= rm -f
RMDIR ?= rm -rf

CFLAGS ?= -Os -Wall -ffunction-sections -fdata-sections -I$(INCLUDE_DIR)

.PHONY: all clean

all: $(LIB_NAME)

$(LIB_NAME): Makefile $(OBJS)
	$(AR) rcs $@ $(OBJS)


$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c Makefile | $(BUILD_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

-include $(DEPS)

clean:
	$(RM) $(LIB_NAME)
	$(RMDIR) $(BUILD_DIR)
