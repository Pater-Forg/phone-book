#pragma once

#include <fstream>
#include <string>
#include <list>
#include <mutex>

struct Entry {
    int Id = 0;
    std::string FirstName;
    std::string MiddleName;
    std::string LastName;
    std::string PhoneNumber;
    std::string Note;

    std::string ToString(char delim);
};

class PhoneBook {
    std::mutex _mutex;              // for blocking _datalist
    std::fstream _datafile;         // csv file with data
    std::string _datapath;          // path to data file
    std::fstream _conffile;         // file with last used ID
    std::string _confpath;          // path to config file
    std::list<Entry> _datalist;     // list of entries
    int _id;                        // last used ID
    char _delimiter = ';';          // delimiter used in datafile

    // Reads last used id from config file
    void _InitId();
    // Updates last used id after insertion
    void _UpdateId();
    // Loads data to _datalist
    void _LoadData();
public:
    PhoneBook(std::string datapath, std::string confpath);
    // Adds new entry to datalist
    void Add(Entry entry);
    // Removes entry by id from datalist
    bool Remove(int id);
    // Gets entry by id
    Entry Get(int id);
    // Gets entries that contains 'value' in First, Middle, Last name and Phone number
    std::list<Entry> Find(std::string value);
    // Gets all entries from datalist
    std::list<Entry>& GetAll();
    // Gets delimiter used in csv datafile
    char GetDelim();
    // Writes data from datalist to datafile
    void SaveData();
};