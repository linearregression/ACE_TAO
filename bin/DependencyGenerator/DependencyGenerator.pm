package DependencyGenerator;

# ************************************************************
# Description   : Runs the correct dependency generator on the file.
# Author        : Chad Elliott
# Create Date   : 2/10/2002
# ************************************************************

# ************************************************************
# Pragmas
# ************************************************************

use strict;

use Preprocessor;
use DependencyWriterFactory;

# ************************************************************
# Subroutine Section
# ************************************************************

sub new {
  my($class)    = shift;
  my($macros)   = shift;
  my($ipaths)   = shift;
  my($replace)  = shift;
  my($type)     = shift;
  my($noinline) = shift;
  my($self)     = bless {'pre'      => new Preprocessor($macros, $ipaths),
                         'replace'  => $replace,
                         'dwrite'   => DependencyWriterFactory::create($type),
                         'noinline' => $noinline,
                        }, $class;

  ## Set the current working directory, but
  ## escape regular expression special characters
  $self->{'cwd'} = Cwd::getcwd() . '/';
  $self->{'cwd'} =~ s/([\+\-\\\$\[\]\(\)\.])/\\$1/g;

  ## Sort the replace keys to get the longest key first.  This way
  ## when we are replacing portions of the file path, we replace the
  ## most we can.
  my(@repkeys) = sort { length($b) <=> length($a) } keys %$replace;
  $self->{'repkeys'} = \@repkeys;

  return $self;
}


sub process {
  my($self)    = shift;
  my($file)    = shift;
  my($objects) = shift;
  my($replace) = $self->{'replace'};
  my($cwd)     = $self->{'cwd'};

  ## Generate the dependency string
  my($depstr) = $self->{'dwrite'}->process(
                   $objects,
                   $self->{'pre'}->process($file, $self->{'noinline'}));

  ## Perform the replacements on the dependency string
  $depstr =~ s/$cwd//go;
  foreach my $rep (@{$self->{'repkeys'}}) {
    $depstr =~ s/$rep/$$replace{$rep}/g;
  }

  return $depstr;
}


1;
