$str = "IFEQUAL +{{+ aaa
      kfajsdfl+kajsfsomething
+}}+
tone 
  +}}+
";

if($str =~ m/(\+\{\{\+(.*?|\n)*\+\}\}\+)/){
	print "1before [$`]\n";
	print "1match [$&]\n";
	print "1after [$']\n";
}

$str = "IFEQUAL +{{+ aaa
      kfajsdfl+kajsfsomething
	  +{{+   TTTT
	  PPP
+}}+
tone 
  +}}+
";

if($str =~ m/(\+\{\{\+(.*?|\n)*\+\}\}\+)/){
	print "2before [$`]\n";
	print "2match [$&]\n";
	print "2after [$']\n";
}
if($str =~ m/(\+\{\{\+(.*|\n)*\+\}\}\+)/){
	print "3before [$`]\n";
	print "3match [$&]\n";
	print "3after [$']\n";
}
if($str =~ m/(\+\{\{\+.*?\+\}\}\+)/msp){		# ms : multiple line , p : MATCH
	print "4PREMATCH [${^PREMATCH}]\n";
	print "4MATCH [${^MATCH}]\n";
	print "4POSTMATCH [${^POSTMATCH}]\n";
}
if($str =~ m/([^(\+\{\{\+|\+\}\}\+)]*?)\+\}\}\+/msp){		# ms : multiple line , p : MATCH  , consider recursive loop
	print "5PREMATCH [${^PREMATCH}]\n";
	print "5MATCH [${^MATCH}]\n";
	print "5POSTMATCH [${^POSTMATCH}]\n";
	print "5match \$1 [$1]\n";
}
print "end\n";

$org = " LLL
IFEQUAL(ITKEY eq  \"O\") +{{+ aaa
    kfajsdflkajsfsomething
    a += 3;
    {
        b = 5+4;
        c++;
    }
    IFEQUAL(KT2KEY eq item)+{{+   TTTT
        IF;
    +}}+
    tone  += 4;
+}}+

other = q +t;
";
print "\nstr = $str\n";


my $str = $org;
#while($str =~ m/\s*IFEQUAL\s*\(([^\(\)]|(?R))*\)\s*\+\{\{\+([^((\+\{\{\+)|(\+\}\}\+)|(IFEQUAL))]*?)\+\}\}\+/msp){		# ms : multiple line , p : MATCH  , consider recursive loop
#while($str =~ m/\s*IFEQUAL\s*\(([^\(\)]|(?R))*\)\s*\+\{\{\+(((?:(?!\+\{\{\+).)*)|((?:(?!\+\}\}\+).)*)|((?:(?!IFEQUAL))))\+\}\}\+/msp){		# ms : multiple line , p : MATCH  , consider recursive loop
#while($str =~ m/\s*IFEQUAL\s*\(([^\(\)]|(?R))*\)\s*\+\{\{\+((?:(?!\+\{\{\+)).)*\+\}\}\+/msp){		# ms : multiple line , p : MATCH  , consider recursive loop --> go 1 step
#while($str =~ m/\s*IFEQUAL\s*\(([^\(\)]|(?R))*\)\s*\+\{\{\+((?!\+\{\{\+).)*\+\}\}\+/msp){		# ms : multiple line , p : MATCH  , consider recursive loop --> go 1 step
#while($str =~ m/\s*IFEQUAL\s*\(([^\(\)]|(?R))*\)\s*\+\{\{\+((?!\+\}\}\+).)*\+\}\}\+/msp){		# ms : multiple line , p : MATCH  , consider recursive loop --> go 1 step
 while($str =~ m/\s*IFEQUAL\s*\(([^\(\)]|(?R))*\)\s*\+\{\{\+((?!IFEQUAL).)*\+\}\}\+/msp){		# ms : multiple line , p : MATCH  , consider recursive loop --> go 1 step
#while($str =~ m/\s*IFEQUAL\s*\(([^\(\)]|(?R))*\)\s*\+\{\{\+(((?:(?!\+\{\{\+)).)*|((?:(?!\+\}\}\+)).)*)\+\}\}\+/msp){		# ms : multiple line , p : MATCH  , consider recursive loop
    $match = ${^MATCH};
    $pre = ${^PREMATCH};
    $post = ${^POSTMATCH};
	print "6PREMATCH [${^PREMATCH}]\n";
	print "6MATCH [$match]\n";
	print "6POSTMATCH [${^POSTMATCH}]\n";
	print "6match \$1 [$1]\n";
    $match =~ m/(\s*)IFEQUAL\s*\(([^\(\)]|(?R))*\)\s*\+\{\{\+((?!\+\}\}\+).)*\+\}\}\+/msp;
    $space = $1;
	$prematch = ${^PREMATCH};
    $fmatch = ${^MATCH};
	$postmatch = ${^POSTMATCH};
	print "6-Final PRE [$prematch]\n";
	print "6-Final SPACE [$space]\n";
	print "6-Final MATCH [$fmatch]\n";
	print "6-Final POST [$postmatch]\n";
     print "\n==========\n";
     print "PRE = [$pre]\n";
     print "MATCH PRE = [$prematch]\n";
     print "MATCH SPACE= [$space]\n";
     print "MATCH Final= [$fmatch]\n";
     print "MATCH POST= [$postmatch]\n";
     print "POST = [$post]\n";
     print "==========\n";
    $str = $pre . $prematch . $postmatch . $post;
     ## $str =~ s/\s*IFEQUAL\(([^\(\)]|(?R))*\)\s*\+\{\{\+([^(\+\{\{\+|\+\}\}\+|IFEQUAL)]*?)\+\}\}\+//ms;		# ms : multiple line , p : MATCH  , consider recursive loop 
     #$str =~ s/$ttt//;
     print "\nstr = $str\n";
}
print "end\n";

$org = " LLL
IFEQUAL( (ITKEY eq  \"O\")) +{{+ aaa
 ()
( p )
()
";
print "\norg = $org\n";
if($org =~ m/\((?:\(.*\)|[^\(])*\)/msp){              # https://stackoverflow.com/questions/15301708/perl-regular-expression-match-nested-brackets
	print "7-1 PREMATCH [${^PREMATCH}]\n";
	print "7-1 MATCH [${^MATCH}]\n";
	print "7-1 POSTMATCH [${^POSTMATCH}]\n";
	print "7-1 match \$1 [$1]\n";
}
if($org =~ m/(\(([^\(\)]|(?R))*\))/msp){              #  <- This is better.
	print "7-2 PREMATCH [${^PREMATCH}]\n";
	print "7-2 MATCH [${^MATCH}]\n";
	print "7-2 POSTMATCH [${^POSTMATCH}]\n";
	print "7-2 match \$1 [$1]\n";
}
