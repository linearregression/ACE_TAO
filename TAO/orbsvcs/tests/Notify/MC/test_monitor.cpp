// $Id$

#include "MonitorTestInterfaceS.h"
#include "orbsvcs/Notify/MonitorControl/NotificationServiceMCC.h"
#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtC.h"
#include "ace/Get_Opt.h"

class MonitorTestInterface_i: public virtual POA_MonitorTestInterface
{
public:
  MonitorTestInterface_i (
    CosNotification::NotificationServiceMonitorControl_ptr nsm)
   : nsm_ (CosNotification::NotificationServiceMonitorControl::_duplicate (nsm)) {
  }

  virtual void running (MonitorTestInterface::Which proc)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void finished (MonitorTestInterface::Which proc)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  ACE_CString base_;
  CosNotification::NotificationServiceMonitorControl_var nsm_;
};

void
MonitorTestInterface_i::running (MonitorTestInterface::Which proc)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_CString str;
  CosNotification::NotificationServiceMonitorControl::Data_var data;
  CosNotification::NotificationServiceMonitorControl::NameList list;
  CosNotification::NotificationServiceMonitorControl::Numeric num;

  switch(proc)
    {
    case MonitorTestInterface::NotifyService:
      data =
        nsm_->get_statistic(NotifyMonitoringExt::EventChannelFactoryNames);
      list = data->list ();
      if (list.length () != 1)
        ACE_ERROR ((LM_ERROR, "ERROR: There should be only one Event "
                              "Channel Factory\n"));
      this->base_ = list[0];
      this->base_ += "/";

      str = this->base_ + NotifyMonitoringExt::ActiveEventChannelCount;
      data = nsm_->get_statistic(str.c_str ());
      num = data->num ();
      if (num.last != 0)
        ACE_ERROR ((LM_ERROR, "ERROR: There should be no active Event "
                              "Channels\n"));

      str = this->base_ + NotifyMonitoringExt::InactiveEventChannelCount;
      data = nsm_->get_statistic(str.c_str ());
      num = data->num ();
      if (num.last != 0)
        ACE_ERROR ((LM_ERROR, "ERROR: There should be no inactive Event "
                              "Channels\n"));
      break;
    case MonitorTestInterface::Consumer:
      str = this->base_ + NotifyMonitoringExt::ActiveEventChannelNames;
      data =
        nsm_->get_statistic (str.c_str ());
      list = data->list ();
      if (list.length () != 1)
        ACE_ERROR ((LM_ERROR, "ERROR: There should be only one active "
                              "Event Channel\n"));

      // Base will now be the factory plus the event channel
      this->base_ = list[0];
      this->base_ += "/";

      str = this->base_ + NotifyMonitoringExt::EventChannelConsumerCount;
      data = nsm_->get_statistic(str.c_str ());
      num = data->num ();
      if (num.last != 1)
        ACE_ERROR ((LM_ERROR, "ERROR: There should be only one Consumer\n"));

      str = this->base_ + NotifyMonitoringExt::EventChannelQueueElementCount;
      data = nsm_->get_statistic(str.c_str ());
      num = data->num ();
      if (num.last != 0)
        ACE_ERROR ((LM_ERROR, "ERROR: There should be no events queued\n"));
      break;
    case MonitorTestInterface::Supplier:
      str = this->base_ + NotifyMonitoringExt::EventChannelSupplierCount;
      data = nsm_->get_statistic(str.c_str ());
      num = data->num ();
      if (num.last != 1)
        ACE_ERROR ((LM_ERROR, "ERROR: There should be only one Supplier\n"));
      break;
    default:
      ACE_ERROR ((LM_ERROR, "ERROR: Impossible enum value %d\n", proc));
    }
}

void
MonitorTestInterface_i::finished (MonitorTestInterface::Which proc)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_CString str;
  CosNotification::NotificationServiceMonitorControl::Data_var data;
  CosNotification::NotificationServiceMonitorControl::Numeric num;

  switch(proc)
    {
    case MonitorTestInterface::NotifyService:
      ACE_ERROR ((LM_ERROR, "ERROR: This should not be called\n"));
      break;
    case MonitorTestInterface::Consumer:
      // By the time the consumer finishes, it should have destroyed
      // the event channel
      this->running (MonitorTestInterface::NotifyService);
      break;
    case MonitorTestInterface::Supplier:
      str = this->base_ + NotifyMonitoringExt::EventChannelConsumerCount;
      data = nsm_->get_statistic(str.c_str ());
      num = data->num ();
      if (num.last != 1)
        ACE_ERROR ((LM_ERROR, "ERROR: There should still be one Consumer\n"));

      str = this->base_ + NotifyMonitoringExt::EventChannelQueueElementCount;
      data = nsm_->get_statistic(str.c_str ());
      num = data->num ();
      if (num.last == 0)
        ACE_ERROR ((LM_ERROR, "ERROR: There should be at least one "
                              "event queued\n"));
      break;
    default:
      ACE_ERROR ((LM_ERROR, "ERROR: Impossible enum value %d\n", proc));
    }
}

static const ACE_TCHAR* ior_output_file = ACE_TEXT ("test_monitor.ior");
static const ACE_TCHAR* monitor_ior = 0;

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        monitor_ior = get_opts.opt_arg ();
        break;
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage: %s "
                           "-k <ior> "
                           "-o <file> "
                           "\n",
                           argv [0]),
                          -1);
      }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int status = 0;
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      CORBA::Object_var obj =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (monitor_ior));
      CosNotification::NotificationServiceMonitorControl_var nsm =
        CosNotification::NotificationServiceMonitorControl::_narrow (obj.in ());

      if (CORBA::is_nil (nsm.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to locate the "
                             "Notification Service Monitor\n"),
                            1);
        }

      MonitorTestInterface_i* mti = 0;
      ACE_NEW_RETURN (mti, MonitorTestInterface_i (nsm.in ()), 1);
      PortableServer::ServantBase_var owner_transfer (mti);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::ObjectId_var id = root_poa->activate_object (mti);
      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      MonitorTestInterface_var test =
        MonitorTestInterface::_narrow (object.in ());
      CORBA::String_var ior = orb->object_to_string (test.in ());

      // Test the case where there are no consumers or suppliers first
      // before we write out our IOR
      mti->running (MonitorTestInterface::NotifyService);

      FILE *output_file= ACE_OS::fopen (ior_output_file, ACE_TEXT ("w"));
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s\n",
                           ior_output_file),
                           1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      PortableServer::POAManager_var poa_manager = root_poa->the_POAManager ();
      poa_manager->activate ();

      orb->run ();
      orb->destroy ();
    }
  catch(const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("test_monitor: ");
      status++;
    }

  return status;
}
