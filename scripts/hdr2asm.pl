#!/usr/bin/env perl
use strict;
use warnings;

use Carp;
use Carp::Assert;

# Version (required by perlcritic --brutal)
our $VERSION = '0.1a';

sub main {

    my ($path) = @ARGV;

    assert( -e $path );

    open my $fd, '<', $path || croak 'Can\'t open file ' . $path;  # $OS_ERROR ?
    my @lines = <$fd>;
    close $fd || croak 'Can\'t close file ' . $path;

    foreach my $line (@lines) {
        my ( $name, $value, $comment ) =
          $line =~ m/^[#][\s]*define[\s]([\S]*)[\s]+([\S]*)(.*)$/sxm;

        if ( !$name eq '' && !$value eq ''  && !($comment =~ m/HDR2ASM_SKIP/sxm)) {

            # remove any kind of static typing
            $value =~ s/[uU]+[lL]*$//sxm;
            print "\t.equ " . $name . ', ' . $value . qq{\n};
        }
    }
}

# run
main;
