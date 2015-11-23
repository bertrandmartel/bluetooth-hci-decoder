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
	hcidecoder.cpp
	HCI decoder

	@author Bertrand Martel
	@version 1.0
*/
#include "hci_decoder/hcidecoder.h"
#include "hci_decoder/hci_cmd_packet.h"
#include "hci_decoder/hci_event_packet.h"
#include "hci_decoder/command_complete.h"
#include "json/json.h"

using namespace std;

HciDecoder::HciDecoder()
{
}

HciDecoder::~HciDecoder()
{
	for (std::vector<IHciFrame*>::iterator it = frame_list.begin(); it != frame_list.end();++it){
		(*it)->clear();
		delete (*it);
	}
	frame_list.clear();
}

std::vector<IHciFrame*> HciDecoder::getFrameList(){
	return frame_list;
}

std::string HciDecoder::toJson(bool beautify){

	Json::Value output(Json::arrayValue);
	for (std::vector<IHciFrame*>::iterator it = frame_list.begin(); it != frame_list.end();++it){
		output.append((*it)->toJsonObj());
	}

	if (!beautify){
		Json::StreamWriterBuilder builder;
		builder.settings_["indentation"] = "";
		return Json::writeString(builder, output);
	}
	else{
		return output.toStyledString();
	}
}

