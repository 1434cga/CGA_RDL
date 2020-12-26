#!/usr/bin/perl

# help URL :  http://search.cpan.org/~hmbrand/Spreadsheet-Read/Read.pm

use 5.010;
use File::Basename qw(dirname);
use Cwd  qw(abs_path);
use lib dirname(dirname abs_path $0) . '/perllib';
use lib abs_path . '/perllib';
use lib abs_path . '/../perllib';
use lib abs_path . '/../../perllib';
use lib abs_path . '/../../../perllib';

use Getopt::Long;
my $input_file   = "./1_example.xlsx";
my $output_file = "./default.GV";
my $help=0;
my $verbose;

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

our %macro;

	foreach $mym (keys %macro){
		print_fp("1MACRO NAME : $mym\n",DBG,STDOUT);
		print_fp("1MACRO content : $macro{$mym}{content}\n",DBG,STDOUT);
		print_fp("1MACRO arg count : $macro{$mym}{paramcount}\n",DBG,STDOUT);
		foreach $mymn (sort {$a <=> $b}  keys %{$macro{$mym}{param}}){
			print_fp("1MACRO arg : $mymn\n",DBG,STDOUT);
			print_fp("1MACRO arg name : $macro{$mym}{param}{$mymn}{name}\n",DBG,STDOUT);
		}
	}

if($macro{checked}{0} eq ""){
	print "checked \$macro{checked}{0} is null\n";
}
# 위에서 checked,0 을 물으면서 {checked}는 초기화가 되어져있는 것이다. (자동으로)
if($macro{checked} eq ""){
	print "checked \$macro{checked} is null\n";
}
if(not defined($macro{defined}{0})){
	print "defined \$macro{defined}{0} is null\n";
}
if(not exists($macro{"exists"}{0})){
	print "exists \$macro{exists}{0} is null\n";
}

	foreach $mym (keys %macro){
		print_fp("2MACRO NAME : $mym\n",DBG,STDOUT);
		print_fp("2MACRO content : $macro{$mym}{content}\n",DBG,STDOUT);
		print_fp("2MACRO arg count : $macro{$mym}{paramcount}\n",DBG,STDOUT);
		foreach $mymn (sort {$a <=> $b}  keys %{$macro{$mym}{param}}){
			print_fp("2MACRO arg : $mymn\n",DBG,STDOUT);
			print_fp("2MACRO arg name : $macro{$mym}{param}{$mymn}{name}\n",DBG,STDOUT);
		}
	}
