#!/usr/bin/perl

# help URL :  http://search.cpan.org/~hmbrand/Spreadsheet-Read/Read.pm

use 5.010;
use Spreadsheet::Read;
use File::Basename;
use Cwd  qw(abs_path);

use lib dirname(dirname abs_path $0) . '/perllib';
use lib abs_path . '/../../../CGA_RDL/perllib';
use lib abs_path . '/../../CGA_RDL/perllib';
use lib abs_path . '/../CGA_RDL/perllib';
use lib abs_path . '/CGA_RDL/perllib';
use lib abs_path . '/perllib';
use lib abs_path . '/../perllib';
use lib abs_path . '/../../perllib';
use lib abs_path . '/../../../perllib';
use MY::CHARLES qw(traverse_tree_to_file);
use MY::CHARLES qw(traverse_hash_tree);

use Getopt::Long;
our $input_file   = "./1_example.xlsx";
our $excel_version_input_file   = "";
our $output_file = "./default.GV";
our $csv_file = "./1_example.csv";
our $help=0;
our $excelVersionMajor;
our $excelVersionMinor;
our $excelVersionDev;
our $py=0;
our $ldest="input_file_name";
our $lid="tidl";
our $lpasswd="tidl1234";
our $lhost="abc.com";
our $lloc="~/warehouse/";
GetOptions ("output=s" => \$output_file,    # string
		"input=s"   => \$input_file,      # string
		"excel_version_input=s"   => \$excel_version_input_file,      # string
		"csv_output=s"   => \$csv_file,      # string
		"python"  => sub { $py = 1 } ,  # python  command
		"destinfo=s"  => \$ldest ,  # destination information file name
		"id=s"  => \$lid ,  # account id
		"passwd=s"  => \$lpasswd ,  # passwd
		"host=s"  => \$lhost ,  # host
		"location=s"  => \$lloc ,  # storing location
		"verbose|help"  => sub { $help = 1 })   # flag
or  die(help() . "Error in command line arguments\n");

if($help == 1){
	help();
	exit();
}

print STDERR "version file : $excel_version_input_file\n";

if($excel_version_input_file ne ""){
    open(my $verfh, "<", $excel_version_input_file);
    my $s = <$verfh>;
    $s =~ /\s*\D*\s*(\d+)\.(\d+)\.(\d+)\s*$/;
    $excelVersionMajor = $1;
    $excelVersionMinor = $2;
    $excelVersionDev = $3;
    print STDERR "excel CURRENT VERSION $s => Major $excelVersionMajor, Minor $excelVersionMinor, Dev $excelVersionDev\n";
    close($verfh);
}

print "[" . abs_path . "]\n";

die "input file $input_file is not exist\n" unless (-e $input_file);

print "excel input file = $input_file /  output file = $output_file / csv out file = $csv_file\n";

our %gTitle;
our %VARIABLE;

my $book = ReadData($input_file);
open(my $csvfh, ">", $csv_file)
    or die "Can't open > $csv_file : $!";

say '[0] A1: ' . $book->[0]{A1};
say '[1] A1: ' . $book->[1]{A1};
say '[1] label:' . $book->[1]{label};
say '[1] maxcol:' . $book->[1]{maxcol};
say '[1] maxrow:' . $book->[1]{maxrow};
#say '[2] A1: ' . $book->[2]{A1};
#say '[2] label:' . $book->[2]{label};
#say '[2] maxcol:' . $book->[2]{maxcol};
#say '[2] maxrow:' . $book->[2]{maxrow};
#say '[3] A1: ' . $book->[3]{A1};
##say '[3] label:' . $book->[3]{label};
#say '[3] maxcol:' . $book->[3]{maxcol};
#say '[3] maxrow:' . $book->[3]{maxrow};
#say '[4] A1: ' . $book->[4]{A1};

#our %Manager_Name;

