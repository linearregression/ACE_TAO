// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_sem.h
 *
 *  XSI semaphore facility
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_OS_SEM_H
#define ACE_OS_INCLUDE_SYS_OS_SEM_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/ipc.h"

#if !defined (ACE_LACKS_SYS_SEM_H)
#  include /**/ <sys/sem.h>
#endif /* !ACE_LACKS_SYS_SEM_H */

#if defined (ACE_LACKS_SEMBUF_T)
   struct sembuf
   {
     /// semaphore #
     unsigned short sem_num;

     /// semaphore operation
     short sem_op;

     /// operation flags
     short sem_flg;
   };
#endif /* ACE_LACKS_SEMBUF_T */

#if !defined (ACE_HAS_SEMUN) || (defined (__GLIBC__) && defined (_SEM_SEMUN_UNDEFINED))
   union semun
   {
     /// value for SETVAL
     int val;
     /// buffer for IPC_STAT & IPC_SET
     struct semid_ds *buf;
     /// array for GETALL & SETALL
     u_short *array;
   };
#endif /* !ACE_HAS_SEMUN || (defined (__GLIBC__) && defined (_SEM_SEMUN_UNDEFINED)) */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_OS_SEM_H */
