#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <limits>

using namespace std;

class Member {
private:
    string memberNumber;
    string memberTier;
    string passportNumber;
    int mrz;
    string memberName;
    int mileageBalance;

public:
    Member(string num, string tier, string passport, int MRZ, string name, int balance) {
        memberNumber = num;
        memberTier = tier;
        passportNumber = passport;
        mrz = MRZ;
        memberName = name;
        mileageBalance = balance;
    }

    string getMemNum() const {
        return memberNumber;
    }
    string getName() const {
        return memberName;
    }
    string getTier() const {
        return memberTier;
    }
    string getPassport() const {
        return passportNumber;
    }
    int getMRZ() const {
        return mrz;
    }
    int getBalance() const {
        return mileageBalance;
    }

    void setTier(const string& t) {
        memberTier = t;
    }
    void setPassport(const string& p) {
        passportNumber = p;
    }
    void setName(const string& n) {
        memberName = n;
    }
    void setMRZ(int m) {
        mrz = m;
    }
    void addPoints(int pts) {
        mileageBalance += pts;
    }
    bool deductPoints(int pts) {
        if (pts > mileageBalance) return false;
        mileageBalance -= pts;
        return true;
    }

    void DisplayInfo() const {
        cout << "Member Number: " << memberNumber << endl;
        cout << "Name: " << memberName << endl;
        cout << "Tier: " << memberTier << endl;
        cout << "Passport: " << passportNumber << " (MRZ: " << mrz << ")" << endl;
        cout << "Balance: " << mileageBalance << endl;
    }
};

class Flight {
private:
    string memberNumber;
    string origin;
    string destination;
    string flightNumber;
    string cabinClass;
    string departureDate;
    string creationDate;
    bool updated;

public:
    Flight() {
        memberNumber = "";
        origin = "";
        destination = "";
        flightNumber = "";
        cabinClass = "";
        departureDate = "";
        creationDate = "";
        updated = false;
    }

    Flight(string memNum, string orig, string dest, string fNum,
        string cabin, string depDate, string createDate, bool upd) {
        memberNumber = memNum;
        origin = orig;
        destination = dest;
        flightNumber = fNum;
        cabinClass = cabin;
        departureDate = depDate;
        creationDate = createDate;
        updated = upd;
    }

    string getMemNum() const {
        return memberNumber;
    }
    string getOrigin() const {
        return origin;
    }
    string getDestination() const {
        return destination;
    }
    string getFlightNumber() const {
        return flightNumber;
    }
    string getCabinClass() const {
        return cabinClass;
    }
    string getDepartureDate() const {
        return departureDate;
    }
    string getCreationDate() const {
        return creationDate;
    }
    bool isUpdated() const {
        return updated;
    }

    void markUpdated() {
        updated = true;
    }
};

vector<Member> members;
vector<Flight> flights;
string systemDate = "";
bool dataLoaded = false;

void displayWelcomeMessage();
void displayMainMenu();
bool isValidDate(const string& date);
void setSystemDate(string& outDate);
void loadStartingData();
void openCloseAccount();   // R3
void creditsAndExit();     // R6
string GenMemNum(const vector<Member>& vMember);
int GenMRZ(const string& pNum);
void showAllMemberAccounts();    //R2

bool isValidPassport(const string& p);
bool isValidFlightNumber(const string& fn);
pair<string, string> getRoute(const string& flightNum);
bool getConfirmation();
void clearInput();
void editMemberInfo(Member& m);//R4.1
void updateMileagePoints(Member& m); //R4.2
void createFlightRecord(Member& m); //R4.3
void redeemOrTransfer(Member& m); //R4.4
void showMemberInfoBeforeReturn(const Member& m); //R4.5
void memberAccountOperations(const string& memberNumber);//R4
void generateDailyStatment(const string& memberNumber); //R5

