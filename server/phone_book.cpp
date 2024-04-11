#include "phone_book.h"
#include <iostream>

PhoneBook::PhoneBook(std::string datapath, std::string confpath) {
    _datapath = datapath;
    _confpath = confpath;
    _init_id();
}

void PhoneBook::_init_id() {
    _conffile.open(_confpath);
    if (_conffile.good()) {
        _conffile >> _id;
    }
    else {
        _conffile.close();
        _conffile.open(_confpath, std::ios::out);
        _conffile << 0;
        _id = 0;
    }
    _conffile.close();
}

void PhoneBook::_update_id() {
    _conffile.open(_confpath, std::ios::out);
    _conffile << _id + 1;
    _id += 1;
    _conffile.close();
}

bool PhoneBook::Add(Entry entry) {
    _datafile.open(_datapath, std::ios::app);
    if (_datafile) {
        _update_id();
        entry.Id = _id;
        _datafile << entry.Id << _delimiter
                  << entry.FirstName << _delimiter
                  << entry.MiddleName << _delimiter
                  << entry.LastName << _delimiter
                  << entry.PhoneNumber << _delimiter
                  << entry.Note << _delimiter
                  << std::endl;
        _datafile.close();
        return true;
    }
    else {
        _datafile.close();
        return false;
    }
}

bool PhoneBook::Remove(int id) {
    
}