
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <iterator>
using namespace std;

vector <string> fetchRecord(string, string&, int);
vector <string> newCases(string&);
int totalCases(string&);
int totalDeaths(string&);
string* topN(int);
int highestAge(string&);
int leastAge(string&);
bool status(string&);

//Main driver of the program
int main() {

	string country;
	string option;
	bool isNewCountry = true;
	string months[] = { "January", "February", "March", "April", "May", "June", "July", "August", "October", "September", "November", "December" };

	cout << "Welcome to COVIDMonitor v1.0!\n";
	while (isNewCountry) {
		isNewCountry = false;
		cout << "Type the country you wish to know about : \n";
		cin >> country;
		
		while (true) {
			cout << "What do you want to know about?\n" 
				<< "Type: \n newCases for new cases in an interval(month-month)\n" 
				<< " totalCases for total cases till date\n"
				<< " top for the top N countries \n" 
				<< " highestAge for the age group that is most hit\n" 
				<< " lowestAge for the least hit one \n"
				<< " status to know if a country is in a steep-up or steep-down\n"
				<< " changeCountry to change the country\n"
				<< " !exit to exit the program\n\n";
			cin >> option;

			if (option == "newCases") {
				vector <string> row = newCases(country);
				int start = 1, end = 1;
				if (start <= 0 || end <= 0 || start > 12 || end > 12 || end < start) {
					cout << "Sorry, that's not possible. Aborting...";
					exit(1);
				}
				cout << "Enter the month starting your interval in numbers: (E.g.: January => 1, February => 2)\n";
				cin >> start;
				cout << "Enter the month ending your interval in the same manner: \n";
				cin >> end;
				
				for (int i = start; i <= end; ++i) {
					cout << "In " << months[i - 1] << " there were " << row[i] << " new cases" << endl;
				}
			}
			else if (option == "totalCases") {
				cout << "They total number of cases in " << country << " is " << totalCases(country) << endl;
			}
			else if (option == "top") {
				int n;
				cout << "Enter the length of the top list you want to see \n";
				cin >> n;
				string *top = topN(n);
				for (int i = 0; i < n; i++) {
					cout << i + 1 << ". " << top[i] << endl;
				}
				delete[] top;
			}
			else if (option == "highestAge") {
				cout << "In " << country << ", the most hit age group is " << highestAge(country) << endl;
			}
			else if (option == "lowestAge") {
				cout << "In " << country << ", the least hit age group is " << leastAge(country) << endl;
			}
			else if (option == "status") {
				if (status(country)) {
					cout << country << " is in a steep-up\n";
				}
				else if (status(country) == false) {
					cout << country << " is in a steep-down\n";
				}
				else {
					cout << country << " is in a peak\n";
				}
			}
			else if (option == "changeCountry") {
				isNewCountry = true;
				break;
			}
			else if (option == "!exit") {
				cout << "Thank you for using COVIDMonitor, bye! \n";
				exit(0);
			}
			else {
				cout << "Sorry, unknown command. Try again! \n";
			}
		}
	}
}

//This function reads the record of a given coutry
//path => file path to look in, searchTerm => the country the function will look in its records, numOfColumns => the number of columns in the file

vector<string> fetchRecord(string path, string& searchTerm, int numOfColumns) {
	ifstream fin;
	bool isFound = false;

	fin.open(path);
	if (fin.fail()) {
		cout << "Couldn't open the file. Aborting...";
		exit(1);
	}

	vector<string> row;
	
	string* fields = new string[numOfColumns];

	

	while (getline(fin, fields[0], ',') && !isFound) {
		
		for (int i = 1; i < numOfColumns; i++) {
			if (i == numOfColumns - 1) 
				getline(fin, fields[i], '\n');
			else
				getline(fin, fields[i], ',');
		}
		if (fields[0] == searchTerm) {
			isFound = true;
			for (int i = 0; i < numOfColumns; ++i) {
				row.push_back(fields[i]);
			}
		}
	}

	if (isFound == false) {
		cout << "Sorry, couldn't find this country \nTry another country:\n";
		cin >> searchTerm;
		delete[] fields;
		return fetchRecord(path, searchTerm, numOfColumns);
	}

	delete[] fields;
	fin.close();
	return row;

}

//This function grabs the new cases per country in an interval
vector<string> newCases(string& country) {
	string path = "D:\\Study\\FALL2020\\CSCE 1001\\Project\\Project\\Dataset\\newcases.csv";
	int columns = 12;

	vector<string> row = fetchRecord(path, country, columns);
	
	return row;
}

//This function grabs the total cases per country till date 
int totalCases(string& country) {
	int columns = 12;
	int sum = 0;
	
	vector<string> row = newCases(country);
	for (int i = 1; i < columns; ++i) {
		sum += stoi(row[i]);
	}
	
	return sum;
}

//This function grabs the total deaths per country till date 
int totalDeaths(string& country) {
	string path = "D:\\Study\\FALL2020\\CSCE 1001\\Project\\Project\\Dataset\\totaldeaths.csv";
	int columns = 12;
	int sum = 0;
	vector<string>row = fetchRecord(path, country, columns);
	for (int i = 1; i < columns; ++i)
	{
		sum += stoi(row[i]);
	}
	return sum;
}

//This function grabs the top n countries that are most hit
string* topN(int n) {
	string path = "D:\\Study\\FALL2020\\CSCE 1001\\Project\\Project\\Dataset\\countries.csv";
	ifstream fin;
	vector <string> countries;
	vector <int> cases;
	string current = "";

	string tempStr = "";
	int tempInt;


	fin.open(path);

	if (fin.fail()) {
		cout << "Couldn't open the file. Aborting...";
		exit(1);
	}

	for (int i = 0; !fin.eof(); ++i) {
		getline(fin, current, '\n');
		countries.push_back(current);
	}
	string* topCountries = new string[countries.size()];
	
	for (int i = 0; i < countries.size(); i++) {
		topCountries[i] = countries[i];
	}

	for (int i = 0; i < countries.size(); ++i){
		cases.push_back(totalCases(countries[i]));
	}

	for (int i = 0; i < countries.size(); ++i) {
		for (int j = i + 1; j < countries.size(); ++j) {
			if (cases[i] < cases[j]) {
				tempInt = cases[i];
				cases[i] = cases[j];
				cases[j] = tempInt;

				tempStr = topCountries[i];
				topCountries[i] = topCountries[j];
				topCountries[j] = tempStr;
			}
		}
	}
	fin.close();

	return topCountries;
}

//This function grabs the highest age group that has been affected 
int highestAge(string& country) {
	string path = "D:\\Study\\FALL2020\\CSCE 1001\\Project\\Project\\Dataset\\agegroups.csv";
	int columns = 3;
	vector <string> row = fetchRecord(path, country, columns);
	
	return stoi(row[1]);
}

//This function grabs the highest age group that has been affected 
int leastAge(string& country) {
	string path = "D:\\Study\\FALL2020\\CSCE 1001\\Project\\Project\\Dataset\\agegroups.csv";
	int columns = 3;
	vector <string> row = fetchRecord(path, country, columns);

	return stoi(row[2]);
}

//This function grabs the slope of cases
bool status(string& country) {

	int columns = 12, after = 0, before = 0;

	vector <string> row = newCases(country);
	after = stoi(row[columns - 1]);
	before = stoi(row[columns - 2]);

	if (after - before < 0) {
		return false;
	}
	else if (after - before > 0) {
		return true;
	}
	else {
		return NULL;
	}
}

