// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_glob.h
 *
 *  pathname pattern-matching types
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_OS_GLOB_H
#define ACE_OS_INCLUDE_OS_GLOB_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/types.h"

#if !defined (ACE_LACKS_GLOB_H)
# include /**/ <glob.h>
#endif /* !ACE_LACKS_GLOB_H */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_OS_GLOB_H */
