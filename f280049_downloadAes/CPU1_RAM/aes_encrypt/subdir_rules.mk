################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
aes_encrypt/%.obj: ../aes_encrypt/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"D:/ti/ccs920/ccs/tools/compiler/ti-cgt-c2000_18.12.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 -Ooff --opt_for_speed=2 --include_path="D:/ti/workspace_v9_2/f280049_downloadAes/device" --include_path="D:/ti/workspace_v9_2/f280049_downloadAes/device/driverlib/inc" --include_path="D:/ti/workspace_v9_2/f280049_downloadAes/device/driverlib" --include_path="D:/ti/c2000/C2000Ware_2_00_00_03/libraries/flash_api/f28004x/include/FlashAPI" --include_path="D:/ti/workspace_v9_2/f280049_downloadAes" --include_path="D:/ti/ccs920/ccs/tools/compiler/ti-cgt-c2000_18.12.3.LTS/include" --advice:performance=all --define=LIVE_UPDATE --define=_FLASH --define=BANK1 --define=CPU1 --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="aes_encrypt/$(basename $(<F)).d_raw" --obj_directory="aes_encrypt" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


