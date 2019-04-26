all:
	mkdir -p ./OUTPUT
	mkdir -p ./OUTPUT/tmp
	mkdir -p ./OUTPUT/stc
	perl ../CGA_RDL/1_excel.pl --input=diag.xlsx
	perl ../CGA_RDL/2_replace.pl default.GV configure.ac.stcI
	perl ../CGA_RDL/2_replace.pl default.GV XXXManager.h.stcI
	perl ../CGA_RDL/2_replace.pl default.GV elisp.sh.stcI
	perl ../CGA_RDL/2_replace.pl default.GV interfaceMakefile.am.stcI
	perl ../CGA_RDL/2_replace.pl default.GV IXXXData.cpp.stcI
	perl ../CGA_RDL/2_replace.pl default.GV IXXXManagerReceiver.cpp.stcI
	perl ../CGA_RDL/2_replace.pl default.GV IXXXManagerReceiver.h.stcI	
	perl ../CGA_RDL/2_replace.pl default.GV IXXXManagerService.cpp.stcI
	perl ../CGA_RDL/2_replace.pl default.GV IXXXManagerService.h.stcI
	perl ../CGA_RDL/2_replace.pl default.GV IXXXManagerServiceType.h.stcI
	perl ../CGA_RDL/2_replace.pl default.GV model-common-images.inc.stcI
	perl ../CGA_RDL/2_replace.pl default.GV model-service.inc.stcI
	perl ../CGA_RDL/2_replace.pl default.GV sdkXXXManager.cpp.stcI
	perl ../CGA_RDL/2_replace.pl default.GV servicesMakefile.am.stcI
	perl ../CGA_RDL/2_replace.pl default.GV XXXCommand.h.stcI
	perl ../CGA_RDL/2_replace.pl default.GV XXXInputManager.cpp.stcI
	perl ../CGA_RDL/2_replace.pl default.GV XXXInputManager.h.stcI
	perl ../CGA_RDL/2_replace.pl default.GV XXXManagerService.cpp.stcI
	perl ../CGA_RDL/2_replace.pl default.GV XXXManagerService.h.stcI
	perl ../CGA_RDL/2_replace.pl default.GV XXXManagerService_main.cpp.stcI
	perl ../CGA_RDL/2_replace.pl default.GV XXXmgr.service.stcI
	perl ../CGA_RDL/2_replace.pl default.GV XXXmgr-bin.bb.stcI
	perl ../CGA_RDL/2_replace.pl default.GV XXXmgr-lib.bb.stcI

clean:
	rm -rf *.GV
	rm -rf *.log
	rm -rf OUTPUT
