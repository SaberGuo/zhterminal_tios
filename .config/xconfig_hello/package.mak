#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#

unexport MAKEFILE_LIST
MK_NOGENDEPS := $(filter clean,$(MAKECMDGOALS))
override PKGDIR = xconfig_hello
XDCINCS = -I. -I$(strip $(subst ;, -I,$(subst $(space),\$(space),$(XPKGPATH))))
XDCCFGDIR = package/cfg/

#
# The following dependencies ensure package.mak is rebuilt
# in the event that some included BOM script changes.
#
ifneq (clean,$(MAKECMDGOALS))
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/utils.js:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/utils.js
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/xdc.tci:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/xdc.tci
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/template.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/template.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/om2.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/om2.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/xmlgen.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/xmlgen.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/xmlgen2.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/xmlgen2.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/Warnings.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/Warnings.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/IPackage.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/IPackage.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/package.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/package.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/services/global/Clock.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/services/global/Clock.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/services/global/Trace.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/services/global/Trace.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/bld.js:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/bld.js
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/BuildEnvironment.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/BuildEnvironment.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/PackageContents.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/PackageContents.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/_gen.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/_gen.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/Library.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/Library.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/Executable.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/Executable.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/Repository.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/Repository.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/Configuration.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/Configuration.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/Script.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/Script.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/Manifest.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/Manifest.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/Utils.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/Utils.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/ITarget.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/ITarget.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/ITarget2.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/ITarget2.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/ITarget3.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/ITarget3.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/ITargetFilter.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/ITargetFilter.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/package.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/bld/package.xs
package.mak: config.bld
C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/targets/ITarget.xs:
package.mak: C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/targets/ITarget.xs
C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/targets/C28_large.xs:
package.mak: C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/targets/C28_large.xs
C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/targets/C28_float.xs:
package.mak: C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/targets/C28_float.xs
C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/targets/package.xs:
package.mak: C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/targets/package.xs
C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/targets/arm/elf/IArm.xs:
package.mak: C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/targets/arm/elf/IArm.xs
C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/targets/arm/elf/package.xs:
package.mak: C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/targets/arm/elf/package.xs
package.mak: package.bld
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/tools/configuro/template/compiler.opt.xdt:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/tools/configuro/template/compiler.opt.xdt
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/services/io/File.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/services/io/File.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/services/io/package.xs:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/services/io/package.xs
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/tools/configuro/template/compiler.defs.xdt:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/tools/configuro/template/compiler.defs.xdt
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/tools/configuro/template/custom.mak.exe.xdt:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/tools/configuro/template/custom.mak.exe.xdt
C:/ti73/xdctools_3_50_03_33_core/packages/xdc/tools/configuro/template/package.xs.xdt:
package.mak: C:/ti73/xdctools_3_50_03_33_core/packages/xdc/tools/configuro/template/package.xs.xdt
endif

ti.targets.arm.elf.M4F.rootDir ?= C:/ti73/ccsv7/tools/compiler/ti-cgt-arm_16.9.4.LTS
ti.targets.arm.elf.packageBase ?= C:/ti/simplelink_msp432p4_sdk_1_50_00_12/kernel/tirtos/packages/ti/targets/arm/elf/
.PRECIOUS: $(XDCCFGDIR)/%.oem4f
.PHONY: all,em4f .dlls,em4f .executables,em4f test,em4f
all,em4f: .executables,em4f
.executables,em4f: .libraries,em4f
.executables,em4f: .dlls,em4f
.dlls,em4f: .libraries,em4f
.libraries,em4f: .interfaces
	@$(RM) $@
	@$(TOUCH) "$@"

.help::
	@$(ECHO) xdc test,em4f
	@$(ECHO) xdc .executables,em4f
	@$(ECHO) xdc .libraries,em4f
	@$(ECHO) xdc .dlls,em4f


all: .executables 
.executables: .libraries .dlls
.libraries: .interfaces

PKGCFGS := $(wildcard package.xs) package/build.cfg
.interfaces: package/package.xdc.inc package/package.defs.h package.xdc $(PKGCFGS)

-include package/package.xdc.dep
package/%.xdc.inc package/%_xconfig_hello.c package/%.defs.h: %.xdc $(PKGCFGS)
	@$(MSG) generating interfaces for package xconfig_hello" (because $@ is older than $(firstword $?))" ...
	$(XSRUN) -f xdc/services/intern/cmd/build.xs $(MK_IDLOPTS) -m package/package.xdc.dep -i package/package.xdc.inc package.xdc

.dlls,em4f .dlls: hello.pem4f

-include package/cfg/hello_pem4f.mak
-include package/cfg/hello_pem4f.cfg.mak
ifeq (,$(MK_NOGENDEPS))
-include package/cfg/hello_pem4f.dep
endif
hello.pem4f: package/cfg/hello_pem4f.xdl
	@


ifeq (,$(wildcard .libraries,em4f))
hello.pem4f package/cfg/hello_pem4f.c: .libraries,em4f
endif

package/cfg/hello_pem4f.c package/cfg/hello_pem4f.h package/cfg/hello_pem4f.xdl: override _PROG_NAME := hello.xem4f
package/cfg/hello_pem4f.c: package/cfg/hello_pem4f.cfg
package/cfg/hello_pem4f.xdc.inc: package/cfg/hello_pem4f.xdl
package/cfg/hello_pem4f.xdl package/cfg/hello_pem4f.c: .interfaces

