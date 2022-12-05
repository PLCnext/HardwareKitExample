#include "RFID_DRIVER/RFID_DRIVER.hpp"

namespace RFID
{

RFID_DRIVER::~RFID_DRIVER(void){	
	log.Info("-------------------RFID_DRIVER Deconstructor");
	if(handle)
		hid_close(handle);

	/* Free static HIDAPI objects. */
	hid_exit();
}
	
RFID_DRIVER::RFID_DRIVER(void) {
	log.Info("-------------------RFID_DRIVER Constructor");
	if (hid_init())
	{
		log.Error("----Init HID Failed");
	}
	memset(buf, 0, sizeof(buf));
	init_USB();	
}

void RFID_DRIVER::init_USB(){
	log.Info("---Init USB");
	devs = hid_enumerate(0x0, 0x0);

	print_devs(devs);
	hid_free_enumeration(devs);	

	if (devs != nullptr)
		log.Info("---DEVS not null", devs->interface_number);

	log.Info("---Open HID handle");	
	// Open the device using the VID, PID,
	// and optionally the Serial number.
	if (handle != nullptr)
		hid_close(handle);
	handle = hid_open(vid, pid, NULL);


	if (handle == nullptr) {
		log.Error("---unable to open USB device\n");
		sleep(1);
	}
	else {
		log.Info("---Device opened\n");
		hid_set_nonblocking(handle, 1);
		log.Info("---nonblocking mode set");
	}
}

void RFID_DRIVER::print_dev(struct hid_device_info *cur_dev)
{
	std::wstring ws;
	log.Info("--Device Found");	
	log.Info("  type: {0:X} : {1:X}", cur_dev->vendor_id, cur_dev->product_id);
	log.Info("  path: {0}", cur_dev->path);
	
	if (cur_dev->serial_number != nullptr) {
		ws = std::wstring(cur_dev->serial_number);
		std::string serial_number(ws.begin(), ws.end());
		log.Info("  serial_number: {0}", serial_number.c_str());
	}

	if (cur_dev->manufacturer_string != nullptr) {
		ws = std::wstring(cur_dev->manufacturer_string);
		std::string manufacturer_string(ws.begin(), ws.end());
		log.Info("  Manufacturer: {0}", manufacturer_string.c_str());
	}
	if (cur_dev->product_string != nullptr) {
		ws = std::wstring(cur_dev->product_string);
		std::string product_string(ws.begin(), ws.end());
		log.Info("  Product:     {0}", product_string.c_str());
	}

	log.Info("  Release:     {0}", (int)devs->release_number);
	log.Info("  Interface:    {0}",  cur_dev->interface_number);
	log.Info("  Usage (page): 0x{0} (0x{1})", cur_dev->usage, cur_dev->usage_page);
}

void RFID_DRIVER::print_devs(struct hid_device_info *cur_dev) {
	log.Info("print_devs");
	while (cur_dev != NULL) {
		print_dev(cur_dev);
		cur_dev = cur_dev->next;
	}
}

void RFID_DRIVER::Execute()
{	
	log.Debug("---Execute");
	if (handle == nullptr) {
		log.Error("open USB device first!");
		//hid_exit();
		this->init_USB();		
		sleep(1);
	}
	else {
		rfid = "";
		log.Debug("---Reading Data:  ");
		// Wait for CR termination of message.
		// This call is blocking!
		while (buf[2] != 40) {
			if (Stop == true)
				break;			
			res = hid_read_timeout(handle, buf, sizeof(buf), 2000);
			if (res < 0) {
				log.Error("---Unable to read USB");
				if( handle != nullptr)
				{	
					log.Info("---reset handle");
					hid_close(handle);
					handle = nullptr;
					hid_exit();
				}
				break;
			}

		
			if (res == 3) {				
				if (buf[2] > 3) {
					// choose Keyboard Map and add symbol to string.
					if (buf[0] == 1)
						rfid += keys_page[buf[2]];
					if (buf[0] == 2) {
						rfid += shift_keys_page[buf[2]];
					}
				}
			}

			res = 0;
		}
		buf[2] = 0;		
		// Result
		log.Debug("---RFID: {0}", rfid.c_str());
		if (rfid.length()>=10)
			rfids.push(String(rfid));
	}
}
/// <summary>
/// Return oldest RFID.
/// </summary>
/// <param name="s"> </param>
/// <returns></returns>
int RFID_DRIVER::getRFID(Arp::String &s){
	if ( rfids.size() == 0 ){
		return -1;
	}
	else		
	{	
		s = rfids.front();
		rfids.pop();
		return 0;
	}	
}

int RFID_DRIVER::getRFIDcount() {
	return rfids.size();
}

} // end of namespace RFID_DRIVER