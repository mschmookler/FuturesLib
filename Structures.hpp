/*
FuturesLib is a package for interacting with futures and options on futures.

Version 0.2.0

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
 * FuturesLib Structures
 * POD structs that form the building blocks of the library.
 */

#ifndef FUTURESLIB_STRUCTURES_H
#define FUTURESLIB_STRUCTURES_H

#include <boost\date_time\posix_time\posix_time.hpp>
#include <boost\date_time\gregorian\gregorian_types.hpp>

/*! \brief POD struct for product specifications.
 *
 *  Instrument details that are invariant between contracts
 *  of the same type. e.g. contract size, currency, etc. Uniquely
 *  determined by tag 6937-Asset and tag 762-SecuritySubType.
 */
struct ProductSpecs
{
	/*!
	\brief Default constructor
	*/
	ProductSpecs() = default;

	/*!
	\brief tag 207-SecurityExchange
	
	Exchange code: XCME, XCBT, XNYM, XCEC, etc.
	*/
	char exchange[8] = { '\0' };


	/*!
	\brief tag 462-UnderlyingProduct
	
	Aka product complex or asset class. Valid values
	given in enum pcomplex.
	*/
	int product_complex = 0;

	/*!
	\brief tag 1300-MarketSegmentID
	*/
	int mkt_seg_id = 0;

	/*!
	\brief tag 1151-SecurityGroup

	Exchange symbol for the given product group.
	A product group can contain multiple related products.
	Ex. symbols ZL, ZM, and ZS all belong to the ZS group.
	*/
	char security_group[8] = { '\0' };

	/*!
	\brief tag 6937-Asset
	
	Exchange symbol for the given product.
	Not to be confused with tag 1151-SecurityGroup.
	e.g. ES, ZN, ZC, ZE (GE Opt), GE
	Corresponds to tag 55 = Symbol for iLink Order Entry
	*/
	char product_symbol[8] = { '\0' };

	/*! \brief tag 762-SecuritySubType
	
	Indicates spread or combo type.
	Ex. SP, BF, DF, CF, VT
	*/
	char security_type[4] = { '\0' };

	/*!
	\brief tag 15-Currency
	
	Currency used in price.
	*/
	char currency[4] = { '\0' };

	/*!
	\brief tag 1142-MatchAlgorithm
	
	CME GLOBEX assigned values:
	F, K, C, A, T, O, S, Q, Y
	*/
	char match_algorithm = '\0';

	/*!
	\brief tag 1147-UnitOfMeasureQty
	
	Contract size for each instrument. Use with tag 996-UOM.
	*/
	float unit_of_measure_qty = 0;

	/*!
	\brief tag 9787-DisplayFactor
	
	Muliplier to convert display price to conventional price.
	Ex. ES: 0.010000000. ZT: 1.000000000
	*/
	float display_factor = 0;

};

/*! \brief POD struct for contract specs.
 *
 *  Instrument details that may vary between contracts of the same
 *  group and strategy. e.g. expiration date, tick size. Does not
 *  include unique identifiers of a contract/instrument.
 */
struct ContractSpecs
{
	/*!
	\brief Default constructor
	*/
	ContractSpecs() = default;

	/*!
	\brief tag 200-MaturityMonthYear
	
	TODO: change type to MonthYear type?
	Limited to 15 chars.
	*/
	char matruity_month_year[16] = { '\0' };

	/*!
	\brief tag 969-MinPriceIncrement
	
	Minimum tick value.
	*/
	float min_price_increment = 0;

	/*!
	\brief True if price ticks in fractions. 
	
	Ex. notes, bonds, grains
	TODO: Store tag 872 in uint32_t and replace
	this var with method that checks bit value.
	*/
	bool is_fractional_price = false;

	/*!
	\brief tag 37702-MainFraction
	
	Denominator of main fraction. Ex. 32 for notes, bonds
	*/
	unsigned int main_fraction = 0;

	/*!
	\brief tag 37703-SubFraction
	
	Denominator of sub fraction. Ex. 8 for ZT, 4 for ZF, 2 for ZN
	*/
	unsigned int sub_fraction = 0;

	/*!
	\brief tag 9800-PriceDisplayFormat
	
	Number of digits to the right of tick mark or location of
	tick mark between whole and non-whole numbers.
	*/
	int price_display_format = 0;

	/*!
	\brief tag 865 = 5(Activation)
	
	Contract activation datetime
	*/
	boost::posix_time::ptime activation_time = boost::posix_time::not_a_date_time;

	/*!
	\brief tag 865 = 7(Expiration)
	
	Contract expiration datetime
	*/
	boost::posix_time::ptime expiration_time = boost::posix_time::not_a_date_time;
};

/*! \brief POD struct for instrument identifiers
 *
 *
 */
struct InstrumentID
{
	/*!
	\brief Default constructor
	*/
	InstrumentID() = default;

	/*!
	\brief tag 48-SecurityID
	*/
	int security_id = 0;

	/*!
	\brief tag 55-Symbol
	
	ASCII string that uniquely identifies an instrument.
	e.g. ESM9, ZCN9-ZCZ9, GE:BF H9-M9-U9
	Corresponds to tag 107 = SecurityDesc for iLink Order Entry
	Limit to 24 chars.
	*/
	char instrument_symbol = { '\0' };
};

struct OptionSpecs
{
	/*!
	\brief Default constructor
	*/
	OptionSpecs() = default;

	/*!
	\brief tag 201-PutOrCall
	
	'0' = Put, '1' = Call 
	*/
	char put_call = '\0';

	/*!
	\brief tag 202-StrikePrice
	*/
	float strike_price = 0;

	/*!
	\brief tag 9779-UserDefinedInstrument
	
	'Y' or 'N'
	*/
	char is_UDS = '\0';

	/*!
	\brief tag 6350-TickRule
	
	Valid values: 1, 2, 3, 4, 10, 11, 12
	see https://www.cmegroup.com/confluence/display/EPICSANDBOX/MDP+3.0+Variable+Tick+Table
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

enum order_duration {
	DAY, GTC, GTD
};

struct Order
{
	char account[12];
	char buy_sell;
	uint32_t quantity;
	InstrumentID* piid;

	// Mkt = '1', Limit = '2',
	// Stop = '3', STL = '4',
	// Market-Limit = 'K'
	char order_type;
	
	double limit_price;
	double stop_price;
	order_duration duration = DAY;
	
	// Add various order ID's?
	// Or put in separate struct
};

// IDK where to put this yet
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