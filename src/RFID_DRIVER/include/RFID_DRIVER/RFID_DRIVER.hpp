#pragma once
#include "Arp/System/Core/Arp.h"
#include "libusb-1.0/libusb.h"
#include <stdio.h>
#include <sys/types.h>
#include "Arp/System/Commons/Logging.h"
//
#include "hidapi.h"
#include "keymap.h"
//#include "hidapi_libusb.h"
#include <wchar.h>
#include <string>

#include <queue>
namespace RFID
{

using namespace Arp;

#define IN_EP  0x00
#define OUT_EP 0x00

class RFID_DRIVER: private Loggable<RFID_DRIVER>
{
public: // typedefs
    typedef std::shared_ptr<RFID_DRIVER> Ptr;

public: // construction/destruction
    /// <summary>Constructs an <see cref="RFID_DRIVER" /> instance.</summary>
    RFID_DRIVER(void) ;
    /// <summary>Copy constructor.</summary>
    RFID_DRIVER(const RFID_DRIVER& arg) = default;
    /// <summary>Assignment operator.</summary>
    RFID_DRIVER& operator=(const RFID_DRIVER& arg) = default;
    /// <summary>Destructs this instance and frees all resources.</summary>
    ~RFID_DRIVER(void);

public: // operators

public: // static operations

public: // setter/getter operations
    int getRFID(Arp::String& s );
    int getRFIDcount();

public: // operations
    void Execute();

protected: // operations
    void print_dev(hid_device_info *devs);
    void print_devs(hid_device_info *devs);
    void init_USB();

private: // static methods

private: // methods


public: // fields

    int res;
    unsigned char buf[256];
    std::string rfid;
    std::queue<String> rfids;

    #define MAX_STR 255
    wchar_t wstr[MAX_STR];
    hid_device *handle = nullptr;
    int i;

    struct hid_device_info *devs;

    unsigned int vid=0x16c0; // Vendor ID
    unsigned int pid=0x27db; // Device ID

    bool Stop = false;

private: // static fields

};

} // end of namespace RFID_DRIVER