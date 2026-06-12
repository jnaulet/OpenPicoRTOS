#!/usr/bin/env perl
use strict;
use warnings;

use Carp;
use Carp::Assert;

# Version (required by perlcritic --brutal)
our $VERSION = '0.1a';

sub file_to_array {
    my $path = shift;

    assert( -e $path );

    open my $fd, '<', $path || croak 'Can\'t open file ' . $path;  # $OS_ERROR ?
    my @lines = <$fd>;
    close $fd || croak 'Can\'t close file ' . $path;

    return @lines;
}

use Readonly;
Readonly my $IDLE  => 0;
Readonly my $TOKEN => 1;

sub main {

    my ($path) = @ARGV;
    my $state = $IDLE;

    assert( -e $path );

    my @lines = file_to_array($path);
    foreach my $line (@lines) {

        # IDLE
        if ( $line =~ /^\/[*]{2}/sxm ) {
            $state = $TOKEN;
            my ($opt) = $line =~ m/^\/[*]{2}(.*)$/sxm;
            if ( $opt =~ /§/sxm ) {
                print qq{\n} || return;
            }
            else {
                print '---' . qq{\n} || return;
            }
            next;
        }

        # TOKEN
        if ( $state == $TOKEN ) {
            if ( $line =~ /[*]\//sxm ) {
                $state = $IDLE;
                print qq{\n} || return;
                next;
            }

            my ($curline) = $line =~ m/[\s]*[*]+[\s]?(.*)$/sxm;
            print( length($curline) == 0 ? qq{\n} : $curline ) || return;
        }
    }

    return;
}

# run
main;
