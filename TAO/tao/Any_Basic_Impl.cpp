// $Id$

#include "tao/Any_Basic_Impl.h"
#include "tao/TypeCode.h"
#include "tao/Any.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"

#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_string.h"

ACE_RCSID (tao,
           Any_Basic_Impl,
           "$Id$")

namespace TAO
{
  Any_Basic_Impl::Any_Basic_Impl (CORBA::TypeCode_ptr tc,
                                  void *value)
    : Any_Impl (0, tc),
      kind_ (CORBA::tk_null)
  {
    if (!CORBA::is_nil (tc))
      {
        ACE_DECLARE_NEW_CORBA_ENV;
        this->kind_ = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_CHECK;
      }

    CORBA::TCKind const tckind = static_cast<CORBA::TCKind> (this->kind_);

    switch (tckind)
    {
      case CORBA::tk_short:
        this->u_.s = *static_cast<CORBA::Short *> (value);
        break;
      case CORBA::tk_ushort:
        this->u_.us = *static_cast<CORBA::UShort *> (value);
        break;
      case CORBA::tk_long:
        this->u_.l = *static_cast<CORBA::Long *> (value);
        break;
      case CORBA::tk_ulong:
         this->u_.ul = *static_cast<CORBA::ULong *> (value);
        break;
      case CORBA::tk_float:
        this->u_.f = *static_cast<CORBA::Float *> (value);
        break;
      case CORBA::tk_double:
        this->u_.d = *static_cast<CORBA::Double *> (value);
        break;
      case CORBA::tk_boolean:
        this->u_.b = *static_cast<CORBA::Boolean *> (value);
        break;
      case CORBA::tk_char:
        this->u_.c = *static_cast<CORBA::Char *> (value);
        break;
      case CORBA::tk_octet:
        this->u_.o = *static_cast<CORBA::Octet *> (value);
        break;
      case CORBA::tk_longlong:
        this->u_.ll = *static_cast<CORBA::LongLong *> (value);
        break;
      case CORBA::tk_ulonglong:
        this->u_.ull = *static_cast<CORBA::ULongLong *> (value);
        break;
      case CORBA::tk_longdouble:
        this->u_.ld = *static_cast<CORBA::LongDouble *> (value);
        break;
      case CORBA::tk_wchar:
        this->u_.wc = *static_cast<CORBA::WChar *> (value);
        break;
      default:
        break;
    }
  }

  Any_Basic_Impl::~Any_Basic_Impl (void)
  {
  }

  void
  Any_Basic_Impl::insert (CORBA::Any &any,
                          CORBA::TypeCode_ptr tc,
                          const void *value)
  {
    Any_Basic_Impl *new_impl = 0;
    ACE_NEW (new_impl,
             Any_Basic_Impl (tc,
                             const_cast<void *> (value)));
    any.replace (new_impl);
  }

