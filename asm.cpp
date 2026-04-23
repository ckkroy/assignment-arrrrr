#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

string GenMemNum(const vector<Member>& vMember);
int GenMRZ(const string& pNum);

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
        int attempts = 0;
        bool success = true;

        while (attempts < 3) {
            attempts++;
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Passport (e.g., A12345678): ";
            cin >> passport;
            cout << "Enter Tier (Green/Silver/Gold/Diamond): ";
            cin >> tier;

            if (!(passport.length() == 9 && isupper(passport[0]) && isdigit(passport[1]) && isdigit(passport[2]) && isdigit(passport[3]) && isdigit(passport[4]) && isdigit(passport[5]) && isdigit(passport[6]) && isdigit(passport[7]) && isdigit(passport[8]))) {
                success = false;
                break;
            }

            if (tier != "Green" && tier != "Silver" && tier != "Gold" && tier != "Diamond") {
                success = false;
                break;
            }

            if (!success && attempts < 3) {
                cout << "Invalid input! Remaining attempts: " << 3 - attempts << endl;
            }

            if (success) {
                string Mnum = GenMemNum(vMember);
                int MRZ = GenMRZ(passport);
                Member newMember(Mnum, name, tier, passport, MRZ, 0);
                vMember.push_back(newMember);
				cout << "\nAccount created successfully!" << endl;

            }
            if (attempts >= 3) {
                cout << "Failed to create account after 3 attempts. Return to main page" << endl;
            }
        }
    }
}

string GenMemNum(const vector<Member>& vMember) {
    string newNumber;
    bool unique;
    string year = "2026";

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
    vector<Member> vMember;
    char input;
    do {
        cout << "\nEnter your option ('3' to test, 'q' to quit): ";
        cin >> input;

        if (input == '3') {
            R3(vMember);
        }

    } while (input != 'q');

    return 0;
}