int main() {
    srand(time(0));
    vector<Member> vMember;

    int choice;
    bool exitProgram = false;

    displayWelcomeMessage();

    while (!exitProgram) {
        displayMainMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number between 1 and 6.\n";
            continue;
        }
        cin.ignore(1000, '\n');

        if ((choice >= 2 && choice <= 5) && !dataLoaded) {
            cout << "Error: Please load starting data (Option 1) first.\n";
            continue;
        }

        string memNum;
        switch (choice) {
        case 1:
            loadStartingData();
            break;
        case 2:
            showAllMemberAccounts();
            break;
        case 3:
            openCloseAccount();
            break;
        case 4:
            cout << "\nEnter the member number: ";
            cin >> memNum;
            memberAccountOperations(memNum);
            break;
        case 5:
            cout << "Enter the member number: \n";
            cin >> memNum;
            generateDailyStatment(memNum);
            break;
        case 6:
            creditsAndExit();
            exitProgram = true;
            break;
        default:
            cout << "Invalid option. Please enter 1-6.\n";
        }
        cout << endl;
    }
    return 0;
}

void displayWelcomeMessage() {
    cout << "============================================\n";
    cout << "   Welcome to Frequent Flyer Program (FFP)  \n";
    cout << "          Group Project SEHH2042            \n";
    cout << "============================================\n\n";
}

void displayMainMenu() {
    cout << "*** FFP Main Menu ***\n";
    cout << "[1] Load Starting Data\n";
    cout << "[2] Show All Member Accounts\n";
    cout << "[3] Open or Close Member Account\n";
    cout << "[4] Member Account Operations\n";
    cout << "[5] Generate Daily Statement\n";
    cout << "[6] Credits and Exit\n";
    cout << "******************************\n";
    cout << "Option (1-6): ";
}

bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;

    int d = (date[0] - '0') * 10 + (date[1] - '0');
    int m = (date[3] - '0') * 10 + (date[4] - '0');
    int y = (date[6] - '0') * 1000 + (date[7] - '0') * 100
        + (date[8] - '0') * 10 + (date[9] - '0');

    if (y != 2025) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > 31) return false;

    if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30) return false;
    if (m == 2 && d > 28) return false;

    return true;
}

// R1.2 
void setSystemDate(string& outDate) {
    int attempts = 0;
    const string DEFAULT_DATE = "30-06-2025";
    string input;

    cout << "\n*** Set System Date for updating Mileage Points ***\n";
    cout << "Please enter system date (DD-MM-YYYY): ";

    while (attempts < 3) {
        getline(cin, input);
        if (isValidDate(input)) {
            outDate = input;
            cout << "System date set to: " << outDate << "\n";
            return;
        }
        else {
            attempts++;
            if (attempts < 3) {
                cout << "Invalid format or out of range. Try again ("
                    << (3 - attempts) << " attempts left): ";
            }
        }
    }

    outDate = DEFAULT_DATE;
    cout << "\nWarning: Three invalid attempts. Default date "
        << DEFAULT_DATE << " is set.\n";
}

// R1
void loadStartingData() {
    members.clear();
    flights.clear();

    members.push_back(Member("202456734", "Gold", "A566778904", GenMRZ("A566778904"), "WONG Claire", 45000));
    members.push_back(Member("202333890", "Green", "C786789085", GenMRZ("C786789085"), "MA Kathy", 10000));
    members.push_back(Member("202067856", "Silver", "E388768901", GenMRZ("E388768901"), "CHAN Peter", 53200));
    members.push_back(Member("202211843", "Gold", "E389000787", GenMRZ("E389000787"), "CHEUNG Alice", 30000));


    flights.push_back(Flight("202211843", "Hong Kong", "London", "CC81", "First", "28-05-2025", "01-05-2025", false));
    flights.push_back(Flight("202211843", "London", "Hong Kong", "CC82", "First", "10-06-2025", "01-05-2025", false));
    flights.push_back(Flight("202333890", "London", "Dubai", "CC61", "Economy", "12-06-2025", "10-06-2025", false));
    flights.push_back(Flight("202067856", "Hong Kong", "Dubai", "CC31", "Business", "05-07-2025", "20-06-2025", false));
    flights.push_back(Flight("202067856", "Dubai", "London", "CC62", "Business", "08-07-2025", "20-06-2025", false));
    flights.push_back(Flight("202456734", "Dubai", "Hong Kong", "CC32", "Business", "05-08-2025", "02-08-2025", false));

    cout << "\nStarting data loaded successfully!\n";
    setSystemDate(systemDate);
    dataLoaded = true;
}

