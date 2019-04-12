#include "FuturesContract.hpp"
#include "J:/SCHMOOKLER/src/hffix.hpp"
#include <fstream>		// For writing log file
#include "C:\Users\mschmookler\boost\boost_1_69_0\boost\date_time\posix_time\time_formatters.hpp"

int g_seq_num = 1;
int g_ord_num = 12300001;
char buffer[1 << 12];
ptime send_time;

// User/Session settings
int MyAccount = 12345678;
string MyCompID = "XYZ6N2N";
string MyTag50 = "mschmookler";
string MyTag142 = "US,IL";

// Log file
string LogFile = "H:\\Log.csv";

// Timestamp converter
// Takes a ptime and returns timestamp of the form
// "YYYYMMDD-HH:MM:SS.sss"
string to_UTC_timestamp(ptime pt) {
	string temp1 = to_iso_string(pt);
	string temp2 = to_simple_string(pt);
	return (temp1.substr(0, 8) + temp2.erase(0, 12));
}

// Places a new limit order message in the buffer
// In the future or possibly in a wrapper, we might want this to return Exchange order id
int NewLimitOrder(const char buy_sell, int order_qty, FuturesContract instrument, string price) {
	// Timestamp
	send_time = microsec_clock::universal_time();

	// Create mw
	hffix::message_writer mw(buffer, buffer + sizeof(buffer));

	// Begin header
	mw.push_back_header("FIXT.1.1");	// BeginString & BodyLength
	mw.push_back_string(hffix::tag::MsgType, "D");
	mw.push_back_int(hffix::tag::MsgSeqNum, g_seq_num++);
	mw.push_back_string(hffix::tag::SenderCompID, MyCompID);
	mw.push_back_string(hffix::tag::SenderSubID, MyTag50);
	mw.push_back_timestamp(hffix::tag::SendingTime, send_time);
	mw.push_back_string(hffix::tag::TargetCompID, "CME");
	mw.push_back_int(hffix::tag::TargetSubID, instrument.GetMktSegID());
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
	string log_string = "";
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
	log_string.append(MyTag142);
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