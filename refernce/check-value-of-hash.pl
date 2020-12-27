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

our %pNull = {};
sub getHashRef {
	my ($name) = @_;        # gCan{9}
	my $first;
	#print "F " . $name . "\n";
	$name =~ s/([^\{]+)//;
	$first = $1;
	my $hn = \%{$first};
	#print "F " . $hn . "\n";
	while($name =~ s/^\{([^\}]*)\}//){
		my $uu = $1;
		$hn = $hn->{$uu};
		#print "G " . $uu . "   $hn\n";
	}

	#print "I " . $hn . "\n";
	if($hn == 0){
		return \%pNull;
	} else {
		return $hn;
	}
}

sub isDefinedHash {
	my ($name) = @_;        # gCan{9}
	my $first;
	print "F " . $name . "\n";
	$name =~ s/([^\{]+)//;
	$first = $1;
	my $hn = \%{$first};
	#print $hn;
	#print "\n";
	while($name =~ s/^\{([^\}]*)\}//){
		my $uu = $1;
		$hn = $hn->{$uu};
		print "G " . $uu;
		print $hn;
		print "\n";
		if($hn eq ""){
			return "";
		}
	}

	print "I ";
	print $hn;
	print "\n";
	if($hn == 0){
		return "";
	} else {
		return $hn;
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

if(not defined($macro{checked}) ){
	print "checked \$macro{checked} is undefined\n";
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
print isDefinedHash("macro{checked}");
print "\n";
print isDefinedHash("macro{checked}{0}");
print "\n";
$aa = "macro{defined}";
if(isDefinedHash($aa) eq ""){
	print "checked \$macro{defined} is null\n";
}
	foreach $mym (keys %macro){
		print_fp("3MACRO NAME : $mym\n",DBG,STDOUT);
		print_fp("3MACRO content : $macro{$mym}{content}\n",DBG,STDOUT);
		print_fp("3MACRO arg count : $macro{$mym}{paramcount}\n",DBG,STDOUT);
		foreach $mymn (sort {$a <=> $b}  keys %{$macro{$mym}{param}}){
			print_fp("3MACRO arg : $mymn\n",DBG,STDOUT);
			print_fp("3MACRO arg name : $macro{$mym}{param}{$mymn}{name}\n",DBG,STDOUT);
		}
	}

if($macro{checked}{0} eq ""){
	print "3MACRO checked \$macro{checked}{0} is null\n";
}
print "3MACRO ";
print isDefinedHash("macro{checked}");
print "\n";
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
		print_fp("4MACRO NAME : $mym\n",DBG,STDOUT);
		print_fp("4MACRO content : $macro{$mym}{content}\n",DBG,STDOUT);
		print_fp("4MACRO arg count : $macro{$mym}{paramcount}\n",DBG,STDOUT);
		foreach $mymn (sort {$a <=> $b}  keys %{$macro{$mym}{param}}){
			print_fp("4MACRO arg : $mymn\n",DBG,STDOUT);
			print_fp("4MACRO arg name : $macro{$mym}{param}{$mymn}{name}\n",DBG,STDOUT);
		}
	}

$aa = "macro{defined}";
if(isDefinedHash($aa) eq ""){
	print "checked \$macro{defined} is null\n";
}

$test{test}{0} = 3;
$aa = "test{test}{0}";
print "$aa is " . isDefinedHash($aa) . "\n";
$aa = "test{test}";
print "$aa is " . isDefinedHash($aa) . "\n";
$test{test}{2} = "";
$aa = "test{test}{2}";
print "$aa is " . isDefinedHash($aa) . "\n";
