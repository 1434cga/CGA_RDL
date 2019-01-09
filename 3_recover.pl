#!/usr/bin/perl

use 5.010;
use File::Basename qw(dirname);
use Cwd  qw(abs_path);
use lib dirname(dirname abs_path $0) . '/perllib';
use lib abs_path . '/perllib';
use lib abs_path . '/../perllib';
use MY::CHARLES qw(traverse_tree_to_file);
use MY::CHARLES qw(traverse_hash_tree);

use Getopt::Long;
my $workingFile   = "./3_working.cpp";
my $templateFile   = "./3_template.cpp";
my $mergeFile = "./3_merge.cpp";
my $help=0;
my $verbose;
GetOptions ("working|changed=s" => \$workingFile,    # string
		"template=s"   => \$templateFile,      # string
		"merge=s"   => \$mergeFile,      # string
		"verbose|help"  => sub { $help = 1 })   # flag
or  die(help() . "Error in command line arguments\n");

if($help == 1){
	help();
	exit();
}


print "[IN]  working file => $workingFile\n";
print "[IN]  template file => $templateFile\n";
print "[OUT] merge file => $mergeFile\n";

die help() . "input working file $workingFile is not exist\n" unless (-e $workingFile);
die help() . "input merge file $templateFile is not exist\n" unless (-e $templateFile);
die help() . "Must have different file name among input and output files.\n" if ($workingFile eq $mergeFile);
die help() . "Must have different file name among input and output files.\n" if ($templateFile eq $mergeFile);

our %gBlock;
open(INPUTWORK , "<$workingFile");
my $myBlockData = "";
my $myBlockName = "";
my $myFlag = 0;
my $myLineCount = 0;
while($in = <INPUTWORK>){
	$myLineCount++;
	if($in =~ /^\s*\/\/\@\s*(CGA_VARIANT\s*:.*)\s*start\s*$/){
		die "[WORK] $myLineCount:$in $myBlockName is not finished. (no end)\n" unless ($myFlag == 0);
		$myFlag = 1;
		$myBlockData = "";
		$myBlockName = removeSpace($1);
		print "v1 $myBlockName START\n";
		die "[WORK] $myLineCount:$in $myBlockName already existed.\n" unless ($gBlock{$myBlockName} eq "");
	} elsif($in =~ /^\s*\/\/\@\s*(CGA_VARIANT\s*:.*)\s*end\s*$/){
		my $myMatch = removeSpace($1);
		print "v2 $myMatch [$myBlockName] END\n";
		if($myBlockName ne $myMatch){
			$myBlockData .= $in;
			die "[WORK] $myLineCount:$in Block($myBlockName) is not started. (no start)\n" unless ($myFlag == 1);
		} else {
			die "[WORK] $myLineCount:$in Block($myBlockName) is not started. (no start)\n" unless ($myFlag == 1);
			$gBlock{$myBlockName} = $myBlockData;
			print "v2 $myBlockName END\n";
			print "v2 $myBlockData";
			$myFlag = 0;
			$myBlockName = "";
			$myBlockData = "";
		}
	} else {
		$myBlockData .= $in;
	}
};

die "[WORK] End of File : $myBlockName is not finished. (no end)\n" unless ($myFlag == 0);

foreach my $key (sort keys %gBlock){
	print "\n||| [gBlock->$key]\n";
	print "==> [$gBlock{$key}]";
}

close(INPUTWORK);

open(TEMPLATE, "<$templateFile");
open(MERGE, ">$mergeFile");
$myBlockData = "";
$myBlockName = "";
$myFlag = 0;
$myLineCount = 0;
while($in = <TEMPLATE>){
	$myLineCount++;
	if($in =~ /^\s*\/\/\@\s*(CGA_VARIANT\s*:.*)\s*start\s*$/){
		die "[TEMPLATE] $myLineCount:$in $myBlockName is not finished. (no end)\n" unless ($myFlag == 0);
		$myFlag = 1;
		$myBlockData = "";
		$myBlockName = removeSpace($1);
		print "v3 $myBlockName START\n";
		print MERGE $in;
	} elsif($in =~ /^\s*\/\/\@\s*(CGA_VARIANT\s*:.*)\s*end\s*$/){
		my $myMatch = removeSpace($1);
		print "v4 $myMatch [$myBlockName] END\n";
		if($myBlockName ne $myMatch){
			$myBlockData .= $in;
			die "[TEMPLATE] $myLineCount:$in Block($myBlockName) is not started. (no start)\n" unless ($myFlag == 1);
		} else {
			die "[TEMPLATE] $myLineCount:$in Block($myBlockName) is not started. (no start)\n" unless ($myFlag == 1);
			print "v4 [$myBlockName] END\n";
			print "v4 [$myBlockData]\n";
			print "v4 [$gBlock{$myBlockName}]\n";
			if($gBlock{$myBlockName} eq ""){
				print MERGE "$myBlockData";
			} else {
				print MERGE "#if 0\n";
				print MERGE "$myBlockData";
				print MERGE "#endif //if 0\n";
				print MERGE "$gBlock{$myBlockName}";
			}
			print MERGE $in;
			$myFlag = 0;
			$myBlockName = "";
			$myBlockData = "";
		}
	} else {
		$myBlockData .= $in;
		if($myFlag == 0){
			print MERGE $in;
		}
	}
};

die "[TEMPLATE] End of File : $myBlockName is not finished. (no end)\n" unless ($myFlag == 0);

close(TEMPLATE);
close(MERGE);



sub removeSpace {
	my $s = shift;
	my $default = shift;
	$s =~ s/^\s*//g;
	$s =~ s/\s*$//g;
	$s =~ s/ /_/g;
	return $s eq "" ? $default : $s;
}

sub help 
{
	printf("Help :\n");
	printf("\tperl 3_recover.pl --working|changed=[working file] --template=[template file] --merge=[new merged file]\n");
	printf("\t\t: Merge [working contents] into [new merged file] from [template file]\n");
	printf("\t--working=[input working file]\n");
	printf("\t--changed=[input changed file]\n");
	printf("\t--template=[input template file]\n");
	printf("\t--merge=[output merge file]\n");
	printf("\t--help|verbose\n");
}