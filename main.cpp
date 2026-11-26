#include "atm.h"
#include "ui.h"

#ifdef _WIN32
#include <windows.h>
#endif

void ShowStatus(const ATM &atm)
{
    PrintStatus({{"State", atm.GetStateName()},
                 {"Money", std::to_string(atm.GetMoney()) + " UAH"},
                 {"Tape", Num(atm.GetTapeLength()) + " m"},
                 {"No tape", atm.GetNoTape() ? "YES" : "NO"},
                 {"User", atm.GetLevelName()}});
}

void ShowControls()
{
    std::cout << "\n[ CONTROLS ]\n";
    std::cout << "1. Login as guest\n";
    std::cout << "2. Login as admin\n";
    std::cout << "3. Logout\n";
    std::cout << "4. Set money (admin)\n";
    std::cout << "5. Set tape length (admin)\n";
    std::cout << "6. Withdraw money\n";
    std::cout << "0. Exit\n";
    std::cout << "\nSelect action: ";
}

void LoginAdmin(ATM &atm)
{
    std::string password;
    std::cout << "Enter password: ";
    std::cin >> password;
    if (atm.Authorize(UserLevel::Admin, password))
        Done("Logged in as ADMIN");
    else
        Warn("Wrong password! Access denied.");
}

void ChangeMoney(ATM &atm)
{
    double sum;
    if (!ReadValue("Enter money (0 - 100000, multiple of 5): ", sum))
        return;
    if (atm.GetLevel() != UserLevel::Admin)
        Warn("Only ADMIN can change money!");
    else if (atm.SetMoney(static_cast<int>(sum)))
        Done();
    else
        Warn("Wrong sum! It must be from 0 to 100000 and multiple of 5.");
}

void ChangeTape(ATM &atm)
{
    double meters;
    if (!ReadValue("Enter tape length in meters (0 - 30): ", meters))
        return;
    if (atm.GetLevel() != UserLevel::Admin)
        Warn("Only ADMIN can change tape!");
    else if (atm.SetTapeLength(meters))
        Done();
    else
        Warn("Wrong length! It must be from 0 to 30 m.");
}

void TakeMoney(ATM &atm)
{
    double sum;
    if (!ReadValue("Enter sum to withdraw: ", sum))
        return;
    int money = static_cast<int>(sum);
    if (atm.GetState() == AtmState::NotWorking) {
        Warn("ATM is not working!");
        return;
    }
    if (atm.Withdraw(money)) {
        Done("Take your money: " + std::to_string(money) + " UAH");
        if (atm.GetNoTape())
            Warn("Receipt tape is almost over!");
    } else if (atm.GetState() == AtmState::Error) {
        Warn("Not enough money in ATM. State: ERROR (call admin)");
    } else {
        Warn("Wrong sum! It must be multiple of 5 and not bigger than money in ATM.");
    }
}

void ControlPanel(ATM &atm)
{
    while (true) {
        PrintBox("ATM CONTROL PANEL");
        ShowStatus(atm);
        ShowControls();

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        if (choice == 0)
            return;

        switch (choice) {
        case 1:
            atm.Authorize(UserLevel::Guest);
            Done("Logged in as GUEST");
            break;
        case 2: LoginAdmin(atm); break;
        case 3:
            atm.Logout();
            Done("Logged out");
            break;
        case 4: ChangeMoney(atm); break;
        case 5: ChangeTape(atm); break;
        case 6: TakeMoney(atm); break;
        default: Warn("Unknown action!");
        }
        WaitEnter();
    }
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    ATM atm;
    atm.Reset();
    ControlPanel(atm);

    PrintBox("ATM IS TURNED OFF. GOODBYE!");
    return 0;
}
