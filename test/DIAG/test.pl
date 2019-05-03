$iterate_lines = <<'END_MESSAGE';
class IWifiManagerService : public android::IInterface
{
public:
    DECLARE_META_INTERFACE(WifiManagerService);
    //< Interfaces for IWifiManagerService

    virtual error_t attachReceiver(serviceid_t id, android::sp<IWifiManagerReceiver>& receiver) = 0;
    virtual error_t unregisterReceiver( const android::sp<IWifiManagerReceiver>& receiver) = 0;
    virtual android::sp<Buffer> SLDDRequestDataID (uint16_t  request_id)=0;
    virtual error_t SLDDResponseDataID (uint16_t request_id, android::sp<Buffer> buf) = 0;

	// auto CGA start
	virtual error_t wishtoUseAPI_1(
    wishtoUseAPI_1
        IFEQUAL( (1 eq 1)&& (1 eq 1))+{{+
            IFEQUAL( (""  eq "") && (1 eq 1))+{{+
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
        IFEQUAL( (2 eq 1)&& (2 eq 1))+{{+
            IFEQUAL( (""  ne "") && (2 eq 1))+{{+
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
            IFEQUAL( (""  ne "") && (3 eq 1))+{{+
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
    // android::sp<WifiData>& mwifiData
    )=0;
	virtual error_t wishtoUseAPI_2(
    wishtoUseAPI_2
        IFEQUAL( (1 eq 1)&& (1 eq 1))+{{+
            IFEQUAL( (""  ne "") && (1 eq 1))+{{+
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
    // android::sp<WifiData>& mwifiData
    )=0;
	virtual error_t wishtoUseAPI_3(
    wishtoUseAPI_3
        IFEQUAL( (1 eq 1)&& (1 eq 1))+{{+
            IFEQUAL( (""  ne "") && (1 eq 1))+{{+
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
    // android::sp<WifiData>& mwifiData
    )=0;
	virtual error_t wishtoUseAPI_4(
    wishtoUseAPI_4
        IFEQUAL( (1 eq 1)&& (1 eq 1))+{{+
            IFEQUAL( (""  ne "") && (1 eq 1))+{{+
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
    // android::sp<WifiData>& mwifiData
    )=0;
	virtual error_t wishtoUseAPI_5(
    wishtoUseAPI_5
        IFEQUAL( ( eq 1)&& ( eq 1))+{{+
            IFEQUAL( (""  ne "") && ( eq 1))+{{+
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
    // android::sp<WifiData>& mwifiData
    )=0;
	virtual error_t wishtoUseAPI_6(
    wishtoUseAPI_6
        IFEQUAL( ( eq 1)&& ( eq 1))+{{+
            IFEQUAL( (""  ne "") && ( eq 1))+{{+
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
    // android::sp<WifiData>& mwifiData
    )=0;
	virtual error_t wishtoUseAPI_7(
    wishtoUseAPI_7
        IFEQUAL( ( eq 1)&& ( eq 1))+{{+
            IFEQUAL( (""  ne "") && ( eq 1))+{{+
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
    // android::sp<WifiData>& mwifiData
    )=0;
	virtual error_t wishtoUseAPI_8(
    wishtoUseAPI_8
        IFEQUAL( ( eq 1)&& ( eq 1))+{{+
            IFEQUAL( (""  ne "") && ( eq 1))+{{+
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
    // android::sp<WifiData>& mwifiData
    )=0;
	// auto CGA end

	virtual error_t requestDataID(android::sp<WifiData>& mwifiData) =0;
	virtual error_t responseDataID (android::sp<WifiData>& mwifiData) =0;
	virtual error_t SlddTestCmd(uint8_t id, uint8_t cmd, uint8_t cmd2, android::sp<Buffer> buf) = 0;
};
END_MESSAGE

$pttn = "\+\{\{\+";
if($iterate_lines =~ m/\+\{\{\+(\s*(.*?|\n)*\s*)+\}\}\+/){   # recursion for nesting
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
	
	$final =~ s/^\s*\n//;
	$final =~ s/\n\s*\+\}\}\+\s*//;
	print "final:($final)\n";
	print "before($before)\n";
	for($i=0;$i< $#a;$i++) {
		$before .= $pttn . $a[$i];
	}
	print "before($before)\n";
}
if($a[$#a] =~ m/(IFEQUAL|IFNOTEQUAL)(\s*)/){
	$i_before = $`;
	$i_match = $&;
	$i_after = $';
	$iif = $1;
	print "ND1-2 i_before[$iif:$i_before]\n";
	print "ND1-2 i_match[$i_match]\n";
	print "ND1-2 i_after[$i_after]\n";
}
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
		$before .= "\n$final\n";
	}
}
	print "before($before$after)\n";


