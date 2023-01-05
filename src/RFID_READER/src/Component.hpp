#pragma once
#include "Arp/System/Core/Arp.h"
#include "Arp/System/Acf/ComponentBase.hpp"
#include "Arp/System/Acf/IApplication.hpp"
#include "Arp/Plc/Commons/Esm/ProgramComponentBase.hpp"
#include "ComponentProgramProvider.hpp"
#include "Arp/Plc/Commons/Meta/MetaLibraryBase.hpp"
#include "Arp/System/Commons/Logging.h"

#include "RFID_DRIVER/RFID_DRIVER.hpp"
#include "Arp/System/Acf/IControllerComponent.hpp"
#include "Arp/System/Commons/Threading/WorkerThread.hpp"


namespace RFID
{

using namespace Arp;
using namespace Arp::System::Acf;
using namespace Arp::Plc::Commons::Esm;
using namespace Arp::Plc::Commons::Meta;

//#component
class Component : public ComponentBase, public ProgramComponentBase, public IControllerComponent, private Loggable<Component>
{
public: // typedefs

public: // construction/destruction
    Component(IApplication& application, const String& name);
    virtual ~Component() = default;

public: // IComponent operations
    void Initialize() override;
    void LoadConfig() override;
    void SetupConfig() override;
    void ResetConfig() override;
    void PowerDown() override;

public: // IControllerComponent
    void Start();
    void Stop();

public: // ProgramComponentBase operations
    void RegisterComponentPorts() override;

private: // methods
    Component(const Component& arg) = delete;
    Component& operator= (const Component& arg) = delete;

public: // static factory operations
    static IComponent::Ptr Create(Arp::System::Acf::IApplication& application, const String& name);

private: // fields
    ComponentProgramProvider programProvider;    
    WorkerThread workerThread;

public:
    void Execute();
    Arp::String tempString = {"false"};

public: 
           struct Ports 
           {
               //#name(last5RFIDsRead)
               //#attributes(Output|Opc)
               StaticString<80> last5RFIDs[5] = {"false"};
               //#name(lastRFIDRead)
               //#attributes(Output|Opc)
               uint8 index = {0};
               // The GDS name is "<componentName>/NameOfPort" if the struct is declared as Hidden
               // otherwise the GDS name is "<componentName>/PORTS.NameOfPort"
           };
           
           //#port
           Ports RFIDsRead;
public:
    RFID::RFID_DRIVER RFID_D;
};

inline IComponent::Ptr Component::Create(Arp::System::Acf::IApplication& application, const String& name)
{
    return IComponent::Ptr(new Component(application, name));
}

} // end of namespace RFID