std::vector<IHciFrame*> HciDecoder::decode(std::vector<char> data){

	switch (data[0]){

		case HCI_TYPE_COMMAND:
		{
			uint8_t ogf = get_ogf(data[2]);
			uint8_t ocf = get_ocf(data[2],data[1]);

			switch (ogf){

				case HCI_CMD_OGF_LINK_CONTROl_COMMANDS:
				{
					cout << "[NOT DECODED] ogf : " << unsigned(ogf) << " | ocf : " << unsigned(ocf) << endl;
					break;
				}
				case HCI_CMD_OGF_LINK_POLICY_COMMANDS:
				{
					switch(ocf)
					{
						case HCI_CMD_OCF_LINK_POLICY_WRITE_DEFAULT_LINK_POLICY_SETTINGS_COMMAND:
						{
							frame_list.push_back(new write_default_link_policy_settings_cmd_t(data));
							break;
						}
						default:
						{
							cout << "[NOT DECODED] ogf : " << unsigned(ogf) << " | ocf : " << unsigned(ocf) << endl;
						}
					}
					break;
				}
				case HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS:
				{
					switch(ocf)
					{
						case HCI_CMD_OCF_CTRL_BSB_RESET_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS,HCI_CMD_OCF_CTRL_BSB_RESET_COMMAND));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_WRITE_LOCAL_NAME_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_write_local_name_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_READ_LOCAL_NAME_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS,HCI_CMD_OCF_CTRL_BSB_READ_LOCAL_NAME_COMMAND));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_READ_CLASS_OF_DEVICE_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_read_class_of_device_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_WRITE_CLASS_OF_DEVICE_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_write_class_of_device_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_READ_INQUIRY_MODE_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS,HCI_CMD_OCF_CTRL_BSB_READ_INQUIRY_MODE_COMMAND));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_WRITE_INQUIRY_MODE_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_write_inquiry_mode_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_READ_LE_HOST_SUPPORT_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS,HCI_CMD_OCF_CTRL_BSB_READ_LE_HOST_SUPPORT_COMMAND));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_WRITE_LE_HOST_SUPPORT_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_write_le_host_support_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_WRITE_INQUIRY_SCAN_ACTIVITY_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_write_inquiry_scan_activity_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_READ_INQUIRY_SCAN_ACTIVITY_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS,HCI_CMD_OCF_CTRL_BSB_READ_INQUIRY_SCAN_ACTIVITY_COMMAND));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_READ_VOICE_SETTING_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS,HCI_CMD_OCF_CTRL_BSB_READ_VOICE_SETTING_COMMAND));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_WRITE_VOICE_SETTING_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_write_voice_settings_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_READ_CURRENT_IAC_LAP_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS,HCI_CMD_OCF_CTRL_BSB_READ_CURRENT_IAC_LAP_COMMAND));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_WRITE_CURRENT_IAC_LAP_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_write_iac_lap_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_READ_PAGE_SCAN_TYPE_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS,HCI_CMD_OCF_CTRL_BSB_READ_PAGE_SCAN_TYPE_COMMAND));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_WRITE_PAGE_SCAN_TYPE_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_write_page_scan_type_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_READ_SIMPLE_PAIRING_MODE_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS,HCI_CMD_OCF_CTRL_BSB_READ_SIMPLE_PAIRING_MODE_COMMAND));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_WRITE_SIMPLE_PAIRING_MODE_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_write_simple_pairing_mode_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_READ_EXTENDED_INQUIRY_RESPONSE_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS,HCI_CMD_OCF_CTRL_BSB_READ_EXTENDED_INQUIRY_RESPONSE_COMMAND));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_WRITE_EXTENDED_INQUIRY_RESPONSE_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_write_extended_inquiry_response_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_READ_SCAN_ENABLE_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS,HCI_CMD_OCF_CTRL_BSB_READ_SCAN_ENABLE_COMMAND));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_WRITE_SCAN_ENABLE_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_write_scan_enable_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_READ_INQUIRY_SCAN_TYPE_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS,HCI_CMD_OCF_CTRL_BSB_READ_INQUIRY_SCAN_TYPE_COMMAND));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_WRITE_INQUIRY_SCAN_TYPE_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_write_inquiry_scan_type_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_SET_EVENT_MASK_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_set_event_mask_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_READ_PAGE_TIMEOUT_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_CONTROLLER_BASEBAND_COMMANDS,HCI_CMD_OCF_CTRL_BSB_READ_PAGE_TIMEOUT_COMMAND));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_WRITE_PAGE_TIMEOUT_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_write_page_timeout_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_CTRL_BSB_HOST_BUFFER_SIZE_COMMAND:
						{
							frame_list.push_back(new ctrl_bsb_host_buffer_size_cmd_t(data));
							break;
						}
						default:
						{
							cout << "[NOT DECODED] ogf : " << unsigned(ogf) << " | ocf : " << unsigned(ocf) << endl;
						}
					}
					break;
				}
				case HCI_CMD_OGF_INFORMATIONAL_PARAMETERS:
				{
					switch (ocf){
						case HCI_CMD_OCF_INFORMATIONAL_READ_LOCAL_SUPPORTED_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_INFORMATIONAL_PARAMETERS,HCI_CMD_OCF_INFORMATIONAL_READ_LOCAL_SUPPORTED_COMMAND));
							break;
						}
						case HCI_CMD_OCF_INFORMATIONAL_READ_BUFFER_SIZE_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_INFORMATIONAL_PARAMETERS,HCI_CMD_OCF_INFORMATIONAL_READ_BUFFER_SIZE_COMMAND));
							break;
						}
						case HCI_CMD_OCF_INFORMATIONAL_READ_BD_ADDR:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_INFORMATIONAL_PARAMETERS,HCI_CMD_OCF_INFORMATIONAL_READ_BD_ADDR));
							break;
						}
						case HCI_CMD_OCF_INFORMATIONAL_READ_LOCAL_VERSION_INFORMATION_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_INFORMATIONAL_PARAMETERS,HCI_CMD_OCF_INFORMATIONAL_READ_LOCAL_VERSION_INFORMATION_COMMAND));
							break;
						}
						case HCI_CMD_OCF_INFORMATIONAL_READ_LOCAL_EXTENDED_FEATURES_COMMAND:
						{
							frame_list.push_back(new informational_read_local_extended_features_cmd_t(data));
							break;
						}
						default:
						{
							cout << "[NOT DECODED] ogf : " << unsigned(ogf) << " | ocf : " << unsigned(ocf) << endl;
						}
					}
					break;
				}
				case HCI_CMD_OGF_STATUS_PARAMETERS:
				{
					cout << "[NOT DECODED] ogf : " << unsigned(ogf) << " | ocf : " << unsigned(ocf) << endl;
					break;
				}
				case HCI_CMD_OGF_TESTING_COMMANDS:
				{
					cout << "[NOT DECODED] ogf : " << unsigned(ogf) << " | ocf : " << unsigned(ocf) << endl;
					break;
				}
				case HCI_CMD_OGF_LE_CONTROLLER_COMMANDS:
				{
					switch (ocf){

						case HCI_CMD_OCF_LE_SET_SCAN_PARAMETERS_COMMAND	:
						{
							frame_list.push_back(new le_set_scan_parameters_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_LE_SET_SCAN_ENABLE_COMMAND:
						{
							frame_list.push_back(new le_set_scan_enable_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_LE_SET_ADVERTISING_PARAMETERS_COMMAND:
						{
							frame_list.push_back(new le_set_advertising_parameters_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_LE_CLEAR_WHITE_LIST_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_LE_CONTROLLER_COMMANDS,HCI_CMD_OCF_LE_CLEAR_WHITE_LIST_COMMAND));
							break;
						}
						case HCI_CMD_OCF_LE_READ_WHITE_LIST_SIZE_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_LE_CONTROLLER_COMMANDS,HCI_CMD_OCF_LE_READ_WHITE_LIST_SIZE_COMMAND));
							break;
						}
						case HCI_CMD_OCF_LE_SET_ADVERTISING_DATA_COMMAND:
						{
							frame_list.push_back(new le_set_advertising_data_cmd_t(data));
							break;
						}
						case HCI_CMD_OCF_LE_READ_BUFFER_SIZE_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_LE_CONTROLLER_COMMANDS,HCI_CMD_OCF_LE_READ_BUFFER_SIZE_COMMAND));
							break;
						}
						case HCI_CMD_OCF_LE_READ_LOCAL_SUPPORTED_FEATURES_COMMAND:
						{
							frame_list.push_back(new void_cmd_t(data,HCI_CMD_OGF_LE_CONTROLLER_COMMANDS,HCI_CMD_OCF_LE_READ_LOCAL_SUPPORTED_FEATURES_COMMAND));
							break;
						}
						default:
						{
							cout << "[NOT DECODED] ogf : " << unsigned(ogf) << " | ocf : " << unsigned(ocf) << endl;
						}
					}
					break;
				}
				case HCI_CMD_OGF_VENDOR_SPECIFIC: 
				{
					frame_list.push_back(new vendor_specific_cmd_t(data));
					break;
				}
				default:
				{
					cout << "[NOT DECODED] ogf : " << unsigned(ogf) << " | ocf : " << unsigned(ocf) << endl;
				}
			}
			break;
		}
		case HCI_TYPE_ACL_DATA:
		{
			cout << "[NOT DECODED] HCI_TYPE_ACL_DATA" << endl;
			break;
		}
		case HCI_TYPE_SCO_DATA:
		{
			cout << "[NOT DECODED] HCI_TYPE_SCO_DATA" << endl;
			break;
		}
		case HCI_TYPE_EVENT:
		{
			switch(data[1]) {

				case HCI_EVENT_COMMAND_COMPLETE:
				{
					frame_list.push_back(new CommandComplete(data));
					break;
				}
				case HCI_EVENT_LE_META:
				{
					le_meta_event event(data);

					switch (event.subevent_code){
						case HCI_EVENT_LE_CONNECTION_COMPLETE:
						{
							frame_list.push_back(new le_meta_connection_complete_event(data));
							break;
						}
						case HCI_EVENT_LE_ADVERTISING_REPORT:
						{
							frame_list.push_back(new le_meta_advertising_report_event(data));
							break;
						}
						case HCI_EVENT_LE_CONNECTION_UPDATE_COMPLETE:
						{
							break;
						}
						case HCI_EVENT_LE_READ_REMOTE_USED_FEATURES_COMPLETE:
						{
							break;
						}
						case HCI_EVENT_LE_LONG_TERM_KEY_REQUEST:
						{
							break;
						}
						case HCI_EVENT_LE_REMOTE_CONNECTION_PARAMETER_REQUEST:
						{
							break;
						}
						case HCI_EVENT_LE_DATA_LENGTH_CHANGE:
						{
							break;
						}
						case HCI_EVENT_LE_READ_LOCAL_P256_PUBLIC_KEY_COMPLETE:
						{
							break;
						}
						case HCI_EVENT_LE_GENERATE_DHKEY_COMPLETE:
						{
							break;
						}
						case HCI_EVENT_LE_ENHANCED_CONNECTION_COMPLETE:
						{
							break;
						}
						case HCI_EVENT_LE_DIRECT_ADVERTISING_REPORT:
						{
							break;
						}
						default:
							cout << "[NOT DECODED] HCI_EVENT : " << data[1] << endl;
							break;
					}
					break;
				}
				default:
				{
					cout << "[NOT DECODED] HCI_EVENT : " << data[1] << endl;
				}
			}
			break;
		}
		default:
		{
			cout << "[NOT DECODED]" << endl;
		}
	}

	return frame_list;
}