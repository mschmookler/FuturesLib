/*
FuturesLib is a package for interacting with futures and options on futures.

Version 0.1.0

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

/*!
 * \file
 * \brief FuturesLib Structures
 * POD structs that form the building blocks of the library.
 */

#ifndef FUTURESLIB_STRUCTURES_H
#define FUTURESLIB_STRUCTURES_H

#include <string>

#include "C:\Users\mschmookler\boost\boost_1_69_0\boost\date_time\posix_time\posix_time.hpp"
#include "C:\Users\mschmookler\boost\boost_1_69_0\boost\date_time\gregorian\gregorian_types.hpp"

/*!
\brief POD struct for product specs.

Contract specs that are invariant between contracts.
e.g. contract size, currency, etc. Uniquely determined
by tag 1151-SecurityGroup and tag 762-SecuritySubType.
*/
struct ProductSpecs
{
	/*! \brief Default constructor */
	ProductSpecs() = default;

	/*! \brief Main constructor */
	ProductSpecs(std::string exch, int p_complex, int ms_id,
			std::string sec_group, std::string sec_type,
			std::string curr, char match_algo,
			float uomq, float mpi, float df) :
		exchange(exch),
		product_complex(p_complex),
		mkt_seg_id(ms_id),
		security_group(sec_group),
		security_type(sec_type),
		currency(curr),
		match_algorithm(match_algo),
		unit_of_measure_qty(uomq),
		min_price_increment(mpi),
		display_factor(df){}

	//! \brief tag 207-SecurityExchange
	//!
	//! Exchange code: XCME, XCBT, XNYM, XCEC, etc.
	//! Limit to 8 chars.
	std::string exchange = "";

	//! \brief tag 462-UnderlyingProduct
	//!
	//! Aka product complex or asset class.
	int product_complex = 0;

	//! \brief tag 1300-MarketSegmentID
	//!
	int mkt_seg_id = 0;

	//! \brief tag 1151-Security Group
	//!
	//! Exchange symbol for the given product group.
	//! Not to be confused with tag 6937 = Asset.
	//! Ex. ES, ZN, ZC, ZE (GE Opt), GE
	//! Corresponds to tag 55 = Symbol for iLink Order Entry
	//! Limit to 8 chars.
	std::string security_group = "";

	//! \brief tag 762-SecuritySubType
	//!
	//! Indicates spread or combo type.
	//! Ex. SP, BF, DF, CF, VT
	//! Limit to 4 chars.
	std::string security_type = "";

	//! \brief tag 15-Currency
	//!
	//! Currency used in price.
	//! Limit to 4 chars.
	std::string currency = "";

	//! \brief tag 1142-MatchAlgorithm
	//!
	//! CME GLOBEX assigned values:
	//! F, K, C, A, T, O, S, Q, Y
	char match_algorithm = '\0';

	//! \brief tag 1147-UnitOfMeasureQty
	//!
	//! Contract size for each instrument. Use with tag 996-UOM.
	float unit_of_measure_qty = 0;

	//! \brief tag 9787-DisplayFactor
	//!
	//! Muliplier to convert display price to conventional price.
	//! Ex. ES: 0.010000000. ZT: 1.000000000
	float display_factor = 0;

};

/*!
\brief POD struct for contract specs.

Instrument details that may vary between contracts of the same
group and strategy. e.g. expiration date, tick size. Does not
include unique identifiers of a contract/instrument.
*/
struct ContractSpecs
{
	/* \brief Default constructor */
	ContractSpecs() = default;

	/* \brief Main constructor */
	ContractSpecs(std::string mmy, float mpi, bool ifp,
				  uint32_t mf, uint32_t sf, int pdf) :
		min_price_increment(mpi),
		is_fractional_price(ifp),
		main_fraction(mf),
		sub_fraction(sf),
		price_display_format(pdf) {}

