add recover function

## Description of Syntax (2_replace.pl)
### Special case of use +<+ ....  +>+
    - ex) +<+ uc($fish) +>+ : convert $ fish to upper characters
### Control and Programming in RDL
    - ex) test.cpp.stc
## recover (3_recover.sh 3_recover.pl)
    - Rule : ```if($in =~ /^\s*\/\/\s*(CGA_VARIANT\s*:.*)\s*START\s*$/){```
        - ex) // CGA_VARIANT:DiagInputManager.cpp:DiagInputManager:connectToPPP(void):variant START
