all:
	mkdir -p ./OUTPUT
	mkdir -p ./OUTPUT/tmp
	mkdir -p ./OUTPUT/stc
	perl 1_excel.pl --input=1_example.xlsx --csv_output=1_example.csv --version_input=./excel_version
	perl 2_replace.pl default.GV 2_example.cpp.stc
	perl 3_recover.pl --template=./OUTPUT/stc/src/2_example.cpp --working=./3_working.cpp.data --merge=3_merge.cpp
test: test.pl test.xlsx
	perl test.pl --input=test.xlsx
	perl 2_replace.pl default.GV test.cpp.stc
test2: test/TIDL.xlsx test/test2.cpp.stcI
	perl 1_excel.pl --input=./test/TIDL.xlsx
	perl 2_replace.pl default.GV test/test2.cpp.stcI
test_recover: 3_recover.sh
	sh 3_recover.sh OUTPUT/stc test_recover/working test_recover/merged
ex: 1_excel.pl ./time/Code_Generator.xlsx
	perl 1_excel.pl --input=./time/Code_Generator.xlsx --csv_out=./time/c.csv
csv: 1_csv.pl ./time/Code_Generator.csv
	perl 1_csv.pl --input=./time/Code_Generator.csv --csv_out=./time/o.csv --host=lotto645.lge.com
	perl 1_csv.pl --input=./time/Code_Generator.csv --csv_out=./time/o.csv --python
merge:
	perl 3_recover.pl --template=./OUTPUT/stc/src/2_example.cpp --working=./3_working.cpp.data --merge=./c/d/a.cpp
toc:
	markdown-pp README.mdpp -o README.md        # DOC.md  is basic file in README.mdpp
clean:
	rm -rf *.log
	rm -rf *.modulename
	rm -rf *.csv
	rm -rf OUTPUT
	rm -rf *.GV
	rm -rf 3_merge.cpp
