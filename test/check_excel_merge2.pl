use Spreadsheet::ParseExcel;   # 2003 excel

our %data;
my $p = Spreadsheet::ParseExcel->new();
my $WB = $p->parse("./Workbook1.xls");
if (!defined $WB) {
    die $p->error(), ".\n";
}
for my $WS ($WB->worksheets()) {
    my ( $row_min, $row_max ) = $WS->row_range();
    my ( $col_min, $col_max ) = $WS->col_range();

    for my $row ( $row_min .. $row_max ) {
        for my $col ( $col_min .. $col_max ) {

            my $cell = $WS->get_cell( $row, $col );
            next unless $cell;

            print "Row, Col    = ($row, $col)\n";
            print "Value       = ", $cell->value(),       "\n";
            print "Unformatted = ", $cell->unformatted(), "\n";
            print "\n";
			$data{$row}{$col} = $cell->value();
        }
    }
	print("===hash===\n");
	for $key (keys %$WS){
		print($key . "=" . $WS->{$key} . "\n");
	}
	print("===origin data===\n");
	for my $i (sort keys %data){
		for my $j (sort keys %{ $data{$i} }){
			print("[$i:$j:$data{$i}{$j}]");
		}
		print("\n");
	}
	print("===MergedArea===\n");
	for my $area ( @{ $WS->{MergedArea} } ){
		print($area  . "\n");   # ARRAY...
		print( @{ $area } . "\n");  # count
		print($area->[0] . "  ");
		print($area->[1] . "  (" . $data{$area->[0]}{$area->[1]} . ")  ");
		print($area->[2] . "  ");
		print($area->[3] . "  (" . $data{$area->[2]}{$area->[3]} . ")\n");
		my $v = $data{$area->[0]}{$area->[1]};
		for my $i ($area->[0] .. $area->[2]){
			for my $j ($area->[1] .. $area->[3]){
				$data{$i}{$j} = $v;
			}
		}

	}
}

print("===merged data===\n");
for my $i (sort keys %data){
	for my $j (sort keys %{ $data{$i} }){
		print("[$i:$j:$data{$i}{$j}]");
	}
	print("\n");
}


