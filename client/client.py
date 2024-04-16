import socket

HOST = "127.0.0.1"
PORT = 8080

print(
'''
Available commands:
getall          get all entries
delete [id]     removes entry with given id
get [id]        get entry with given id
find [value]    get all entries that contains value in first,middle,last name or phone number
add [FirstName];[MiddleName];[LastName];[PhoneNumber];[Note];
    adds new entry, fields could be empty

/quit           closes connection
'''
)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    while True:
        msg = input("> ")
        msg = msg.lower()
        if msg == "/quit":
            s.close()
            break
        s.sendall(msg.encode(encoding='ascii'))
        resp = s.recv(16000)
        if len(resp) == 0:
            print("Disconnected from server")
            s.close()
            break
        print(resp.decode())