  CORBA::Boolean
  Any_Basic_Impl::extract (const CORBA::Any &any,
                           CORBA::TypeCode_ptr tc,
                           void *_tao_elem)
  {
    ACE_TRY_NEW_ENV
      {
        CORBA::TypeCode_ptr any_tc = any._tao_get_typecode ();
        CORBA::Boolean _tao_equiv =
          any_tc->equivalent (tc
                              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        if (_tao_equiv == 0)
          {
            return 0;
          }

        TAO::Any_Impl *impl = any.impl ();

        ACE_Message_Block *mb = impl->_tao_get_cdr ();

        if (mb == 0)
          {
            TAO::Any_Basic_Impl *narrow_impl =
              dynamic_cast<TAO::Any_Basic_Impl *> (impl);

            if (narrow_impl == 0)
              {
                return 0;
              }

            Any_Basic_Impl::assign_value (_tao_elem,
                                          narrow_impl);
            return 1;
          }

        TAO::Any_Basic_Impl *replacement =
          TAO::Any_Basic_Impl::create_empty (any_tc);

        auto_ptr<TAO::Any_Basic_Impl> replacement_safety (replacement);

        TAO_InputCDR cdr (mb->data_block (),
                          ACE_Message_Block::DONT_DELETE,
                          mb->rd_ptr () - mb->base (),
                          mb->wr_ptr () - mb->base (),
                          impl->_tao_byte_order (),
                          TAO_DEF_GIOP_MAJOR,
                          TAO_DEF_GIOP_MINOR);

        // Get the kind of the type where we are extracting in ie. the
        // aliased  type if there are any. Passing the aliased kind
        // will not help.
        CORBA::TCKind tck =
          tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;

        CORBA::Boolean result =
          replacement->demarshal_value (cdr,
                                        (CORBA::Long) tck);

        if (result == 1)
          {
            Any_Basic_Impl::assign_value (_tao_elem,
                                          replacement,
                                          tck);
            const_cast<CORBA::Any &> (any).replace (replacement);
            replacement_safety.release ();
            return 1;
          }

        // Duplicated by Any_Impl base class constructor.
        CORBA::release (any_tc);
      }
    ACE_CATCHANY
      {
      }
    ACE_ENDTRY;

    return 0;
  }

  CORBA::Boolean
  Any_Basic_Impl::marshal_value (TAO_OutputCDR &cdr)
  {
    CORBA::TCKind tckind = static_cast<CORBA::TCKind> (this->kind_);

    switch (tckind)
    {
      case CORBA::tk_short:
        return cdr << this->u_.s;
      case CORBA::tk_ushort:
        return cdr << this->u_.us;
      case CORBA::tk_long:
        return cdr << this->u_.l;
      case CORBA::tk_ulong:
        return cdr << this->u_.ul;
      case CORBA::tk_float:
        return cdr << this->u_.f;
      case CORBA::tk_double:
        return cdr << this->u_.d;
      case CORBA::tk_boolean:
        return cdr << CORBA::Any::from_boolean (this->u_.b);
      case CORBA::tk_char:
        return cdr << CORBA::Any::from_char (this->u_.c);
      case CORBA::tk_octet:
        return cdr << CORBA::Any::from_octet (this->u_.o);
      case CORBA::tk_longlong:
        return cdr << this->u_.ll;
      case CORBA::tk_ulonglong:
        return cdr << this->u_.ull;
      case CORBA::tk_longdouble:
        return cdr << this->u_.ld;
      case CORBA::tk_wchar:
        return cdr << CORBA::Any::from_wchar (this->u_.wc);
      default:
        return 0;
    }
  }

  CORBA::Boolean
  Any_Basic_Impl::demarshal_value (TAO_InputCDR &cdr)
  {
    return this->demarshal_value (cdr,
                                  this->kind_);
  }

  CORBA::Boolean
  Any_Basic_Impl::demarshal_value (TAO_InputCDR &cdr,
                                   CORBA::Long tck)
  {
    CORBA::TCKind const tckind = static_cast<CORBA::TCKind> (tck);
    switch (tckind)
    {
      case CORBA::tk_short:
        return cdr >> this->u_.s;
      case CORBA::tk_ushort:
        return cdr >> this->u_.us;
      case CORBA::tk_long:
        return cdr >> this->u_.l;
      case CORBA::tk_ulong:
        return cdr >> this->u_.ul;
      case CORBA::tk_float:
        return cdr >> this->u_.f;
      case CORBA::tk_double:
        return cdr >> this->u_.d;
      case CORBA::tk_boolean:
        return cdr >> CORBA::Any::to_boolean (this->u_.b);
      case CORBA::tk_char:
        return cdr >> CORBA::Any::to_char (this->u_.c);
      case CORBA::tk_octet:
        return cdr >> CORBA::Any::to_octet (this->u_.o);
      case CORBA::tk_longlong:
        return cdr >> this->u_.ll;
      case CORBA::tk_ulonglong:
        return cdr >> this->u_.ull;
      case CORBA::tk_longdouble:
        return cdr >> this->u_.ld;
      case CORBA::tk_wchar:
        return cdr >> CORBA::Any::to_wchar (this->u_.wc);
      default:
        return 0;
    }
  }

  void
  Any_Basic_Impl::_tao_decode (TAO_InputCDR &cdr
                               ACE_ENV_ARG_DECL)
  {
    if (! this->demarshal_value (cdr))
      {
        ACE_THROW (CORBA::MARSHAL ());
      }
  }

  Any_Basic_Impl *
  Any_Basic_Impl::create_empty (CORBA::TypeCode_ptr tc)
  {
    ACE_DECLARE_NEW_CORBA_ENV;
    CORBA::TCKind const kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);

    TAO::Any_Basic_Impl * retval = 0;

    switch (kind)
    {
      case CORBA::tk_longlong:
        {
          CORBA::LongLong tmp = ACE_CDR_LONGLONG_INITIALIZER;
          ACE_NEW_RETURN (retval,
                          TAO::Any_Basic_Impl (tc, &tmp),
                          0);
        }

        break;
      case CORBA::tk_longdouble:
        {
          CORBA::LongDouble tmp = ACE_CDR_LONG_DOUBLE_INITIALIZER;
          ACE_NEW_RETURN (retval,
                          TAO::Any_Basic_Impl (tc, &tmp),
                          0);
        }

        break;
      default:
        {
          CORBA::ULongLong tmp = 0;
          ACE_NEW_RETURN (retval,
                          TAO::Any_Basic_Impl (tc, &tmp),
                          0);
        }

        break;
    }

    return retval;
  }

  void
  Any_Basic_Impl::assign_value (void *dest, Any_Basic_Impl *src)
  {
    Any_Basic_Impl::assign_value (dest,
                                  src,
                                  src->kind_);
  }

  void
  Any_Basic_Impl::assign_value (void *dest,
                                Any_Basic_Impl *src,
                                CORBA::Long tck)
  {
    CORBA::TCKind const kind = static_cast<CORBA::TCKind> (tck);

    switch (kind)
      {
      case CORBA::tk_short:
        *static_cast<CORBA::Short *> (dest) = src->u_.s;
        break;
      case CORBA::tk_ushort:
        *static_cast<CORBA::UShort *> (dest) = src->u_.us;
        break;
      case CORBA::tk_long:
        *static_cast<CORBA::Long *> (dest) = src->u_.l;
        break;
      case CORBA::tk_ulong:
        *static_cast<CORBA::ULong *> (dest) = src->u_.ul;
        break;
      case CORBA::tk_float:
        *static_cast<CORBA::Float *> (dest) = src->u_.f;
        break;
      case CORBA::tk_double:
        *static_cast<CORBA::Double *> (dest) = src->u_.d;
        break;
      case CORBA::tk_boolean:
        *static_cast<CORBA::Boolean *> (dest) = src->u_.b;
        break;
      case CORBA::tk_char:
        *static_cast<CORBA::Char *> (dest) = src->u_.c;
        break;
      case CORBA::tk_octet:
        *static_cast<CORBA::Octet *> (dest) = src->u_.o;
        break;
      case CORBA::tk_longlong:
        *static_cast<CORBA::LongLong *> (dest) = src->u_.ll;
        break;
      case CORBA::tk_ulonglong:
        *static_cast<CORBA::ULongLong *> (dest) = src->u_.ull;
        break;
      case CORBA::tk_longdouble:
        *static_cast<CORBA::LongDouble *> (dest) = src->u_.ld;
        break;
      case CORBA::tk_wchar:
        *static_cast<CORBA::WChar *> (dest) = src->u_.wc;
        break;
      default:
        break;
      }
  }
}
