#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

#define MIN_BALANCE 500
class InsufficientFunds{};

class Account
{
private:
		long accountNumber;
		string firstName;
		string lastName;
		float balance;
		static long NextAccountNumber;
public:
		Account() {}
		Account(string fname, string lname, float b)
		{
			NextAccountNumber++;
			accountNumber = NextAccountNumber;
			firstName = fname;
			lastName = lname;
			this->balance = b;
		}

		long getAccNo()
		{
			return accountNumber;
		}
		string getFirstName()
		{
			return firstName;
		}
		string getLastName()
		{
			return lastName;
		}
		float getBalance()
		{
			return balance;
		}
		void Deposit(float amount)
		{
			balance += amount;
		}
		void Withdraw(float amount)
		{
			if (balance - amount < MIN_BALANCE)
				throw InsufficientFunds();
			balance -= amount;
		}
		static void setLastAccNo(long accountNumber);
		static long getLastAccNo();
		friend ofstream& operator<<(ofstream& ofs, Account& acc);
		friend ifstream& operator>>(ifstream& ifs, Account& acc);
		friend ostream& operator<<(ostream& os, Account& acc);
};
long Account::NextAccountNumber = 0;

class Bank
{
private:
		map<long, Account> accounts;
public:
		Bank();
		Account OpenAccount(string fname, string lname, float balance);
		Account BalanceEnquiry(long accountNumber);
		Account Deposit(long accountNumber, float amount);
		Account Withdraw(long accountNumber, float amount);
		void CloseAccount(long accountNumber);
		void ShowAllAccounts();
		~Bank();
};

int main()
{
	Bank b;
	Account acc;
	int choice;
	string fname, lname;
	long accountNumber;
	float balance;
	float amount;
	cout << "***Banking system***" << endl;

	do
	{
		cout << "\n\tSelect one option below: ";
		cout << "\n\t1. Open an Account";
		cout << "\n\t2. Balance Enquiry";
		cout << "\n\t3. Deposit";
		cout << "\n\t4. Withdrawl";
		cout << "\n\t5. Close an Account";
		cout << "\n\t6. Show All Accounts";
		cout << "\n\t7. Exit";
		cout << "\nEnter your choice: ";
		cin >> choice;

		switch (choice)
		{
			case 1:
				cout << "Enter first name: ";
				cin >> fname;
				cout << "Enter last name: ";
				cin >> lname;
				cout << "Enter initial balance: ";
				cin >> balance;
				acc = b.OpenAccount(fname, lname, balance);
				cout << endl << "Congratulations!! Your account is created!" << endl;
				cout << acc;
				break;
			case 2:
				cout << "Enter account number: ";
				cin >> accountNumber;
				acc = b.BalanceEnquiry(accountNumber);
				cout << endl << "Your bank details: " << endl;
				cout << acc;
				break;
			case 3:
				cout << "Enter account number: ";
				cin >> accountNumber;
				cout << "Enter Balance: ";
				cin >> amount;
				acc = b.Deposit(accountNumber, amount);
				cout << endl << "Amount is deposited" << endl;
				cout << acc;
				break;
			case 4:
				cout << "Enter account number: ";
				cin >> accountNumber;
				cout << "Enter Balance: ";
				cin >> amount;
				acc = b.Withdraw(accountNumber, amount);
				cout << endl << "Amount Withdrawn" << endl;
				cout << acc;
				break;
			case 5:
				cout << "Enter account number: ";
				cin >> accountNumber;
				b.CloseAccount(accountNumber);
				cout << endl << "Account is closed" << endl;
				cout << acc;
				break;
			case 6:
				b.ShowAllAccounts();
				break;
			case 7:
				break;
			default:
				cout << "\nEnter correct choice";
				exit(0);
		}
	} while (choice != 7);

	return 0;
}

//***************************************************************************************************
//FUNCTION DECLARATIONS FOR CLASS ACCOUNT

void Account::setLastAccNo(long accountNumber)
{
	NextAccountNumber = accountNumber;
}
long Account::getLastAccNo()
{
	return NextAccountNumber;
}
ofstream& operator<<(ofstream& ofs, Account& acc)
{
	ofs << acc.accountNumber << endl;
	ofs << acc.firstName << endl;
	ofs << acc.lastName << endl;
	ofs << acc.balance << endl;
	return ofs;
}
ifstream& operator>>(ifstream& ifs, Account& acc)
{
	ifs >> acc.accountNumber >> acc.firstName >> acc.lastName >> acc.balance;
	return ifs;
}
ostream& operator<<(ostream& os, Account& acc)
{
	os << "First name: " << acc.getFirstName() << endl;
	os << "Last name: " << acc.getLastName() << endl;
	os << "Account number: " << acc.getAccNo() << endl;
	os << "Balance: " << acc.getBalance() << endl;
	return os;
}

//*****************************************************************************************************
//FUNCTION DECLARATIONS FOR CLASS BANK

Bank::Bank()
{
	Account account;
	ifstream infile;
	infile.open("Bank.data");
	if (!infile)
	{
		//cout << "Error in opening! File not found!!" << endl;
		return;
	}
	while (!infile.eof())
	{
		infile >> account;
		accounts.insert(pair<long, Account>(account.getAccNo(), account));
	}
	Account::setLastAccNo(account.getAccNo());
	infile.close();
}

Account Bank::OpenAccount(string fname, string lname, float balance)
{
	ofstream outfile;
	Account account(fname, lname, balance);
	accounts.insert(pair<long, Account>(account.getAccNo(), account));

	outfile.open("Bank.data",ios::trunc);
	
	map<long, Account>::iterator itr;
	for (itr = accounts.begin(); itr != accounts.end(); itr++)
	{
		outfile << itr->second;
	}
	outfile.close();
	return account;
}

Account Bank::BalanceEnquiry(long accountNumber)
{
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	return itr->second;
}

Account Bank::Deposit(long accountNumber, float amount)
{
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	itr->second.Deposit(amount);
	return itr->second;
}

Account Bank::Withdraw(long accountNumber, float amount)
{
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	itr->second.Withdraw(amount);
	return itr->second;
}

void Bank::CloseAccount(long accountNumber)
{
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	cout << "Account closed" << itr->second;
	accounts.erase(accountNumber);
}

void Bank::ShowAllAccounts()
{
	map<long, Account>::iterator itr;
	for (itr = accounts.begin(); itr != accounts.end(); itr++)
	{
		cout << "Account " << itr->first << endl << itr->second << endl;
	}
}

Bank::~Bank()
{
	ofstream outfile;
	outfile.open("Bank.data", ios::trunc);

	map<long, Account>::iterator itr;
	for (itr = accounts.begin(); itr != accounts.end(); itr++)
	{
		outfile << itr->second;
	}
	outfile.close();
}
