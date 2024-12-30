#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <clocale>
#include <chrono>
#include <vector>
#include <set>

using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

int interations = 0;
int listNum = 1;

struct VocRecords {
    string fullName;
    sys_days startDate;
    sys_days endDate;
    int days;

    bool operator<(const VocRecords& other) const {
        return fullName < other.fullName ;
    }
};

vector<VocRecords> records;
set <string> printedNames;

static sys_days splitDate(const string& str) {
    int d, m, y;
    sscanf_s(str.c_str(), "%d.%d.%d", &d, &m, &y);
    return year(y) / month(m) / day(d);
}

static int calcDays(sys_days date1, sys_days date2) {
    return (date2 - date1).count();
}

static void Flush() {
    records.clear();
    printedNames.clear();
    setlocale(LC_ALL, "ru");
    interations = 0;
    listNum = 1;
    fstream f("C:\\Users\\User\\Desktop\\Base.txt", ios::out);
    if (!f.is_open()) {
        cerr << "Error" << endl;
    }
    f.close();
    cout << "Файл очищен!" << endl;
}

static void inputInfoIntoFile() {
    setlocale(LC_ALL, "ru");
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

    countDays = calcDays(splitDate(startDate), splitDate(endDate));

    ofstream outFile("C:\\Users\\User\\Desktop\\Base.txt", ios::app);
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

static bool isDateInRange(sys_days VoidDate, sys_days startDate, sys_days endDate) {
    return VoidDate >= startDate && VoidDate <= endDate;
}

class getInfoFromBase {
public:
    void getinfoPerQ() {
        setlocale(LC_ALL, "ru");

        string voidDate;

        cout << "Введите дату для запроса(ДД.ММ.ГГГГ): "; cin >> voidDate;
        cout << endl;
        if (voidDate != "") {
                ifstream fromFile("C:\\Users\\User\\Desktop\\Base.txt");
                if (!fromFile.is_open()) {
                    cerr << "Error";
                    return;
                }
                
                string line;

                while (getline(fromFile, line)) {
                    if (line.find("ФИО: ") != string::npos) {
                        VocRecords currentRecord;
                        currentRecord.fullName = line.substr(line.find(":") + 2);
                        while (getline(fromFile, line)) {
                            if (line.find("Дата ухода в отпуск: ") != string::npos) {
                                currentRecord.startDate = splitDate(line.substr(line.find(":") + 2));
                            }
                            else if (line.find("Дата выхода из отпуска: ") != string::npos) {
                                currentRecord.endDate = splitDate(line.substr(line.find(":") + 2));
                                records.push_back(currentRecord);
                                break;
                            }
                        }
                    }
                }

            sys_days inputDate = splitDate(voidDate);
            cout << "Сотрудники в отпуске на " << voidDate << ":" << endl;
            cout << endl;
            
            bool found = false;
            for (const auto& record : records) {
                if (isDateInRange(inputDate, record.startDate, record.endDate)) {
                    if (printedNames.find(record.fullName) == printedNames.end()) {
                        cout << record.fullName << " - Дата выхода на работу: " << record.endDate << endl;
                        cout << endl;
                        printedNames.insert(record.fullName);
                        found = true;
                    }
                }
            }
            if (!found) {
                cout << "Никто не в отпуске на эту дату." << endl;
            }
        }
    }
    void getAllRecords() {
        ifstream fromFile("C:\\Users\\User\\Desktop\\Base.txt");
        if (!fromFile.is_open()) {
            cerr << "Error";
            return;
        }
        else {
            string line;
            while (getline(fromFile,line)) {
                if (line.find("Запись номер: ") != string::npos) {
                    cout << line;
                }
                else if (line.find("ФИО: ") != string::npos) {
                    cout << line;
                }
                else if (line.find("Дата ухода в отпуск: ") != string::npos) {
                    cout << line;
                }
                else if (line.find("Дата выхода из отпуска: ") != string::npos) {
                    cout << line;
                }
                else if (line.find("-") != string::npos) {
                    cout << line;
                }
                cout << endl;
            }
        }
    }
};

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
            getInfoFromBase getInfo;
            int SecondaryMenu;
            cout << "1. Получить запись по запросу" << endl;
            cout << "2. Получить все записи" << endl;
            cout << "Ваш выбор: ";cin >> SecondaryMenu;
            switch (SecondaryMenu)
            {
            case 1:
                getInfo.getinfoPerQ();
                break;
            case 2:
                getInfo.getAllRecords();
                break;
            default:
                cerr << "Error state";
                break;
            }
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