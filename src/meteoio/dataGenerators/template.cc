/***********************************************************************************/
/*  Copyright 2013 WSL Institute for Snow and Avalanche Research    SLF-DAVOS      */
/***********************************************************************************/
/* This file is part of MeteoIO.
    MeteoIO is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    MeteoIO is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with MeteoIO.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <meteoio/dataGenerators/template.h>

namespace mio {

void TEMPLATE::parse_args(const std::vector<std::string>& vecArgs)
{
	//Get the optional arguments for the algorithm. For example, process 1 argument
	/*if (vecArgs.size()==1) {
		const bool status = IOUtils::convertString(constant, vecArgs[0]);
		if (!status)
			throw InvalidArgumentException("Can not parse the argument of the "+algo+" generator", AT);
	} else { //incorrect arguments, throw an exception
		throw InvalidArgumentException("Wrong number of arguments supplied for the "+algo+" generator", AT);
	}*/
}

bool TEMPLATE::generate(const size_t& param, MeteoData& md)
{
	double &value = md(param);
	if (value == IOUtils::nodata) {
		//whatever should be done to generate a data point
	}

	return true; //all missing values could be filled
}

bool TEMPLATE::create(const size_t& param, std::vector<MeteoData>& vecMeteo)
{
	if (vecMeteo.empty()) return true;

	for (size_t ii=0; ii<vecMeteo.size(); ii++) {
		//either call generate() on each point or process the vector in one go.
		//when working on the whole vector, some optimizations might be implemented.
		if (!generate(param, vecMeteo[ii])) return false;
	}

	return true; //all missing values could be filled
}

} //namespace
