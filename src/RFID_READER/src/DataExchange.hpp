#pragma once
#include "Arp/System/Core/Arp.h"
#include "Arp/Plc/Commons/Esm/ProgramBase.hpp"
#include "Arp/System/Commons/Logging.h"
#include "Component.hpp"

namespace RFID
{

using namespace Arp;
using namespace Arp::System::Commons::Diagnostics::Logging;
using namespace Arp::Plc::Commons::Esm;

//#program
//#component(RFID::Component)
class DataExchange : public ProgramBase, private Loggable<DataExchange>
{
public: // typedefs

public: // construction/destruction
    DataExchange(RFID::Component& componentArg, const String& name);
    DataExchange(const DataExchange& arg) = delete;
    virtual ~DataExchange() = default;

public: // operators
    DataExchange&  operator=(const DataExchange& arg) = delete;

public: // properties

public: // operations
    void    Execute() override;

public: /* Ports
           =====
           Ports are defined in the following way:
           //#port
           //#attributes(Input|Retain)
           //#name(NameOfPort)
           boolean portField;

           The attributes comment define the port attributes and is optional.
           The name comment defines the name of the port and is optional. Default is the name of the field.
        */

private: // fields
    RFID::Component& component;
 
};

///////////////////////////////////////////////////////////////////////////////
// inline methods of class ProgramBase
inline DataExchange::DataExchange(RFID::Component& componentArg, const String& name)
: ProgramBase(name)
, component(componentArg)
{
    log.Info("-----------Program Constructor");
}

} // end of namespace RFID
