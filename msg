performance up & add options : --debug  --debugdetail

performance up & add options : --cga_rld_version  --inputstci --outputdb --debug  --debugdetail  --original --nolog

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
	--debug		  debug mode : ITERATE  IFEQUAL IFNOTEQUAL
	--debugdetail		  --debug + ITERATE detail
		  debug mode : ITERATE  IFEQUAL IFNOTEQUAL  ITERATE detail
	--original		  run NO performance mode
	--nolog		  print log into /dev/null
	--help

debug_mode:
	perl ../CGA_RDL/2_replace.pl --debug --outputdb=default.GV --inputstci=stci/xxx-service/service/XXXInputManager.cpp.stcI 
debug_detail:
	perl ../CGA_RDL/2_replace.pl --debugdetail --outputdb=default.GV --inputstci=stci/xxx-service/service/XXXInputManager.cpp.stcI 

general:
	perl ../CGA_RDL/2_replace.pl --outputdb=default.GV --inputstci=stci/xxx-service/service/XXXInputManager.cpp.stcI 
	# performance mode is default mode

nolog:
	perl ../CGA_RDL/2_replace.pl --nolog --outputdb=default.GV --inputstci=stci/xxx-service/service/XXXInputManager.cpp.stcI
	# /dev/null

version_example:
	perl ../CGA_RDL/2_replace.pl --outputdb=default.GV --inputstci=stci/xxx-service/service/XXXInputManager.cpp.stcI --cga_rdl_version=cga_rdl_version

nolog_example:
	perl ../CGA_RDL/2_replace.pl --nolog --outputdb=default.GV --inputstci=stci/xxx-service/service/XXXInputManager.cpp.stcI 

noperformance_example:
	perl ../CGA_RDL/2_replace.pl --nolog --outputdb=default.GV --inputstci=stci/xxx-service/service/XXXInputManager.cpp.stcI --original
	# --original  is slow mode but it is accurate

perf_test:
	# compare time.
	time perl ../CGA_RDL/2_replace.pl --nolog --outputdb=default.GV --inputstci=stci/xxx-service/service/XXXInputManager.cpp.stcI
	time perl ../CGA_RDL/2_replace.pl --nolog --outputdb=default.GV --inputstci=stci/xxx-service/service/XXXInputManager.cpp.stcI --original

