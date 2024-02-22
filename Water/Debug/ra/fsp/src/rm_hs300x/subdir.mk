################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/rm_hs300x/rm_hs300x.c \
../ra/fsp/src/rm_hs300x/rm_hs300x_ra_driver.c 

C_DEPS += \
./ra/fsp/src/rm_hs300x/rm_hs300x.d \
./ra/fsp/src/rm_hs300x/rm_hs300x_ra_driver.d 

OBJS += \
./ra/fsp/src/rm_hs300x/rm_hs300x.o \
./ra/fsp/src/rm_hs300x/rm_hs300x_ra_driver.o 

SREC += \
blinkyOutput.srec 

MAP += \
blinkyOutput.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/rm_hs300x/%.o: ../ra/fsp/src/rm_hs300x/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -gdwarf-4 -D_RENESAS_RA_ -D_RA_CORE=CM33 -D_RA_ORDINAL=1 -I"D:/e2/blinkyOutput/src" -I"." -I"D:/e2/blinkyOutput/ra/fsp/inc" -I"D:/e2/blinkyOutput/ra/fsp/inc/api" -I"D:/e2/blinkyOutput/ra/fsp/inc/instances" -I"D:/e2/blinkyOutput/ra/arm/CMSIS_5/CMSIS/Core/Include" -I"D:/e2/blinkyOutput/ra_gen" -I"D:/e2/blinkyOutput/ra_cfg/fsp_cfg/bsp" -I"D:/e2/blinkyOutput/ra_cfg/fsp_cfg" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

