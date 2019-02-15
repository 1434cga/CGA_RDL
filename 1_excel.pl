#!/usr/bin/perl

# help URL :  http://search.cpan.org/~hmbrand/Spreadsheet-Read/Read.pm

use 5.010;
use Spreadsheet::Read;
use File::Basename qw(dirname);
use Cwd  qw(abs_path);
use lib dirname(dirname abs_path $0) . '/perllib';
use lib abs_path . '/perllib';
use lib abs_path . '/../perllib';
use lib abs_path . '/../../perllib';
use lib abs_path . '/../../../perllib';
use MY::CHARLES qw(traverse_tree_to_file);
use MY::CHARLES qw(traverse_hash_tree);

use Getopt::Long;
my $input_file   = "./1_example.xlsx";
my $output_file = "./default.GV";
my $help=0;
my $verbose;
GetOptions ("output=s" => \$output_file,    # string
		"input=s"   => \$input_file,      # string
		"verbose|help"  => sub { $help = 1 })   # flag
or  die(help() . "Error in command line arguments\n");

if($help == 1){
	help();
	exit();
}

print "[" . abs_path . "]\n";

die "input file $input_file is not exist\n" unless (-e $input_file);

print "excel input file = $input_file /  output file = $output_file\n";

our %gTitle;

my $book = ReadData($input_file);

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
	my @rows = Spreadsheet::Read::rows($book->[1]);
	my @title;
	my $headerCnt = 0;
	my $titleCnt = 0;
	my $titleName = "";
	foreach my $i (1 .. scalar @rows) {
		print " $i  $rows[$i-1][0] \n";
		next if($rows[$i-1][0] =~ /^\s*$/) ;
		checkHeader($rows[$i-1][0]);
		if($rows[$i-1][0] =~ /^\s*\[HEADER\]\s*(\S*)/){
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
			for my $j ($hederCnt .. $#title) {		# Data
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

# $gPrintHashName{"LXID_ROTATE"} = "telltale of type ROTATE";
$file = "./default.GV";
unlink $file;  # or warn "Could not unlink $file: $!";
traverse_tree_to_file(\%gTitle,"gTitle",">>",$file);
foreach my $key (sort keys %gTitle){
	traverse_tree_to_file(\%$key,"$key",">>",$file);
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
	$s =~ s/ /_/g;
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
	printf("\t--output=[output file with global variables]\n");
	printf("\t\t  default output file name : $output_file\n");
	printf("\t--help\n");
}
