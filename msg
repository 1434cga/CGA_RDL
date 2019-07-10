Solve the issue #15

- use +<+...+>+ (replacement syntax) (ex. KEY=Wifi)
     - stcI_FILEPREFIX:+<+lc(KEY)+>+I => wifiI
     - stcI_FILEPOSTFIX:+<+uc(KEY)+>+man => WIFIman
     - SetI : $MODULENAME = +<+uc(KEY)+>+ => $MODULENAME = WIFI
- example (KEY = Wifi)
    - IXXXManagerService.h.stcI
        - stcI_FILEPREFIX : +<+uc(KEY)+>+ManagerService/include/I
        - stcI_FILEPOSTFIX : ManagerService
        - stcI_EXTENSION : h
        - SetI : $MODULENAME = +<+uc(KEY)+>+
    - OUTPUT/tmp/WifiManagerService_include_IWifiManagerService.h.stc
        - FileName : WIFIManagerService/include/IWifiManagerService.h
        - Set : $MODULENAME = WIFI
