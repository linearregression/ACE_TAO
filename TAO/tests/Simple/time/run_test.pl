#$Id$
# -*- perl -*-
eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

unshift @INC, '../../../../bin';
require ACEutils;

$iorfile = "time.ior";

$SV = Process::Create ("server$Process::EXE_EXT", "-o $iorfile ");

ACE::waitforfile ($iorfile);

$status  = system ("client$Process::EXE_EXT -f $iorfile -x");

$SV->Kill (); $SV->Wait ();

unlink $iorfile;

exit $status;
