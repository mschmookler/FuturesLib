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
#include "C:\Users\mschmookler\boost\boost_1_69_0\boost\date_time\posix_time\posix_time_types.hpp"
#include "C:\Users\mschmookler\boost\boost_1_69_0\boost\date_time\gregorian\gregorian_types.hpp"


#ifndef NO_BOOST_DATETIME
#ifdef DATE_TIME_TIME_HPP___ // Header include guard from boost/date_time/time.hpp
#ifdef DATE_TIME_DATE_HPP___ // Header include guard from boost/date_time/date.hpp
#define BOOST_DATETIME
#endif
#endif
#endif

// Struct to store data for one futures contract.
// Inherits from FuturesProduct, adding the necessary fields.
struct FuturesContract :FuturesProduct
{
public:
	// TODO: GetTradingDTE()
	// TODO: GetCalendarDTE()
	// TODO: GetTickValue()
	// TODO: GetExpireDatetime()

protected:

private:
	// tag 55 = Symbol
	// Exchange symbol for a specific futures contract (instrument).
	// Ex. ESH9, ZNM9, ZCN9-ZCZ9, GE:BF H9-M9-U9
	// Corresponds to tag 107 = SecurityDesc for iLink Order Entry
	char instrument_symbol[16];

	// tag 48 = SecurityID
	int security_id = 0;

	// tag 200 = MaturityMonthYear
	// TODO: change type to MonthYear type?
	char maturity_month_year[16] = {};

	// tag 865 = 5(Activation)
	// Contract activation datetime
	// Type date_time
	boost::posix_time::ptime activation_time;

	// tag 865 = 7(Expiration)
	// Contract expiration datetime
	// Type date_time
	boost::posix_time::ptime expiration_time;

	// tag779 = LastUpdateTime
	// UTC Timestamp
	boost::posix_time::ptime last_update_time;
};
#endif // FUTURESLIB_FUTURESCONTRACT_H