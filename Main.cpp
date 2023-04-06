#include <iostream>
#include <fstream> 
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

void demo_perms(fs::perms p) // Для измения прав пользователя (для Linux)
{
	std::cout << ((p & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
		<< ((p & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
		<< ((p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
		<< ((p & fs::perms::group_read) != fs::perms::none ? "r" : "-")
		<< ((p & fs::perms::group_write) != fs::perms::none ? "w" : "-")
		<< ((p & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
		<< ((p & fs::perms::others_read) != fs::perms::none ? "r" : "-")
		<< ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-")
		<< ((p & fs::perms::others_exec) != fs::perms::none ? "x" : "-")
		<< '\n';
}

class User {
    string _name;
    string _login;
    string _pass;
public:
	User(string name, string login, string pass) : _name(name), _login(login), _pass(pass) {}

	friend fstream& operator >>(fstream& is, User& userdata);               // Чтобы иметь доступ к объектам
	friend ostream& operator <<(ostream& os, const User& userdata);         // класса в private
	
	
};

fstream& operator >>(fstream& is, User& userdata) //перегрузка операторов >> и <<
{
	is >> userdata._name;
	is >> userdata._login;
	is >> userdata._pass;
	return is;
}
ostream& operator <<(ostream& os, const User& userdata)
{
	os << userdata._name;
	os << ' ';
	os << userdata._login;
	os << ' ';
	os << userdata._pass;
	return os;
}

class Message {
    string _text;
    string _sender;
    string _receiver;
public:

	Message(string text, string sender, string receiver) : _text(text), _sender(sender), _receiver(receiver) {}

	friend fstream& operator >>(fstream& is, Message& messagedata);
	friend ostream& operator <<(ostream& os, const Message& messagedata);

};

fstream& operator >>(fstream& is, Message& messagedata)
{
	is >> messagedata._text;
	is >> messagedata._sender;
	is >> messagedata._receiver;
	return is;
}
ostream& operator <<(ostream& os, const Message& messagedata)
{
	os << messagedata._text;
	os << ' ';
	os << messagedata._sender;
	os << ' ';
	os << messagedata._receiver;
	return os;
}

int main(){

	fstream user_file;         //Создаем файлы и проипысваем в аргументах ввод вывод и обновление (а не перезаписывание) фала
	user_file.open("UserList.txt", fstream::in | fstream::out | fstream::app);    
	fstream message_file;
	message_file.open("MessageList.txt", fstream::in | fstream::out | fstream::app);

	fs::permissions("UserList.txt",                // Оставляем права только для дного пользователя в Linux
		fs::perms::group_all | fs::perms::others_all,
		fs::perm_options::remove);
	std::cout << "After removing rights in UserList.txt:   " << endl;
	demo_perms(fs::status("UserList.txt").permissions());
	fs::permissions("MessageList.txt",
		fs::perms::group_all | fs::perms::others_all,
		fs::perm_options::remove);
	std::cout << "After removing users rights in MessageList.txt:  " << endl;
	demo_perms(fs::status("MessageList.txt").permissions());
	

	if (!user_file.is_open()) // Проверка доступности файлов
	{
		cout << "UserList file dont open" << endl;
	}
	else
	{
		cout << "UserList file  open" << endl;
	}
	if (!message_file.is_open())
	{
		cout << "MessageList file dont open" << endl;
	}
	else
	{
		cout << "MessageList file  open" << endl;
	}
		
	do //Основной интерфейс программы
    {
		
		char value;

		cout << "To write and read data to the User list enter 1" << endl;
		cout << "To write and read data to the Message list enter 2" << endl;
		cout << "To exit enter 0" << endl;
		cin >> value;

		if (value == '0') //Выход
		{
			return 0;
		}
		if (value == '1') // Запись и чтение данных из спсика пользователей
		{
			string name_, login_, pass_;
			cout << "Enter the username, login and password separated by a space: " << endl;
			cin >> name_;
			cin >> login_;
			cin >> pass_;
			User userdata(name_, login_, pass_);
			user_file << userdata << "\n";
			
			cout << "User List: " << endl;
			user_file.seekg(0, ios_base::beg);
			while (!user_file.eof())
			{
				user_file >> userdata;
				cout << userdata << endl;
				
			}
			return 0;
		}
		
		if (value == '2') //Запись и чтение ищ файла списка сообщений
		{
			string text_, sender_, receiver_;
			cout << "Enter the title of the letter, the name of the sender and the receiver, separated by a space.: " << endl;
			cin >> text_;
			cin >> sender_;
			cin >> receiver_;
			Message messagedata(text_, sender_, receiver_);
			message_file << messagedata << "\n";

			cout << "Message List: " << endl;
			message_file.seekg(0, ios_base::beg);
			while (!message_file.eof())
			{
				message_file >> messagedata;
				cout << messagedata << endl;

			}
			return 0;

		}
		
		else //Если выбран другой вариант
		{
			cout << "You must select one of the options" <<endl;
		}

	} while (true);
	user_file.close();
	message_file.close();
	return 0;
}