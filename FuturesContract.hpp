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

#include "Structures.hpp"


#ifndef NO_BOOST_DATETIME
#ifdef DATE_TIME_TIME_HPP___ // Header include guard from boost/date_time/time.hpp
#ifdef DATE_TIME_DATE_HPP___ // Header include guard from boost/date_time/date.hpp
#define BOOST_DATETIME
#endif
#endif
#endif

using namespace boost::posix_time;
using namespace boost::gregorian;

/*! \brief One outright future.
 *
 *
 *
 */
class FuturesContract
{
public:
	/*! \brief Construct by symbol lookup.
	 *
	 *
	 *
	 *
	 */
protected:
private:
	ProductSpecs product;
	ContractSpecs contract;
	InstrumentIDs iid;
	
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
	

	// tag 55 of underlying
	string underlying_symbol = "";

	// tag 48 of underlying
	int underlying_id = 0;
};*/
#endif // FUTURESLIB_FUTURESCONTRACT_H