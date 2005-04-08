// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// TAO_IDL - Generated from
// be\be_codegen.cpp:291


#include "ServantLocatorC.h"
#include "tao/CDR.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_string.h"

#if defined (__BORLANDC__)
#pragma option -w-rvl -w-rch -w-ccc -w-aus -w-sig
#endif /* __BORLANDC__ */

// TAO_IDL - Generated from
// be\be_visitor_arg_traits.cpp:69

#if (TAO_HAS_MINIMUM_POA == 0)

// Arg traits specializations.
namespace TAO
{
}


// TAO_IDL - Generated from
// be\be_visitor_interface/interface_cs.cpp:60

// Traits specializations for PortableServer::ServantLocator.

PortableServer::ServantLocator_ptr
TAO::Objref_Traits<PortableServer::ServantLocator>::duplicate (
    PortableServer::ServantLocator_ptr p
  )
{
  return PortableServer::ServantLocator::_duplicate (p);
}

void
TAO::Objref_Traits<PortableServer::ServantLocator>::release (
    PortableServer::ServantLocator_ptr p
  )
{
  CORBA::release (p);
}

PortableServer::ServantLocator_ptr
TAO::Objref_Traits<PortableServer::ServantLocator>::nil (void)
{
  return PortableServer::ServantLocator::_nil ();
}

CORBA::Boolean
TAO::Objref_Traits<PortableServer::ServantLocator>::marshal (
    PortableServer::ServantLocator_ptr p,
    TAO_OutputCDR & cdr
  )
{
  return CORBA::Object::marshal (p, cdr);
}

PortableServer::ServantLocator::ServantLocator (void)
{}

PortableServer::ServantLocator::~ServantLocator (void)
{}

void 
PortableServer::ServantLocator::_tao_any_destructor (void *_tao_void_pointer)
{
  ServantLocator *_tao_tmp_pointer =
    static_cast<ServantLocator *> (_tao_void_pointer);
  CORBA::release (_tao_tmp_pointer);
}

PortableServer::ServantLocator_ptr
PortableServer::ServantLocator::_narrow (
    CORBA::Object_ptr _tao_objref
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  if (CORBA::is_nil (_tao_objref))
    {
      return ServantLocator::_nil ();
    }
  
  ServantLocator_ptr proxy =
    dynamic_cast<ServantLocator_ptr> (_tao_objref);
  
  return ServantLocator::_duplicate (proxy);
}

PortableServer::ServantLocator_ptr
PortableServer::ServantLocator::_unchecked_narrow (
    CORBA::Object_ptr _tao_objref
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  if (CORBA::is_nil (_tao_objref))
    {
      return ServantLocator::_nil ();
    }
  
  ServantLocator_ptr proxy =
    dynamic_cast<ServantLocator_ptr> (_tao_objref);
  
  return ServantLocator::_duplicate (proxy);
}

PortableServer::ServantLocator_ptr
PortableServer::ServantLocator::_duplicate (ServantLocator_ptr obj)
{
  if (! CORBA::is_nil (obj))
    {
      obj->_add_ref ();
    }
  
  return obj;
}

void
PortableServer::ServantLocator::_tao_release (ServantLocator_ptr obj)
{
  CORBA::release (obj);
}

CORBA::Boolean
PortableServer::ServantLocator::_is_a (
    const char *value
    ACE_ENV_ARG_DECL_NOT_USED
  )
{
  if (
      !ACE_OS::strcmp (
          value,
          "IDL:omg.org/PortableServer/ServantManager:1.0"
        ) ||
      !ACE_OS::strcmp (
          value,
          "IDL:omg.org/PortableServer/ServantLocator:2.3"
        ) ||
      !ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/LocalObject:1.0"
        ) ||
      !ACE_OS::strcmp (
          value,
          "IDL:omg.org/CORBA/Object:1.0"
        )
    )
    {
      return true; // success using local knowledge
    }
  else
    {
      return false;
    }
}

const char* PortableServer::ServantLocator::_interface_repository_id (void) const
{
  return "IDL:omg.org/PortableServer/ServantLocator:2.3";
}

CORBA::Boolean
PortableServer::ServantLocator::marshal (TAO_OutputCDR &)
{
  return false;
}

// TAO_IDL - Generated from
// be\be_visitor_root/root.cpp:1528

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class
    TAO::Objref_Traits<
        PortableServer::ServantLocator
      >;

  template class
    TAO_Objref_Var_T<
        PortableServer::ServantLocator
      >;
  
  template class
    TAO_Objref_Out_T<
        PortableServer::ServantLocator
      >;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate \
    TAO::Objref_Traits< \
        PortableServer::ServantLocator \
      >

# pragma instantiate \
    TAO_Objref_Var_T< \
        PortableServer::ServantLocator
      >
  
# pragma instantiate \
    TAO_Objref_Out_T< \
        PortableServer::ServantLocator
      >

#endif /* !ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */ 

#endif /* TAO_HAS_MINIMUM_CORBA == 0 */
