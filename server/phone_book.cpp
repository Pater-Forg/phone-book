#include "phone_book.h"
#include <iostream>
#include <sstream>
#include <algorithm>

PhoneBook::PhoneBook(std::string datapath, std::string confpath) {
    _datapath = datapath;
    _confpath = confpath;
    _init_id();
    _load_data();
}

void PhoneBook::_load_data() {
    _datafile.open(_datapath);
    if (!_datafile.good()) {
        _datafile.close();
        return;
    }
    std::string row;
    while (std::getline(_datafile, row)) {
        std::stringstream row_stream(row);
        Entry entry;
        row_stream >> entry.Id;
        row_stream.get();
        std::getline(row_stream, entry.FirstName, _delimiter);
        std::getline(row_stream, entry.MiddleName, _delimiter);
        std::getline(row_stream, entry.LastName, _delimiter);
        std::getline(row_stream, entry.PhoneNumber, _delimiter);
        std::getline(row_stream, entry.Note, _delimiter);
        _datalist.push_back(entry);
    }
    _datafile.close();
}

void PhoneBook::SaveData() {
    _datafile.open(_datapath, std::ios::trunc | std::ios::out);
    if (!_datafile.good()) {
        std::cout << "Error saving data" << std::endl;
        _datafile.close();
        return;
    }
    for (Entry entry : _datalist) {
        _datafile << entry.Id << _delimiter
                  << entry.FirstName << _delimiter
                  << entry.MiddleName << _delimiter
                  << entry.LastName << _delimiter
                  << entry.PhoneNumber << _delimiter
                  << entry.Note << _delimiter
                  << std::endl;
    }
    _datafile.close();
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
    _id += 1;
    _conffile << _id;
    _conffile.close();
}

std::list<Entry>& PhoneBook::GetAll() {
    return _datalist;
}

bool PhoneBook::Add(Entry entry) {
    _update_id();
    entry.Id = _id;
    _datalist.push_back(entry);
    return true;
}

bool PhoneBook::Remove(int id) {
    auto it = std::find_if(
        _datalist.begin(),
        _datalist.end(),
        [id](Entry entry){ return entry.Id == id; });
    if (it == _datalist.end()) {
        return false;
    }
    _datalist.erase(it);
    return true;
}

Entry PhoneBook::Get(int id) {
    auto it = std::find_if(
        _datalist.begin(),
        _datalist.end(),
        [id](Entry entry){ return entry.Id == id; });
    if (it == _datalist.end()) {
        Entry empty_entry = {0}; 
        return empty_entry;
    }
    return *it;
}

Entry PhoneBook::Find(std::string value) {
    auto it = std::find_if(
        _datalist.begin(),
        _datalist.end(),
        [value](Entry entry) {
            return entry.FirstName == value ||
                   entry.MiddleName == value ||
                   entry.LastName == value ||
                   entry.PhoneNumber == value;
            });
    if (it == _datalist.end()) {
        Entry empty_entry = {0}; 
        return empty_entry;
    }
    return *it;
}