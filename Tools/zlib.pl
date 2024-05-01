#!/usr/bin/perl
#
# Perl script (zlib.pl) to inflate and deflate AppleHDA resource files in Mountain Lion DP3 and greater.
#
# Version 0.1 - Copyright (c) 2012 by RevoGirl <DutchHockeyGoalie@yahoo.com>
#

use strict;
use warnings;

use Compress::Zlib;

my $data = '';
my ($output, $status);

binmode STDOUT;

sub inflate {
	my $x = inflateInit() or die "Cannot create a inflation stream\n";

	if (open(FILE, $ARGV[1])) {
		binmode FILE;

		while (read(FILE, $data, 4096)) {
			($output, $status) = $x->inflate(\$data);

			print $output if $status == Z_OK or $status == Z_STREAM_END;

			last if $status != Z_OK;
		}

		close (FILE);

		die "inflation failed\n" unless $status == Z_STREAM_END;
	}
}

sub deflate {
	my $x = deflateInit(-Level => Z_BEST_COMPRESSION) or die "Cannot create a deflation stream\n";

	if (open(FILE, $ARGV[1])) {
		binmode FILE;

		while (read(FILE, $data, 4096)) {
			$data =~ s/[\n\t]*//g;
			($output, $status) = $x->deflate(\$data);

			$status == Z_OK or die "deflation failed\n";

			print $output;
		}
	
		($output, $status) = $x->flush();
	
		$status == Z_OK or die "deflation failed\n";

		print $output;
	}
}

sub main() {
	if ($ARGV[0] eq "inflate") {
		inflate()
	}
	
	if ($ARGV[0] eq "deflate") {
		deflate()
	}
}

main();
exit(0);
