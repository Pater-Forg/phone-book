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
    void _init_id();
    void _update_id();
    void _load_data();
public:
    PhoneBook(std::string datapath, std::string confpath);
    bool Add(Entry entry);
    bool Remove(int id);
    Entry Get(int id);
    Entry Find(std::string value);
    std::list<Entry>& GetAll();
    void SaveData();
};