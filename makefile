CC := arm-none-eabi-gcc
CP := arm-none-eabi-objcopy
MCU := -mcpu=cortex-m0 -mthumb
LDSCRIPT := STM32F051K6TX_FLASH.ld
LIBS := -lc -lm -lnosys 
LDFLAGS := -specs=nano.specs -T$(LDSCRIPT) $(LIBS) -Wl,--gc-sections -Wl,--print-memory-usage
MAIN_SRC_DIR := Src
SRC_DIR := Startup \
	Src \
	Drivers/STM32F0xx_HAL_Driver/Src
SRC := $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.[cs]))
OBJ := $(SRC:%.[cs]=%.o)
INCLUDES :=  \
	-IInc \
	-IDrivers/STM32F0xx_HAL_Driver/Inc \
	-IDrivers/CMSIS/Include \
	-IDrivers/CMSIS/Device/ST/STM32F0xx/Include 
VALUES :=  \
	-DUSE_MAKE \
	-DHSE_VALUE=8000000 \
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
	-DPREFETCH_ENABLE=1
CFLAGS = $(MCU) $(VALUES) $(INCLUDES) -O2 -Wall -fdata-sections -ffunction-sections
CFLAGS += -D$(TARGET)
CFLAGS += -MMD -MP -MF $(@:%.bin=%.d)
TARGETS := FD6288 MP6531 IFLIGHT TMOTOR55 TMOTOR45 HGLRC SISKIN

.PHONY : clean all
clean :
	rm -f Src/*.o
all : $(TARGETS)

$(TARGETS) :
	$(MAKE) TARGET=$@ $@.bin

$(TARGETS:%=%.bin) : clean $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET).elf $(OBJ)
	$(CP) -O binary $(TARGET).elf $(TARGET).bin
	$(CP) $(TARGET).elf -O ihex  $(TARGET).hex
