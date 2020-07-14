2020.07.14 [-b master] v1.0.4 solve #45 : backup inputfile (csv or excel) to warehouse

# solve #45 : backup inputfile (csv or excel) to warehouse

## change options
```
perl 1_csv.pl --help
Help :
        --input=[input excel file]
                  default input file name : ./1_example.csv
        --excel_version_input=[excel version input file]
                  default excel version of input file name :
                  if null , we ignore the version between excel version input file and [VARIABLE]Excel_Version  in excel file (./1_example.csv)
        --output=[output file with global variables]
                  default output file name : ./default.GV
        --csv_out=[csv output file]
                  default csv output file name : ./1_example.csv
        --python
                  run command with python scripti to backup
        --destinfo=[input file name with info]
                  default : input_file_name
        --id=[host id ]
                  default : tidl
        --passwd=[passwd of id]
                  default : tidl1234
        --host=[host name]
                  default : abc.com
        --location=[warehouse location]
                  default : ~/warehouse/
        --help

```

- `perl 1_csv.pl --input=./time/Code_Generator.csv --csv_out=./time/o.csv --host=lotto645.abc.com`
```
sshpass -p 'tidl1234' scp ./time/Code_Generator.csv tidl@lotto645.abc.com:~/warehouse/2020-07-14-13-15-38-cheoljoo.lee-Time-v2.9.5-Code_Generator.csv START
sshpass -p 'tidl1234' scp ./time/Code_Generator.csv tidl@lotto645.abc.com:~/warehouse/2020-07-14-13-15-38-cheoljoo.lee-Time-v2.9.5-Code_Generator.csv END

```


-------------------------------------------
2020.07.14 [-b master] v1.0.4 solve #43 : Automatic fill out the empty cell with proper value 

# solve #43  : we wanna have the same result bwtween Origiand and After table.

## Original  : we should fill out completely

|  [HEADER]A | [HEADER]B | value |
|-------|-----|----|
| receiver1 | A | One |
| receiver1 | B | Two |

## After : Automatics fill out the form

|  [HEADER]A | [HEADER]B | value |
|-------|-----|----|
| receiver1 | A | One |
| | B | Two |

-------------------------------------------

add in($a,$b) sub-function : is $b in $a?

200424--------------
- add in sub-function
- IFEQUAL("+<+in($modulename,tidl)+>+","")

200401--------------
you can run with  csv file instead of xlsx file.
First of all , you convert the file into csv file when you have error of perl package for excel.
if you use MAC OSX , your csv file will have as one line with ^M. so you can replace ^M with \r\n

--------------
support sldd & %+<+$API+>+ and restore $g_y policy (plus)

v1.0.4
- support sldd 
- support ```ITERATE %+<+$API+>+ ``` syntax
    - add ```$iterate_var_name = replace_var_with_value($iterate_var_name);```
- restore $g_y policy (plus)
    - replace_var_with_value does not break becasue break contents to improve speed  is impossible to keep the order when we count with plus().

