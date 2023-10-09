#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
// add color
void blue()
{
    printf("\033[0;34m");
}

void green()
{
    printf("\033[0;32m");
}

void white()
{
    printf("\033[0;37m");
}

// Structure to store customer information
struct Customer
{
    char name[50];
    int accountNumber;
    double balance;
    char dob[12]; // Date of birth (format: yyyy-mm-dd)
    char address[100];
    char nationality[50];
    int age;
    char gender; // 'M' for Male, 'F' for Female, 'O' for Other
};

// Function to create a new customer
struct Customer createCustomer(char name[], int accountNumber, double balance, char dob[], char address[], char nationality[], int age, char gender)
{
    struct Customer customer;
    strcpy(customer.name, name);
    customer.accountNumber = accountNumber;
    customer.balance = balance;
    strcpy(customer.dob, dob);
    strcpy(customer.address, address);
    strcpy(customer.nationality, nationality);
    customer.age = age;
    customer.gender = gender;
    return customer;
}

// Function to deposit funds
void deposit(struct Customer *customer, double amount)
{
    customer->balance += amount;
}

// Function to withdraw funds
void withdraw(struct Customer *customer, double amount)
{
    if (customer->balance >= amount)
    {
        customer->balance -= amount;
    }
    else
    {
        printf("Insufficient balance\n");
    }
}

// Function to check account balance
void checkBalance(struct Customer customer)
{
    printf("Account Number   : %d\n", customer.accountNumber);
    printf("Customer Name    : %s\n", customer.name);
    printf("Date of Birth    : %s\n", customer.dob);
    printf("Address          : %s\n", customer.address);
    printf("Nationality      : %s\n", customer.nationality);
    printf("Age              : %d\n", customer.age);
    printf("Gender           : %c\n", customer.gender);
    printf("Account Balance  : %.2lf\n", customer.balance);
}

