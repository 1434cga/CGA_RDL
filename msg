performance up & add options : --cga_rld_version  --inputstci --outputdb --debug  --original --nolog


v1.0.3
check CGA_RDL version in cga_rdl_version file in the same directory of 2_replace.pl
check nolog to remove the log for reducing the space and running time
check debug to explain easily the detailed information in the source code how to convert
Help :
        --inputstci=[stcI or stc file]
                  default input stc or stcI file name :
        --outputdb=[output file]
                  default output DB file name :
        --cga_rdl_version_input=[version file name]
                  default input version file name :
                  if null , we ignore the version between excel version input file and [VARIABLE]Excel_Version  in excel file ()
        --debug           add stcI syntax on output file
        --original             run no performance mode
        --nolog           print log into /dev/null
        --help

