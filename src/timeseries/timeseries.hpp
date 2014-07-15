#pragma once


#include <string>
#include <fstream>
#include <vector>
#include <ctime>

#include <boost/date_time/posix_time/posix_time.hpp> // for boost::posix

#include <boost/variant.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/utility.hpp>


#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_vector.h>

#include "regex_tokenizer.hpp"
#include "exception.hpp"
#include "logger.h"
#include "timestep.hpp"
#include "crc_hash_compare.hpp"


    
/*
Class: forcing_data
        Holds the meterological data.
        must currently not be copied or assigned from other instances.
 */
class time_series : boost::noncopyable 
{

public:
    
    //mesh elements need to see these
    //two different types: boost::variant solves this, but is very slow 
    // needs to be either a boost::posix_time or double, and is almost always a double
    typedef tbb::concurrent_vector< double > variable_vec;
    typedef tbb::concurrent_vector<  boost::posix_time::ptime > date_vec; 
    

    time_series();
 
    ~time_series();
    

    
    variable_vec get_time_series(std::string variable);
    date_dec get_date_timeseries();
        typedef tbb::concurrent_vector<  boost::posix_time::ptime > date_vec; 
    //list of all the variables this timeseries knows about
    std::vector<std::string> list_variables();

    
    int get_timeseries_size();
    
    void init(std::set<std::string> variables, date_vec datetime, int size);
    
    /*
    Function: begin
             Returns an iterator at the start of the observations

    Parameters: 
            None

    Throws:
            Never

    Returns:   
            iterator - Iterator positioned at the begining of the observation
     */
    iterator begin();


   
        /*
    Function: end
             Returns an iterator of one past the end of the observations

    Parameters: 
            None

    Throws:
            Never

    Returns:   
            iterator - Iterator positioned one past the end of the observations
     */
    iterator end();

    /*
    Function: open
             Opens an observation file. An observation file is organized in a tab, ",", or space delimited  columns, with
            each column representing an independent observation, and each row is a timesteps measurement. For example:
            >Date					Rh	Tair	Precip	Notes
            >20080220T000000		50	-12		2		Station_1
            >20080221T000015		40	-10		0		Station_1
            >		[...]
            Some restrictions:
                    - No more than 2147483647 steps. At 1s intervals, this equates to roughly 68 years.
                    - Consistent units. You mustn't have mm on one line, then meters on the next, for the same observation
                    - Has to be on a constant time step. The first interval is taken as the interval for the rest of the file
                    - Missing values are not currently allowed - that is, each row must be complete with n entries where n is number of variables.
                    - Whitespace, tab or comma delimited. Allows for mixed usage. ex 1234, 4543 890 is legal
                    - Values can be one of
                            - String (special characters are allowed, no commas)
                                    Will match any thing with:
                                    >~`!@#$%^&*(){[}]|\:;"'<>.?/
                                    >OR has upper case / lower case letters
                            - Integer
                                    Will match the following styles:
                                    >+1234
                                    >-1234
                                    >1234567890
                            - Floating point
                                    Will match the following styles:
                                    >12.34
                                    >12.
                                    >.34
                                    >12.345
                                    >1234.45
                                    >+12.34
                                    >-12.34
                                    >+1234.567e-89
                                    >-1234.567e89
                            - Time
                                    - Must be in one column in the following ISO 8601 date time form:
                                            - 20080131T235959
                                              YYYYMMDDThhmmss


    Parameters: 
            std::string path - Fully quantified path to the file

    Throws:
            std::runtime_error - On error

    Returns:   
             void 
     */
    void open(std::string path);

    /*
    Function: to_file
             Writes the internal observation structure to file. Order of columns is not preserved due to how the data is stored.

    Parameters: 
            std::string file - File to save to

    Throws:
            std::runtime_error On error

    Returns:   
            void 
     */
    void to_file(std::string file);



    /*
    Function: is_open
             Determines if a file was successfully opened

    Parameters: 

    Throws:
            Never

    Returns:   
            bool - True if opened
     */
    bool is_open();

    std::string get_opened_file();
    
    
private:
    

 
//        typedef std::vector< double > variable_vec; 


    typedef tbb::concurrent_hash_map<std::string, variable_vec, crc_hash_compare> ts_hashmap;


    // This is a hashmap interface, vector back end
    // "var1"        |     "var2"     |     "var3"   |      
    // ------------------------------------------------      
    //      [...]    |      [...]     |      [...]   |
    //    vector 1   |     vector 2   |     vector 3 |
    //      [...]    |      [...]     |      [...]   |
    ts_hashmap _variables;
    date_vec _date_vec;
    
    int _cols;
    int _rows;
    bool _isOpen;
    std::string _file;
    int _timeseries_size;

    
    //pushes variables back, only useful for reading from a file
    void push_back(double data, std::string variable);

    

};


/* Class: iterator
 Used to iterate over the timeseries instance.
 Thread safe.
 Dereference returns a timestep object.
  */
 class time_series::iterator : public boost::iterator_facade<
                         time_series::iterator,
                         time_series::variable_vec,
                         boost::bidirectional_traversal_tag,
                         timestep> 
 {
 public:
    iterator();
    iterator(const iterator& src);
    ~iterator();
    iterator& operator=(const iterator& rhs);
 private:
     //the following satisfies the reqs for a boost::facade bidirectional iterator
     friend class boost::iterator_core_access;
     friend class time_series;

     const timestep& dereference() const;
     bool equal(iterator const& other) const;
     void increment();
     void decrement();

     //iterators for the current step
     timestep _currentStep;

 };


    