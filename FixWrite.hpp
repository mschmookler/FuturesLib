/*
FuturesLib is a package for interacting with futures and options on futures.

Version 0.0.0

Copyright(C) 2019 Matthew A Schmookler

This program is free software : you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License v3 as published
by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef FUTURESLIB_FIXFUNCTIONSW_H
#define FUTURESLIB_FIXFUNCTIONSW_H

#include "FuturesContract.hpp"
#include "J:/SCHMOOKLER/src/hffix.hpp"
#include <fstream>		// For writing log file

int g_seq_num = 1;
int g_ord_num = 12300001;
char buffer[1 << 12];
ptime send_time;

// User/Session settings
int MyAccount = 12345678;
std::string MyCompID = "XYZ6N2N";
std::string MyTag50 = "mschmookler";
std::string MyTag142 = "US,IL";
std::string MyTag1603 = "FLIB";
std::string MyTag1604 = "1.0";
std::string MyTag1605 = "MAS";

// Log file
std::string LogFile = "H:\\Log.csv";

// Timestamp converter
// Takes a ptime and returns timestamp of the form
// "YYYYMMDD-HH:MM:SS.sss"
std::string to_UTC_timestamp(ptime pt)
{
	std::string temp1 = to_iso_string(pt);
	std::string temp2 = to_simple_string(pt);
	return (temp1.substr(0, 8) + "-" + temp2.erase(0, 12));
}

/*string calculateHMAC(string &key, string&canonicalRequest){
	return encodedHmac;
}*/

// Places a Logon message (Session Layer) in the buffer and
// constructs canonical FIX msg used to generate HMAC
int Logon_msgw(int MSGW_id)
{
	// Timestamp
	send_time = microsec_clock::universal_time();
	
	// tag 34
	std::string canonical = std::to_string(g_seq_num);
	canonical.push_back('\n');
	// tag 49
	canonical.append(MyCompID);
	canonical.push_back('\n');
	// tag 50
	canonical.append(MyTag50);
	canonical.push_back('\n');
	// tag 52
	canonical.append(to_UTC_timestamp(send_time));
	canonical.push_back('\n');
	// tag 57
	canonical.append(std::to_string(MSGW_id));
	canonical.push_back('\n');
	// tag 108
	canonical.append("60");
	canonical.push_back('\n');
	// tag 142
	canonical.append(MyTag142);
	canonical.push_back('\n');
	/* tag 369		// Optional tag
	canonical.append("60");
	canonical.push_back('\n'); */
	// tag 1603-ApplicationSystemName
	canonical.append(MyTag1603);	// FuturesLib
	canonical.push_back('\n');
	// tag 1604-ApplicationSystemVersion
	canonical.append(MyTag1604);
	canonical.push_back('\n');
	// tag 1605-ApplicationSystemVendor
	canonical.append(MyTag1605);

	// HMAC signature
	// string encrypted_password = calculateHMAC(key, canonical);

	// Create mw
	hffix::message_writer mw(buffer, buffer + sizeof(buffer));

	// Begin header
	mw.push_back_header("FIX.4.2");	// BeginString & BodyLength
	mw.push_back_string(hffix::tag::MsgType, "A");
	mw.push_back_int(hffix::tag::MsgSeqNum, g_seq_num);
	mw.push_back_string(hffix::tag::SenderCompID, MyCompID);
	mw.push_back_string(hffix::tag::SenderSubID, MyTag50);
	mw.push_back_timestamp(hffix::tag::SendingTime, send_time);
	mw.push_back_int(hffix::tag::TargetSubID, MSGW_id);
	mw.push_back_int(hffix::tag::HeartBtInt, 60);
	mw.push_back_string(hffix::tag::SenderLocationID, MyTag142);
	// End header

	mw.push_back_string(1603, MyTag1603);
	mw.push_back_string(1604, MyTag1604);
	mw.push_back_string(1605, MyTag1605);

	// tag 354-EncodedTextLen
	// Contains length of AccessKeyID
	// mw.push_back_int(354, MyTag354);

	// tag 355-EncodedText
	// Contains the AccessKeyID
	// mw.push_back_string(355, MyTag355);

	// tag 1400-EncryptedPasswordMethod
	// Contains the AlgorithmID
	// Ex. "CME-1-SHA-256"
	// mw.push_back_string(1400, "CME-1-SHA-256");

	// tag 1401-EncryptedPasswordLen
	// Contains the length of the HMAC signature
	// mw.push_back_int(1401, 34);

	// tag 1402-EncryptedPassword
	// Contains the HMAC signature. Must be encoded as follows:
	// see https://tools.ietf.org/html/rfc4648#section-5
	// mw.push_back_string(1402, encrypted_password);

	return 0;
}

