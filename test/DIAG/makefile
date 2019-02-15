all:
	mkdir -p ./OUTPUT
	mkdir -p ./OUTPUT/tmp
	mkdir -p ./OUTPUT/stc
	perl ../../1_excel.pl --input=diag.xlsx
	perl ../../2_replace.pl default.GV DiagInputManager.cpp.stc
	perl ../../2_replace.pl default.GV DiagInputManager.h.stc
	perl ../../2_replace.pl default.GV IXXXManagerService.h.stcI   	# output is in OUTPUT/stc/TTT/I.....h
	perl ../../2_replace.pl default.GV IXXXManagerService.cpp.stcI		
	perl ../../2_replace.pl default.GV XXXManagerService.h.stcI
	perl ../../2_replace.pl default.GV XXXManagerService.cpp.stcI

clean:
	rm -rf *.GV
	rm -rf *.log
	rm -rf OUTPUT
