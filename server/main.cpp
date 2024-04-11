#include <iostream>
#include "client_server.h"
#include "phone_book.h"

int main() {
    PhoneBook book("data.csv", "conf.txt");
    for (int i = 0; i < 10; i++) {
        Entry entry = {
            0,
            "First",
            "Middle",
            "Last",
            "8-800-555-3535",
            "Lorem ipsum dolores sit amet"
        };
        book.Add(entry);
    }
    return 0;
}