// Places a new limit order message in the buffer
// In the future or possibly in a wrapper, we might want this to return Exchange order id
int NewLimitOrder(const char buy_sell, int order_qty, FuturesContract instrument, std::string price)
{
	// Timestamp
	send_time = microsec_clock::universal_time();

	// Create mw
	hffix::message_writer mw(buffer, buffer + sizeof(buffer));

	// Begin header
	mw.push_back_header("FIX.4.2");	// BeginString & BodyLength
	mw.push_back_string(hffix::tag::MsgType, "D");
	mw.push_back_int(hffix::tag::MsgSeqNum, g_seq_num++);
	mw.push_back_string(hffix::tag::SenderCompID, MyCompID);
	mw.push_back_string(hffix::tag::SenderSubID, MyTag50);
	mw.push_back_timestamp(hffix::tag::SendingTime, send_time);
	mw.push_back_string(hffix::tag::TargetCompID, "CME");
	mw.push_back_int(hffix::tag::TargetSubID, instrument.product.mkt_seg_id);
	mw.push_back_string(hffix::tag::SenderLocationID, MyTag142);
	// End header

	// Begin body
	mw.push_back_int(hffix::tag::Account, MyAccount);
	mw.push_back_int(hffix::tag::ClOrdID, g_ord_num++);
	mw.push_back_int(hffix::tag::OrderQty, order_qty);
	mw.push_back_char(hffix::tag::OrdType, '2');	// 2 = Limit Order
	mw.push_back_string(hffix::tag::Price, price);
	mw.push_back_char(hffix::tag::Side, buy_sell);
	mw.push_back_string(hffix::tag::Symbol, instrument.GetSecurityGroup());
	mw.push_back_timestamp(hffix::tag::TransactTime, send_time);
	mw.push_back_char(hffix::tag::ManualOrderIndicator, 'Y');
	mw.push_back_string(hffix::tag::SecurityDesc, instrument.GetInstrumentSymbol());
	mw.push_back_int(hffix::tag::CustomerOrFirm, 1);
	mw.push_back_char(hffix::tag::CtiCode, '1');
	// End body

	mw.push_back_trailer();

	// For testing only. Replace later.
	std::cout.write(buffer, (mw.message_end() - buffer));
	std::cout << '\n';

	// Logging string.
	std::string log_string = "";
	// 1. Sending timestamp
	log_string.append(to_UTC_timestamp(send_time));
	log_string.push_back(',');
	// 2. Receiving timestamp
	log_string.push_back(',');
	// 3.  Message Direction
	log_string.append("TO CME,");
	// 4.  Operator ID
	log_string.append("mschmookler,");
	// 5.  SMP ID
	log_string.push_back(',');
	// 6.  Account Number
	log_string.append(std::to_string(MyAccount));
	log_string.push_back(',');
	// 7.  Session ID
	log_string.append(MyCompID.substr(3, 3));
	log_string.push_back(',');
	// 8.  Executing Firm ID
	log_string.append(MyCompID.substr(0, 3));
	log_string.push_back(',');
	// 9.  Manual Order Identifier
	log_string.push_back('Y');
	log_string.push_back(',');
	// 10. Message Type
	log_string.push_back('D');
	log_string.push_back(',');
	// 11. Customer Type Indicator
	log_string.push_back('1');
	log_string.push_back(',');
	// 12. Origin
	log_string.push_back('1');
	log_string.push_back(',');
	// 13.
	log_string.push_back(',');
	// 14. Message Link ID
	log_string.append("MessageLinkID,");
	// 15. Order Flow ID
	log_string.append("OrderFlowID,");
	// 16.
	// 17. Instrument Description
	log_string.append(instrument.GetInstrumentSymbol());
	log_string.push_back(',');
	// 18. Market Segment ID
	log_string.append(std::to_string(instrument.GetMktSegID()));
	log_string.push_back(',');
	// 19. Client Order ID
	log_string.append(std::to_string(g_ord_num - 1));
	log_string.push_back(',');
	// 20.
	log_string.push_back(',');
	// 21. Buy/Sell Indicator
	log_string.push_back(buy_sell);
	log_string.push_back(',');
	// 22. Quantity
	log_string.append(std::to_string(order_qty));
	log_string.push_back(',');
	// 23. Limit Price
	log_string.append(price);
	log_string.push_back(',');
	// 24. Stop Price
	log_string.push_back(',');
	// 25. Order Type
	log_string.push_back('2');
	log_string.push_back(',');
	// 26. Order Qualifier
	log_string.push_back('0');
	log_string.push_back(',');
	// 27.
	log_string.push_back(',');
	// 28. Display Quantity
	log_string.push_back(',');
	// 29. Minimum Quantity
	log_string.push_back(',');
	// 30. Country of Origin
	log_string.append(MyTag142.substr(0,2));
	log_string.push_back(',');
	// 31 - 38.
	log_string.append(",,,,,,,,");
	// 39. Cross ID
	log_string.push_back(',');
	log_string.push_back('\n');


	// Write to log file
	std::ofstream log_file(LogFile, std::ios_base::app | std::ios_base::out);
	log_file << log_string;
	log_file.close();

	return 0;
}


#endif // FUTURESLIB_FIXFUNCTIONSW_H