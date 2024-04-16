# phone-book
Тестовое задание для стажировки.  
Серверная часть написана на C++. Входящие соединения проверяются при помощи `select`. В процессе обработки запроса сервер не блокируется, так как для обработки запроса от каждого клиента создается отдельный поток.  
Клиентская часть написана на Python с использованием только стандартной библиотеки `socket`.  
Программа использует порт 8080, при необходимости можно поменять в файлах `client.py` и `main.cpp`.  
Проект был протестирован при следующих параметрах:  
- Система: Ubuntu 22.04.4 LTS
- Компилятор C++: g++-11
- Python: 3.10.12
## Поддерживаемые команды
|Команда  |описание
|---------|--------
|getall   |получить все доступные записи
|get [id] |получить запись по id
|delete [id]|Удалить запись по id
|find [value]|Найти записи содержащие данное значение в имени, фамилии, отчестве или телефонном номере
|add [FirstName];[MiddleName];[LastName];[PhoneNumber];[Note];|добавляет новую запись. Поля могут быть пустыми (но точки с запятой обязательны)
|/quit    |Закрывает соединение (со стороны клиента)|  

Для остановки сервера необходимо написать `stop` в терминале, где запущен сервер.
