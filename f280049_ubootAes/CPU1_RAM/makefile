################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/ti/ccs920/ccs/tools/compiler/ti-cgt-c2000_18.12.3.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./f28004x_dcsm_z1otp.obj" \
"./flashapi_main.obj" \
"./aes_decrypt/aes.obj" \
"./aes_decrypt/test.obj" \
"./device/device.obj" \
"./device/f28004x_codestartbranch.obj" \
"./device/driverlib/adc.obj" \
"./device/driverlib/asysctl.obj" \
"./device/driverlib/can.obj" \
"./device/driverlib/cla.obj" \
"./device/driverlib/clapromcrc.obj" \
"./device/driverlib/cmpss.obj" \
"./device/driverlib/cputimer.obj" \
"./device/driverlib/dac.obj" \
"./device/driverlib/dcc.obj" \
"./device/driverlib/dcsm.obj" \
"./device/driverlib/dma.obj" \
"./device/driverlib/ecap.obj" \
"./device/driverlib/epwm.obj" \
"./device/driverlib/eqep.obj" \
"./device/driverlib/flash.obj" \
"./device/driverlib/fsi.obj" \
"./device/driverlib/gpio.obj" \
"./device/driverlib/hrcap.obj" \
"./device/driverlib/hrpwm.obj" \
"./device/driverlib/i2c.obj" \
"./device/driverlib/interrupt.obj" \
"./device/driverlib/lin.obj" \
"./device/driverlib/memcfg.obj" \
"./device/driverlib/pga.obj" \
"./device/driverlib/pmbus.obj" \
"./device/driverlib/sci.obj" \
"./device/driverlib/sdfm.obj" \
"./device/driverlib/spi.obj" \
"./device/driverlib/sysctl.obj" \
"./device/driverlib/version.obj" \
"./device/driverlib/xbar.obj" \
"../28004x_dcsm_lnk.cmd" \
"../28004x_generic_lnk.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include aes_decrypt/subdir_vars.mk
-include device/subdir_vars.mk
-include device/driverlib/subdir_vars.mk
-include subdir_rules.mk
-include aes_decrypt/subdir_rules.mk
-include device/subdir_rules.mk
-include device/driverlib/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
f280049_ubootAes.out \

EXE_OUTPUTS__QUOTED += \
"f280049_ubootAes.out" \

BIN_OUTPUTS += \
C:/24G_Radar_bin/c289400/uboot_aes.bin \

BIN_OUTPUTS__QUOTED += \
"C:\24G_Radar_bin\c289400\uboot_aes.bin" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "f280049_ubootAes.out" secondary-outputs

# Tool invocations
f280049_ubootAes.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"D:/ti/ccs920/ccs/tools/compiler/ti-cgt-c2000_18.12.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -O1 --opt_for_speed=3 --advice:performance=all --define=LIVE_UPDATE --define=_FLASH --define=BANK1 --define=CPU1 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi -z -m"f280049_ubootAes.map" --stack_size=0x100 --warn_sections -i"D:/ti/ccs920/ccs/tools/compiler/ti-cgt-c2000_18.12.3.LTS/lib" -i"D:/ti/ccs920/ccs/tools/compiler/ti-cgt-c2000_18.12.3.LTS/include" --reread_libs --diag_wrap=off --display_error_number --xml_link_info="f280049_ubootAes_linkInfo.xml" --entry_point=code_start --rom_model -o "f280049_ubootAes.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

C:/24G_Radar_bin/c289400/uboot_aes.bin: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: C2000 Hex Utility'
	"D:/ti/ccs920/ccs/tools/compiler/ti-cgt-c2000_18.12.3.LTS/bin/hex2000" --boot --sci8 --ascii --binary -o "C:/24G_Radar_bin/c289400/uboot_aes.bin" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "f28004x_dcsm_z1otp.obj" "flashapi_main.obj" "aes_decrypt\aes.obj" "aes_decrypt\test.obj" "device\device.obj" "device\f28004x_codestartbranch.obj" "device\driverlib\adc.obj" "device\driverlib\asysctl.obj" "device\driverlib\can.obj" "device\driverlib\cla.obj" "device\driverlib\clapromcrc.obj" "device\driverlib\cmpss.obj" "device\driverlib\cputimer.obj" "device\driverlib\dac.obj" "device\driverlib\dcc.obj" "device\driverlib\dcsm.obj" "device\driverlib\dma.obj" "device\driverlib\ecap.obj" "device\driverlib\epwm.obj" "device\driverlib\eqep.obj" "device\driverlib\flash.obj" "device\driverlib\fsi.obj" "device\driverlib\gpio.obj" "device\driverlib\hrcap.obj" "device\driverlib\hrpwm.obj" "device\driverlib\i2c.obj" "device\driverlib\interrupt.obj" "device\driverlib\lin.obj" "device\driverlib\memcfg.obj" "device\driverlib\pga.obj" "device\driverlib\pmbus.obj" "device\driverlib\sci.obj" "device\driverlib\sdfm.obj" "device\driverlib\spi.obj" "device\driverlib\sysctl.obj" "device\driverlib\version.obj" 
	-$(RM) "device\driverlib\xbar.obj" 
	-$(RM) "flashapi_main.d" "aes_decrypt\aes.d" "aes_decrypt\test.d" "device\device.d" "device\driverlib\adc.d" "device\driverlib\asysctl.d" "device\driverlib\can.d" "device\driverlib\cla.d" "device\driverlib\clapromcrc.d" "device\driverlib\cmpss.d" "device\driverlib\cputimer.d" "device\driverlib\dac.d" "device\driverlib\dcc.d" "device\driverlib\dcsm.d" "device\driverlib\dma.d" "device\driverlib\ecap.d" "device\driverlib\epwm.d" "device\driverlib\eqep.d" "device\driverlib\flash.d" "device\driverlib\fsi.d" "device\driverlib\gpio.d" "device\driverlib\hrcap.d" "device\driverlib\hrpwm.d" "device\driverlib\i2c.d" "device\driverlib\interrupt.d" "device\driverlib\lin.d" "device\driverlib\memcfg.d" "device\driverlib\pga.d" "device\driverlib\pmbus.d" "device\driverlib\sci.d" "device\driverlib\sdfm.d" "device\driverlib\spi.d" "device\driverlib\sysctl.d" "device\driverlib\version.d" "device\driverlib\xbar.d" 
	-$(RM) "f28004x_dcsm_z1otp.d" "device\f28004x_codestartbranch.d" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

