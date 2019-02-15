#!/bin/perl

use File::Path qw(make_path);

our $tttime;
our $cchange_start_time;
our $outputdir = "OUTPUT";
our %local_var_set;

sub __SUB__ { return  (caller 2)[3] . "|" . (caller 2)[2] . "-" . (caller 1)[3] . "|" . (caller 1)[2] . "-" . (caller 0)[2] . ": " }

sub start_time_log {
	my $tmpLogInit = shift @_;
	($Second, $Minute, $Hour, $Day, $Month, $Year, $WeekDay, $DayOfYear, $IsDST) = localtime(time) ;
	$Month++;
	$Year += 1900;
	print_fp( "$tmpLogInit structg : CChange START ($stc_filename_input) - $Month-$Day-$Year : $Hour : $Minute : $Second\n",DBG,TIME_DBG);
	$tttime = $Hour * 3600 + $Minute * 60 + $Second;
	$cchange_start_time = $Hour * 3600 + $Minute * 60 + $Second;
}
sub mid_time_log {
	my $tmpLogInit = shift @_;
	($Second, $Minute, $Hour, $Day, $Month, $Year, $WeekDay, $DayOfYear, $IsDST) = localtime(time) ;
	$Month++;
	$Year += 1900;
	$ddtime = ($Hour * 3600 + $Minute * 60 + $Second) - $tttime;
	$tttime = $Hour * 3600 + $Minute * 60 + $Second;
	print_fp( "$tmpLogInit structg ($ddtime) : CChange ITERATE recursion END 1 ($stc_filename_input) - $Month-$Day-$Year : $Hour : $Minute : $Second\n",DBG,TIME_DBG);
}
sub end_time_log {
	my $tmpLogInit = shift @_;
	($Second, $Minute, $Hour, $Day, $Month, $Year, $WeekDay, $DayOfYear, $IsDST) = localtime(time) ;
	$Month++;
	$Year += 1900;
	$ddtime = ($Hour * 3600 + $Minute * 60 + $Second) - $cchange_start_time;
	print_fp( "$tmpLogInit structg CChange duration ($ddtime) : CChange END ($stc_filename_input) - $Month-$Day-$Year : $Hour : $Minute : $Second\n",DBG,TIME_DBG);
}

sub print_fp
{
	my $print_contents;
	my $to_file;

	$print_contents = shift @_;
	foreach $to_file (@_) {
		## STD라는 것은 DBG에 대해서 만들어둔 것으로 STD 나 DBG이나 같은 값을 의미한다.  
		print $to_file $print_contents;
	}
}

sub IChange
{
	my $iterate_key;
	my $iterate_value;
	my $iterate_hash_name;
	my $iterate_lines = "";
	my $in;
	my $in_start;
	my $in_end;
	my $iterate_cnt = 0;
	my $stcI_filename_input ;
	my $stcI_filename_output ;
	my $stcI_output_dir;
	my $stcI_for;
	my $stcI_extension="";
	my $stcI_filepostfix="";
	my $stcI_fileprefix="";
	my $stcI_lines = "";
	my @stcI_lines;
	my $stcfilename="";
	my $tmpstcfilename="";
	my $tttime;
	my $ddtime;

	$stcI_filename_input = shift @_;
	$stcI_output_dir = shift @_;

	print_fp("\n\n\nIchange $stcI_filename_input\n",STDOUT,DBG);
	open(STCI_INPUT , "<$stcI_filename_input");

($Second, $Minute, $Hour, $Day, $Month, $Year, $WeekDay, $DayOfYear, $IsDST) = localtime(time) ;
$Month++;
$Year += 1900;
print_fp( "structg : Ichange START ($stcI_filename_input) - $Month-$Day-$Year : $Hour : $Minute : $Second\n", TIME_DBG);
$tttime = $Hour * 3600 + $Minute * 60 + $Second;

	while ($in = <STCI_INPUT>) {
		if($in =~ /^stcI_HASH\s*\:\s*(\S+)\s*$/){
			$stcI_for = $1;
		} elsif($in =~ /^stcI_EXTENSION\s*\:\s*(\w+)\s*$/){
			$stcI_extension = $1;
		} elsif($in =~ /^stcI_FILEPOSTFIX\s*\:\s*(\w+)\s*$/){
			$stcI_filepostfix = $1;
		} elsif($in =~ /^stcI_FILEPREFIX\s*\:\s*([^\s]+)\s*$/){
			$stcI_fileprefix = $1;
		} else {
			$stcI_lines .= $in;
		}
	}
	print_fp("stcI_HASH : $stcI_for\n",STDOUT,DBG);
	print_fp("stcI_EXTENSION : $stcI_extension\n",STDOUT,DBG);
	print_fp("stcI_FILEPOSTFIX : $stcI_filepostfix\n",STDOUT,DBG);
	print_fp("stcI_FILEPREFIX : $stcI_fileprefix\n",STDOUT,DBG);
	foreach $temp (keys %$stcI_for){
		print_fp("stcI_HASH : $stcI_for key $temp\n",STDOUT,DBG);
		$tmpstcfilename = "$temp" . "$stcI_filepostfix";
		$stcfilename = "$stcI_fileprefix" . "$temp" . "$stcI_filepostfix";
		print_fp("STCI tmp OUTPUT FileName :  $tmpstcfilename\n",STDOUT,DBG);
        my $stcNoDirectoryName = $stcfilename;
        $stcNoDirectoryName =~ s/\//_/g;
		my $tmpStcIFileName = "$outputdir/tmp/$stcNoDirectoryName\.$stcI_extension\.stc";
		open(STCI_OUTPUT , ">$tmpStcIFileName");
		print STCI_OUTPUT "FileName : $stcfilename\.$stcI_extension\n";
		print "FileName : $stcfilename\.$stcI_extension\n";
		@stcI_lines = split("\n",$stcI_lines);
		foreach my $ttt (@stcI_lines){
#print "ttt $ttt\n";
			if($ttt =~ /^SetI\s*\:/){
#print "A0 $ttt\n";
				$ttt =~ s/KEY/$temp/g;
				$ttt =~ s/VALUE/$$stcI_for{$temp}/g;
#print "A1 $ttt\n";
				$ttt = replace_var_with_value($ttt);
				$ttt =~ s/^SetI/Set/;
#print "A2 $ttt\n";
				print STCI_OUTPUT "$ttt\n";
			} else {
				print STCI_OUTPUT "$ttt\n";
			}
		}
		close(STCI_OUTPUT);
		CChange("$tmpStcIFileName","stc","","DEBUG_ON");			# OUTPUT¾È¿¡ ÀúÀåÇÏ°í, makefile¿¡µµ ³Ö¾î¶ó.
	}
	close(STCI_INPUT);

($Second, $Minute, $Hour, $Day, $Month, $Year, $WeekDay, $DayOfYear, $IsDST) = localtime(time) ;
$Month++;
$Year += 1900;
$ddtime = ($Hour * 3600 + $Minute * 60 + $Second) - $tttime;
print_fp( "structg Ichange duration ($ddtime) : Ichange END ($stcI_filename_input) - $Month-$Day-$Year : $Hour : $Minute : $Second\n",TIME_DBG);

}

