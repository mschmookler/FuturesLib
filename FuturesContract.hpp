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

#ifndef FUTURESLIB_FUTURESCONTRACT_H
#define FUTURESLIB_FUTURESCONTRACT_H

#include "FuturesProduct.hpp"
#include "C:\Users\mschmookler\boost\boost_1_69_0\boost\date_time\posix_time\posix_time.hpp"
#include "C:\Users\mschmookler\boost\boost_1_69_0\boost\date_time\gregorian\gregorian_types.hpp"


#ifndef NO_BOOST_DATETIME
#ifdef DATE_TIME_TIME_HPP___ // Header include guard from boost/date_time/time.hpp
#ifdef DATE_TIME_DATE_HPP___ // Header include guard from boost/date_time/date.hpp
#define BOOST_DATETIME
#endif
#endif
#endif

using namespace boost::posix_time;
using namespace boost::gregorian;

// Struct to store data for one futures contract.
// Inherits from FuturesProduct, adding the necessary fields.
struct FuturesContract : public FuturesProduct
{
public:
	// Default Constructor
	FuturesContract() = default;


	// Main Constructor
	FuturesContract(string tag55, int tag48,
		ptime activation, ptime expiration, ptime last_update) :
		instrument_symbol(tag55),
		security_id(tag48),
		activation_time(activation),
		expiration_time(expiration),
		last_update_time(last_update) {}

	// Construct from FuturesProduct
	FuturesContract(FuturesProduct product, string tag55, int tag48,
		ptime activation, ptime expiration, ptime last_update) :
		FuturesProduct(product.GetExchange(), product.GetComplex(), product.GetMktSegID(),
			product.GetSecurityGroup(), product.GetSecurityType(), product.GetCurrency(),
			product.GetMatchAlgo(), product.GetUOMQ(), product.TickSize(), product.GetDF()),
		instrument_symbol(tag55),
		security_id(tag48),
		activation_time(activation),
		expiration_time(expiration),
		last_update_time(last_update){}

	// Construct from secdef text/dat file.
	// Takes a string containing the security definition for a
	// futures contract, parses the string, and constructs the
	// proper object.
	FuturesContract(string secdef_msg)
	{
		// TODO: Rewrite this whole constructor using a single char pointer and hffix methods
		size_t begin, end;
		string q;
		if (secdef_msg[3] != 'd') {		// 35=d => SecurityDefinition
			FuturesContract();			// default constructor
		}
		else {
			// Set tag 1300-MarketSegmentID
			q = '\x01' + "1300=";
			begin = secdef_msg.find(q) + 6;							// sizeof("\x011300=") = 6
			SetMktSegID(std::stoi(secdef_msg.substr(begin, 2)));	// Always 2 digits

			// Set tag 462-UnderlyingProduct
			q = '\x01' + "462=";
			begin = secdef_msg.find(q, begin) + 5;					// points to start of field
			end = secdef_msg.find('\x01', begin);					// points to next SOH char
			SetComplex(std::stoi(secdef_msg.substr(begin, begin - end)));

			// Set tag 207-SecurityExchange
			q = '\x01' + "207=";
			begin = secdef_msg.find(q, end) + 5;
			end = secdef_msg.find('\x01', begin);
			SetExchange(secdef_msg.substr(begin, begin - end));

			// Set tag 1151-SecurityGroup
			q = '\x01' + "1151=";
			begin = secdef_msg.find(q, end) + 6;
			end = secdef_msg.find('\x01', begin);
			SetSecurityGroup(secdef_msg.substr(begin, begin - end));

			// Set tag 55-Symbol
			q = '\x01' + "55=";
			begin = secdef_msg.find(q, end) + 4;
			end = secdef_msg.find('\x01', begin);
			SetExchange(secdef_msg.substr(begin, begin - end));
			
			// Set tag 48-SecurityID
			q = '\x01' + "48=";
			begin = secdef_msg.find(q, end) + 4;
			end = secdef_msg.find('\x01', begin);
			SetSecurityID(std::stoi(secdef_msg.substr(begin, begin - end)));

			// Set tag 15-Currency
			q = '\x01' + "15=";
			begin = secdef_msg.find(q, end) + 4;
			end = secdef_msg.find('\x01', begin);
			SetCurrency(secdef_msg.substr(begin, begin - end));

			// Set tag 1142-MatchAlgorithm
			q = '\x01' + "1142=";
			begin = secdef_msg.find(q, end) + 6;
			SetMatchAlgo(secdef_msg[begin]);

			// Set tag 969-MinPriceIncrement
			q = '\x01' + "969=";
			begin = secdef_msg.find(q, end) + 5;
			end = secdef_msg.find('\x01', begin);
			SetMPI(std::stof(secdef_msg.substr(begin, begin - end)));

			// Set Activation Time
			q = '\x01' + "865=5";
			begin = secdef_msg.find(q, end) + 12;		// "\x01 865=5 \x01 1145="
			end = secdef_msg.find('\x01', begin);
			string temp = secdef_msg.substr(begin, begin - end);
			temp.insert(begin + 8, 1, 'T');		// Add 'T' for iso form
			SetActivation(from_iso_string(temp));

			// Set Expiration Time
			q = '\x01' + "865=7";
			begin = secdef_msg.find(q, end) + 12;		// "\x01 865=7 \x01 1145="
			end = secdef_msg.find('\x01', begin);
			temp = secdef_msg.substr(begin, begin - end);
			temp.insert(begin + 8, 1, 'T');		// Add 'T' for iso form
			SetExpiration(from_iso_string(temp));

			SetLastUpdateTime(second_clock::universal_time());
		}
	}


