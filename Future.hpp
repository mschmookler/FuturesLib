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

#ifndef FUTURESLIB_FUTURE_H
#define FUTURESLIB_FUTURE_H

#include <iostream>
#include <fstream>
#include "Structures.hpp"
#include "FixRead.hpp"
#include "hffix.hpp"

#ifndef NO_BOOST_DATETIME
#ifdef DATE_TIME_TIME_HPP___
#ifdef DATE_TIME_DATE_HPP___
#define BOOST_DATETIME
#endif
#endif
#endif

/*! \brief One futures contract.
 *
 *  This object is mainly an interface. It is constructed by
 *  FutureFactory, while the data is stored in component structs.
 */
class FuturesContract
{
public:
	/*!
	\brief Default constructor
	*/
	FuturesContract() = default;

	ProductSpecs* get_PSpecs()
	{
		return &product;
	}

	ContractSpecs* get_CSpecs()
	{
		return &contract;
	}

	InstrumentID* get_IID()
	{
		return &iid;
	}

	friend class FutureFactory;

protected:
private:
	ProductSpecs product;
	ContractSpecs contract;
	InstrumentID iid;
	
};

/*! \brief Class for creating FuturesContracts
 *
 *  This class contains methods for constructing FuturesContracts
 *  and other ancillary operations pertaining to the secdef file
 *  published by Globex. Secdef functions will be separated out shortly.
 */
class FutureFactory
{
public:
	FutureFactory() = default;

	/*!
	\brief Construct a FuturesContract by secdef lookup
	
	\param[in] product_symbol Tag 55-Symbol for the future instrument
	\param[in] secdef_filename Full path to secdef file to use
	\param[out] &future Pointer to the constructed object
	*/
	FuturesContract* FutureFromSecdef(std::string product_symbol, std::string secdef_filename)
	{
		FuturesContract future;

		std::ifstream myfile(secdef_filename);
		std::string mystr;
		// Loop through lines in file.
		while (std::getline(myfile, mystr))
		{
			std::size_t start_of_tag_field = mystr.find("\x01\x35\x35\x3D");	// "\x01" + "55="
			if (start_of_tag_field != std::string::npos)
			{
				if (mystr.substr(start_of_tag_field + 4, product_symbol.length() + 1) == product_symbol + "\x01")
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
						future.product.mkt_seg_id = i->value().as_int<int>();
						i++;
					}
					if (mr.find_with_hint(hffix::tag::UnderlyingProduct, i))
					{
						std::cout << "complex found!" << '\n';
						future.product.product_complex = i->value().as_int<int>();
						i++;
					}
					if (mr.find_with_hint(hffix::tag::SecurityExchange, i))
					{
						//product.exchange.append(i->value().begin(), i->value().end() - i->value().begin());
						strncpy_s(future.product.exchange, i->value().begin(), i->value().size());
						i++;
					}
					if (mr.find_with_hint(hffix::tag::SecurityID, i))
					{
						future.iid.security_id = i->value().as_int<int>();
						i++;
					}
					if (mr.find_with_hint(hffix::tag::MaturityMonthYear, i))
					{
						strncpy_s(future.contract.matruity_month_year, i->value().begin(), i->value().size());
						i++;
					}
				}
			}
		}

		return &future;
	}

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
		flf::FilterSecdef("H:\\secdef\\secdef_fut.txt", filename1, 6937, product_symbol);

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

	OptionsContract() = default;

	InstrumentID* get_underlying_IID() {
		return &underlying_iid;
	}

protected:
private:
	InstrumentID underlying_iid;

};*/
#endif // FUTURESLIB_FUTURE_H