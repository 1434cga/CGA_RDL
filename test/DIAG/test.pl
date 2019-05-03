$il = <<'END_MESSAGE';
	virtual error_t wishtoUseAPI_1(
        IFEQUAL( (1 eq 1)&& (1 eq 1))+{{+
            IFEQUAL( (""  ne "") )+{{+
                    android::sp<WifiData>& 
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                       
                +}}+
                IFEQUAL(""  ne "")+{{+
                    size_t   
                    ,    
                +}}+
            +}}+
        +}}+
        IFEQUAL(1 ne 1)+{{+
            IFEQUAL(""  ne "")+{{+
                    , android::sp<WifiData>& 
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                    ,    
                +}}+
                IFEQUAL(""  ne "")+{{+
                    , size_t   
                    ,    
                +}}+
            +}}+
        +}}+
        IFEQUAL( (2 eq 1)&& (2 eq 1))+{{+
            IFEQUAL( (""  ne "") )+{{+
                    android::sp<WifiData>& att
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                    uint8_t att  
                +}}+
                IFEQUAL(""  ne "")+{{+
                    size_t   
                    , uint8_t att  
                +}}+
            +}}+
        +}}+
        IFEQUAL(2 ne 1)+{{+
            IFEQUAL(""  ne "")+{{+
                    , android::sp<WifiData>& att
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                    , uint8_t att  
                +}}+
                IFEQUAL(""  ne "")+{{+
                    , size_t   
                    , uint8_t att  
                +}}+
            +}}+
        +}}+
        IFEQUAL( (3 eq 1)&& (3 eq 1))+{{+
            IFEQUAL( (""  ne "") )+{{+
                    android::sp<WifiData>& data
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL("dataLen"  eq "")+{{+
                    uint8_t* data  
                +}}+
                IFEQUAL("dataLen"  ne "")+{{+
                    size_t dataLen  
                    , uint8_t* data  
                +}}+
            +}}+
        +}}+
        IFEQUAL(3 ne 1)+{{+
            IFEQUAL(""  ne "")+{{+
                    , android::sp<WifiData>& data
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL("dataLen"  eq "")+{{+
                    , uint8_t* data  
                +}}+
                IFEQUAL("dataLen"  ne "")+{{+
                    , size_t dataLen  
                    , uint8_t* data  
                +}}+
            +}}+
        +}}+
    )=0;
	virtual error_t wishtoUseAPI_2(
        IFEQUAL( (1 eq 1)&& (1 eq 1))+{{+
            IFEQUAL( (""  ne "") )+{{+
                    android::sp<WifiData>& xxData
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                    xxData xxData  
                +}}+
                IFEQUAL(""  ne "")+{{+
                    size_t   
                    , xxData xxData  
                +}}+
            +}}+
        +}}+
        IFEQUAL(1 ne 1)+{{+
            IFEQUAL(""  ne "")+{{+
                    , android::sp<WifiData>& xxData
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                    , xxData xxData  
                +}}+
                IFEQUAL(""  ne "")+{{+
                    , size_t   
                    , xxData xxData  
                +}}+
            +}}+
        +}}+
    )=0;
	virtual error_t wishtoUseAPI_3(
        IFEQUAL( (1 eq 1)&& (1 eq 1))+{{+
            IFEQUAL( (""  ne "") )+{{+
                    android::sp<WifiData>& ttt
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL("tttLen"  eq "")+{{+
                    uint8_t* ttt  
                +}}+
                IFEQUAL("tttLen"  ne "")+{{+
                    size_t tttLen  
                    , uint8_t* ttt  
                +}}+
            +}}+
        +}}+
        IFEQUAL(1 ne 1)+{{+
            IFEQUAL(""  ne "")+{{+
                    , android::sp<WifiData>& ttt
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL("tttLen"  eq "")+{{+
                    , uint8_t* ttt  
                +}}+
                IFEQUAL("tttLen"  ne "")+{{+
                    , size_t tttLen  
                    , uint8_t* ttt  
                +}}+
            +}}+
        +}}+
    )=0;
	virtual error_t wishtoUseAPI_4(
        IFEQUAL( (1 eq 1)&& (1 eq 1))+{{+
            IFEQUAL( (""  ne "") )+{{+
                    android::sp<WifiData>& did
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                    uint16_t did  
                +}}+
                IFEQUAL(""  ne "")+{{+
                    size_t   
                    , uint16_t did  
                +}}+
            +}}+
        +}}+
        IFEQUAL(1 ne 1)+{{+
            IFEQUAL(""  ne "")+{{+
                    , android::sp<WifiData>& did
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                    , uint16_t did  
                +}}+
                IFEQUAL(""  ne "")+{{+
                    , size_t   
                    , uint16_t did  
                +}}+
            +}}+
        +}}+
    )=0;
	virtual error_t wishtoUseAPI_5(
        IFEQUAL( ( eq 1)&& ( eq 1))+{{+
            IFEQUAL( (""  ne "") )+{{+
                    android::sp<WifiData>& 
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                       
                +}}+
                IFEQUAL(""  ne "")+{{+
                    size_t   
                    ,    
                +}}+
            +}}+
        +}}+
        IFEQUAL( ne 1)+{{+
            IFEQUAL(""  ne "")+{{+
                    , android::sp<WifiData>& 
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                    ,    
                +}}+
                IFEQUAL(""  ne "")+{{+
                    , size_t   
                    ,    
                +}}+
            +}}+
        +}}+
    )=0;
	virtual error_t wishtoUseAPI_6(
        IFEQUAL( ( eq 1)&& ( eq 1))+{{+
            IFEQUAL( (""  ne "") )+{{+
                    android::sp<WifiData>& 
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                       
                +}}+
                IFEQUAL(""  ne "")+{{+
                    size_t   
                    ,    
                +}}+
            +}}+
        +}}+
        IFEQUAL( ne 1)+{{+
            IFEQUAL(""  ne "")+{{+
                    , android::sp<WifiData>& 
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                    ,    
                +}}+
                IFEQUAL(""  ne "")+{{+
                    , size_t   
                    ,    
                +}}+
            +}}+
        +}}+
    )=0;
	virtual error_t wishtoUseAPI_7(
        IFEQUAL( ( eq 1)&& ( eq 1))+{{+
            IFEQUAL( (""  ne "") )+{{+
                    android::sp<WifiData>& 
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                       
                +}}+
                IFEQUAL(""  ne "")+{{+
                    size_t   
                    ,    
                +}}+
            +}}+
        +}}+
        IFEQUAL( ne 1)+{{+
            IFEQUAL(""  ne "")+{{+
                    , android::sp<WifiData>& 
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                    ,    
                +}}+
                IFEQUAL(""  ne "")+{{+
                    , size_t   
                    ,    
                +}}+
            +}}+
        +}}+
    )=0;
	virtual error_t wishtoUseAPI_8(
        IFEQUAL( ( eq 1)&& ( eq 1))+{{+
            IFEQUAL( (""  ne "") )+{{+
                    android::sp<WifiData>& 
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                       
                +}}+
                IFEQUAL(""  ne "")+{{+
                    size_t   
                    ,    
                +}}+
            +}}+
        +}}+
        IFEQUAL( ne 1)+{{+
            IFEQUAL(""  ne "")+{{+
                    , android::sp<WifiData>& 
            +}}+
            IFEQUAL(""  eq "")+{{+
                IFEQUAL(""  eq "")+{{+
                    ,    
                +}}+
                IFEQUAL(""  ne "")+{{+
                    , size_t   
                    ,    
                +}}+
            +}}+
        +}}+
    )=0;
