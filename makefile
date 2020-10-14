CC := arm-none-eabi-gcc
CFLAGS :=  -std=gnu99 -g -O2 -Wall -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m0 \
	-fsingle-precision-constant -Wdouble-promotion
LDFLAGS := -TSTM32F051K6TX_FLASH.ld --specs=nosys.specs
MAIN_SRC_DIR := Src
SRC_DIR := Startup \
	Src \
	Drivers/STM32F0xx_HAL_Driver/Src
SRC := $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.[cs]))
OBJ := $(SRC:%.[cs]=%.o)
INCLUDES := -IInc \
	-IDrivers/STM32F0xx_HAL_Driver/Inc \
	-IDrivers/CMSIS/Include \
	-ICore/Inc \
	-IDrivers/STM32F0xx_HAL_Driver/Inc/Legacy \
	-IDrivers/CMSIS/Device/ST/STM32F0xx/Include
VALUES := -DHSE_VALUE=8000000 \
	-DSTM32F051x8 \
	-DHSE_STARTUP_TIMEOUT=100 \
	-DLSE_STARTUP_TIMEOUT=5000 \
	-DLSE_VALUE=32768 \
	-DDATA_CACHE_ENABLE=0 \
	-DINSTRUCTION_CACHE_ENABLE=0 \
	-DVDD_VALUE=3300 \
	-DLSI_VALUE=40000 \
	-DHSI_VALUE=8000000 \
	-DUSE_FULL_LL_DRIVER \
	-DPREFETCH_ENABLE=1 \
	-DUSE_MAKE
CFLAGS += $(INCLUDES) $(VALUES)
CFLAGS += -D$(TARGET)
TARGETS := FD6288 TTRC4IN1 MP6531 tmotor55 tmotor45 hglrc siskin

.PHONY : clean all
clean :
	rm -f Src/*.o
all : $(TARGETS)

$(TARGETS) :
	$(MAKE) TARGET=$@ $@.bin

$(TARGETS:%=%.bin) : clean $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET).elf $(OBJ)
	arm-none-eabi-objcopy -O binary $(TARGET).elf $(TARGET).bin
