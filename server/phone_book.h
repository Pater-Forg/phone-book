#include <fstream>
#include <string>

struct Entry {
    int Id = 0;
    std::string FirstName;
    std::string MiddleName;
    std::string LastName;
    std::string PhoneNumber;
    std::string Note;
};

class PhoneBook {
    std::fstream _datafile;
    std::string _datapath;
    std::fstream _conffile;
    std::string _confpath;
    int _id;
    char _delimiter = ';';
    void _init_id();
    void _update_id();
public:
    PhoneBook(std::string datapath, std::string confpath);
    bool Add(Entry entry);
    bool Remove(int id);
    Entry Get(int id);
    Entry Find(std::string field, std::string value);
};