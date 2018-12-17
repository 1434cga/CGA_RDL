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
	my $titleCnt = 0;
	my $titleValueIndex = -1;
	my $titleName = "";
	foreach my $i (1 .. scalar @rows) {
		print " $i  $rows[$i-1][0] \n";
		next if($rows[$i-1][0] =~ /^\s*$/) ;
		if($rows[$i-1][0] =~ /^\s*\[HEADER\]\s*(\S*)/){
			$titleCnt = 0;
			$titleValueIndex = -1;
			delete @title[0 .. $#title];
			say "$1";
			$titleName = $1;
			$title[$titleCnt] = $1;
			$gTitle{$titleName}{$titleCnt} = $title[$titleCnt];
			my $myhash = \%{$titleName};
			print "AAA $titleName : $$titleName myhash :$myhash\n";
			#foreach my $key (keys %Manager_Name){ print "MN: $key  "; }
			#print "\n";
			$titleCnt++;
			my @row = Spreadsheet::Read::row($book->[1], $i);
			for my $j (1 .. $#row) {
				next if($row[$j] =~ /^\s*$/) ;
				#say "$book->[1]{label} sheet Header:" . chr(65+$j) . (1) . ' ' . ($row[$j] // '');
				$title[$titleCnt] = removeSpace($row[$j]);
				if($title[$titleCnt] =~ /\[VALUE\]/){
					$titleValueIndex = $titleCnt;
				}
				$gTitle{$titleName}{$titleCnt} = $title[$titleCnt];
				$titleCnt++;
			}
			#$gPrintHashName{$title[0]} = "Title $i Row";
			for my $j (0 .. $#title) {
				say "sheet Header:$j " . chr(65+$j) . ' ' . ($title[$j] // '');
			}
		} elsif( not ($rows[$i-1][0] =~ /^\s*$/) ){		# must have contents in firt column
			#my $details = getDetails(3, \%{ $D{classes}{$classes}{public_methods}{members}{$members}{detailed}{doc} });
				#my $myhash = shift;
				#foreach my $tmpKey (sort_keys(\%{$myhash})){
					#print "getDetails type [" . $myhash->{$tmpKey}{type} . "]\n";
			my @row = Spreadsheet::Read::row($book->[1], $i);
			my $myValue = ($titleValueIndex < 0) ? "" : $row[$titleValueIndex];
			say "ROW[$i]  @row : [$titleValueIndex] $myValue";
			my $myhash = \%{ $titleName };
			print "BBB myhash $titleName : $myhash\n";
			for my $j (0 .. $#row) {
				print "=> $j $row[$j]  max $#row\n";
				print "=> $myhash $myhash->{$row[$j]}\n";
				#next if($row[$j] =~ /^\s*$/) ;
				if( ($j+1) == $titleValueIndex){
					print "=> LAST $j $row[$j] ValueIndex=$titleValueIndex\n";
					$myhash->{$row[$j]} = $row[$j+1];
					last;
				} elsif($j == $#row){
					print "=> END $j $row[$j]\n";
					$myhash->{$row[$j]} = "";
					last;
				}
				unless (exists $myhash->{$row[$j]}){
					$myhash->{$row[$j]} = {};
					print "=> unless $myhash $myhash->{$row[$j]}\n";
				}
				say "$book->[1]{label} sheet Header:" . chr(65+$j) . (1) . ' ' . ($row[$j] // '');
				$myhash = \%{ $myhash->{$row[$j]} };
				#traverse_hash_tree(\%_G_,"_G_","",STDOUT);
			}
		}
	}
}
print "===KKK1\n";
traverse_hash_tree(\%gTitle,"gTitle","",STDOUT);
foreach my $key (sort keys %gTitle){
	print "key: $key\n";
	traverse_hash_tree(\%$key,"$key","",STDOUT);
}

# $gPrintHashName{"LXID_ROTATE"} = "telltale of type ROTATE";
$file = "./default.GV";
unlink $file or warn "Could not unlink $file: $!";
traverse_tree_to_file(\%gTitle,"gTitle",">>",$file);
foreach my $key (sort keys %gTitle){
	traverse_tree_to_file(\%$key,"$key",">>",$file);
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
