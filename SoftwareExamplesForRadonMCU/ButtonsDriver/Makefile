LIB_NAME := libButtonsDriver.a
LOGGER_DIR := ../EquiniosLogger
LOGGER_LIB := $(LOGGER_DIR)/libEquiniosLogger.a
BUILD_DIR := build
LOGGER_OBJ_DIR := $(BUILD_DIR)/logger_objs

SRCS := $(wildcard src/*.c)
OBJS := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

CROSS_COMPILE ?= nios2-elf-
CC := $(CROSS_COMPILE)gcc
AR := $(CROSS_COMPILE)ar
MAKE ?= make
RM ?= rm -f
RMDIR ?= rm -rf
MKDIR_P ?= mkdir -p

CFLAGS ?= -Os -Wall -ffunction-sections -fdata-sections -I. -Iinclude -Iapi -I$(LOGGER_DIR)/api -I../RadonHelloWorld_bsp -I../RadonHelloWorld_bsp/drivers/inc -I../RadonHelloWorld_bsp/HAL/inc

.PHONY: all clean logger

all: $(LIB_NAME)

logger:
	$(MAKE) -C $(LOGGER_DIR)

$(LOGGER_LIB): logger

$(LIB_NAME): Makefile $(OBJS) $(LOGGER_LIB)
	$(RM) $@
	$(AR) rcs $@ $(OBJS)
	$(MKDIR_P) $(LOGGER_OBJ_DIR)
	cd $(LOGGER_OBJ_DIR) && $(AR) x $(abspath $(LOGGER_LIB))
	$(AR) rcs $@ $(LOGGER_OBJ_DIR)/*.o

$(BUILD_DIR):
	$(MKDIR_P) $@

$(BUILD_DIR)/%.o: src/%.c Makefile | $(BUILD_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

-include $(DEPS)

clean:
	$(RM) $(LIB_NAME)
	$(RMDIR) $(BUILD_DIR)
