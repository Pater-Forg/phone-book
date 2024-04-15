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
    std::mutex _mutex;
    std::fstream _datafile;
    std::string _datapath;
    std::fstream _conffile;
    std::string _confpath;
    std::list<Entry> _datalist;
    int _id;
    char _delimiter = ';';
    void _InitId();
    void _UpdateId();
    void _LoadData();
public:
    PhoneBook(std::string datapath, std::string confpath);
    bool Add(Entry entry);
    bool Remove(int id);
    Entry Get(int id);
    Entry Find(std::string value);
    std::list<Entry>& GetAll();
    char GetDelim();
    void SaveData();
};