sub CChange
{
	my $iterate_key;
	my $iterate_value;
	my $iterate_var_name;
	my $iterate_var_type;
	my $iterate_lines = "";
	my %file_output;
	my $in;
	my $in_start;
	my $in_end;
	my $iterate_cnt = 0;
	my $stc_filename_input ;
	my $stc_filename_output ;
	my $stc_debug ;
	my $stc_output_dir;
	my $tttime;
	my $ddtime;
	my $cchange_start_time;
	my $myiterateNewLineType = "";

	$stc_filename_input = shift @_;
	$stc_output_dir = shift @_;
	$stc_filename_output = shift @_;
	$stc_debug = shift @_;
	print "STC input $stc_filename_input , dir= $stc_output_dir , output= $stc_filename_output , debug= $stc_debug\n";
	#
	#방법 1. 
	# 쌍을 알기위해서 recusive를 사용하게 되고,
	# 그 안에서 나온 결과를 이용하여
	# 실제 들아갈 값을 찾는 것은 loop를 계속 돌며, ITERATOR가 없을때까지 수행을 하면 된다. 
	#  	loop를 돌때는 ITERATOR가 한번 발견되면 그 규칙을 끝까지 지키는 것을 원칙으로 한다.
	# 그리고, 나머지 loop들도 돌면서 처리를 계속한다. 
	#
	# recusive모양은 찾는 것은 좋으나, 거꾸로 풀면서 가면 위에서 지정한 loop의 argument를 사용할수 없기에 loop를 더 추가한 것이다.
	#
	#방법 2.
	# ITERATOR에 대한 변수를 두어 처음부터 잡아서 해당 것의 끝까지 저장을 한다.
	# ITERATOR A start
	# 	AAA
	# 	ITERATOR B start
	# 		BBB
	#   B End
	# A End
	# 라고 할때, 위의 4줄을 모두 한 변수에 집어 넣는다. (AAA ~ B End)
	# ITERATOR A의 규칙대로 대치를 시킨다. (복수의 line을 뽑아내어 변수에 저장)
	# 대치시킨 결과를 CChange() 에서 하는 것 처럼 또 돌려준다. 
	# ITERATOR B ~ B End까지를 또 풀어준다.
	#
	# operation 우선순위
	#  ITERATE 가 제일 먼저 처리 된다.
	#  그 후에 , +<+$...+>+ 이 처리가 된다.  co_log_main.c안에서  Get_HashoData를 참조하시요.
	print_fp("CChange $stc_filename_input\n",STDOUT,DBG);
	open(INPUTC , "<$stc_filename_input");

	if($stc_debug eq "DEBUG_ON"){ start_time_log("==time_debug=="); }

	# for make file : but not use yet
my $comment =  <<END_COMMENT;
	if($stc_filename_output ne ""){
		$stg_stc_file{$stc_filename_output} = $stc_filename_output;
		print_fp("STC FileName 1 : stg_stc_file : $stg_stc_file{$stc_filename_output} : $stc_filename_output\n",STDOUT,DBG);
		open(OUTPUTC , ">$outputdir/$stc_output_dir/$stc_filename_output");
		if( ($stc_filename_output =~ /\.c\s*$/) || ($stc_filename_output =~ /\.cpp\s*$/) || ($stc_filename_output =~ /\.pc\s*$/) ){	
			#print "INCLUDE1\n";
			print OUTPUTC "\n#include \"$FileName\"\n\n\n";		# .c안에서는 되는데 .l에서 문제가 발생함.
		} else {
			#NONE#
			# .l일때는 처음에 include를 선언하면 error가 발생한다.
		}
		if($stc_output_dir eq ""){
			if($stc_filename_output =~ /\.c/){
				$filelist{$stc_filename_output} = "CFILE";
			}
		}
	}
END_COMMENT


	while ($in = <INPUTC>) {
		if($in =~ /^FileName\s*\:\s*(\S+)\s*$/){
			$stc_filename_output = $1;
			$stg_stc_file{$1} = $1;
			print_fp("STC FileName 2 : stg_stc_file : $stg_stc_file{$1} : $stc_filename_output\n",STDOUT,DBG);
			$file_output{$stc_filename_output} = "";        # init

my $comment =  <<END_COMMENT;		# if need this statements , you change to put the values into hash variables.
			if( ($stc_filename_output =~ /\.c\s*$/) || ($stc_filename_output =~ /\.cpp\s*$/) || ($stc_filename_output =~ /\.pc\s*$/) ){	
				#print "INCLUDE2\n";
				print OUTPUTC "\n#include \"$FileName\"\n\n\n";		# .c안에서는 되는데 .l에서 문제가 발생함.
			} else {
				#NONE#
				# .l일때는 처음에 include를 선언하면 error가 발생한다.
			}
END_COMMENT

			# HASH : filelist
			my $tmpDir;
			if($stc_output_dir eq ""){
				$tmpDir = "";
			} else {
				$tmpDir = "$stc_output_dir\/";
			}
			if($stc_filename_output =~ /\.c/){
				$filelist{$tmpDir.$stc_filename_output} = "CFILE";
			} elsif($stc_filename_output =~ /\.l/){
				$filelist{$tmpDir.$stc_filename_output} = "LEXFILE";
				$temp = $stc_filename_output;
				if($temp =~ /(.*)\/(.*).l/){
					$temp = "lex\.$2\.c";
				} else {
					$temp =~ s/(.*)\.l/lex\.$1\.c/;
				}
				$filelist{$tmpDir.$temp} = "CFILE";
			}
			$hashName{filelist} = $stc_filename_output;
			next;
		} elsif($in =~ /^Set\s*\:\s*\$(\w+)\s*\=\s*(\S+)/){		# Set: $A = B 
			my $temp_set_var = $1;
			my $temp_set_value = $2;
print "LLL $iterate_comments : [$1]  [$2]\n";
			$$1 = $2;
#print DBG "LLL $iterate_comments : [$1] [$2] \n";
			#CCHANGE_DEBUG print DBG "SET : $1 = $2\n";
			#  이게 SET하는 것이 잘 안되는군.... 다음 방법은 될까?  <- set을 잘못하여 시험함.
			# 위의 방법 1과 아래의 방법 2 모두 잘 처리됨.
##			if($temp_set_var eq "iterate_comments" ){
##				print "iterate_comments : [$1] [$2]\n";
##				if("OFF" eq $temp_set_value){
##					#$iterate_comments = "OFF";
##				} else {
##					#$iterate_comments = "ON";
##				}
##			}
##			# 첫번째의 위방법은 이상하게 한 번만 Set되고 되지 않는다. 
##			# 두번째인 비교를 하여 직접 값을 써주는 것은 가능하다. ㅁ
			$undefined_name = "stc_Set_var_$stc_filename_output";
			$undefined_typedef{$undefined_name} = "HASH_$undefined_name";
			$$undefined_name{$temp_set_var} = "$temp_set_value";
			$local_var_set{$stc_filename_output}{$temp_set_var} = $temp_set_value;
			$hashName{local_var_set} = "STC Local Set Variable";
			print "$stc_filename_output : $in\n";
			next;
		} elsif($in =~ /^Set\s*\:\s*\$(\w+)\s*\{\s*(\w+)\s*\}\s*\=\s*(\w+)/){		# Set: $A{B} = C
			print "SET : ERROR $1 $2 $3\n";
			my $temp_set_var = $1;
			my $temp_set_hash = $2;
			my $temp_set_value = $3;
#print DBG "LLL $iterate_comments : $1  $2 $3\n";
			#  $hashName is the list of hash variables to print or to write into file.
			$hashName{$1} = $3;
			$$1{$2} = $3;
#print DBG "LLL $iterate_comments : [$1] [$2]  $3\n";
			#CCHANGE_DEBUG print DBG "SET :  $1 \{ $2 \} = $3\n";
			#$undefined_name = "stc_Set_var_$stc_filename_output";
			#$undefined_typedef{$undefined_name} = "HASH_$undefined_name";
			#$$undefined_name{"$temp_set_var" . "$temp_set_hash"} = "$temp_set_value";
			next;
		}


#print_fp("Line 1 icnt=$iterate_cnt : $in",DBG);
		if(0 == $iterate_cnt){
			$in = replace_var_with_value($in);
		}
#print_fp("Line 2 icnt=$iterate_cnt : $in",DBG);

		if ($in =~ /^\s*ITERATE(WithoutNewLine\s+|\s+)([+-]?[KV]?[%@&+])(\S+)\s+\+<<\+\s+(\S+)\s+(\S+)\s*$/){
			#ITERATOR_DEBUG 
			print DBG "ITERATE Mstart $1 $2 $3 $4 $5\n"; 
			if(0 == $iterate_cnt){
				$in_start = $in;
				($iterate_var_type , $iterate_var_name , $iterate_key , $iterate_value) = ($2,$3,$4 , $5);
				$myiterateNewLineType = $1;
			} else {
				$iterate_lines .= $in;
			}
			$myiterateNewLineType =~ s/\s*$//;
			#print STDERR "[$myiterateNewLineType]\n";
			$iterate_cnt ++;
			#ITERATOR_DEBUG 
			print DBG "ITERATE iterate_cnt $iterate_cnt : $in";
		} elsif ($in =~ /^(.*)\+>>\+/){
			$iterate_cnt--;
			#ITERATOR_DEBUG 
			print DBG "ITERATE iterate_cnt $iterate_cnt : $in";
			if(0 == $iterate_cnt){
				if($stc_debug eq "DEBUG_ON"){ mid_time_log("==MID time_debug=="); }
				$in_end = $in;
				#ITERATOR_DEBUG 
				print DBG "iterate_comments [" . $iterate_comments . "]\n";
				if( "ON" eq $iterate_comments){
					$temp1=$in_start;
					$temp2=$iterate_lines;
					$temp3=$in_end;
					# /** */ 으로 묶는 안에 /* */이 있으면 안되므로 <* *>으로 변환을 시켜주는 것이다.  
					$temp1 =~ s/\/\*/\<\*/g;
					$temp1 =~ s/\*\//\*\>/g;
					$temp2 =~ s/\/\*/\<\*/g;
					$temp2 =~ s/\*\//\*\>/g;
					$temp2 =~ s/IFNOTEQUAL/ifNOTequal/g;
					$temp2 =~ s/IFEQUAL/ifequal/g;
					$temp3 =~ s/\/\*/\<\*/g;
					$temp3 =~ s/\*\//\*\>/g;
					#ITERATOR_DEBUG 
					print DBG "\/\*\*\n$temp1$temp2$temp3\*\/\n";
				}
				# Iterator_recursion은 단지 확장을 위한 것이다. 그러므로 , 확장을하는 것만 해주면 된다.  
				$iterate_lines = Iterator_recursion($iterate_var_type , $iterate_var_name,$iterate_key,$iterate_value,$iterate_lines);
				#ITERATOR_DEBUG  
				#$iterate_lines =~ s/\+<\+\s*\$(\S+)\s*\+>\+/$$1/g;		# 	+<+$stg_hash_del_timeout+>+ ==> 10

				if($stc_debug eq "DEBUG_ON"){ mid_time_log("==MID time_debug=="); }

				# 이런식으로 처리하면 많은 %값들을 만들지 않아도 되며, define같은 값들을 지저분하게 군데군데 만들어줄 필요가 없다. 
				#print DBG "Set Hash 10 : $iterate_lines\n";
				if(1){      # It is mendatory
					#  because of processing speed.  when it is replacement , it scans whole string. So I break down into substring.
					my $iter_lena = length($iterate_lines);
					my $iterate_lines_org = $iterate_lines;
					$iterate_lines ="";
					for(my $itt = 0;$itt <= $iter_lena ; $itt += 10000){
						$iterate_lines .= replace_var_with_value(substr($iterate_lines_org, $itt, 10000));
					}

					$temp = length($iterate_lines);
					$iterate_lines = replace_var_with_value($iterate_lines);

					if($stc_debug eq "DEBUG_ON"){ mid_time_log("==MID time_debug 2=="); }

				} else {            # This is old version's code.
					$iterate_lines = replace_var_with_value($iterate_lines);
				}
				#print DBG "Set Hash 11 : $iterate_lines\n";
				print DBG "RETURN \$iterate_lines = \n\[\n$iterate_lines\n\]\n";

				$iterate_lines = recover_special_code($iterate_lines);
				#print_fp("$iterate_lines\n" , OUTPUTC);
				#print STDERR "1[$myiterateNewLineType]\n";
				if($myiterateNewLineType ne "WithoutNewLine"){
					$file_output{$stc_filename_output} .= $iterate_lines;
				} else {
					#print STDERR "1{$iterate_lines}\n\n";
					#print STDERR "1[ $file_output{$stc_filename_output} ]\n\n";
					$file_output{$stc_filename_output} =~ s/\n$/ /;
					#print STDERR "1[ $file_output{$stc_filename_output} ]\n\n";
					$iterate_lines =~ s/\n/ /g;
					$iterate_lines =~ s// /g;
					$file_output{$stc_filename_output} .= $iterate_lines . "\n";
					$myiterateNewLineType = "";
					#print STDERR "2{$iterate_lines}\n\n";
					#print STDERR "2[ $file_output{$stc_filename_output} ]\n\n";
				}

				$iterate_lines = "";
#print DBG "Mend \n";
			} else {			# if(0 == $iterate_cnt)
				$iterate_lines .= $in;
			} 					#if(0 == $iterate_cnt)
		} else {
			if(0 == $iterate_cnt){
				#print_fp($in, OUTPUTC);
				$file_output{$stc_filename_output} .= $in;
			} else {
				$iterate_lines .= $in;
			}
		}
	}

	print DBG __SUB__ . " iterate_equal\n";
	foreach my $tmpKey  (sort keys  %file_output){
		if($stc_debug eq "DEBUG_ON"){ mid_time_log("==MID equal start =="); }
		my $iter_len = length($file_output{$tmpKey});
		my $linesOrg = $file_output{$tmpKey};
		my $lines ="";
		if(0){
			# for performance
			for(my $itt = 0;$itt <= $iter_len ; $itt += 1000){
				$lines .= iterate_equal(substr($linesOrg, $itt, 1000));
			}
			$lines = iterate_equal($lines);
		} else {
			$lines = iterate_equal($linesOrg);
		}
		$lines = replace_var_with_value($lines);
		print DBG "FFFF $lines\n";
		#$lines =~ s/STG_SHARP_/\#/g;
		if($stc_debug eq "DEBUG_ON"){ mid_time_log("==MID equal end =="); }

        #print "tmpKey $tmpKey\n";
        if($tmpKey =~ /[^\/]*$/){
			my $m_before = $`;
			my $m_match = $&;
            #print "tmpKey before[$m_before] , match [$m_match]\n";
            if($m_before ne ""){
                if($tmpKey =~ /^\//){
                    print "make_path $m_before\n";
                    make_path("$m_before");
                } else {
                    print "make_path $outputdir/$stc_output_dir/$m_before\n";
                    make_path("$outputdir/$stc_output_dir/$m_before");
                }
            }
        }
        if($tmpKey =~ /^\//){
		    open(OUTPUTC , "> $tmpKey");
        } else {
		    open(OUTPUTC , "> $outputdir/$stc_output_dir/$tmpKey");
        }
		$lines =~ s///g;
		print OUTPUTC $lines;
		close(OUTPUTC);
	}

	close(INPUTC);

	if($stc_debug eq "DEBUG_ON"){ end_time_log("==END CChange =="); }
}

sub iterate_equal(){
	# Rules
	# IFEQUAL | IFNOTEQUAL ( condition with general rules :&& ,|| ,etc)  /#       
	#       Contents with multiple lines
	# #/
	my $iterate_lines="";
	my $ifequal_one="";
	my $ifequal_two="";
	my $ifequal_parm="";
	my $len;
	my $if_before="";
	my $if_match="";
	my $if_after="";
    my $if_eval = "";

	$iterate_lines = shift @_;
	print DBG __SUB__ . " RD1 $iterate_lines]]]]]\n";

	while($iterate_lines =~ m/(\n[\t ]*)(IFEQUAL|IFNOTEQUAL)(\s*)\(/){
		#$iterate_lines =~ m/\n([\t ]*)(IFEQUAL|IFNOTEQUAL)\s*([^\n#\/]*)\s*\/\#/;
        # iterate_lines = $if_before . $if_match . $if_after
		my $indent = $1;
		my $order = $2;
		$if_before = $`;
		$if_match = $1 . $2 . $3;
		$if_after = $';
		#my $if_len = length($if_before);
		my $m_before = "";
		my $m_match = "";
		my $m_after = "";
        my $recursive = "";
		print DBG __SUB__ . "RD2 indent[$indent] order[$order] \n";
		$if_after = "\(" . $if_after;;
        if($if_after =~ m/(\(([^\(\)]|(?R))*\))/){
            # iterate_lines = $if_before . $if_match . ($m_before . $m_match . $m_after)
			$m_before = $`;
			$m_match = $&;
			$m_after = $';
			print DBG __SUB__ . "RD2 if_match[$m_match]\n";
			my $condition = $m_match;
			$if_eval = eval($condition);
			if($if_eval){
				print DBG "EQUAL_SUCCESS\n";
			}
			# rule : IF(?NOT)EQUAL (....) { .... }
			#    each gap does not have any characters.
			# So we can not have any string before matching text.
			if($m_before =~ m/\S+/){
				print STDOUT __SUB__ . "[$m_before]\n";
				exit;
			}
		} else {
			print STDOUT "ERROR : IFEQUAL|IFNOTEQUAL has condition with (...) \n";
			print DBG "ERROR : IFEQUAL|IFNOTEQUAL has condition with (...) \n";
			print DBG $if_after;
			exit;
		}
		# get block from +{{+ to +}}+
		print DBG __SUB__ . "RD2-2 m_after [$m_after]\n";
		if($m_after =~ m/\+\{\{\+(\s*(.*?|\n)*\s*)+\}\}\+/){   # recursion for nesting
            # iterate_lines = $if_before . $if_match . ($m_before . $m_match . ($contents_before . contents_match . contents_after))
            #                 ....         IFEQUAL      ___         (a eq b)    _                  +{{+abcd+}}+      .......
			# we need a new code for recursion. (refer to match.pl)
			# if($m_after =~ s/(\{([^\{\}]|(?R))*\})//)
			# if($if_after =~ m/((?:\/\#(?:[^#]|(?:\#+[^#\/]))*\#+\/))/)
			my $contents_group1 = $1;
			my $contents_before = $`;
			my $contents_match = $&;
			my $contents_match_inner = $contents_match;
			my $contents_after = $';
            if($contents_match =~ m/(IFEQUAL|IFNOTEQUAL)/){     # recursion for nesting
                print STDOUT "1 = $1\n";
                print STDOUT "contents_match = [$contents_match]\n";
                print DBG "RD3-0 contents_match = [$contents_match]\n";
                print STDOUT "match = [$&]\n";
                $contents_match_inner = $if_match . $m_before . $m_match . $contents_before . iterate_equal($contents_match);
                $recursive = 1;
                print DBG "RD3-0 contents_result = [$contents_match_inner]\n";
                print STDOUT "contents_result = [$contents_match_inner]\n";
            } else {
			    print DBG __SUB__ . " RD3-1 contents_group1 [$contents_group1]\n";
			    print DBG __SUB__ . " RD3-1 contents_match [$contents_match]\n";
			    $contents_match_inner =~ s/^\s*\+\{\{\+//;
			    $contents_match_inner =~ s/\s*\+\}\}\+\s*$//;
            }
			print DBG __SUB__ . " RD3-2 if_eval($m_match=>$if_eval) contents_match [$contents_match]\n";
			print DBG __SUB__ . " RD3-2 if_eval($m_match=>$if_eval) contents_match_inner [$contents_match_inner]\n";

			# rule : IF(?NOT)EQUAL (....) { .... }
			#    each gap does not have any characters.
			# So we can not have any string before matching text.
			if($contents_before =~ m/\S+/){
				print STDOUT __SUB__ . "[$contents_before]\n";
				exit;
			}

			# eval and process
            if($recursive){
                $iterate_lines = $if_before . $contents_match_inner . $contents_after;
            } else {
                if($order eq "IFEQUAL"){
                    if($if_eval){
                        $iterate_lines = $if_before . $contents_match_inner . $contents_after;
                    } else {
                        $iterate_lines = $if_before . $contents_after;
                    }
                } else {  # IFNOTEQUAL
                    if(not $if_eval){
                        $iterate_lines = $if_before . $contents_match_inner . $contents_after;
                    } else {
                        $iterate_lines = $if_before . $contents_after;
                    }
                }
            }
			print DBG __SUB__ . " RD4 if_eval($m_match=>$if_eval) : recursive=$recursive\n";
			print DBG "[$iterate_lines]\n";
		} else {
			print STDOUT "ERROR : IFEQUAL|IFNOTEQUAL has contents with {...} \n";
			print DBG "ERROR : IFEQUAL|IFNOTEQUAL has contents with {...} \n";
			print DBG $m_after;
			exit;
		}
	}
	#$iterate_lines =~ m/\s*IFEQUAL\s*((?:\/\#(?:[^#]|(?:\#+[^#\/]))*\#+\/))/;
	# my @pp = $a =~ s/((?:\/\*(?:[^*]|(?:\*+[^*\/]))*\*+\/)|(?:\/\/.*))//g;
	#if($a =~ /(\{([^\{\}]|(?R))*\})/){

	#$iterate_lines =~ s/STG_SHARP_/\#/g;


	return $iterate_lines;

	#$iterate_lines =~ s/#(\s*PARSING_RULE\s*:[^#]*)#/--$1--/g;
	#ITERATOR_DEBUG   print DBG "1111RETURN \$iterate_lines = \n\[\n$iterate_lines\n\]\n";
	# $iterate_lines 에는 +<+...+>+ 등의 문자가 없음 위에서 모두 해결된 것임. (값들만 들어감)
	#
	#$iterate_lines =~ s/STG_SHARP_/\#/g;
	#print_fp("$iterate_lines\n" , OUTPUTC);
	#if($iterate_lines =~ /(IFEQUAL.*)/){ print ": $1 \n: IFEQUAL .. #{ }#일때 }# 뒤에 문자가 오면 안됨\n"; print ": #{ }# 안에 # 문자가 오면 안됨.\n";  die $error = 612348;}
}

our %pNull = {};
sub getHashRef {
	my ($name) = @_;        # gCan{9}
	my $first;
	#print "F " . $name . "\n";
	$name =~ s/([^\{]+)//;
	$first = $1;
	my $hn = \%{$first};
	#print "F " . $hn . "\n";
	while($name =~ s/^\{([^\}]*)\}//){
		my $uu = $1;
		$hn = $hn->{$uu};
		#print "G " . $uu . "   $hn\n";
	}

	#print "I " . $hn . "\n";
	if($hn == 0){
		return \%pNull;
	} else {
		return $hn;
	}
}

sub max_keys
{
	my $iterate_var_name;
	my $allDigit = 1;
	my $max=0;
	($iterate_var_name ) = @_;
	foreach my $tmpKey ( keys %{getHashRef($iterate_var_name)}){
		if(not ($tmpKey =~ /^\s*\d*\s*$/)){
			$allDigit = 0;
			last;
		}
		if($max < $tmpKey){ $max = $tmpKey; }
	}
	
	if($allDigit == 1){ return  $max; }
	else { return  0; }
}

sub sort_keys
{
	my $iterate_var_name;
	my $iterate_op_type;
	my $allDigit = 1;
	my $tmpReverse = "+";
	my $tmpValue = 0;
	my $debug = "DEBUG_OFF";
	my @ret;
	($iterate_var_name ,$iterate_op_type,$debug) = @_;
	if($iterate_op_type =~ m/-/){ $tmpReverse = "-"; }
	if($iterate_op_type =~ m/V/){ $tmpValue = "V"; }
	if($tmpValue eq "V"){
		foreach my $tmpKey ( keys %{getHashRef($iterate_var_name)}){
			if(not (getHashRef($iterate_var_name)->{$tmpKey} =~ /^\s*\d*\s*$/)){
				$allDigit = 0;
				last;
			}
		}
	} else {
		foreach my $tmpKey ( keys %{getHashRef($iterate_var_name)}){
			if(not ($tmpKey =~ /^\s*\d*\s*$/)){
				$allDigit = 0;
				last;
			}
		}
	}
		
	if($allDigit == 1){ 
		if($tmpValue eq "V"){
			@ret = sort {getHashRef($iterate_var_name)->{$a} <=> getHashRef($iterate_var_name)->{$b}} keys %{getHashRef($iterate_var_name)};
		} else {
			@ret = sort {$a <=> $b} keys %{getHashRef($iterate_var_name)};
		}
	}
	else { @ret =  sort keys %{getHashRef($iterate_var_name)}; }

	if($tmpReverse eq "-"){
		@ret = reverse @ret;
	}

	if($debug eq "DEBUG_ON"){
		print DBG __SUB__;
		foreach (@ret){
			print DBG "  $_";
		}
		print DBG "\n";
	}

	return @ret
}

sub Iterator_recursion
{
	my $iterate_var_type;
	my $iterate_var_name;
	my $iterate_key;
	my $iterate_value;
	my $iterate_lines;
	my $result = "";
	my $in;
	my $iterate_cnt = 0;
	my @lines;
	my $myiterate_newline_type = "";

	($iterate_var_type , $iterate_var_name , $iterate_key , $iterate_value, $iterate_lines) = @_;
	#print_fp( "O : @_\n", OUTPUTC);
	print DBG __SUB__ . "RC : $iterate_var_type , $iterate_var_name , $iterate_key , $iterate_value\n";

	print DBG __SUB__ . "RC : Iterator_recursion : \$iterate_lines = $iterate_lines ]]]\n";

	# Various Operation  :
	#   % : hash
	#   @ : array
	#   & : reverse array
	#   + : 0 ~ max key value (only digits)
	if($iterate_var_type =~ m/^[+-]?[KV]?\%/){
		print DBG __SUB__ . "RC : HASH Iterator_recursion : \$iterate_var_name = $iterate_var_name ]]]\n";
		#$tmp1 = eval $$iterate_var_name;
		#print DBG __SUB__ . "RC-1 : " . (sort_keys($iterate_var_name) ) . "\n";
		foreach $stg_key_hash (sort_keys($iterate_var_name,$iterate_var_type,"DEBUG_ON")){
			print DBG __SUB__ . "RC : HASH Iterator_recursion : \$key = $stg_key_hash value=[ $$iterate_var_name{$stg_key_hash} ] [ $iterate_var_name ] [ " .  getHashRef($iterate_var_name)->{$stg_key_hash} . "]\n";
			my $myvalue = getHashRef($iterate_var_name)->{$stg_key_hash};
			$temp = $iterate_lines;
			$temp =~ s/$iterate_key/$stg_key_hash/g;
			$temp =~ s/$iterate_value/$myvalue/g;
			$result .= $temp;
		}
	} elsif($iterate_var_type eq "\@"){
		my $my_cnt;
		$my_cnt = @$iterate_var_name;
		#ITERATOR_DEBUG print DBG "--> ARRAY : $iterate_var_name  size =  $my_cnt\n";
		for(my $i = 0 ; $i < $my_cnt ; $i++){
			#ITERATOR_DEBUG print DBG "array : \$$iterate_var_name \[ $i \] = $$iterate_var_name[$i]\n";
			$temp = $iterate_lines;
			$temp =~ s/$iterate_key/$i/g;
			$temp =~ s/$iterate_value/$$iterate_var_name[$i]/g;
			$result .= $temp;
		}
	} elsif($iterate_var_type eq "\&"){
		my $my_cnt;
		$my_cnt = @$iterate_var_name;
		#ITERATOR_DEBUG print DBG "--> REVERSE ARRAY : $iterate_var_name  size =  $my_cnt\n";
		for(my $i = $my_cnt - 1 ; $i >= 0 ; $i--){
			#ITERATOR_DEBUG print DBG "REVERSE array : \$$iterate_var_name \[ $i \] = $$iterate_var_name[$i]\n";
			$temp = $iterate_lines;
			$temp =~ s/$iterate_key/$i/g;
			$temp =~ s/$iterate_value/$$iterate_var_name[$i]/g;
			$result .= $temp;
		}
	} elsif($iterate_var_type eq "\+"){
		my $my_max = max_keys($iterate_var_name);
		#ITERATOR_DEBUG 
		print DBG "--> MAX : $iterate_var_name  MAX COUNT =  $my_max\n";
		for(my $i = 0 ; $i <= $my_max ; $i++){
			#ITERATOR_DEBUG print DBG "array : \$$iterate_var_name \[ $i \] = $$iterate_var_name[$i]\n";
			$temp = $iterate_lines;
			$temp =~ s/$iterate_key/$i/g;
			$temp =~ s/$iterate_value/$$iterate_var_name{$i}/g;
			$result .= $temp;
		}
	} else {
		print "ERROR : unknown iterate var type  : $iterate_var_type\n";
		die $error = 500;
	}
	#print DBG __SUB__ . "RC : Iterator_recursion : \$result = $result ]]]\n";


	# Various Operation
	$iterate_lines = "";
	if($result =~            /\s*ITERATE(WithoutNewLine\s+|\s+)([+-]?[KV]?[%@&+])(\S+)\s+\+<<\+\s+(\S+)\s+(\S+)/){ 
		@lines = split("\n",$result);
		$result = "";
		foreach my $it_line (@lines){
			if ($it_line =~ /^\s*ITERATE(WithoutNewLine\s+|\s+)([+-]?[KV]?[%@&+])(\S+)\s+\+<<\+\s+(\S+)\s+(\S+)/){  
#print DBG "Set Hash 20 : $iterate_lines \n";
				$it_line = replace_var_with_value($it_line);
#print DBG "Set Hash 21 : $iterate_cnt : $it_line\n";
				$it_line =~ /^\s*ITERATE(WithoutNewLine\s+|\s+)([+-]?[KV]?[%@&+])(\S+)\s+\+<<\+\s+(\S+)\s+(\S+)/;  
				if(0 == $iterate_cnt){
#print  DBG "Sstart $1 $2 $3 $4\n"; 
					($iterate_var_type , $iterate_var_name , $iterate_key , $iterate_value) = ($2,$3,$4,$5);
					$myiterate_newline_type = $1;
				} else {
					$iterate_lines .= $it_line . "\n";
				}
				$iterate_cnt ++;
				$myiterate_newline_type =~ s/\s*$//;
			}
			elsif ($it_line =~ /^(.*)\+>>\+/){
#print DBG "SUB_ITERATE : $iterate_cnt : $it_line\n";
				$iterate_cnt--;
				if(0 == $iterate_cnt){
					$iterate_lines = Iterator_recursion($iterate_var_type , $iterate_var_name,$iterate_key,$iterate_value,$iterate_lines);
#print  DBG "Send result 30 :: $iterate_lines\n"; 
					#$iterate_lines = replace_var_with_value($iterate_lines);
					if($myiterate_newline_type ne "WithoutNewLine"){
						$result .= $iterate_lines;
					} else {
						#print STDERR "3{$iterate_lines}\n\n";
						#print STDERR "3[$result]\n\n";
						chomp($result);
						$iterate_lines =~ s/\n/ /g;
						$iterate_lines =~ s// /g;
						$result .= $iterate_lines . "\n";
						$myiterate_newline_type = "";
						#print STDERR "4{$iterate_lines}\n\n";
						#print STDERR "4[$result]\n\n";
					}
					#print OUTPUTC "SUB RETURN \$iterate_lines = \n\[\n$iterate_lines\n\]\n";
#print  DBG "Send result 31 :: $result\n"; 
					$iterate_lines = "";
				} else {
					$iterate_lines .= $it_line . "\n";
				}
			} 
			else {
#print DBG "SUB_ITERATE 40 : $iterate_cnt : $it_line\n";
				if(0 == $iterate_cnt){
					$it_line = replace_var_with_value($it_line);
#print DBG "SUB_ITERATE 41 : $iterate_cnt : $it_line\n";
					$result .= $it_line . "\n";
				} else {
					$iterate_lines .= $it_line . "\n";
#print DBG "TTT : \$iterate_lines = $iterate_lines ]]]\n";
				}
			} 
		}
	}
	return $result;
}


sub replace_var_with_value
{
	my $replace_in;
	my $in_cnt = 0;
	$replace_in = shift @_;

	#print DBG __SUB__ . ":" . __LINE__ . " AAAA : before $replace_in\n";
	while($replace_in =~ /(\d+)\s*\+\+\+\+/){		# 	++++     1을 더해 준다. 
		my $temp_num;
		$temp_num = $1;
		$temp_num++;
		$replace_in =~ s/\d+\s*\+\+\+\+/$temp_num/;
		$in_cnt ++;
	}
	#print DBG __SUB__ . ":" . __LINE__ . " ++++ : in_cnt $in_cnt\n";
	while($replace_in =~ /(\d+)\s*\-\-\-\-/){		# 	----     1을 빼준다.
		my $temp_num;
		$temp_num = $1;
		$temp_num--;
		$replace_in =~ s/\d+\s*\-\-\-\-/$temp_num/;
		$in_cnt ++;
	}
	#print DBG __SUB__ . ":" . __LINE__ . " ---- : in_cnt $in_cnt\n";
	#while(
	#($replace_in =~ s/\+<\+\s*\$([\w\d\.]+)\s*\+>\+/$$1/)		# 	+<+$stg_hash_del_timeout+>+ ==> 10
	#|| ($replace_in =~ s/\+<\+\s*\$([\w\d\.]+)\s*\[\s*(\d*)\s*\]\s*\+>\+/$$1[$2]/)	# +<+$typedef_name[54]+>+  ==> COMBI_Accum
	#|| ($replace_in =~ s/\+<\+\s*\$([\w\d\.]+)\s*\{\s*([^\}]+)\s*\}\s*\+>\+/$$1{"$2"}/) 	# +<+$HASH_KEY_TYPE{uiIP}+>+ ==> IP4
	#){
	#print DBG __SUB__ . __LINE__ . " BBBB : 1 $replace_in\n";
	#$in_cnt ++;
	#print DBG "Set Hash replace2 in_cnt=$in_cnt: $replace_in \n";
	#}			# +<+$type{+<+$HASH_KEY_TYPE{uiIP}+>+}+>+  ==> int

	#####  while($replace_in =~ /\+<\+\s*(\$[\w\d\.\)\(]+\s*[^\+>]*)\+>\+/)		# 	+<+$stg_hash_del_timeout+>+ ==> 10
	while($replace_in =~ /\+<\+\s*([^\+>]*)\s*\+>\+/)		# 	+<+$stg_hash_del_timeout+>+ ==> 10
	{
		my $match = $&;
		my $val = eval($1);
		print DBG __SUB__ . ":" . " $match =>  $val\n";
		$replace_in =~ s/\+<\+\s*([^\+>]*)\s*\+>\+/$val/;
		$in_cnt ++;
	};
	#print DBG __SUB__ . ":" . __LINE__ . " +<+ \$... +>+ : in_cnt $in_cnt\n";
	#print DBG __SUB__ . ":" . __LINE__ . " AAAA : after $replace_in\n";

	return $replace_in;
}

sub change_special_code {
	my ($s) = @_;
	$s =~ s/\{/#\+#\+#\+\+###/g;
	$s =~ s/\}/#\-#\-#\-\-###/g;
	$s =~ s/\\/#\=#\=#\=\=###/g;
	$s =~ s/\n/#\%#\%#\%\%###/g;
	$s =~ s/\"/#\&#\&#\&\&###/g;
	return $s;
}
sub recover_special_code {
	my ($s) = @_;
	$s =~ s/#\+#\+#\+\+###/\{/g;
	$s =~ s/#\-#\-#\-\-###/\}/g;
	$s =~ s/#\=#\=#\=\=###/\\/g;
	$s =~ s/#\%#\%#\%\%###/\n/g;
	$s =~ s/#\&#\&#\&\&###/\"/g;
	return $s;
}
sub traverse_hash_tree_to_recover_special_code {
	my ($TAXA_TREE,$vn,$lstr,$fh)    = @_;
	#print "sub $TAXA_TREE\n";
	foreach my $key (sort{$a<=>$b} keys %{$TAXA_TREE}) {
		if (ref $TAXA_TREE->{$key} eq 'HASH') {
			#print "K:$key lstr=$lstr\n";
			traverse_hash_tree_to_recover_special_code($TAXA_TREE->{$key},$vn,$lstr . "\{" . recover_special_code($key) . "\}",$fh);
		} else {
			#print "$lstr $key = $TAXA_TREE->{$key}\n";
			print $fh "\$$vn$lstr\{" . recover_special_code($key) ."\}=\"" . recover_special_code($TAXA_TREE->{$key}) . "\"\n";
		}
	}
}
sub traverse_hash_tree {
	my ($TAXA_TREE,$vn,$lstr,$fh)    = @_;
	traverse_hash_tree_to_recover_special_code($TAXA_TREE,$vn,$lstr,$fh);
}


mkdir "OUTPUT";
mkdir "OUTPUT/stc";
mkdir "OUTPUT/tmp";
# set the variables from file
print "arguments count : $#ARGV\n";
print STDERR "arguments count : $#ARGV\n";
($filename,$stcfilename) = (@ARGV);
if($stcfilename eq ""){
	$filename = "default.GVm";
	$stcfilename = "default.stc";
}

## init file open
open(DBG,">debug.log");
open(TIME_DBG,">time_debug.log");
open(VAR_DBG,">var_debug.log");


print "fname = $filename , stc fname = $stcfilename\n";
print STDERR "fname = $filename , stc fname = $stcfilename\n";
open(FH, "<",$filename) or die "Can't open < $filename: $!";
my $lcnt = 0 ;
while(<FH>){
	$s = $s_org = $_;
	chop($s);
	eval $s;
	$s =~ /^\$([^\{]+)/;
	$hashName{$1} = "Done";
	if($1 eq "gCan"){ print "== $s\n"; }
	#recover_hash_value(\%{$vname},$s);
	#LOG1 print $s_org;
	#if($lcnt >50){ last; }
	#$lcnt++;
}
close(FH);
foreach my $key (keys %gVariables){
	$$key = $gVariables{$key};
}



foreach my $key (keys %hashName){
	print VAR_DBG "----[$key]----\n";
	traverse_hash_tree(\%{$key},$key,"",VAR_DBG);
}

if($stcfilename =~ /\.stc$/){
	CChange($stcfilename,"stc","","DEBUG_ON");
} elsif($stcfilename =~ /\.stcI$/){
	IChange($stcfilename,"stc");
}

exit;

print \%{gCol} . "\n";
print getHashRef("gCol") . "\n";
foreach my $key (sort{$a<=>$b} keys %{getHashRef("gCol")}) { print "$key  ";} print "\n";
print $gCol{9} . "\n";
print \%{"gCol{9}"} . "\n";
print getHashRef("gCol{9}") . "\n";
$tmp = $gCol{9};
foreach my $key (sort{$a<=>$b} keys %$tmp) { print "A$key  ";} print "\n";
foreach my $key (sort{$a<=>$b} keys %{(getHashRef("gCol"))->{9}}) { print "B$key  ";} print "\n";
foreach my $key (sort{$a<=>$b} keys %{getHashRef("gCol{9}")}) { print "C$key  ";} print "\n";
foreach my $key (sort{$a<=>$b} keys %{getHashRef("gCan{3-4}")}) { print "C$key  ";} print "\n";
foreach my $key (sort{$a<=>$b} keys %{getHashRef("gCan{9}")}) { print "C$key  ";} print "\n";
foreach my $key (sort{$a<=>$b} keys %{getHashRef("gCan{1}")}) { print "C$key  ";} print "\n";
foreach my $key (sort{$a<=>$b} keys %{getHashRef("gCan{2}")}) { print "C$key  ";} print "\n";
getHashRef(gCan)->{100}->{10} = 11111;
foreach my $key (sort{$a<=>$b} keys %{getHashRef("gCan")}) { print "C$key  ";} print "\n";
foreach my $key (sort{$a<=>$b} keys %{getHashRef("gCan{100}")}) { print "C$key  ";} print "\n";

my $comment =  <<END_COMMENT;
		foreach my $key (keys %hashName){
			print VAR_DBG "----[$key]----\n";
			traverse_hash_tree(\%{$key},$key,"",VAR_DBG);
		}
END_COMMENT