string GenMemNum() {
    string newNumber;
    bool unique;
    string year = systemDate.substr(systemDate.length() - 4);
    do {
        unique = true;
        int randomDigits = rand() % 90000 + 10000;
        newNumber = year + to_string(randomDigits);

        for (int i = 0; i < members.size(); i++) {
            if (members[i].getMemNum() == newNumber) {
                unique = false;
                break;
            }
        }
    } while (!unique);
    return newNumber;
}

int GenMRZ(const string& pNum) {
    int weights[] = { 7, 3, 1, 7, 3, 1, 7, 3, 1 };
    long long total = 0;
    total += (pNum[0] - 'A' + 10) * weights[0];

    for (int i = 1; i < 9; i++) {
        total += (pNum[i] - '0') * weights[i];
    }
    return total % 10;
}

// R2
static int dateToInt(const string& date) {
    int d, m, y;
    char dash1, dash2;
    istringstream iss(date);
    iss >> d >> dash1 >> m >> dash2 >> y;
    return y * 10000 + m * 100 + d;
}

void displayMemberTable() {
    cout << "\n=== Member Account Records ===\n";
    cout << left
        << setw(12) << "MemberNo"
        << setw(10) << "Tier"
        << setw(12) << "Passport"
        << setw(6) << "MRZ"
        << setw(20) << "Name"
        << setw(10) << "Points"
        << endl;
    cout << string(70, '-') << endl;
    vector<Member> sortedMembers = members;
    sort(sortedMembers.begin(), sortedMembers.end(),
        [](const Member& a, const Member& b) {
            return a.getName() < b.getName();
        });

    for (const auto& m : sortedMembers) {
        cout << left
            << setw(12) << m.getMemNum()
            << setw(10) << m.getTier()
            << setw(12) << m.getPassport()
            << setw(6) << m.getMRZ()
            << setw(20) << m.getName()
            << setw(10) << m.getBalance()
            << endl;
    }
    cout << string(70, '-') << endl;
}

void displayFlightTable() {
    cout << "\n=== Flight Records (Not Updated, Sorted by Departure Date Desc) ===\n";
    cout << left
        << setw(12) << "MemberNo"
        << setw(12) << "Origin"
        << setw(12) << "Dest"
        << setw(10) << "Flight"
        << setw(12) << "Cabin"
        << setw(14) << "Departure"
        << setw(14) << "Creation"
        << setw(8) << "Updated"
        << endl;
    cout << string(94, '-') << endl;

    vector<Flight> validFlights;
    for (const auto& f : flights) {
        if (!f.isUpdated()) {
            validFlights.push_back(f);
        }
    }

    sort(validFlights.begin(), validFlights.end(),
        [](const Flight& a, const Flight& b) {
            return dateToInt(a.getDepartureDate()) > dateToInt(b.getDepartureDate());
        });

    for (const auto& f : validFlights) {
        cout << left
            << setw(12) << f.getMemNum()
            << setw(12) << f.getOrigin()
            << setw(12) << f.getDestination()
            << setw(10) << f.getFlightNumber()
            << setw(12) << f.getCabinClass()
            << setw(14) << f.getDepartureDate()
            << setw(14) << f.getCreationDate()
            << setw(8) << (f.isUpdated() ? "1" : "0")
            << endl;
    }
    cout << string(94, '-') << endl;
}

void showAllMemberAccounts() {
    if (!dataLoaded) {
        cout << "No data loaded. Please use option [1] first.\n";
        return;
    }
    displayMemberTable();
    displayFlightTable();
    cout << "\nPress Enter to return to Main Menu...";

    cin.get();
}

