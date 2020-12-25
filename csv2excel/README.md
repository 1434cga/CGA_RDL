## csv2excel - design
- csv를 만들때 얼마의 단위로 만들 것인가?   hpp 파일 하나의 단위인가?  아니면 hpp들을 모은 단위인가?
- field (header) below the header 에 #insert(color)_의미  와 같은 것을 넣어 있다면 이런 처리를 가지고, csv로부터 xlsx를 만드는 것이다.
	- #set_color(color) - bg_color: yellow , text_color : blue
	- #set_width(size) - width : best , auto or 30 , 40 ..etc    <- this is located in first line of csv
	- #set_font(color) xxxxxxx
- excel을 먼저 빨리 만들어서, code에서 사용하는 값들을 모두 나열해야 할 것으로 보인다.
- csv2excel : source code

## csv2excel - implementation
- 3 stage
	- ===stage 1 : set_color set_width===
	- ===stage 2 : HEADER VARIABLE font===
	- ===stage 3 : HEADER no_data===
    - add Makefile
- Usage: csv2excel.py [options] -i inputfile(csv) -o outfile(xlsx)
'''
    Options:
      -h, --help            show this help message and exit
      -i INFILENAME, --in=INFILENAME
                            read data from csv FILENAME
      -o OUTFILENAME, --out=OUTFILENAME
                            write data from excel FILENAME
      -d, --debug           print debug message
      -v, --verbose
'''
- #set_width(auto),10,10,5,auto,10,auto,10,¶
    - change the width of column
    - auto is maximum size of the same column
	- ex) auto , best , number(size)
- "#(red)this is a single variable not hash.  +<+$VARIABLE{""Date""}+>+",,,,,,,,,,,,,,,,,¶
    - this is comments with color of font
- # (green) this is update time of  header each file.,,,,,,,,,,,,,,,,,¶
    - this is comments with color of font
- #set_color(lightgreen),lightblue,lightgreen,lightblue,,,,lightgreen,lightblue,,,orange,
    - change color of column
- #set_color(lightgreen),orange,lightgreen,orange,,,,lightgreen,lightblue,,,orange,¶
    - change color of column

