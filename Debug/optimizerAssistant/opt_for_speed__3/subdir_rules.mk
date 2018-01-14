################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
MSP_EXP432P401R.obj: ../MSP_EXP432P401R.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti73/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --opt_for_speed=3 --include_path="C:/Users/saber/workspace_v7/zh_terminal_tios" --include_path="C:/ti73/simplelink_msp432p4_sdk_1_50_00_12/source/third_party/CMSIS/Include" --include_path="C:/ti73/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/ti73/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --advice:power="1,2,3,4,5,7,9,10,12" --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="MSP_EXP432P401R.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti73/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --opt_for_speed=3 --include_path="C:/Users/saber/workspace_v7/zh_terminal_tios" --include_path="C:/ti73/simplelink_msp432p4_sdk_1_50_00_12/source/third_party/CMSIS/Include" --include_path="C:/ti73/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/sysbios/posix" --include_path="C:/ti73/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include" --advice:power="1,2,3,4,5,7,9,10,12" --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

build-1758288879:
	@$(MAKE) -Onone -f subdir_rules.mk build-1758288879-inproc

build-1758288879-inproc: ../zh_terminal_tios.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"C:/ti73/xdctools_3_50_03_33_core/xs" --xdcpath= xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.msp432:MSP432P401R -r release -c "C:/ti73/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS" --compileOptions "-mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --opt_for_speed=3 --include_path=\"C:/Users/saber/workspace_v7/zh_terminal_tios\" --include_path=\"C:/ti73/simplelink_msp432p4_sdk_1_50_00_12/source/third_party/CMSIS/Include\" --include_path=\"C:/ti73/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/sysbios/posix\" --include_path=\"C:/ti73/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS/include\" --advice:power=\"1,2,3,4,5,7,9,10,12\" --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on  " "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: build-1758288879 ../zh_terminal_tios.cfg
configPkg/compiler.opt: build-1758288879
configPkg/: build-1758288879


