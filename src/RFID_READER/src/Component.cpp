#include "Component.hpp"
#include "Arp/Plc/Commons/Esm/ProgramComponentBase.hpp"
#include "RFID_READERLibrary.hpp"

namespace RFID
{

    Component::Component(IApplication& application, const String& name)
        : ComponentBase(application, ::RFID::RFID_READERLibrary::GetInstance(), name, ComponentCategory::Custom)
        , programProvider(*this)
        , ProgramComponentBase(::RFID::RFID_READERLibrary::GetInstance().GetNamespace(), programProvider)
        , workerThread(Arp::make_delegate(this, &RFID::Component::Execute), 500, "RFID_POLLING")
    {
        log.Info("-------------------RFID_Component Constructor");
    }

    void Component::Initialize()
    {
        // never remove next line
        ProgramComponentBase::Initialize();

        // subscribe events from the event system (Nm) here
    }

    void Component::LoadConfig()
    {
        // load project config here
    }

    void Component::SetupConfig()
    {
        // never remove next line
        ProgramComponentBase::SetupConfig();

        // setup project config here     

    }

    void Component::ResetConfig()
    {
        // never remove next line
        ProgramComponentBase::ResetConfig();

        // implement this inverse to SetupConfig() and LoadConfig()
    }

    void Component::PowerDown()
    {
        // implement this only if data must be retained even on power down event
        // Available with 2021.6 FW
    }
    
    void Component::Start(void) {
        log.Info("---------------- Start");
        try {
            RFID_D.Stop = false;
            workerThread.Start();
        }
        catch (Exception& e) {
            log.Error("---------------- Error thread start:{0}", e.GetMessage());
        }
    }

    void Component::Stop(void) {
        log.Info("----------------Stop:");
        try {
            log.Info("---------------- Thread:{0} Running:{1} ",
                "Worker", workerThread.IsRunning());
            RFID_D.Stop = true;
            // Stopping WorkerThread synchronously.
            workerThread.Stop();
        }
        catch (Exception& e) {
            log.Error("---------------- Error thread Stop:{0}", e.GetMessage());
        }
    }
    /// <summary>
    /// Thread method.
    /// </summary>
    void Component::Execute() {
       
        try {
            RFID_D.Execute();
            if (RFID_D.getRFIDcount() > 0)
            {
                if (RFID_D.getRFID(tempString) == 0)
                {
                    RFIDsRead.index++;
                    if (RFIDsRead.index == 4)
                    {
                        RFIDsRead.last5RFIDs[0] = "";
                    }
                    else if (RFIDsRead.index > 4) {
                        RFIDsRead.index = 0;
                        RFIDsRead.last5RFIDs[RFIDsRead.index + 1] = "";
                    }
                    else {
                        RFIDsRead.last5RFIDs[RFIDsRead.index + 1] = "";
                    }

                    RFIDsRead.last5RFIDs[RFIDsRead.index] = tempString;
                    log.Info("RFID:{0}", tempString);
                }
            }
        }
        catch (...)
        {
            log.Error("Exception caught in thread");
        }
    }

} // end of namespace RFID
