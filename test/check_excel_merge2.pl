use Spreadsheet::ParseExcel;   # 2003 excel

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
        }
    }
	print("===hash===\n");
	for $key (keys %$WS){
		print($key . "=" . $WS->{$key} . "\n");
	}
	print("===MergedArea===\n");
	for my $area ( @{ $WS->{MergedArea} } ){
		print($area  . "\n");   # ARRAY...
		print( @{ $area } . "\n");  # count
		print($area->[0] . "  ");
		print($area->[1] . "  ");
		print($area->[2] . "  ");
		print($area->[3] . "\n");
	}
}


