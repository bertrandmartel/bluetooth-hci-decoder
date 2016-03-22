/************************************************************************************
 * The MIT License (MIT)                                                            *
 *                                                                                  *
 * Copyright (c) 2015 Bertrand Martel                                               *
 *                                                                                  * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy     * 
 * of this software and associated documentation files (the "Software"), to deal    * 
 * in the Software without restriction, including without limitation the rights     * 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        * 
 * copies of the Software, and to permit persons to whom the Software is            * 
 * furnished to do so, subject to the following conditions:                         * 
 *                                                                                  * 
 * The above copyright notice and this permission notice shall be included in       * 
 * all copies or substantial portions of the Software.                              * 
 *                                                                                  * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       * 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         * 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      * 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           * 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    * 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN        * 
 * THE SOFTWARE.                                                                    * 
 ************************************************************************************/
/**
	btsnoopmonitor.cpp

	Monitoring implementation of ibtsnooplistener

	@author Bertrand Martel
	@version 0.1
*/

#include "btsnoopmonitor.h"
#include "btsnoop/ibtsnooplistener.h"
#include "btsnoop/btsnoopfileinfo.h"
#include "hci_decoder/hci_global.h"
#include "hci_decoder/hci_event_packet.h"
#include "hci_decoder/IHciEventFrame.h"

using namespace std;

BtSnoopMonitor::BtSnoopMonitor()
{
}

BtSnoopMonitor::~BtSnoopMonitor(){
}

/**
 * @brief
 *      called when a new packet record has been received
 * @param fileInfo
 *      file info object
 * @param packet
 *      snoop packet record object
 */
void BtSnoopMonitor::onSnoopPacketReceived(BtSnoopFileInfo fileInfo,BtSnoopPacket packet){

	//fileheader.printInfo();
	HciDecoder hci_decoder;
	IHciFrame * frame = hci_decoder.decode(packet.getPacketData());

	if (frame!=0){

		if (frame->getPacketType() == HCI_TYPE_EVENT){

			IHciEventFrame* event_frame = dynamic_cast<IHciEventFrame*> (frame);

			if ((event_frame->getEventCode() == HCI_EVENT_LE_META) && (event_frame->getSubEventCode() == HCI_EVENT_LE_ADVERTISING_REPORT)){

				le_meta_advertising_report_event *ad_report = dynamic_cast<le_meta_advertising_report_event*> (frame);

				unsigned int i = 0;
				for (i = 0; i  < ad_report->ad_report_items.size();i++){
					cout << ad_report->ad_report_items[i]->toJson(true) << endl;
				}
				cout << "-----------------------------------" << endl;
			}
		}
	}
    //packet.printInfo();
    //cout << "_________________________" << endl;
}
