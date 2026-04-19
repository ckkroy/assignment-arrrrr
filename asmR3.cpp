#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

class Member {
public:
    Member(string num, string tier, string passport, int mrzVal, string name, int balance = 0)
        : memberNumber(num), memberTier(tier), passportNumber(passport), mrz(mrzVal), memberName(name), mileageBalance(balance) {
    }

    string getMemberNumber() const { return memberNumber; }
    string getMemberName() const { return memberName; }

    void displayInfo() const {
        cout << "Member Number: " << memberNumber << endl;
        cout << "Name: " << memberName << endl;
        cout << "Tier: " << memberTier << endl;
        cout << "Passport: " << passportNumber << " (MRZ: " << mrz << ")" << endl;
        cout << "Balance: " << mileageBalance << endl;
    }

private:
    string memberNumber;
    string memberTier;
    string passportNumber;
    int mrz;
    string memberName;
    int mileageBalance;
};

vector<Member> memberList;

void handleR3() {
    string inputID;
    cout << "Please enter Member Number: ";
    cin >> inputID;

    // 檢查編號是否存在 [cite: 105, 106, 107]
    bool exists = false;
    int index = -1;
    for (int i = 0; i < memberList.size(); i++) {
        if (memberList[i].getMemberNumber() == inputID) {
            exists = true;
            index = i;
            break;
        }
    }

    if (exists) {
        memberList[index].displayInfo();
        char confirm;
        cout << "Confirm to close account? (y/n): "; 
        cin >> confirm;
        if (confirm == 'y' || confirm == 'Y') {
            memberList.erase(memberList.begin() + index); 
            cout << "Account closed successfully." << endl; 
        }
    }
    else {
        string name, passport, tier;
        int attempts = 0;
        bool success = false;

        while (attempts < 3) {
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Passport (e.g., A12345678): "; 
            cin >> passport;
            cout << "Enter Tier (Green/Silver/Gold/Diamond): ";
            cin >> tier;

            if (passport.length() == 9 && isupper(passport[0])) {
                success = true;
                break;
            }
            else {
                attempts++;
                cout << "Invalid input! Remaining attempts: " << 3 - attempts << endl;
            }
        }

        if (success) {
            // 1. 自動生成 Member Number (2026 + 隨機5位) [cite: 44, 111]
            string newID = "2026" + to_string(rand() % 90000 + 10000);

            // 2. 計算 MRZ (這部分需要實作 R1.1 的算法) [cite: 54, 70, 111]
            int calculatedMRZ = 3; // 假設計算結果為 3

            // 3. 加入系統 [cite: 111]
            Member newMember(newID, tier, passport, calculatedMRZ, name, 0);
            memberList.push_back(newMember);
            cout << "New account created! ID: " << newID << endl;
        }
        else {
            cout << "Failed to create account after 3 attempts." << endl;
        }
    }
}

int main() {
    char input;
    do {
        cout << "hi" << endl;
        cin >> input;
        if (input == 3)
            handleR3();
    } while (input != 'q');
    return 0;
}