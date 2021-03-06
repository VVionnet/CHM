//
// Canadian Hydrological Model - The Canadian Hydrological Model (CHM) is a novel
// modular unstructured mesh based approach for hydrological modelling
// Copyright (C) 2018 Christopher Marsh
//
// This file is part of Canadian Hydrological Model.
//
// Canadian Hydrological Model is free software: you can redistribute it and/or
// modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Canadian Hydrological Model is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Canadian Hydrological Model.  If not, see
// <http://www.gnu.org/licenses/>.
//

#pragma once

#include <boost/date_time/posix_time/posix_time.hpp> // for boost::posix

#ifdef USE_SPARSEHASH
    #include <sparsehash/dense_hash_map>
#else
    #include <unordered_map>
#endif

#include <tbb/concurrent_vector.h>
#include <vector>
#include <cstddef>
#include <unordered_map>

#include "exception.hpp"


#include "logger.hpp"

/**
\class timestep

Conceptualizes a timestep within a timeseries. Holds a map of iterators into the main timeseries vectors, allowing for easy access of variables, and ensuring all variables at at the same timestep.
 */
class timestep
{
 
public:
    //two different. boost::variant solves this, but is very slow 
    // needs to be either a boost::posix_time or double, and is almost always a double

    typedef std::vector< double > variable_vec;
    typedef std::vector<  boost::posix_time::ptime > date_variable;

    /**
    * Default empty constructor
    */
    timestep();


    /**
    * Copy const.
    */
    timestep(const boost::shared_ptr<timestep> src);

    ~timestep();


    /**
    * Converts the current time step to a std::string. There is no guarantee made about variable output order.Dates will be in the form 2008-Feb-23 23:59:59
    */
    std::string to_string();

    /**
    * Returns the current hour, 24-hour format
    */
    int hour();

    /**
    * Returns the current minute
    */
    int min();

    /**
    * Returns the current second
    */
    int sec();

    /**
    *  Current month number, starting with Jan = 1.
    */
    int month();


    /**
    *  Current day number starting at 1
    */
    int day();


    /**
    * Current year
    */
    int year();

    /**
    * Returns the boost::gregoiran structure for time calculations.
    */
    boost::gregorian::date get_gregorian();

    /**
    * Returns the boost::posix_time structure for time calculations
    */
    boost::posix_time::ptime get_posix();

    /**
    * Gets the value associated with a variable. If it doesn't exist, return NaN
    * \param variable Variable name
    */
    double get(const std::string &variable) ;

    /**
     * Returns true if the specified variable is available in the timeseries.
     */
    bool has(const std::string &variable);

    /**
    * Returns a copy of the current iterator for this variable. That is, an iterator into the underlying variable vector. Useful for std algorithms, eg std::max
    */
    variable_vec::iterator get_itr(const std::string &varName);

    /**
    * Sets the given variable's value for this timestep
    * \param variable Variable name
    * \param value the value
    */
    void set(const std::string &variable, const double &value);


    
private:
    friend class timeseries;

#ifdef USE_SPARSEHASH
    typedef google::dense_hash_map<std::string,variable_vec::iterator> itr_map;
#else
    typedef std::unordered_map<std::string,variable_vec::iterator> itr_map;
#endif

    //holds the iterators for the current timestep. 
    //these are iterators into each vector in the variable hashmap
    itr_map _itrs; 
    date_variable::iterator _date_itr;
};
