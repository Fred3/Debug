#!/usr/bin/perl

use strict;

my %counts0;
my %counts1;

while(1) {

    print "\033[2J";    #clear the screen
    print "\033[0;0H";  #jump to 0,0
    print "CPU0 rate, CPU1 rate, name\n\n";

    open(my $IRQFILE, "</proc/interrupts")
	or die "Can't open /proc/interrupts";

    while(<$IRQFILE>) {
	if(/([^:]+):\s+(\d+)\s+(\d+)\s+(.+)/) {

	    if(not defined $counts0{$1}) {

#		print "New IRQ $1 : $4\n";
		$counts0{$1} = $2;
		$counts1{$1} = $3;

	    } elsif($2 or $3) {

		printf("%8d %8d : %s\n", $2-$counts0{$1},  $3-$counts1{$1}, $4);
		$counts0{$1} = $2;
		$counts1{$1} = $3;
	    }

#	    print "IRQ $1 CPU0 $2 CPU1 $3 Name $4\n";
	}

    }
    close $IRQFILE;
    sleep(1);

}
