#include "phone_book.h"
#include <iostream>
#include <sstream>
#include <algorithm>

std::string Entry::ToString(char delim) {
    return std::to_string(Id) + delim +
           FirstName + delim +
           MiddleName + delim +
           LastName + delim +
           PhoneNumber + delim +
           Note + delim;
}

PhoneBook::PhoneBook(std::string datapath, std::string confpath) {
    _datapath = datapath;
    _confpath = confpath;
    _InitId();
    _LoadData();
}

char PhoneBook::GetDelim() {
    return _delimiter;
}

void PhoneBook::_LoadData() {
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

void PhoneBook::_InitId() {
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

void PhoneBook::_UpdateId() {
    _conffile.open(_confpath, std::ios::out);
    _id += 1;
    _conffile << _id;
    _conffile.close();
}

std::list<Entry>& PhoneBook::GetAll() {
    return _datalist;
}

void PhoneBook::Add(Entry entry) {
    _mutex.lock();
    _UpdateId();
    entry.Id = _id;
    _datalist.push_back(entry);
    _mutex.unlock();
}

bool PhoneBook::Remove(int id) {
    _mutex.lock();
    auto it = std::find_if(
        _datalist.begin(),
        _datalist.end(),
        [id](Entry entry){ return entry.Id == id; });
    if (it == _datalist.end()) {
        _mutex.unlock();
        return false;
    }
    _datalist.erase(it);
    _mutex.unlock();
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

// convert value to lowercase string
std::string StrToLower(std::string& s) {
    std::string res(s);
    std::transform(s.begin(), s.end(), res.begin(),
        [](unsigned char c){return std::tolower(c);});
    return res;
}

std::list<Entry> PhoneBook::Find(std::string value) {
    std::list<Entry> matches;
    std::copy_if(_datalist.begin(),
        _datalist.end(),
        std::back_inserter(matches),
        [value](Entry entry) {
            return StrToLower(entry.FirstName) == value ||
                   StrToLower(entry.MiddleName) == value ||
                   StrToLower(entry.LastName) == value ||
                   entry.PhoneNumber == value;
        });
    return matches;
}