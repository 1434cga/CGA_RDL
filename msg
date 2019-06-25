add recovery function when you update newer version

- 3_recover.pl : add this funciton.  merge file between template and (working or worked) file
    - usage : show makefile
- 3_recover.sh : merge directories
    - $ 3_recover.sh [template based generated dir] [working(modified) base dir] [merged(output) dir]
        - when we add template , our merged directory will have result file from template.
        - if some files exist in working dir only , they will be copied into  merged(output) dir.
        - finally merged(output) dir will have all files both working(modified) directory and template based generated directory.
- README.TOC.md with numbered TOC
    - make toc