clean:: clean,em4f
	-$(RM) package/cfg/hello_pem4f.cfg
	-$(RM) package/cfg/hello_pem4f.dep
	-$(RM) package/cfg/hello_pem4f.c
	-$(RM) package/cfg/hello_pem4f.xdc.inc

clean,em4f::
	-$(RM) hello.pem4f
.executables,em4f .executables: hello.xem4f

hello.xem4f: |hello.pem4f

-include package/cfg/hello.xem4f.mak
hello.xem4f: package/cfg/hello_pem4f.oem4f 
	$(RM) $@
	@$(MSG) lnkem4f $@ ...
	$(RM) $(XDCCFGDIR)/$@.map
	$(ti.targets.arm.elf.M4F.rootDir)/bin/armcl -fs $(XDCCFGDIR)$(dir $@). -q -u _c_int00 --silicon_version=7M4 -z --strict_compatibility=on  -o $@ package/cfg/hello_pem4f.oem4f   package/cfg/hello_pem4f.xdl  -w -c -m $(XDCCFGDIR)/$@.map -l $(ti.targets.arm.elf.M4F.rootDir)/lib/libc.a
	
hello.xem4f: export C_DIR=
hello.xem4f: PATH:=$(ti.targets.arm.elf.M4F.rootDir)/bin/;$(PATH)
hello.xem4f: Path:=$(ti.targets.arm.elf.M4F.rootDir)/bin/;$(PATH)

hello.test test,em4f test: hello.xem4f.test

hello.xem4f.test:: hello.xem4f
ifeq (,$(_TESTLEVEL))
	@$(MAKE) -R -r --no-print-directory -f $(XDCROOT)/packages/xdc/bld/xdc.mak _TESTLEVEL=1 hello.xem4f.test
else
	@$(MSG) running $<  ...
	$(call EXEC.hello.xem4f, ) 
endif

clean,em4f::
	-$(RM) $(wildcard .tmp,hello.xem4f,*)


clean:: clean,em4f

clean,em4f::
	-$(RM) hello.xem4f
%,copy:
	@$(if $<,,$(MSG) don\'t know how to build $*; exit 1)
	@$(MSG) cp $< $@
	$(RM) $@
	$(CP) $< $@
hello_pem4f.oem4f,copy : package/cfg/hello_pem4f.oem4f
hello_pem4f.sem4f,copy : package/cfg/hello_pem4f.sem4f

$(XDCCFGDIR)%.c $(XDCCFGDIR)%.h $(XDCCFGDIR)%.xdl: $(XDCCFGDIR)%.cfg $(XDCROOT)/packages/xdc/cfg/Main.xs | .interfaces
	@$(MSG) "configuring $(_PROG_NAME) from $< ..."
	$(CONFIG) $(_PROG_XSOPTS) xdc.cfg $(_PROG_NAME) $(XDCCFGDIR)$*.cfg $(XDCCFGDIR)$*

.PHONY: release,xconfig_hello
ifeq (,$(MK_NOGENDEPS))
-include package/rel/xconfig_hello.tar.dep
endif
package/rel/xconfig_hello/xconfig_hello/package/package.rel.xml: package/package.bld.xml
package/rel/xconfig_hello/xconfig_hello/package/package.rel.xml: package/build.cfg
package/rel/xconfig_hello/xconfig_hello/package/package.rel.xml: package/package.xdc.inc
package/rel/xconfig_hello/xconfig_hello/package/package.rel.xml: .force
	@$(MSG) generating external release references $@ ...
	$(XS) $(JSENV) -f $(XDCROOT)/packages/xdc/bld/rel.js $(MK_RELOPTS) . $@

xconfig_hello.tar: package/rel/xconfig_hello.xdc.inc package/rel/xconfig_hello/xconfig_hello/package/package.rel.xml
	@$(MSG) making release file $@ "(because of $(firstword $?))" ...
	-$(RM) $@
	$(call MKRELTAR,package/rel/xconfig_hello.xdc.inc,package/rel/xconfig_hello.tar.dep)


release release,xconfig_hello: all xconfig_hello.tar
clean:: .clean
	-$(RM) xconfig_hello.tar
	-$(RM) package/rel/xconfig_hello.xdc.inc
	-$(RM) package/rel/xconfig_hello.tar.dep

clean:: .clean
	-$(RM) .libraries $(wildcard .libraries,*)
clean:: 
	-$(RM) .dlls $(wildcard .dlls,*)
#
# The following clean rule removes user specified
# generated files or directories.
#

ifneq (clean,$(MAKECMDGOALS))
ifeq (,$(wildcard package))
    $(shell $(MKDIR) package)
endif
ifeq (,$(wildcard package/cfg))
    $(shell $(MKDIR) package/cfg)
endif
ifeq (,$(wildcard package/lib))
    $(shell $(MKDIR) package/lib)
endif
ifeq (,$(wildcard package/rel))
    $(shell $(MKDIR) package/rel)
endif
ifeq (,$(wildcard package/internal))
    $(shell $(MKDIR) package/internal)
endif
endif
clean::
	-$(RMDIR) package

include custom.mak