	//! \brief tag 200-MaturityMonthYear
	//!
	//! TODO: change type to MonthYear type?
	//! Limit to 16 chars.
	string matruity_month_year = "";
	
	//! \brief tag 969-MinPriceIncrement
	//!
	//! Minimum tick value.
	float min_price_increment = 0;

	//! \brief True if price ticks in fractions. 
	//!
	//! Ex. notes, bonds, grains
	//! TODO: Store tag 872 in uint32_t and replace
	//! this var with method that checks bit value.
	bool is_fractional_price = false;

	//! \brief tag 37702-MainFraction
	//!
	//! Denominator of main fraction. Ex. 32 for notes, bonds
	unsigned int main_fraction = 0;

	//! \brief tag 37703-SubFraction
	//!
	//! Denominator of sub fraction. Ex. 8 for ZT, 4 for ZF, 2 for ZN
	unsigned int sub_fraction = 0;

	//! \brief tag 9800-PriceDisplayFormat
	//!
	//! Number of digits to the right of tick mark or location of
	//! tick mark between whole and non-whole numbers.
	int price_display_format = 0;

	//! \brief tag 865 = 5(Activation)
	//!
	//! Contract activation datetime
	boost::posix_time::ptime activation_time = not_a_date_time;

	//! \brief tag 865 = 7(Expiration)
	//!
	//! Contract expiration datetime
	boost::posix_time::ptime expiration_time = not_a_date_time;
};

/*!
\brief POD struct for instrument identifiers
*/
struct InstrumentIDs
{
	/* \brief Default constructor */
	InstrumentIDs() = default;

	/* \brief Main constructor */
	InstrumentIDs(int tag48, std::string tag55) :
		security_id(tag48),
		instrument_symbol(tag55) {}

	//! \brief tag 48-SecurityID
	int security_id = 0;

	/** \brief tag 55-Symbol
	 *
	 *  ASCII string that uniquely identifies an instrument.
	 *  e.g. ESM9, ZCN9-ZCZ9, GE:BF H9-M9-U9
	 *  Corresponds to tag 107 = SecurityDesc for iLink Order Entry
	 *  Limit to 24 chars.
	 */
	string instrument_symbol = "";
};

struct OptionSpecs
{
	/* \brief Default constructor */
	OptionSpecs() = default;

	/* \brief Main constructor */
	OptionSpecs();

	//! \brief tag 201-PutOrCall
	//!
	//! '0' = Put, '1' = Call
	char put_call = '\0';

	//! \brief tag 202-StrikePrice
	float strike_price = 0;

	//! \brief tag 9779-UserDefinedInstrument
	//!
	//! 'Y' or 'N'
	char is_UDS = '\0';

	/** \brief tag 6350-TickRule
	 *
	 *  Valid values: 1, 2, 3, 4, 10, 11, 12
	 *  see https://www.cmegroup.com/confluence/display/EPICSANDBOX/MDP+3.0+Variable+Tick+Table
	 */
	int tick_rule = 0;
};

/*! \brief POD struct for session statistics
 *
 *
 *
 */
struct SessionStats
{
	double open;
	double close;
	double low;
	double high;
	double settlement;
	uint32_t volume;
	uint32_t open_interest;
};

struct RealTimeData
{
	
	double bid;
	double ask;
	uint32_t bid_qty;
	uint32_t ask_qty;
	uint32_t imp_bid_qty;
	uint32_t imp_ask_qty;
	uint32_t last_trade;
	uint32_t last_trade_qty;
	double change_last_trade;
	boost::posix_time::ptime last_trade_time;
};

struct ESPrice_t
{
	double price = 0.0;
	double tick = 25.0;
};

namespace pcomplex {
	enum {
		CommodityAg = 2,
		Currency = 4,
		Equity = 5,
		Other = 12,
		InterestRate = 14,
		FXCash = 15,
		Energy = 16,
		Metals = 17
	};
}

#endif // FUTURESLIB_STRUCTURES_H