################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
app/collecte_image/collecte_image.obj: ../app/collecte_image/collecte_image.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti73/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --opt_for_speed=3 --include_path="C:/Users/saber/workspace_v7/zh_terminal_tios" --include_path="C:/ti73/simplelink_msp432p4_sdk_1_50_00_12/source/third_party/CMSIS/Include" --include_path="C:/ti73/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/ti73/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --advice:power="1,2,3,4,5,7,9,10,12" --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="app/collecte_image/collecte_image.d_raw" --obj_directory="app/collecte_image" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


