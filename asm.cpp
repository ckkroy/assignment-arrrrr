#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <ctime> 
using namespace std;

class Member {
public:
    Member(string num, string name, string tier, string passport, int MRZ, int balance) {
        memberNumber = num;
        memberName = name;
        memberTier = tier;

        passportNumber = passport;
        mrz = MRZ;
        mileageBalance = balance;
    }

    string getMemNum() const {
        return memberNumber;
    }

    void DisplayInfo() const {
        cout << "Member Number: " << memberNumber << endl;
        cout << "Name: " << memberName << endl;
        cout << "Tier: " << memberTier << endl;
        cout << "Passport: " << passportNumber << " (MRZ: " << mrz << ")" << endl;
        cout << "Balance: " << mileageBalance << endl;
    }

private:
    string memberName;
    string passportNumber;
    string memberTier;
    string memberNumber;
    int mrz;
    int mileageBalance;
};

string GenMemNum(const vector<Member>& vMember);
int GenMRZ(const string& pNum);

void R3(vector<Member>& vMember) {
    string inputID;
    int index = -1;

    cout << "\nPlease enter Member Number: ";
    cin >> inputID;

    for (int i = 0; i < vMember.size(); i++) {
        if (vMember[i].getMemNum() == inputID) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        vMember[index].DisplayInfo();

        char confirm;
        cout << "Are you sure you want to close this account? (y/n): ";
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            vMember.erase(vMember.begin() + index);
            cout << "Account has been closed." << endl;
        }
        else {
            cout << "Operation cancelled." << endl;
        }
    }
    else {
        cout << "Member Not Found." << endl;
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

            if (passport.length() != 9 ) {
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
            
            if (success == true) {
                string Mnum = GenMemNum(vMember);
                int MRZ = GenMRZ(passport);
                Member newMember(Mnum, name, tier, passport, MRZ, 0);
                vMember.push_back(newMember);
                cout << "\nAccount created successfully!" << endl;
				break;  
            }
            else if (i == 2) {
                cout << "Failed to create account after 3 attempts. Return to main page" << endl;
            }
            else {
                cout << "Invalid input! Remaining attempts: " << 2 - i << endl;
            }
		}
    }
}

string GenMemNum(const vector<Member>& vMember) {
    string newNumber;
    bool unique;
    time_t t = time(0);
    tm* now = localtime(&t);
    int year_int = now->tm_year + 1900;
    string year = to_string(year_int);

    do {
        unique = true;
        int randomDigits = rand() % 90000 + 10000;
        newNumber = year + to_string(randomDigits);

        for (int i = 0; i < vMember.size(); i++) {
            if (vMember[i].getMemNum() == newNumber) {
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

int main() {
    srand(time(0));
    vector<Member> vMember;

    vMember.push_back(Member("202611111", "Test User", "Gold", "T12345678", 6, 50000));
    vMember.push_back(Member("202522222", "Another User", "Silver", "U87654321", 2, 25000));

    char input;
    do {
        cout << "\nEnter your option ('3' to test,'d' to display, 'q' to quit): ";
        cin >> input;

        if (input == '3') {
            R3(vMember);
        }
        else if (input == 'd') {
            cout << "\n--- Displaying All Member Accounts ---" << endl;

            // First, check if the vector is empty
            if (vMember.empty()) {
                cout << "There are no members in the system to display." << endl;
            }
            else {
                // If it's not empty, loop through each member
                for (int i = 0; i < vMember.size(); i++) {
                    // Call the DisplayInfo() function on the member at the current position
                    vMember[i].DisplayInfo();
					cout << endl;   
                }
            }
            cout << "--------------------------------------" << endl;
        }
    } while (input != 'q');

    return 0;
}
