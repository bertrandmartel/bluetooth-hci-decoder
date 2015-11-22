/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Bertrand Martel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
/**
	hci_event_packet.h
	HCI event stuctures for each supported HCI Event

	@author Bertrand Martel
	@version 1.0
*/
#ifndef HCIEVENTPACKET_H
#define HCIEVENTPACKET_H

#include <iostream>

/**********************************************************************/
/**********************************************************************/
/********************* HCI EVENTS** ***********************************/
/**********************************************************************/
/**********************************************************************/

/* HCI Event 0x3E : LE META Event*/
typedef struct le_meta_event{

	uint8_t command;                 /* 1B | */
	uint8_t parameter_length;        /* 1B | parameter length */
	uint8_t subevent_code;           /* 1B | Subevent code for LE Connection Complete event */

	le_meta_event(const std::vector<char> &data){
		this->command = HCI_EVENT_LE_META;
		this->parameter_length = data[EVENT_FRAME_OFFSET];
		this->subevent_code = data[EVENT_FRAME_OFFSET+1];
	}

} le_meta_event_t;

/**********************************************************************/
/**********************************************************************/
/********************* LE META SUBEVENT *******************************/
/**********************************************************************/
/**********************************************************************/

/*advertising report struct */
typedef struct {

	uint8_t event_type;              /* 1B | 0x00 Connectable undirected advertising (ADV_IND) | 0x01 Connectable directed advertising (ADV_DIRECT_IND) | 0x02 Scannable undirected advertising (ADV_SCAN_IND) | 0x03 Non connectable undirected advertising (ADV_NONCONN_IND) | 0x04 Scan Response (SCAN_RSP)*/
	uint8_t address_type;            /* 1B | 0x00 Public Device Address | 0x01 Random Device Address | 0x02 Public Identity Address (Corresponds to Resolved Private Address) | 0x03 Random (static) Identity Address (Corresponds to Resolved Private Address)*/
	uint8_t address[6];              /* 6B | Public Device Address, Random Device Address, Public Identity Address or Random (static) Identity Address of the advertising device*/
	uint8_t length_data;             /* 1B | Length of the Data[i] field for each device which responded */
	uint8_t data[];                  /* XB | Length_Data[i] octets of advertising or scan response data */
	uint8_t rssi;                    /* 1B | Size: 1 Octet (signed integer) Range: -127 ≤ N ≤ +20 Units: dBm */

}le_advertising_report;


//HCI Event 0x3E | subevent_code : 0x02 : LE Advertising Report Event
typedef struct {

	uint8_t command;                  /* 1B | */
	uint8_t subevent_code;            /* 1B | Subevent code for LE Advertising Report event */
	uint8_t num_reports;              /* 1B | Number of responses in event */
	le_advertising_report advertising_report[];

} le_advertising_report_event_t;

struct advertising_report{

	uint8_t event_type;                /* 1B | 0x00 : Connectable undirected advertising (ADV_IND)
													 0x01 : Connectable directed advertising (ADV_DIRECT_IND)
													 0x02 : Scannable undirected advertising (ADV_SCAN_IND)
													 0x03 : Non connectable undirected advertising (ADV_NONCONN_IND)
													 0x04 : Scan Response (SCAN_RSP) */

	uint8_t address_type;              /* 1B | 0x00 : Public Device Address 
													 0x01 : Random Device Address 
													 0x02 : Public Identity Address (Corresponds to Resolved Private Address)
													 0x03 : Random (static) Identity Address */

	bt_address address;                /* 6B | Public Device Address, Random Device Address, Public Identity
													 Address or Random (static) Identity Address of the advertising device*/

	uint8_t data_length;               /* 1B | 0x00-0x1F : Length of the Data[i] field for each device which responded */

	std::vector<uint8_t> data;              /* XB | Length_Data[i] octets of advertising or scan response data formatted */

	uint8_t rssi;                      /* 1B | Range: -127 ≤ N ≤ +20; Units: dBm */

	Json::Value toJson(){

		Json::Value output;
		output["event_type"] = event_type;
		output["address_type"] = address_type;
		output["address"] = address.toString();
		output["data_length"] = data_length;
		output["rssi"] = rssi;

		Json::Value data_array(Json::arrayValue);

		unsigned int i = 0;
		for (i = 0; i < data.size();i++){
			data_array.append(data[i]);
		}
		output["data"] = data_array;

		return output;
	}

};

