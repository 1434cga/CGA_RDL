all:
	mkdir -p ./OUTPUT
	mkdir -p ./OUTPUT/tmp
	mkdir -p ./OUTPUT/stc
	perl ../../1_excel.pl --input=../TIDL.xlsx
	perl ../../2_replace.pl default.GV XXXCommand.h.stcI

clean:
	rm -rf *.GV
	rm -rf *.log
	rm -rf OUTPUT
