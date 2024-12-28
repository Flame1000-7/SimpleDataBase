#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <clocale>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int interations = 0;
int listNum = 1;

void splitDate(string str,int& day, int& month, int& year) {
    sscanf_s(str.c_str(), "%d.%d.%d", &day, &month, &year);
}

class DateCalculator {
public:
    DateCalculator(int day, int month, int year) : day(day), month(month), year(year) {}
    static int countDate(const DateCalculator start, const DateCalculator end) {

        if (start.year > end.year || (start.year == end.year && start.month > end.month) || (start.year == end.year && start.month == end.month && start.day > end.day) ) {

            return -1;

        }

        int totalDays = 0;
        int daysInPeriod = 0;
        if (start.month == end.month && start.year == end.year) {
            totalDays += end.day - start.day;
        }
        else if (start.month != end.month && start.year == end.year) {
            if (start.day == end.day) {
                totalDays += daysInMonth(start.month, start.year);
            }
            else {
                totalDays += daysInMonth(start.month, start.year) + (end.day - start.day);
            }
        }
        else if (start.year != end.year) {
            if (start.month == end.month) {
                totalDays += (end.day - start.day) + (isLeapYear(start.year) ? 366 : 365) + ((isLeapYear(end.year) ? 366 : 365) - 365);
            }
            else {
                for (int i = __min(start.month, end.month); i < __max(end.month,start.month); i++) {
                    daysInPeriod += daysInMonth(i, start.year);
                }
                if (start.day == end.day) {
                    if (start.month > end.month) {
                        totalDays += (isLeapYear(start.year) ? 366 : 365) + ((isLeapYear(end.year) ? 366 : 365) - 365) - daysInPeriod;
                    }
                    else {
                        totalDays += (isLeapYear(start.year) ? 366 : 365) + ((isLeapYear(end.year) ? 366 : 365) - 365) + daysInPeriod;
                    }
                }
                else {
                    if (start.month > end.month) {
                        totalDays += (isLeapYear(start.year) ? 366 : 365) + (end.day - start.day) - daysInPeriod;
                    }
                    else {
                        totalDays += (isLeapYear(start.year) ? 366 : 365) + (end.day - start.day) + daysInPeriod;
                    }
                    
                }
            }
        }

        return totalDays;
    }

private:

    int day, month, year;

    static bool isLeapYear(int year) {

        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

    }
    static int daysInMonth(int month, int year) {

        switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
        case 4: case 6: case 9: case 11: return 30;
        case 2: return isLeapYear(year) ? 29 : 28;
        default: return 0;

        }
    }
};

void Flush() {
    setlocale(LC_ALL, "ru");
    interations = 1;
    listNum = 1;
    fstream f("C:\\Users\\Kolip\\Desktop\\Base.txt", ios::out);
    if (!f.is_open()) {
        cerr << "Error" << endl;
    }
    f.close();
    cout << "Файл очищен!" << endl;
}

void inputInfoIntoFile() {
    setlocale(LC_ALL, "ru");

    int Sd, Sm, Sy;
    int Ed, Em, Ey;
    int countDays;

    string fulName;
    string startDate;
    string endDate;

    cout << "Введите ФИО: ";
    getline(cin, fulName);

    cout << "Введите дату ухода в отпуск(ДД.ММ.ГГГГ): ";
    getline(cin, startDate);

    cout << "Введите дату выхода из отпуска(ДД.ММ.ГГГГ): ";
    getline(cin, endDate);

    splitDate(startDate, Sd, Sm, Sy);
    splitDate(endDate, Ed, Em, Ey);

    DateCalculator sDate(Sd, Sm, Sy);
    DateCalculator eDate(Ed, Em, Ey);

    countDays = DateCalculator::countDate(sDate, eDate);

    ofstream outFile("C:\\Users\\Kolip\\Desktop\\Base.txt", ios::app);
    if (!outFile) {
        cerr << "Error opening" << endl;
        return;
    }
    outFile << "----------Запись номер: " << listNum <<"----------" << endl;
    outFile << "ФИО: " << fulName << endl;
    outFile << "Дата ухода в отпуск: " << startDate << endl;
    outFile << "Дата выхода из отпуска: " << endDate << endl;
    outFile << "Количество дней в отпуске: " << countDays << endl;
    outFile << "---------------------------" << endl;
    outFile << endl;
    outFile.close();
    listNum++;
    interations++;
    cout << endl;
}

bool isDateInRange(string start, string end,string voidDate) {
    int Sd, Sm, Sy;
    int Ed, Em, Ey;
    int Vd, Vm, Vy;

    splitDate(start, Sd, Sm, Sy);
    splitDate(end, Ed, Em, Ey);
    splitDate(voidDate, Vd, Vm, Vy);

    DateCalculator sDate(Sd, Sm, Sy);
    DateCalculator eDate(Ed, Em, Ey);
    DateCalculator vDate(Vd, Vm, Vy);

    if (DateCalculator::countDate(sDate, eDate) != -1) {
        return (DateCalculator::countDate(sDate, eDate) <= DateCalculator::countDate(sDate, vDate));
    }

}

void getinfoFromBase() {
    setlocale(LC_ALL, "ru");

    string voidDate;
    string fullName;
    string startDate;
    string vocationLen;
    string endDate;

    cout << "Введите дату для запроса(ДД.ММ.ГГГГ): "; cin >> voidDate;
    
    if (voidDate != "") {
        ifstream fromFile("C:\\Users\\Kolip\\Desktop\\Base.txt");

        if (!fromFile.is_open()) {
            cerr << "Error";
            return;
        }

        string line;
        while (getline(fromFile, line)) {
            if (line.find("ФИО: ") != string::npos) {
                fullName = line.substr(line.find(":") + 2);
                getline(fromFile, line);
                startDate = line.substr(line.find(":") + 2);
                getline(fromFile, line);
                endDate = line.substr(line.find(":") + 2);
                getline(fromFile, line);
                vocationLen = line.substr(line.find(":") + 2);
                if (isDateInRange(startDate, endDate, voidDate)) {
                    cout << fullName << " - Находиться в отпуске" << endl;
                }
            }
        }
        fromFile.close();
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    bool state = true;
    int menu;
    do {
        cout << "1. Сделать запись" << endl;
        cout << "2. Очистить базу данных" << endl;
        cout << "3. Сделать запрос" << endl;
        cout << "4. Выход" << endl;
        cout << "Ваш выбор: "; cin >> menu;
        cout << endl;
        switch (menu)
        {
        case 1:
            int countInputs;
            cout << "Сколько записей вы хотите сделать: "; cin >> countInputs;
            cin.ignore();
            cout << endl;
            do {
                cout << "Запись номер: " << listNum;
                cout << endl;
                inputInfoIntoFile();
            } while (interations < countInputs);
            break;
        case 2:
            Flush();
            break;
        case 3:
            getinfoFromBase();
            break;
        case 4:
            cout << endl;
            cout << "Programm finished!";
            state = false;
            break;
        default:
            cerr << "Error state" << endl;
            break;
        }
    } while (state);
}