/* HCI LE Meta sub event 0x02 : LE_ADVERTISING_REPORT*/
typedef struct le_meta_advertising_report_event : public IHciEventFrame {

	uint8_t  num_reports;                         /* 1B | 0x01-0x19 : Number of responses in event */
	LE_SUBEVENT_ENUM subevent_code;               /*subevent code*/

	std::vector<advertising_report*> ad_report_items;  /* a list of advertising report*/

	void clear(){
		for (std::vector<advertising_report*>::iterator it = ad_report_items.begin(); it != ad_report_items.end();it++){
			delete (*it);
		}
		ad_report_items.clear();
	}

	le_meta_advertising_report_event(const std::vector<char> &data){
		this->event_code = HCI_EVENT_LE_META;
		this->subevent_code = HCI_EVENT_LE_ADVERTISING_REPORT;
		this->parameter_total_length = data[EVENT_FRAME_OFFSET];
		this->num_reports = data[EVENT_FRAME_OFFSET+2];

		unsigned int i = 0;
		int offset = EVENT_FRAME_OFFSET+3;

		for (i = 0 ;i  < this->num_reports;i++){

			advertising_report *report = new advertising_report();

			report->event_type = data[offset];
			report->address_type = data[offset + 1];

			bt_address addr;
			addr.address[0]=data[offset + 7];
			addr.address[1]=data[offset + 6];
			addr.address[2]=data[offset + 5];
			addr.address[3]=data[offset + 4];
			addr.address[4]=data[offset + 3];
			addr.address[5]=data[offset + 2];

			report->address = addr;

			report->data_length = data[offset + 8];

			std::vector<uint8_t> data_val;

			int j = 0;
			for (j=0;j < report->data_length;j++){
				data_val.push_back(data[offset + 9 + j]);
			}
			report->data = data_val;
			report->rssi = data[report->data_length + offset + 9];
			this->ad_report_items.push_back(report);
			offset+=report->data_length + 10;
		}
	}

	void print(){
		std::cout << "> HCI_EVENT_LE_ADVERTISING_REPORT : \n" << toJson().data() << std::endl;
	}

	std::string toJson(){

		Json::Value output;

		Json::Value event_code;
		event_code["code"] = HCI_EVENT_LE_ADVERTISING_REPORT;
		event_code["value"] = LE_SUBEVENT_STRING_ENUM.at(HCI_EVENT_LE_ADVERTISING_REPORT);
		output["subevent_code"] = event_code;

		init(output);
		output["num_reports"] = num_reports;

		Json::Value report_list(Json::arrayValue);
		unsigned int i = 0;
		for (i = 0; i  < ad_report_items.size();i++){
			report_list.append(ad_report_items[i]->toJson());
		}
		output["reports"] =  report_list;

		return convert_json_to_string(output);
	}

} le_meta_advertising_report_event_t;

/* HCI LE Meta sub event 0x01 : LE_CONNECTION_COMPLETE*/
typedef struct le_meta_connection_complete_event : public IHciEventFrame {

	uint8_t    status;                  /* 1B | 0x00 : Connection successfully completed, 0x01-0xFF : connection failure */
	uint16_t   connection_handle;       /* 2B | Connection_Handle to be used to identify a connection between two
											 Bluetooth devices. The Connection_Handle is used as an identifier
											 for transmitting and receiving data*/
	uint8_t    role;                    /* 1B | 0x00 : Connection is master, 0x01 : Connection is slave */
	uint8_t    peer_address_type;       /* 1B | 0x00 : Peer is using a Public Device Address, 0x01 : Peer is using a Random Device Address */
	bt_address peer_address;            /* 6B | address */
	uint16_t   conn_interval;           /* 2B | Connection interval used on this connection */
	uint16_t   conn_latency;            /* 2B | Slave latency for the connection in number of connection events */
	uint16_t   supervision_timeout;     /* 2B | Connection supervision timeout */
	uint8_t    master_clock_accuracy;   /* 1B | */
	LE_SUBEVENT_ENUM subevent_code;     /*subevent code*/

	le_meta_connection_complete_event(const std::vector<char> &data){
		this->event_code = HCI_EVENT_LE_META;
		this->subevent_code = HCI_EVENT_LE_CONNECTION_COMPLETE;
		this->parameter_total_length = data[EVENT_FRAME_OFFSET];
		this->status = data[EVENT_FRAME_OFFSET+2];
		this->connection_handle = (data[EVENT_FRAME_OFFSET+3]<<8) + data[EVENT_FRAME_OFFSET+4];
		this->role = data[EVENT_FRAME_OFFSET+5];
		this->peer_address_type = data[EVENT_FRAME_OFFSET+6];

		this->peer_address.address[0] = data[EVENT_FRAME_OFFSET+12];
		this->peer_address.address[1] = data[EVENT_FRAME_OFFSET+11];
		this->peer_address.address[2] = data[EVENT_FRAME_OFFSET+10];
		this->peer_address.address[3] = data[EVENT_FRAME_OFFSET+9];
		this->peer_address.address[4] = data[EVENT_FRAME_OFFSET+8];
		this->peer_address.address[5] = data[EVENT_FRAME_OFFSET+7];

		this->conn_interval = (data[EVENT_FRAME_OFFSET+13]<<8) + data[EVENT_FRAME_OFFSET+14];
		this->conn_latency = (data[EVENT_FRAME_OFFSET+15]<<8) + data[EVENT_FRAME_OFFSET+16];
		this->supervision_timeout = (data[EVENT_FRAME_OFFSET+17]<<8) + data[EVENT_FRAME_OFFSET+18];
		this->master_clock_accuracy = data[EVENT_FRAME_OFFSET+19];
	}

	void print(){
		std::cout << "> LE_CONNECTION_COMPLETE : \n" << toJson().data() << std::endl;
	}

	std::string toJson(){

		Json::Value output;
		Json::Value event_code;
		event_code["code"] = HCI_EVENT_LE_CONNECTION_COMPLETE;
		event_code["value"] = LE_SUBEVENT_STRING_ENUM.at(HCI_EVENT_LE_CONNECTION_COMPLETE);
		output["subevent_code"] = event_code;
		init(output);
		output["status"] =  status;
		output["connection_handle"] =  connection_handle;
		output["peer_address_type"] =  peer_address_type;
		output["peer_address"] =  peer_address.toString();
		output["conn_interval"] =  conn_interval;
		output["conn_latency"] =  conn_latency;
		output["supervision_timeout"] =  supervision_timeout;
		output["master_clock_accuracy"] =  master_clock_accuracy;

		return convert_json_to_string(output);
	}

} le_meta_connection_complete_event_t;

#endif //HCIEVENTPACKET_H