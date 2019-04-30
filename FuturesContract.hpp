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

#include <iostream>
#include <fstream>
#include "Structures.hpp"
#include "FixFunctionsR.hpp"
#include "J:\SCHMOOKLER\src\hffix.hpp"

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
	FuturesContract(std::string globex_symbol, std::string secdef_file)
	{
		std::ifstream myfile(secdef_file);
		std::string mystr;
		// Loop through lines in file.
		while (std::getline(myfile, mystr))
		{
			std::size_t start_of_tag_field = mystr.find("\x01\x35\x35\x3D");	// "\x01" + "55="
			if (start_of_tag_field != std::string::npos)
			{
				if (mystr.substr(start_of_tag_field + 4, globex_symbol.length() + 1) == globex_symbol + "\x01")
				{
					std::cout << "Symbol found!" << '\n';
					char buffer[1 << 11];
					hffix::message_writer mw(buffer, buffer + sizeof(buffer));
					mw.push_back_header("FIX.4.2");
					mw.push_back_string(hffix::tag::MsgType, mystr.substr(3));
					mw.push_back_trailer();
					hffix::message_reader mr(mw);
					hffix::message_reader_const_iterator i = mr.begin();
					if (mr.find_with_hint(hffix::tag::MarketSegmentID, i))
					{
						std::cout << "mkt_seg found!" << '\n';
						product.mkt_seg_id = i->value().as_int<int>();
						i++;
					}
					if (mr.find_with_hint(hffix::tag::UnderlyingProduct, i))
					{
						std::cout << "complex found!" << '\n';
						product.product_complex = i->value().as_int<int>();
						i++;
					}
					if (mr.find_with_hint(hffix::tag::SecurityExchange, i))
					{
						//product.exchange.append(i->value().begin(), i->value().end() - i->value().begin());
						strncpy_s(product.exchange, i->value().begin(), i->value().size());
						i++;
					}
					if (mr.find_with_hint(hffix::tag::SecurityGroup, i))
					{
						product.security_group.append(i++->value().begin(), i->value().size());
						//i++;
					}
					if (mr.find_with_hint(hffix::tag::SecurityID, i))
					{
						iid.security_id = i->value().as_int<int>();
						i++;
					}
					if (mr.find_with_hint(hffix::tag::MaturityMonthYear, i))
					{
						contract.matruity_month_year.append(i->value().begin(), i->value().size());
						i++;
						std::cout << product.mkt_seg_id << '\n';
						std::cout << product.product_complex << '\n';
						std::cout << product.exchange << '\n';
						std::cout << product.security_group << '\n';
						std::cout << iid.security_id << '\n';
						std::cout << contract.matruity_month_year << '\n';
					}
				}
			}
		}
	}

	friend class FutureFactory;

protected:
private:
	ProductSpecs product;
	ContractSpecs contract;
	InstrumentIDs iid;
	
};

/*! \brief Class for creating FuturesContracts
 *
 *  This class contains methods for constructing FuturesContracts
 *  and other ancillary operations pertaining to the secdef file
 *  published by Globex.
 */
class FutureFactory
{
public:
	FutureFactory() = default;

	
	/*! \brief Generates text files that contain filtered versions of secdef flat file.
	 *
	 *  The process starts on secdef_fut.txt. First, we filter out all instruments that
	 *  belong to the specific security group. Next, we filter by strategy type i.e. SP,
	 *  BF, DF, CF, etc.
	 */
	void GenerateTextFiles(std::string product_symbol, std::string strategy_type)
	{
		// product symbol file
		std::string filename1 = "H:\\secdef\\secdef_fut_" + product_symbol + ".txt";

		// symbol + strategy file
		std::string filename2 = "H:\\secdef\\secdef_fut_" + product_symbol + "_" + strategy_type + ".txt";

		// Filter specific product from all futures
		flf::FilterSecdef("H:\\secdef\\secdef_fut.txt", filename1, 1151, product_symbol);

		if (strategy_type == "OUT")
		{
			flf::FilterSecdef(filename1, filename2, 762, "");
		}
		else {
			flf::FilterSecdef(filename1, filename2, 762, strategy_type);
		}
	}

private:
	std::vector<std::string> list_of_contracts;


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