// R3  
void openCloseAccount() {
    string inputID;
    cout << "\nPlease enter Member Number: ";
    cin >> inputID;

    int index = -1;
    for (int i = 0; i < members.size(); i++) {
        if (members[i].getMemNum() == inputID) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        members[index].DisplayInfo();

        char confirm;
        cout << "Are you sure you want to close this account? (y/n): ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y')
        {
            for (int i = flights.size() - 1; i >= 0; i--) {
                if (flights[i].getMemNum() == inputID) {
                    flights.erase(flights.begin() + i);
                }
            }
            members.erase(members.begin() + index);
            cout << "Account and related flights have been closed.\n";
        }
        else {
            cout << "Operation cancelled.\n";
        }
    }
    else {
        cout << "Member Not Found. Creating new account...\n";
        string name, passport, tier;

        for (int i = 0; i < 3; ++i) {
            bool success = true;
            cout << "Enter Name: ";
            cin.ignore(10000, '\n');
            getline(cin, name);
            cout << "Enter Passport (e.g., A12345678): ";
            cin >> passport;
            cout << "Enter Tier (Green/Silver/Gold/Diamond): ";
            cin >> tier;

            if (passport.length() != 9) {
                success = false;
            }
            else {
                if (!isupper(passport[0])) {
                    success = false;
                }

                for (int j = 1; j < 9; ++j) {
                    if (!isdigit(passport[j])) {
                        success = false;
                        break;
                    }
                }
            }

            if (tier != "Green" && tier != "Silver" && tier != "Gold" && tier != "Diamond") {
                success = false;
            }

            if (success) {
                string newNum = GenMemNum();
                int mrzVal = GenMRZ(passport);
                Member newMember(newNum, tier, passport, mrzVal, name, 0);
                members.push_back(newMember);
                cout << "\nAccount created successfully!\n";
                members.back().DisplayInfo();
                break;
            }
            else if (i == 2) {
                cout << "Failed to create account after 3 attempts. Returning to main menu.\n";
            }
            else {
                cout << "Invalid input! Remaining attempts: " << 2 - i << "\n";
            }
        }
    }
}

// R4

bool isValidPassport(const string& p) {
    if (p.length() != 9) return false;
    if (!isupper(p[0])) return false;
    for (int i = 1; i < 9; ++i)
        if (!isdigit(p[i])) return false;
    return true;
}

bool isValidFlightNumber(const string& fn) {
    return (fn == "CC81" || fn == "CC82" || fn == "CC31" ||
        fn == "CC32" || fn == "CC61" || fn == "CC62");
}

pair<string, string> getRoute(const string& flightNum) {
    if (flightNum == "CC81") return { "Hong Kong", "London" };
    if (flightNum == "CC82") return { "London", "Hong Kong" };
    if (flightNum == "CC31") return { "Hong Kong", "Dubai" };
    if (flightNum == "CC32") return { "Dubai", "Hong Kong" };
    if (flightNum == "CC61") return { "London", "Dubai" };
    if (flightNum == "CC62") return { "Dubai", "London" };
    return { "", "" };
}

