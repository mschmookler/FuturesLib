#include "FuturesContract.hpp"
#include "J:/SCHMOOKLER/src/hffix.hpp"

int g_seq_num = 1;
int g_ord_num = 12300001;
char buffer[1 << 12];
ptime send_time;

// User/Session settings
int MyAccount = 12345678;
string MyCompID = "XYZ6N2N";
string MyTag50 = "mschmookler";
string MyTag142 = "US,IL";


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
	mw.push_back_int(hffix::tag::TargetSubID, instrument.GetMktSegID);
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

	return 0;
}