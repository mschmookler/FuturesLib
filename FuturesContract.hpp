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
		/*char *str = const_cast<char *>(secdef_msg.c_str());
		char *tok, *next_tok, *sub_tok, *sub_next_tok;
		tok = strtok_s(str, "\x01", &next_tok);
		int tag;
		while (tok != NULL) {
			sub_tok = strtok_s(tok, "=", &sub_next_tok);
			tag = atoint(sub_tok, sub_next_tok - 1);
			switch (tag)
			{
			case 35:

			}
			std::cout << tag << ':' << sub_next_tok << '\n';
			//std::cout << sub_tok << '\t' << sub_next_tok << '\n';
			tok = strtok_s(NULL, "\x01", &next_tok);
		}*/
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