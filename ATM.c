#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Account
{
    int accountNumber;
    int accountPIN[4];
    int confirmAccountPin[4];
    char name[50];
    float accountBalance;
} Account;

Account accounts[100];
int numAccounts = 0;

void createAccount(Account *account)
{
    bool validAccountNumber = false;
    while (!validAccountNumber)
    {
        printf("Welcome to this bank!\nPlease enter your 10-digit account number: ");
        scanf("%d", &account->accountNumber);

        if (account->accountNumber < 1000000000 || account->accountNumber > 9999999999)
        {
            printf("Invalid account number. Please enter a 10-digit account number.\n");
        }
        else
        {
            validAccountNumber = true;
        }
    }

    // Check if account number is already taken
    for (int i = 0; i < numAccounts; i++)
    {
        if (accounts[i].accountNumber == account->accountNumber)
        {
            printf("Account number already taken. Please try again.\n");
            return;
        }
    }

    printf("Enter your name: ");
    scanf("%49s", account->name); // limit the input to 49 characters to avoid buffer overflow

    account->accountBalance = 0.0;
    for (int i = 0; i < 4; i++)
    {
        account->accountPIN[i] = 0;
        account->confirmAccountPin[i] = 0;
    }

    printf("Create a PIN (4 digits): ");
    for (int i = 0; i < 4; i++)
    {
        scanf("%1d", &account->accountPIN[i]);
        printf("%c", 'x');
    }
    printf("\n");

    printf("Confirm PIN: ");
    for (int i = 0; i < 4; i++)
    {
        scanf("%1d", &account->confirmAccountPin[i]);
        printf("%c", 'x');
    }
    printf("\n");

    bool validPIN = true;
    for (int i = 0; i < 4; i++)
    {
        if (account->accountPIN[i] != account->confirmAccountPin[i])
        {
            validPIN = false;
            break;
        }
    }

    if (validPIN)
    {
        accounts[numAccounts++] = *account;
        printf("Account created successfully.\n");
    }
    else
    {
        printf("PINs do not match. Please try again.\n");
    }
}

int findAccount(int accountNumber)
{
    for (int i = 0; i < numAccounts; i++)
    {
        if (accounts[i].accountNumber == accountNumber)
        {
            return i;
        }
    }
    return -1;
}

void checkAccountBalance(int accountNumber)
{
    int index = findAccount(accountNumber);
    if (index != -1)
    {
        printf("Your account balance is: $%.2f\n", accounts[index].accountBalance);
    }
    else
    {
        printf("Account not found.\n");
    }
}

void deposit(int accountNumber)
{
    int index = findAccount(accountNumber);
    if (index != -1)
    {
        float amount = 0.0;
        printf("Enter the amount to deposit: ");
        scanf("%f", &amount); // changed format specifier to %f

        if (amount > 0)
        {
            accounts[index].accountBalance += amount;
            printf("Deposit successful.\n");
        }
        else
        {
            printf("Invalid deposit amount. Please enter a positive value.\n");
        }
    }
    else
    {
        printf("Account not found.\n");
    }
}

void withdraw(int accountNumber, float amount)
{
    int index = findAccount(accountNumber);
    if (index != -1)
    {
        if (amount <= accounts[index].accountBalance)
        {
            accounts[index].accountBalance -= amount;
            printf("Withdrawal successful.\n");
        }
        else
        {
            printf("Insufficient funds. Please enter a lower amount.\n");
        }
    }
    else
    {
        printf("Account not found.\n");
    }
}

void saveAccountsToFile()
{
    FILE *file = fopen("accounts.dat", "wb");
    if (file == NULL)
    {
        printf("Error: Unable to open the file for saving.\n");
        exit(1); // terminate with error
    }

    int numBytesWritten = fwrite(accounts, sizeof(Account), numAccounts, file);
    if (numBytesWritten != numAccounts * sizeof(Account))
    {
        printf("Error: Unable to write all accounts to the file.\n");
        exit(1); // terminate with error
    }

    fclose(file);
}

void loadAccountsFromFile()
{
    FILE *file = fopen("accounts.dat", "rb");
    if (file == NULL)
    {
        printf("Error! File is unable to be opened for loading.\n");
        return;
    }

    int numBytesRead = fread(accounts, sizeof(Account), 100, file);
    if (numBytesRead < numAccounts * sizeof(Account))
    {
        printf("Error: Unable to read all accounts from the file.\n");
        exit(1); // terminate with error
    }

    numAccounts = numBytesRead / sizeof(Account);
    fclose(file);
}

int main()
{
    printf("Welcome to Prime Bank!\n");
    char response;
    Account newAccount = {0};

    loadAccountsFromFile();

    do
    {
        printf("Do you wish to proceed? (Y/N) ");
        scanf(" %c", &response);

        if (response == 'Y' || response == 'y')
        {
            int choice;

            printf("\nATM Menu:\n");
            printf("1. Create Account\n");
            printf("2. Check Balance\n");
            printf("3. Deposit\n");
            printf("4. Withdrawal\n");
            printf("5. Change PIN\n");
            printf("6. Enquiries\n");
            printf("7. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                createAccount(&newAccount);
                break;
            case 2:
                int accountNumber;
                printf("Enter an account number: ");
                scanf("%d", &accountNumber);
                checkAccountBalance(accountNumber);
                break;
            case 3:
                int accountNumber2;
                printf("Enter your account number: ");
                scanf("%d", &accountNumber2);
                deposit(accountNumber2);
                break;
            case 4:
                int accountNumber3;
                float amount;
                printf("Enter your account number: ");
                scanf("%d", &accountNumber3);
                printf("Enter the amount needed to withdraw: ");
                scanf("%f", &amount);
                withdraw(accountNumber3, amount);
                break;
            case 5:
                int accountNumber4;
                float newPIN;
                printf("Enter your account number: ");
                scanf("%d", &accountNumber4);
                printf("Input a new PIN: ");
                scanf("%f", &newPIN);
                // Update the PIN for the account
                break;
            case 6:
                // Enquiries option can be implemented here
                printf("Enquiries are not yet available. Please try again later or go to your bank for any enquiries.\n");
                break;
            case 7:
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
            }
        }
        else if (response == 'N' || response == 'n')
        {
            printf("Thank you for using Prime Bank. Goodbye!\n");
            exit(0);
        }
        else
        {
            printf("Invalid response. Please enter Y or N: ");
            scanf(" %c", &response);
        }
    } while (1);

    saveAccountsToFile();
    return 0;
}