END_MESSAGE

if($ARGV[1] eq ""){ $end = -1; }
$end = int($ARGV[0]);
$cnt = 0;

print("cnt $cnt , end $end\n");

while ($il =~ m/\+\{\{\+(\s*(.*?|\n)*\s*)+\}\}\+/){   # recursion for nesting
    print("cnt $cnt , end $end\n");

    $pttn = "\+\{\{\+";
    if($il =~ m/\+\{\{\+(\s*(.*?|\n)*\s*)+\}\}\+/){   # recursion for nesting
        $before = $`;
        $match = $&;
        $after = $';
        print "ND1-1 match[$match]\n";
        @a = split(/\+\{\{\+/,$match);
        print "ND1-1 \n";
        for($i=0;$i<= $#a;$i++) {
            print "$i:($a[$i])\n";
        }
        $final = $a[$#a];
        delete $a[$#a];
        for($i=0;$i<= $#a;$i++) {
            print "$i:($a[$i])\n";
        }

        print "final1:($final)\n";
        $final =~ s/^\s*\n//;
        print "final2:($final)\n";
        $final =~ s/\n?\s*\+\}\}\+\s*$//;
        print "final3:($final)\n";
        #print "before($before)\n";
        for($i=0;$i< $#a;$i++) {
            $before .= $a[$i] . $pttn;
        }
    }
    if($a[$#a] =~ m/(IFEQUAL|IFNOTEQUAL)(\s*)/){
        $i_before = $`;
        $i_match = $&;
        $i_after = $';
        $iif = $1;
        print "ND1-2 i_before[$iif:$i_before]\n";
        print "ND1-2 i_match[$i_match]\n";
        print "ND1-2 i_after[$i_after]\n";
        $i_before =~ s/\n\s*$//;
        $before .= $i_before;
    } else {
        if($before =~ m/(IFEQUAL|IFNOTEQUAL)(\s*)/){
            $i_before = $`;
            $i_match = $&;
            $i_after = $';
            $iif = $1;
            print "ND1-3 i_before[$iif:$i_before]\n";
            print "ND1-3 i_match[$i_match]\n";
            print "ND1-3 i_after[$i_after]\n";
            $i_before =~ s/\n\s*$//;
            $before = $i_before;
        } 
    }
    print "before($before)\n";
    if($i_after =~ m/(\(([^\(\)]|(?R))*\))/){
        $b_before = $`;
        $b_match = $&;
        $b_after = $';
        print "ND1-2 b_before[$b_before]\n";
        print "ND1-2 b_match[$b_match]\n";
        print "ND1-2 b_after[$b_after]\n";
    }
    if($iif eq "IFEQUAL"){
        $val = eval($b_match);
        print "$b_match -> val:$val\n";
        if($val ne ""){
            $before .= "\n$final";
        }
    } else {
        $val = eval($b_match);
        print "$b_match -> val:$val\n";
        if($val eq ""){
            $before .= "\n$final";
        }
    }
    #print "before($before$after)\n";
    open($fh , ">" , "iter" . $cnt . ".out");
    print $fh "$before$after\n";
    close $fh;
    $il = "$before$after";
    $cnt++;
    if($cnt == $end){ return ; }
    print("cnt $cnt , end $end\n");
};


