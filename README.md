# C++ Bluetooth HCI decoder library #


<i>Last update 22/11/2015</i>

Lightweight Bluetooth HCI decoder library

* decode HCI frames individually
* provide JSON formatted string output

##Build

```
cd bluetooth-hci-decoder
make
```

Library release under `build-hcidecoder-X.X` directory.

##Test

A test small is provided extracting HCI frames from a btsnoop file decoded with https://github.com/akinaru/btsnoop-stream-decoder

Syntax : ``./hci-decoder-test <snoop_file>``

```
./build-hcidecoder-X.X/hci-decoder-test ../snoop_files/btsnoop_hci.log
```

##Decoding

###Decode frame

Input should be a single HCI frame in `std::vector<char> packet` format

Instanciate a `HciDecoder` class and call `IHciFrame* decode(std::vector<char> packet);` :

```
HciDecoder hci_decoder;

IHciFrame * frame = hci_decoder.decode(packet);

```

###Architecture

![architecture](img/arch.png)

###Decoded item list

A `IHciFrame` is returned from featuring following method :


You can retrieve full list of `IHciFrame` decoded with a single `HciDecoder` with `std::vector<IHciFrame*> getFrameList()` method

```
std::vector<IHciFrame*> frame_list = hci_decoder.getFrameList();

for (unsigned int i = 0; i  < frame_list.size();i++){
	frame_list.at(i)->print();
}
```

##JSON output format

Common fields for all HCI frames :

* packet_type

###Event

Common fields for all HCI Event frames:

* event_code
* parameter_total_length

Example :

```
{
   "command_opcode" : {
      "ocf" : {
         "code" : 26,
         "value" : "HCI_CMD_OCF_CTRL_BSB_WRITE_SCAN_ENABLE_COMMAND"
      },
      "ogf" : {
         "code" : 3,
         "value" : "HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS"
      }
   },
   "event_code" : {
      "code" : 14,
      "value" : "HCI_EVENT_COMMAND_COMPLETE"
   },
   "num_hci_command_packets" : 1,
   "packet_type" : {
      "code" : 4,
      "value" : "HCI_TYPE_EVENT"
   },
   "parameter_total_length" : 4,
   "return_parameters" : {
      "values" : {
         "status" : 0
      }
   }
}
```

###Command

Common fields for all HCI Command frames:

* ogf (Opcode Group field)
* ocf (Opcode Command field)

```
{
   "ocf" : {
      "code" : 3,
      "value" : "HCI_CMD_OCF_CTRL_BSB_RESET_COMMAND"
   },
   "ogf" : {
      "code" : 3,
      "value" : "HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS"
   },
   "packet_type" : {
      "code" : 1,
      "value" : "HCI_TYPE_COMMAND"
   },
   "parameter_total_length" : 0
}
```

##Supported Commands / Events

###Commands

* CONTROLLER BASEBAND CMD (0x03)

  * 0x03 RESET
  * 0x13 WRITE LOCAL NAME
  * 0x14 READ LOCAL NAME
  * 0x23 READ CLASS OF DEVICE
  * 0x24 WRITE CLASS OF DEVICE
  * 0x44 READ INQUIRY MODE
  * 0x45 WRITE INQUIRY MODE
  * 0x6C READ LE HOST SUPPORT
  * 0x6D WRITE LE HOST SUPPORT
  * 0x1E WRITE INQUIRY SCAN ACTIVITY
  * 0x1D READ INQUIRY SCAN ACTIVITY
  * 0x25 READ VOICE SETTING
  * 0x26 WRITE VOICE SETTING
  * 0x39 READ CURRENT IAC LAP
  * 0x3A WRITE CURRENT IAC LAP
  * 0x46 READ PAGE SCAN TYPE
  * 0x47 WRITE PAGE SCAN TYPE
  * 0x55 READ SIMPLE PAIRING MODE
  * 0x56 WRITE SIMPLE PAIRING MODE
  * 0x51 READ EXTENDED INQUIRY RESPONSE
  * 0x52 WRITE EXTENDED INQUIRY RESPONSE
  * 0x19 READ SCAN ENABLE
  * 0x1A WRITE SCAN ENABLE
  * 0x42 READ INQUIRY SCAN TYPE
  * 0x43 WRITE INQUIRY SCAN TYPE
  * 0x01 SET EVENT MASK
  * 0x33 HOST BUFFER SIZE
  * 0x17 READ PAGE TIMEOUT
  * 0x18 WRITE PAGE TIMEOUT
  * 0x33 HOST BUFFER SIZE

* INFORMATIONAL (0x04)

  * 0x02 CMD READ LOCAL SUPPORTED COMMANDS
  * 0x05 READ BUFFER SIZE
  * 0x09 READ BD ADDR
  * 0x01 READ LOCAL VERSION INFORMATION
  * 0x04 READ LOCAL EXTENDED FEATURES

* lE CMD (0x08)

  * 0x02 LE READ BUFFER SIZE
  * 0x08 LE SET ADVERTISING DATA
  * 0x0F LE READ WHITE LIST SIZE
  * 0x10 LE CLEAR WHITE LIST
  * 0x0B SET SCAN PARAMETERS
  * 0x06 SET ADVERTISING PARAMETERS
  * 0x0C SET SET SCAN ENABLE
  * 0x03 READ LOCAL SUPPORTED FEATURES

* LINK POLICY (0x02)

  * 0x18 WRITE DEFAULT LINK POLICY SETTINGS

###Events

* 0x0E COMMAND COMPLETE

* 0x3E LE_META
  * 0x01 CONNECTION COMPLETE
  * 0x02 ADVERTISING REPORT

<hr/>

##External library

* json encoding with jsonccp : https://github.com/open-source-parsers/jsoncpp

##Specifications

* Bluetooth specifications v4.2 - Volume 2 - Part E Host Controller Interface Functionnal Specification https://www.bluetooth.org
