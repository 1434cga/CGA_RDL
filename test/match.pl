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