bool getConfirmation() {
    char ch;
    while (true) {
        cout << "Confirm operation? (Y/N): ";
        cin >> ch;
        ch = toupper(ch);
        if (ch == 'Y') return true;
        if (ch == 'N') return false;
        cout << "Invalid input. Please enter Y or N.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void editMemberInfo(Member& m) {
    cout << "\nCurrent Member Info:\n";
    m.DisplayInfo();

    string newName, newPassport, newTier;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter new Member Name (Surname first): ";
    getline(cin, newName);

    // Surname to uppercase
    size_t space = newName.find(' ');
    if (space != string::npos) {
        for (size_t i = 0; i < space; ++i) newName[i] = toupper(newName[i]);
    }
    else {
        for (size_t i = 0; i < newName.length(); ++i) newName[i] = toupper(newName[i]);
    }

    int attempts = 0;
    do {
        cout << "Enter new Passport Number (1 uppercase letter + 8 digits): ";
        cin >> newPassport;
        if (!isValidPassport(newPassport)) {
            cout << "Invalid passport format! ";
            attempts++;
            if (attempts < 3) cout << "You have " << (3 - attempts) << " retries left.\n";
        }
        else break;
    } while (attempts < 3);
    if (attempts == 3) {
        cout << "Too many invalid attempts. Edit cancelled.\n";
        clearInput();
        return;
    }

    cout << "Enter new Member Tier (Green/Silver/Gold/Diamond): ";
    cin >> newTier;
    if (newTier != "Green" && newTier != "Silver" && newTier != "Gold" && newTier != "Diamond") {
        cout << "Invalid tier. Edit cancelled.\n";
        clearInput();
        return;
    }

    if (getConfirmation()) {
        m.setName(newName);
        m.setPassport(newPassport);
        m.setMRZ(GenMRZ(newPassport));
        m.setTier(newTier);
        cout << "Information updated successfully! New MRZ: " << m.getMRZ() << "\n";
    }
    else {
        cout << "Edit cancelled.\n";
    }
}

void updateMileagePoints(Member& m) {
    int sysDateInt = dateToInt(systemDate);
    cout << "\nSystem date: " << systemDate << "\n";
    cout << "Checking flights for member " << m.getMemNum() << " ...\n\n";

    cout << left << setw(15) << "Flight No" << setw(12) << "Cabin"
        << setw(18) << "Departure" << setw(12) << "Earned Pts\n";
    cout << string(57, '-') << "\n";

    int totalEarned = 0;
    for (auto& f : flights) {
        if (f.getMemNum() == m.getMemNum() && !f.isUpdated() &&
            dateToInt(f.getDepartureDate()) <= sysDateInt) {

            int basePoints = 0;
            string orig = f.getOrigin();
            string dest = f.getDestination();
            string cabin = f.getCabinClass();

            if ((orig == "Hong Kong" && dest == "Dubai") || (orig == "Dubai" && dest == "Hong Kong")) {
                if (cabin == "Economy")      basePoints = 2000;
                else if (cabin == "Business") basePoints = 4000;
                else if (cabin == "First")    basePoints = 8000;
            }
            else if ((orig == "Hong Kong" && dest == "London") || (orig == "London" && dest == "Hong Kong")) {
                if (cabin == "Economy")      basePoints = 4000;
                else if (cabin == "Business") basePoints = 8000;
                else if (cabin == "First")    basePoints = 16000;
            }
            else if ((orig == "Dubai" && dest == "London") || (orig == "London" && dest == "Dubai")) {
                if (cabin == "Economy")      basePoints = 2000;
                else if (cabin == "Business") basePoints = 4000;
                else if (cabin == "First")    basePoints = 8000;
            }

            double bonus = 0.0;
            string tier = m.getTier();
            if (tier == "Silver")  bonus = 0.02;
            else if (tier == "Gold")   bonus = 0.04;
            else if (tier == "Diamond") bonus = 0.06;

            int earned = static_cast<int>(basePoints * (1.0 + bonus));
            totalEarned += earned;
            f.markUpdated();

            cout << left << setw(15) << f.getFlightNumber()
                << setw(12) << f.getCabinClass()
                << setw(18) << f.getDepartureDate()
                << earned << " pts\n";
        }
    }

    if (totalEarned == 0) {
        cout << "No unupdated flights with departure <= system date.\n";
    }
    else {
        m.addPoints(totalEarned);
        cout << "\nTotal points earned: " << totalEarned << "\n";
        cout << "New balance: " << m.getBalance() << "\n";
    }
}

void createFlightRecord(Member& m) {
    string flightNum, cabinClass, depDate;
    cout << "Enter Flight Number (CC81/CC82/CC31/CC32/CC61/CC62): ";
    cin >> flightNum;
    if (!isValidFlightNumber(flightNum)) {
        cout << "Invalid flight number. Operation cancelled.\n";
        clearInput();
        return;
    }

    pair<string, string> route = getRoute(flightNum);
    string origin = route.first;
    string dest = route.second;

    cout << "Origin: " << origin << " -> Destination: " << dest << "\n";

    cout << "Enter Cabin Class (First/Business/Economy): ";
    cin >> cabinClass;
    if (cabinClass != "First" && cabinClass != "Business" && cabinClass != "Economy") {
        cout << "Invalid cabin class.\n";
        clearInput();
        return;
    }

    cout << "Enter Departure Date (DD-MM-YYYY): ";
    cin >> depDate;
    if (dateToInt(depDate) <= dateToInt(systemDate)) {
        cout << "Error: Departure date must be after the system date ("
            << systemDate << ").\n";
        clearInput();
        return;
    }

    if (getConfirmation()) {
        flights.push_back(Flight(m.getMemNum(), origin, dest, flightNum,
            cabinClass, depDate, systemDate, false));
        cout << "Flight record created successfully!\n";
    }
    else {
        cout << "Creation cancelled.\n";
    }
}

void redeemOrTransfer(Member& m) {
    cout << "\n[1] Redeem Mileage Points for a Gift\n";
    cout << "[2] Transfer Mileage Points to another member\n";
    cout << "Enter choice: ";
    int sub;
    cin >> sub;

    if (sub == 1) {
        cout << "\nRedemption Catalog:\n";
        cout << "1. Movie voucher            3000 pts\n";
        cout << "2. $100 supermarket voucher 4000 pts\n";
        cout << "3. Airport lounge access    6000 pts\n";
        cout << "Select gift number (1-3): ";
        int gift;
        cin >> gift;
        int cost = 0;
        string desc;
        switch (gift) {
        case 1: cost = 3000; desc = "Movie voucher"; break;
        case 2: cost = 4000; desc = "$100 supermarket voucher"; break;
        case 3: cost = 6000; desc = "Airport lounge access"; break;
        default:
            cout << "Invalid gift number.\n";
            clearInput();
            return;
        }
        if (cost > m.getBalance()) {
            cout << "Insufficient balance! You have " << m.getBalance()
                << " pts, need " << cost << " pts.\n";
            return;
        }
        cout << "Redeem \"" << desc << "\" for " << cost << " points? ";
        if (getConfirmation()) {
            m.deductPoints(cost);
            cout << "Redemption successful. New balance: " << m.getBalance() << "\n";
        }
        else {
            cout << "Redemption cancelled.\n";
        }
    }
    else if (sub == 2) {
        string targetID;
        int amount;
        cout << "Enter target member number: ";
        cin >> targetID;
        // find target by linear search (matches teammate's style)
        int idx = -1;
        for (int i = 0; i < members.size(); ++i)
            if (members[i].getMemNum() == targetID) { idx = i; break; }
        if (idx == -1) {
            cout << "Target member not found.\n";
            clearInput();
            return;
        }
        cout << "Enter amount of points to transfer: ";
        cin >> amount;
        if (amount <= 0) {
            cout << "Amount must be positive.\n";
            return;
        }
        if (amount > m.getBalance()) {
            cout << "Insufficient balance. You have " << m.getBalance() << " pts.\n";
            return;
        }
        cout << "Target member: " << members[idx].getName() << " (" << targetID << ")\n";
        cout << "Transfer " << amount << " points to this member? ";
        if (getConfirmation()) {
            m.deductPoints(amount);
            members[idx].addPoints(amount);
            cout << "Transfer successful.\n";
            cout << "Your new balance: " << m.getBalance() << "\n";
        }
        else {
            cout << "Transfer cancelled.\n";
        }
    }
    else {
        cout << "Invalid choice.\n";
        clearInput();
    }
}

void showMemberInfoBeforeReturn(const Member& m) {
    cout << "\n--- Member Account Summary ---\n";
    cout << "Member Number: " << m.getMemNum() << "\n";
    cout << "Name: " << m.getName() << "\n";
    cout << "Tier: " << m.getTier() << "\n";
    cout << "Passport: " << m.getPassport() << "\n";
    cout << "MRZ: " << m.getMRZ() << "\n";
    cout << "Mileage Points: " << m.getBalance() << "\n";

    cout << "\nUnupdated flights (departure <= today):\n";
    int sysInt = dateToInt(systemDate);
    bool found = false;
    for (const auto& f : flights) {
        if (f.getMemNum() == m.getMemNum() && !f.isUpdated() &&
            dateToInt(f.getDepartureDate()) <= sysInt) {
            cout << f.getFlightNumber() << " " << f.getCabinClass() << " "
                << f.getDepartureDate() << "\n";
            found = true;
        }
    }
    if (!found) cout << "None.\n";
}

void memberAccountOperations(const string& memberNumber) {
    int idx = -1;
    for (int i = 0; i < members.size(); ++i)
        if (members[i].getMemNum() == memberNumber) { idx = i; break; }
    if (idx == -1) {
        cout << "Member not found.\n";
        return;
    }

    Member& current = members[idx];
    int choice;
    do {
        cout << "\n***** Member Account Operations Menu *****\n";
        cout << "Action for Member Number: " << current.getMemNum() << "\n";
        cout << "[1] Edit Member Information\n";
        cout << "[2] Update Mileage Points Balance according to Flight Records\n";
        cout << "[3] Create Flight Records\n";
        cout << "[4] Redeem Mileage Points for a Gift & Transfer\n";
        cout << "[5] Return to Main Menu\n";
        cout << "Option (1-5): ";
        cin >> choice;

        clearInput();

        switch (choice) {
        case 1: editMemberInfo(current); break;
        case 2: updateMileagePoints(current); break;
        case 3: createFlightRecord(current); break;
        case 4: redeemOrTransfer(current); break;
        case 5: showMemberInfoBeforeReturn(current); break;
        default:
            cout << "Invalid option. Please try again.\n";
            clearInput();
        }
    } while (choice != 5);
}

//R5
void generateDailyStatment(const string& memberNumber) {
    int idx = -1;
    for (int i = 0; i < members.size(); ++i)
        if (members[i].getMemNum() == memberNumber) { idx = i; break; }
    if (idx == -1) {
        cout << "Member not found.\n";
        return;
    }

    cout << "Member Name: " << members[idx].getName() << endl;
    cout << "Member Number: " << members[idx].getMemNum() << endl;
    cout << "Statement Date: " << systemDate << endl;
    //time not set
    cout << "----------------------------------------------------------------" << endl;
    cout << "Transaction Summary: " << endl;
    cout << setw(20) << left << "Type" << setw(15) << "Mileage" << "Description";
    //R4.4 record needed
    cout << "----------------------------------------------------------------" << endl;
    cout << "Upcoming Itinerary: " << endl;
    cout << left << setw(15) << "Origin" << setw(20) << "Destination" << setw(20) << "Flight Number" << setw(15) << "Cabin Class" << setw(15) << "Departure Date" << endl;
    for (int i = 0; i < flights.size(); ++i) {
        if (flights[i].getMemNum() == memberNumber) {
            cout << left << setw(15) << flights[i].getOrigin() << setw(20) << flights[i].getDestination() << setw(20) << flights[i].getFlightNumber() << setw(15) << flights[i].getCabinClass() << setw(15) << flights[i].getDepartureDate() << endl;
        }
    }
    cout << "----------------------------------------------------------------" << endl;
    cout << "Member Account Summary: " << endl;
    cout << setw(70) << "Total Mileage Points Balance";
    cout << right << ": " << members[idx].getBalance() << endl;
    cout << setw(70) << "Member Tier";
    cout << right << ": " << members[idx].getTier() << endl;
    cout << setw(70) << "Bonus Mileage Points";
    cout << right << ": ";
    if (members[idx].getTier() == "Green") {
        cout << "0%";
    }
    else if (members[idx].getTier() == "Silver") {
        cout << "2%";
    }
    else if (members[idx].getTier() == "Gold") {
        cout << "4%";
    }
    else cout << "6%";
};

//R6 
void creditsAndExit()
{
    char confirm;
    bool valid = false;

    while (!valid) {
        cout << "\nAre you sure you want to exit? (Y/N): ";
        cin >> confirm;
        confirm = toupper(confirm);
        cin.ignore(1000, '\n');

        if (confirm == 'Y') {
            valid = true;
            cout << "\n============================================\n";
            cout << "               CREDITS                     \n";
            cout << "============================================\n";
            cout << "Student Name: Lam Ka Leong          Student ID: 25128706S   Group: B01A\n";
            cout << "Student Name: LI Yu Hin             Student ID: __________   Group: ____\n";
            cout << "Student Name: FOK Yuk Sang Victor   Student ID: __________   Group: ____\n";
            cout << "Student Name: CHIANG Cheuk Hang     Student ID: __________   Group: ____\n";
            cout << "Student Name: CHAN Ka Kit           Student ID: __________   Group: ____\n";
            cout << "Student Name: HO Hin San            Student ID: __________   Group: ____\n";
            cout << "============================================\n";
            cout << "Thank you for using FFP System. Goodbye!\n";
        }
        else if (confirm == 'N') {
            valid = true;
            cout << "Exit cancelled. Returning to Main Menu...\n";
        }
        else {
            cout << "Invalid input. Please enter Y or N.\n";
        }
    }
}
