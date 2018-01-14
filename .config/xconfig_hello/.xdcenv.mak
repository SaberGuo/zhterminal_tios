#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/simplelink_msp432p4_sdk_1_50_00_12/source;C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages;C:/ti73/ccsv7/ccs_base;C:/Users/saber/workspace_v7/hello_MSP_EXP432P401R_tirtos_ccs/.config
override XDCROOT = C:/ti73/xdctools_3_50_03_33_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/simplelink_msp432p4_sdk_1_50_00_12/source;C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages;C:/ti73/ccsv7/ccs_base;C:/Users/saber/workspace_v7/hello_MSP_EXP432P401R_tirtos_ccs/.config;C:/ti73/xdctools_3_50_03_33_core/packages;..
HOSTOS = Windows
endif
