#include <iostream>
#include "client_server.h"
#include "phone_book.h"

int main() {
    PhoneBook book("data.csv", "conf.txt");
    book.Remove(2);
    Entry entry = {
        0,
        "Dara",
        "",
        "Moskalenko",
        "1-111-111-11-11",
        ""
    };
    book.Add(entry);
    Entry dara = book.Find("Dara");
    std::cout << dara.FirstName << std::endl;
    book.SaveData();
    return 0;
}