	string GetInstrumentSymbol() {
		return instrument_symbol;
	}

	int GetSecurityID() {
		return security_id;
	}

	float GetCalendarDTE() {
		time_period tp(second_clock::universal_time(), expiration_time);
		return (float)(tp.length().hours() / 24);
	}

	ptime GetActivation() {
		return activation_time;
	}

	ptime GetExpiration() {
		return expiration_time;
	}

	ptime LastUpdateTime() {
		return last_update_time;
	}

	// TODO: GetTradingDTE()

protected:

	void SetInstrumentSymbol(string s) {
		instrument_symbol = s;
	}

	void SetSecurityID(int i) {
		security_id = i;
	}

	void SetActivation(ptime pt) {
		activation_time = pt;
	}

	void SetExpiration(ptime pt) {
		expiration_time = pt;
	}

	void SetLastUpdateTime(ptime pt) {
		last_update_time = pt;
	}

private:
	// tag 55 = Symbol
	// Exchange symbol for a specific futures contract (instrument).
	// Ex. ESH9, ZNM9, ZCN9-ZCZ9, GE:BF H9-M9-U9
	// Corresponds to tag 107 = SecurityDesc for iLink Order Entry
	// char instrument_symbol[16];
	string instrument_symbol = "";

	// tag 48 = SecurityID
	int security_id = 0;

	// tag 200 = MaturityMonthYear
	// TODO: change type to MonthYear type?
	// char maturity_month_year[16] = {};
	string matruity_month_year = "";

	// tag 865 = 5(Activation)
	// Contract activation datetime
	// Type date_time
	ptime activation_time = not_a_date_time;

	// tag 865 = 7(Expiration)
	// Contract expiration datetime
	// Type date_time
	ptime expiration_time = not_a_date_time;

	// tag779 = LastUpdateTime
	// UTC Timestamp
	ptime last_update_time = second_clock::local_time();
};
/*
struct OptionsContract : public FuturesContract
{
public:

	string GetUnderlyingSymbol() {
		return underlying_symbol;
	}

	int GetUnderlyingID() {
		return underlying_id;
	}

protected:
private:
	// tag 201 = PutOrCall
	char put_call = '\0';

	//tag 202 = StrikePrice
	float strike_price = 0;

	// tag 9779 = UserDefinedInstrument
	// 'Y' or 'N'
	char is_UDS = '\0';

	// tag 6350 = TickRule
	int tick_rule = 0;

	// tag 55 of underlying
	string underlying_symbol = "";

	// tag 48 of underlying
	int underlying_id = 0;
};*/
#endif // FUTURESLIB_FUTURESCONTRACT_H