all:
	mkdir -p ./OUTPUT
	mkdir -p ./OUTPUT/tmp
	mkdir -p ./OUTPUT/stc
	perl ../../1_excel.pl --input=wifi.xlsx
	#perl ../../2_replace.pl default.GV configure.ac.stcI
	#perl ../../2_replace.pl default.GV interfaceconfigure.ac.stcI
	#perl ../../2_replace.pl default.GV TCUProtocol.h.stcI
	#perl ../../2_replace.pl default.GV ServiceTable.h.stcI
	#perl ../../2_replace.pl default.GV XXXManager.h.stcI
	#perl ../../2_replace.pl default.GV XXXManager.cpp.stcI
	#perl ../../2_replace.pl default.GV elisp.sh.stcI
	#perl ../../2_replace.pl default.GV interfaceMakefile.am.stcI
	#perl ../../2_replace.pl default.GV IXXXData.cpp.stcI
	#perl ../../2_replace.pl default.GV IXXXManagerReceiver.cpp.stcI
	#perl ../../2_replace.pl default.GV IXXXManagerReceiver.h.stcI	
	#perl ../../2_replace.pl default.GV IXXXManagerService.cpp.stcI
	perl ../../2_replace.pl default.GV IXXXManagerService.h.stcI
	#perl ../../2_replace.pl default.GV IXXXManagerServiceType.h.stcI
	#perl ../../2_replace.pl default.GV model-common-images.inc.stcI
	#perl ../../2_replace.pl default.GV model-service.inc.stcI
	#perl ../../2_replace.pl default.GV servicesMakefile.am.stcI
	#perl ../../2_replace.pl default.GV XXXCommand.h.stcI
	#perl ../../2_replace.pl default.GV XXXInputManager.cpp.stcI
	#perl ../../2_replace.pl default.GV XXXInputManager.h.stcI
	#perl ../../2_replace.pl default.GV XXXManagerService.cpp.stcI
	#perl ../../2_replace.pl default.GV XXXManagerService.h.stcI
	#perl ../../2_replace.pl default.GV XXXManagerService_main.cpp.stcI
	#perl ../../2_replace.pl default.GV XXXmgr.service.stcI
	#perl ../../2_replace.pl default.GV XXXmgr-bin.bb.stcI
	#perl ../../2_replace.pl default.GV XXXmgr-lib.bb.stcI

clean:
	rm -rf *.GV
	rm -rf *.log
	rm -rf *.out
	rm -rf OUTPUT