{   #  Title Row starts with [HEADER].
	# Then we will make the variables and values with hash.
	#my @row = Spreadsheet::Read::row($book->[1], 1);
	#for my $i (0 .. $#row) {
		#say "$book->[1]{label} sheet Header:" . chr(65+$i) . (1) . ' ' . ($row[$i] // '');
	#}
    #
    # Title Row starts with [VARIABLE].
    # this is a single varialbe. so we will make %VARIABLE
	my @rows = Spreadsheet::Read::rows($book->[1]);
	my @title;
	my $headerCnt = 0;
	my $titleCnt = 0;
	my $titleName = "";
	foreach my $i (1 .. scalar @rows) {
		print "RAW $i  $rows[$i-1][0] \n";
        {
			my @row = Spreadsheet::Read::row($book->[1], $i);
			for my $j (0 .. $#row) {
                my $myt = FixXML($row[$j]);
                if( ($myt =~ /\"/) || ($myt =~ /,/) ){
                    $myt =~ s/\"/\"\"/g;
                    print $csvfh "\"" . $myt . "\",";
                } else {
                    print $csvfh $myt . ",";
                }
            }
            print $csvfh "\n";
        }
		next if($rows[$i-1][0] =~ /^\s*$/) ;
		next if($rows[$i-1][0] =~ /^\s*#/) ;
		checkHeader($rows[$i-1][0]);
		if($rows[$i-1][0] =~ /^\s*\[VARIABLE\]\s*(\S*)/){
            $gTitle{VARIABLE}{$1} = $rows[$i-1][1];
            $VARIABLE{$1} = $rows[$i-1][1];
        } elsif($rows[$i-1][0] =~ /^\s*\[HEADER\]\s*(\S*)/){
			$headerCnt = 0;
			$titleCnt = 0;
			delete @title[0 .. $#title];
			say "$1";
			$titleName = $1;
			my $myhash = \%{$titleName};
			print "AAA $titleName : $$titleName myhash :$myhash\n";
			#foreach my $key (keys %Manager_Name){ print "MN: $key  "; }
			#print "\n";
			my @row = Spreadsheet::Read::row($book->[1], $i);
			for my $j (0 .. $#row) {
				say "HEADER 1, $j , hC $headerCnt , tC $titleCnt , $book->[1]{label} sheet Header:" . chr(65+$j) . (1) . ' ' . ($row[$j] // '');
				next if($row[$j] =~ /^\s*$/);
				checkHeader($row[$j]);
				if($row[$j] =~ /^\s*\[HEADER\]\s*(\S*)/){
					$title[$titleCnt] = removeSpace($1);
					$gTitle{$titleName}{$titleCnt} = $title[$titleCnt];
					$gTitle{HEADER}{$titleName}{$titleCnt} = $title[$titleCnt];
					$titleCnt++;
					$headerCnt++;
				} else {
					last;
				}
			}
			say "2, $j , hC $headerCnt , tC $titleCnt , $book->[1]{label} sheet Header:" . chr(65+$j) . (1) . ' ' . ($row[$j] // '');
			for my $j ($headerCnt .. $#row) {
				say "3, $j , hC $headerCnt , tC $titleCnt , $book->[1]{label} sheet Header:" . chr(65+$j) . (1) . ' ' . ($row[$j] // '');
				if($row[$j] =~ /^\s*\[HEADER\]\s*(\S*)/){
					die "ERROR : You should have [HEADER] continuously from first column. But, you used [HEADER] in the middle of titles.\n";
				}
				next if($row[$j] =~ /^\s*$/);
				$title[$titleCnt] = removeSpace($row[$j]);
				$gTitle{$titleName}{$titleCnt} = $title[$titleCnt];
				$gTitle{NOHEADER}{$titleName}{$titleCnt-$headerCnt} = $title[$titleCnt];
				$titleCnt++;
			}
			#$gPrintHashName{$title[0]} = "Title $i Row";
			for my $j (0 .. $#title) {
				if($j < $headerCnt){
					say "sheet Header:$j $#title " . chr(65+$j) . ' ' . ($title[$j] // '');
				} else {
					say "sheet Data:$j $#title " . chr(65+$j) . ' ' . ($title[$j] // '');
				}
			}
		} elsif( not ($rows[$i-1][0] =~ /^\s*$/) ){		# must have contents in firt column
			#my $details = getDetails(3, \%{ $D{classes}{$classes}{public_methods}{members}{$members}{detailed}{doc} });
				#my $myhash = shift;
				#foreach my $tmpKey (sort_keys(\%{$myhash})){
					#print "getDetails type [" . $myhash->{$tmpKey}{type} . "]\n";
				#}
			my @row = Spreadsheet::Read::row($book->[1], $i);
			my $myhash = \%{ $titleName };
			print "BBB myhash $titleName : $myhash\n";
			for my $j (0 .. ($headerCnt -1)) {
				print "1=> $j $row[$j]  max $#row  , headerCnt $headerCnt\n";
				print "1=> $myhash $myhash->{$row[$j]}\n";
				#next if($row[$j] =~ /^\s*$/) ;
				my $key = removeSpace($row[$j]);
				if($j == $#row){
					print "=> END $j $row[$j]  $#row\n";
					$myhash->{$key} = "";
					last;
				}
				unless (exists $myhash->{$key}){
					$myhash->{$key} = {};
					print "=> unless $myhash $myhash->{$key}\n";
				}
				say "$book->[1]{label} sheet Header:" . chr(65+$j) . (1) . ' ' . ($key // '');
				$myhash = \%{ $myhash->{removeSpace($key)} };
				#traverse_hash_tree(\%_G_,"_G_","",STDOUT);
			}
			for my $j (0 .. $#title) {		# Data
			        ### for my $j ($headerCnt .. $#title) {		# Data
				if($title[$j] =~ /^\s*$/){
					die "ERROR: j=$j , title does not exist of $row[$j]\n";
				}
				print "2=> $j $row[$j]  max $#row , headerCnt $headerCnt\n";
				print "2=> $myhash $myhash->{$title[$j]}\n";
				print "=> DATA $j $title[$j]\n";
				$myhash->{$title[$j]} = removeSpace($row[$j]);
			}
		}
		hashTraverseSTDOUT();
	}
}

hashTraverseSTDOUT();

if($excel_version_input_file ne ""){
    my $tmpMajor = 0;
    my $tmpMinor = 0;
    my $tmpDev = 0;
    my $s = $gTitle{"VARIABLE"}{"Excel_Version"};
    $s =~ /\s*\D*\s*(\d+)\.(\d+)\.(\d+)\s*$/;
    $tmpMajor = $1;
    $tmpMinor = $2;
    $tmpDev = $3;
    print STDERR "EXCEL FILE VERSION $s => Major $tmpMajor, Minor $tmpMinor, Dev $tmpDev\n";
    if($excelVersionMajor != $tmpMajor) { versionMismatch(); exit(4); }
    if($excelVersionMinor != $tmpMinor) { versionMismatch(); exit(4); }
    if($excelVersionDev != $tmpDev) { versionMismatch(); exit(4); }
}

# $gPrintHashName{"LXID_ROTATE"} = "telltale of type ROTATE";
unlink $output_file;  # or warn "Could not unlink $output_file: $!";
traverse_tree_to_file(\%gTitle,"gTitle",">>",$output_file);
foreach my $key (sort keys %gTitle){
	traverse_tree_to_file(\%$key,"$key",">>",$output_file);
}

foreach $mn (keys %Module_Name){
    my $lmn = lc($mn) . ".modulename";
    open(my $mnfh, ">", $lmn)
     or die "Can't open > $lmn : $!";
    print $mnfh "$csv_file";
    close($mnfh);
}


$lg =  getpwuid($<) || getlogin || "Tidl";
#print STDERR "$lg\n";
my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
my $namePrefix = sprintf("%04d-%02d-%02d-%02d-%02d-%02d-%s-%s-%s", $year+1900, $mon+1, $mday, $hour, $min, $sec,$lg,$VARIABLE{Module_Name},$VARIABLE{Excel_Version});
$lloc =~ s/\/\s*$//;
$bn = basename($input_file);
open(my $infh, ">", $ldest);
print $infh "user,$lg\n";
print $infh "fullname,$namePrefix\n";
print $infh "inputfile,$input_file\n";
print $infh "basename,$bn\n";
print $infh "modulename,$VARIABLE{Module_Name}\n";
print $infh "excelversion,$VARIABLE{Excel_Version}\n";
print $infh "id,$lid\n";
print $infh "passwd,$lpasswd\n";
print $infh "host,$lhost\n";
print $infh "location,$lloc\n";
close($infh);
if($py == 0){
    $cmd = "sshpass -p '$lpasswd' scp $input_file $lid\@$lhost:$lloc/$namePrefix-$bn";
    print STDERR "$cmd START\n";
    system($cmd);
    print STDERR "$cmd END\n";
} else {        # python script for goh
    open(my $infh, "<", "run_command");
    $cmd = <$infh>;
    $cmd =~ s/[\s\n]*$//g;
    $cmd .= " $ldest";
    close($infh);
    print STDERR "$cmd START\n";
    system($cmd);
    print STDERR "$cmd END\n";
}

# END of main


sub versionMismatch {
	print STDERR "Version Mismatch between excel file and this git repository\n";
	print STDERR "1.check Excel_Version in excel file\n";
	print STDERR "2.check Version of excel in $excel_version_input_file file\n";
	print STDERR "Please update Excel version and CGA_RDL .\n";
    exit(4);
}

sub FixXML {
    $parm = $_[0];
    $parm =~ s/&amp;/&/g;
    $parm =~ s/&gt;/>/g;
    $parm =~ s/&lt;/</g;
    $parm =~ s/&quot;/"/g;
    $parm =~ s/&apos;/'/g;
    $parm =~ s/&#xA;/\n/g;
    $parm =~ s/&#xa;/\n/g;
    $parm =~ s/&#xD;/\r/g;
    $parm =~ s/&#xd;/\r/g;
    $parm =~ s/&#x9;/\t/g;
    return($parm);
}

sub checkHeader {
	my $str = shift;
	if($str =~ /^\[\s*(\w+)\s*\]/){
		if( ($1 ne "HEADER") and (uc($1) eq "HEADER") ){
			die "$1 -> HEADER :: should change into CAPITALIZATION";
		} 
	}
}

sub hashTraverseSTDOUT {
	print "===========TTTTTT==============start\n";
	traverse_hash_tree(\%gTitle,"gTitle","",STDOUT);
	foreach my $key (sort keys %gTitle){
		print "key: $key\n";
		traverse_hash_tree(\%$key,"$key","",STDOUT);
	}
	print "===========TTTTTT==============end\n";
}


sub removeSpace {
	my $s = shift;
	my $default = shift;
	$s =~ s/^\s*//g;
	$s =~ s/\s*$//g;
	$s =~ s/\r//g;
    #$s =~ s/ /_/g;
    $s = FixXML($s);
	return $s eq "" ? $default : $s;
}

sub sort_keys
{
	my $myhash = shift;
	my $direction = shift;
	my $allDigit = 1;
	foreach my $tmpKey (keys %{$myhash}){
		if( not ($tmpKey =~ /^\s*\d+\s*$/)){
			$allDigit = 0;
			last;
		}
	}
	if($allDigit == 1){
		if($direction eq "~"){
			return reverse sort {$a <=> $b} keys %{$myhash};
		} else {
			return sort {$a <=> $b} keys %{$myhash};
		}
	} else {
		if($direction eq "~"){
			return reverse sort keys %{$myhash};
		} else {
			return sort keys %{$myhash};
		}
	}
}

sub help 
{
	printf("Help :\n");
	printf("\t--input=[input excel file]\n");
	printf("\t\t  default input file name : $input_file\n");
	printf("\t--excel_version_input=[excel version input file]\n");
	printf("\t\t  default excel version of input file name : $excel_version_input_file\n");
	printf("\t\t  if null , we ignore the version between excel version input file and [VARIABLE]Excel_Version  in excel file ($input_file)\n");
	printf("\t--output=[output file with global variables]\n");
	printf("\t\t  default output file name : $output_file\n");
	printf("\t--csv_out=[csv output file]\n");
	printf("\t\t  default csv output file name : $csv_file\n");
	printf("\t--python\n");
	printf("\t\t  run command with python scripti to backup\n");
	printf("\t--destinfo=[input file name with info]\n");
	printf("\t\t  default : input_file_name\n");
	printf("\t--id=[host id ]\n");
	printf("\t\t  default : tidl\n");
	printf("\t--passwd=[passwd of id]\n");
	printf("\t\t  default : tidl1234\n");
	printf("\t--host=[host name]\n");
	printf("\t\t  default : abc.com\n");
	printf("\t--location=[warehouse location]\n");
	printf("\t\t  default : ~/warehouse/\n");
	printf("\t--help\n");
    #$a = "\"1,2,3,4\"\" , 5 67";
    #print $a . "\n";
    #$a =~ s/\"/\"\"/g;
    #print $a . "\n";
}