// Display Customer list(sorted by bubble sort algorithm)
void showCustomers(struct Customer customers[], int customerCount)
{
    printf("\nList of Customers (Sorted by Account Number):\n");
    // Bubble Sort based on Account Number
    for (int i = 0; i < customerCount - 1; i++)
    {
        for (int j = 0; j < customerCount - i - 1; j++)
        {
            if (customers[j].accountNumber > customers[j + 1].accountNumber)
            {
                struct Customer temp = customers[j];
                customers[j] = customers[j + 1];
                customers[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < customerCount; i++)
    {
        printf("Customer %d:\n", i + 1);
        checkBalance(customers[i]);
        printf("\n");
    }
}

// Binary search for account number
int binarySearch(struct Customer customers[], int low, int high, int target)
{
    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (customers[mid].accountNumber == target)
        {
            return mid; // Found
        }
        else if (customers[mid].accountNumber < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    return -1; // Not found
}

// Function to find the index of a customer by account number
int findCustomerIndex(struct Customer customers[], int customerCount, int accountNumber)
{
    int result = binarySearch(customers, 0, customerCount - 1, accountNumber);
    return result;
}
//
int linearSearch(struct Customer customers[], int n, int target)
{
    for (int i = 0; i < n; i++)
    {
        if (customers[i].accountNumber == target)
        {
            return i;
            break;
        }
    }
}
//

int randomNumber()
{
    // Seed the random number generator with the current time
    srand(time(NULL));

    // Generate an 8-digit random integer
    int randomInt = 0;
    for (int i = 0; i < 5; i++)
    {
        randomInt = randomInt * 10 + (rand() % 10);
    }
}

// Function to calculate age
int calculateAge(int birthYear, int birthMonth, int birthDay)
{
    // Get the current date
    time_t t = time(NULL);
    struct tm currentTime = *localtime(&t);
    int currentYear = currentTime.tm_year + 1900;
    int currentMonth = currentTime.tm_mon + 1;
    int currentDay = currentTime.tm_mday;

    // Calculate the age
    int age = currentYear - birthYear;

    // Adjust the age if the birthdate hasn't occurred this year yet
    if (currentMonth < birthMonth || (currentMonth == birthMonth && currentDay < birthDay))
    {
        age--;
    }

    return age;
}

//

// Function to delete an element from an array
int deleteElement(struct Customer customers[], int *n, int pos)
{
    if (pos < 0 || pos >= *n)
    {
        // Invalid position, return 0 to indicate failure
        return 0;
    }
    else
    {
        // Shift elements to the left to delete the element
        for (int i = pos; i < *n - 1; i++)
        {
            customers[i] = customers[i + 1];
        }

        // Decrement the number of elements
        (*n)--;

        // Return 1 to indicate success
        return 1;
    }
}

int main()
{
    struct Customer customers[100]; // Array to store customer records
    int customerCount = 0;          // Number of customers

    int choice;
    do
    {
        system("clear"); // Clear the screen
        blue();
        printf("\n*************Banking Management System*************\n\n");
        green();
        printf("\t1. Add Customer\n");
        printf("\t2. Deposit\n");
        printf("\t3. Withdraw\n");
        printf("\t4. Check Balance\n");
        printf("\t5. List of Customers\n");
        printf("\t6. Search Account\n"); // Add Search Account option
        printf("\t7. Send money.\n");
        printf("\t8. Delete Customer\n");
        printf("\t9. Exit\n");
        white();
        printf("Enter your choice:\n");
        printf("->");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            char name[50];
            int accountNumber;
            double balance;
            char dob[12];
            char address[100];
            char nationality[50];
            int age;
            char gender;

            printf("Enter customer name: ");
            scanf("%s", name);
            accountNumber = randomNumber();
            printf("Enter date of birth (yyyy-mm-dd): ");
            scanf("%s", dob);
            // Extract year, month, and day from the input string
            int birthYear, birthMonth, birthDay;
            sscanf(dob, "%d-%d-%d", &birthYear, &birthMonth, &birthDay);

            // Calculate the age using the function
            age = calculateAge(birthYear, birthMonth, birthDay);
            printf("Age: %d\n", age);
            if (age < 18)
            {
                printf("Sorry you are not eligable.\n");
            }
            else
            {
                printf("Enter City: ");
                scanf("%s", address);
                printf("Enter nationality: ");
                scanf("%s", nationality);
                // printf("Enter age: ");
                // scanf("%d", &age);
                printf("Enter gender (M/F/O): ");
                scanf(" %c", &gender);
                printf("Your Account number: %d\n", accountNumber);
                printf("Enter initial balance: ");
                scanf("%lf", &balance);

                // scanf("%d", &accountNumber);

                customers[customerCount] = createCustomer(name, accountNumber, balance, dob, address, nationality, age, gender);
                customerCount++;
                printf("Customer added successfully.\n");
            }
            break;
        }
        case 2:
        {
            int accountNumber;
            double amount;
            printf("Enter account number: ");
            scanf("%d", &accountNumber);
            int customerIndex = findCustomerIndex(customers, customerCount, accountNumber);
            if (customerIndex != -1)
            {
                printf("Enter deposit amount: ");
                scanf("%lf", &amount);
                deposit(&customers[customerIndex], amount);
                printf("Deposit successful.\n");
            }
            else
            {
                printf("Account not found. Please enter a valid account number.\n");
            }
            break;
        }
        case 3:
        {
            int accountNumber;
            double amount;
            printf("Enter account number: ");
            scanf("%d", &accountNumber);
            int customerIndex = findCustomerIndex(customers, customerCount, accountNumber);
            if (customerIndex != -1)
            {
                printf("Enter withdrawal amount: ");
                scanf("%lf", &amount);
                withdraw(&customers[customerIndex], amount);
                printf("Withdrawal successful.\n");
            }
            else
            {
                printf("Account not found. Please enter a valid account number.\n");
            }
            break;
        }
        case 4:
        {
            int accountNumber;
            printf("Enter account number: ");
            scanf("%d", &accountNumber);
            int customerIndex = findCustomerIndex(customers, customerCount, accountNumber);

            if (customerIndex != -1)
            {
                green();
                checkBalance(customers[customerIndex]);
                white();
            }

            else
            {
                printf("Account not found. Please enter a valid account number.\n");
            }
            break;
        }
        case 5:
            green();
            showCustomers(customers, customerCount);
            white();
            break;
        case 6:
        {
            int accountNumber;
            printf("Enter account number to search: ");
            scanf("%d", &accountNumber);
            // Binary search for the account number
            int result = binarySearch(customers, 0, customerCount - 1, accountNumber);
            if (result != -1)
            {
                green();
                printf("Account found:\n");
                checkBalance(customers[result]);
                white();
            }
            else
            {
                printf("Account not found. Please enter a valid account number.\n");
            }
            break;
        }

        case 7:
        {
            int SenderAccountNumber;
            double amount;
            printf("Enter sender's account number: ");
            scanf("%d", &SenderAccountNumber);
            int SendercustomerIndex = findCustomerIndex(customers, customerCount, SenderAccountNumber);
            if (SendercustomerIndex != -1)
            {
                int ReceiverAccountNumber;
                double amount;
                printf("Enter receiver's account number: ");
                scanf("%d", &ReceiverAccountNumber);
                int ReceivercustomerIndex = findCustomerIndex(customers, customerCount, ReceiverAccountNumber);
                if (ReceivercustomerIndex != -1)
                {
                    printf("Enter Send amount: ");
                    scanf("%lf", &amount);
                    deposit(&customers[ReceivercustomerIndex], amount);
                    withdraw(&customers[SendercustomerIndex], amount);
                    printf("Send successful.\n");
                }
                else
                {
                    printf("Account not found. Please enter a valid account number.\n");
                }

                // printf("Enter withdrawal amount: ");
                // scanf("%lf", &amount);
                // withdraw(&(head[customerIndex].customer), amount);
                // printf("Withdrawal successful.\n");
                break;
            }
            else
            {
                printf("Account not found. Please enter a valid account number.\n");
            }
            break;
        }
        case 9:
            printf("Exiting the program. Goodbye!\n");
            break;

        case 8:
        {
            int acc;
            printf("Enter Account Number: ");
            scanf("%d", &acc);
            int pos = linearSearch(customers, customerCount, acc);
            printf("location: %d\n", pos);

            int accountNumber = acc;
            // Binary search for the account number
            int result = binarySearch(customers, 0, customerCount - 1, accountNumber);
            if (result != -1)
            {
                green();
                printf("Account found:\n");
                checkBalance(customers[result]);
                white();
                int delet = deleteElement(customers, &customerCount, pos);
                if (delet)
                {
                    printf("delete successful.\n");
                }
            }
            else
            {
                printf("Account not found. Please enter a valid account number.\n");
            }
            break;
        }

        default:
            printf("Invalid choice. Please try again.\n");
        }

        printf("Press Enter to continue...");
        getchar(); // Wait for Enter key press
        getchar(); // Wait for Enter key press again (to clear any remaining input buffer)
    } while (choice != 9);

    return 0;
}
