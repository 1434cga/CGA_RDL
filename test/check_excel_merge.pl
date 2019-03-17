#!/usr/bin/perl

# help URL :  http://search.cpan.org/~hmbrand/Spreadsheet-Read/Read.pm

use 5.010;
use Spreadsheet::Read;
use File::Basename qw(dirname);
use Cwd  qw(abs_path);
use lib dirname(dirname abs_path $0) . '/perllib';
use lib abs_path . '/CGA_RDL/perllib';
use lib abs_path . '/perllib';
use lib abs_path . '/../perllib';
use lib abs_path . '/../../perllib';
use lib abs_path . '/../../../perllib';
use MY::CHARLES qw(traverse_tree_to_file);
use MY::CHARLES qw(traverse_hash_tree);

use Getopt::Long;
my $input_file   = "./Workbook1.xlsx";
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

my $ss = ReadData($input_file,attr => 1, merged => 1)->[1];
#my $book = ReadData($input_file);

print("===hash===\n");
for $key (keys %$ss){
	print($key . "=" . $ss->{$key} . "\n");
}
print("===attr===\n");
for my $attr ( @{ $ss->{attr} }){
	print(@{$attr} . "  \n");
}
print("===merged===\n");
for my $area ( @{ $ss->{merged} }){
	print($area  . "\n");   # ARRAY...
	print(@{ $area } . "\n");
	print($area->[0] . "  ");
	print($area->[1] . "  ");
	print($area->[2] . "  ");
	print($area->[3] . "  \n");
}

exit;

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
	for my $i (0 .. $#row) {
		say "$book->[1]{label} sheet Header:" . chr(65+$i) . (1) . ' ' . ($row[$i] // '');
	}
	my @rows = Spreadsheet::Read::rows($book->[1]);
	my @title;
	my $headerCnt = 0;
	my $titleCnt = 0;
	my $titleName = "";
	foreach my $i (0 .. scalar @rows) {
		print " $i  $rows[$i][0] \n";
		my @row = Spreadsheet::Read::row($book->[1], $i);
		for my $j (0 .. $#row) {
			print "$i $j [" . $row[$j] . "]\n";
		}
	}
}

#hashTraverseSTDOUT();

exit;

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
