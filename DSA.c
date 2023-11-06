#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
// Define the structure for a single transaction
struct Transaction
{
    char type[20];     // Deposit, Withdrawal, or Money Transfer
    int accountNumber; // Account number associated with the transaction
    double amount;
    char timestamp[20]; // Timestamp for the transaction
};

#define MAX_TRANSACTIONS 50
// Define a stack structure for transaction history of the bank
struct TransactionStack
{
    int top;
    struct Transaction items[MAX_TRANSACTIONS];
};

// Initialize the transaction history stack
void initializeTransactionStack(struct TransactionStack *stack)
{
    stack->top = -1;
}

// Check if the transaction history stack is full
int isTransactionStackFull(struct TransactionStack *stack)
{
    return stack->top == MAX_TRANSACTIONS - 1;
}

// Check if the transaction history stack is empty
int isTransactionStackEmpty(struct TransactionStack *stack)
{
    return stack->top == -1;
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

// Structure for a task in the work queue
struct Task
{
    char description[100];
    int priority; // To indicate the importance of the task
};

// Circular Queue to store tasks for the day
struct CircularQueue
{
    int front, rear;
    int size;
    unsigned capacity;
    struct Task *array;
};

// Initialize a Circular Queue with a specific capacity
struct CircularQueue *initQueue(unsigned capacity)
{
    struct CircularQueue *queue = (struct CircularQueue *)malloc(sizeof(struct CircularQueue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1; // Important to set the rear at the end initially
    queue->array = (struct Task *)malloc(queue->capacity * sizeof(struct Task));
    return queue;
}

// Function to display transaction history for the bank
void displayTransactionHistory(struct TransactionStack *stack)
{
    if (isTransactionStackEmpty(stack))
    {
        printf("No transaction history available.\n");
        return;
    }

    printf("\nTransaction History for the Bank (Last 50 transactions):\n");
    for (int i = stack->top; i >= 0; i--)
    {
        printf("Type: %s, Amount: %.2lf, Timestamp: %s\n", stack->items[i].type, stack->items[i].amount, stack->items[i].timestamp);
    }
}

// Check if the queue is full
int isFull(struct CircularQueue *queue)
{
    return (queue->size == queue->capacity);
}

// Check if the queue is empty
int isEmpty(struct CircularQueue *queue)
{
    return (queue->size == 0);
}

// Enqueue a task into the circular queue
void enqueue(struct CircularQueue *queue, struct Task task)
{
    if (isFull(queue))
    {
        printf("Queue is full, can't add more tasks!\n");
        return;
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = task;
    queue->size = queue->size + 1;
    printf("Task '%s' enqueued successfully.\n", task.description);
}

// Dequeue a task from the circular queue
struct Task dequeue(struct CircularQueue *queue)
{
    if (isEmpty(queue))
    {
        printf("Queue is empty, no tasks to dequeue!\n");
        struct Task emptyTask;
        emptyTask.priority = -1; // Assign an invalid priority to indicate an empty task
        return emptyTask;
    }
    struct Task task = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    printf("Task '%s' dequeued successfully.\n", task.description);
    return task;
}

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

// Add this function to display the last 10 transactions
void displayLastTenTransactions(struct TransactionStack *stack)
{
    int count = 0;
    printf("\nTransaction History:\n");
    for (int i = stack->top; i >= 0 && count < 10; i--)
    {
        printf("Account Number: %d, Type: %s, Amount: %.2lf, Timestamp: %s\n",
               stack->items[i].accountNumber, stack->items[i].type,
               stack->items[i].amount, stack->items[i].timestamp);
        count++;
    }
}

// Modify pushTransaction function to save transactions for deposit, withdrawal, and send money
void pushTransaction(struct TransactionStack *stack, struct Transaction transaction)
{
    if (isTransactionStackFull(stack))
    {
        for (int i = 0; i < MAX_TRANSACTIONS - 1; i++)
        {
            stack->items[i] = stack->items[i + 1];
        }
        stack->top--;
    }
    stack->items[++stack->top] = transaction;
}

// Modify the deposit function to log the transaction
void deposit(struct Customer *customer, double amount, struct TransactionStack *transactionStack)
{
    customer->balance += amount;

    struct Transaction depositTransaction;
    strcpy(depositTransaction.type, "Deposit");
    depositTransaction.accountNumber = customer->accountNumber; // Account number
    depositTransaction.amount = amount;
    // Add timestamp to the transaction
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(depositTransaction.timestamp, sizeof(depositTransaction.timestamp), "%Y-%m-%d %H:%M:%S", t);
    pushTransaction(transactionStack, depositTransaction);
}

// Modify the withdraw function to log the transaction
void withdraw(struct Customer *customer, double amount, struct TransactionStack *transactionStack)
{
    if (customer->balance >= amount)
    {
        customer->balance -= amount;

        struct Transaction withdrawalTransaction;
        strcpy(withdrawalTransaction.type, "Withdrawal");
        withdrawalTransaction.accountNumber = customer->accountNumber; // Account number
        withdrawalTransaction.amount = amount;
        // Add timestamp to the transaction
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        strftime(withdrawalTransaction.timestamp, sizeof(withdrawalTransaction.timestamp), "%Y-%m-%d %H:%M:%S", t);
        pushTransaction(transactionStack, withdrawalTransaction);
    }
    else
    {
        printf("Insufficient balance\n");
    }
}

// Modify the money sending function to log the transaction
void sendMoney(struct Customer *sender, struct Customer *receiver, double amount, struct TransactionStack *transactionStack)
{
    if (sender->balance >= amount)
    {
        sender->balance -= amount;
        receiver->balance += amount;

        struct Transaction sendMoneyTransaction;
        strcpy(sendMoneyTransaction.type, "Money Sent");
        sendMoneyTransaction.accountNumber = sender->accountNumber; // Sender's account number
        sendMoneyTransaction.amount = amount;
        // Add timestamp to the transaction
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        strftime(sendMoneyTransaction.timestamp, sizeof(sendMoneyTransaction.timestamp), "%Y-%m-%d %H:%M:%S", t);
        pushTransaction(transactionStack, sendMoneyTransaction);
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

// Function to save customer data to a file
void saveCustomersToFile(struct Customer customers[], int customerCount)
{
    FILE *file = fopen("customer_data.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < customerCount; i++)
    {
        fprintf(file, "%s %d %.2lf %s %s %s %d %c\n", customers[i].name, customers[i].accountNumber, customers[i].balance, customers[i].dob, customers[i].address, customers[i].nationality, customers[i].age, customers[i].gender);
    }

    fclose(file);
}

// Function to load customer data from a file
int loadCustomersFromFile(struct Customer customers[])
{
    FILE *file = fopen("customer_data.txt", "r");
    if (file == NULL)
    {
        printf("No existing customer data.\n");
        return 0;
    }

    int customerCount = 0;
    while (fscanf(file, "%s %d %lf %s %s %s %d %c", customers[customerCount].name, &customers[customerCount].accountNumber, &customers[customerCount].balance, customers[customerCount].dob, customers[customerCount].address, customers[customerCount].nationality, &customers[customerCount].age, &customers[customerCount].gender) != EOF)
    {
        customerCount++;
    }

    fclose(file);
    return customerCount;
}

struct Admin
{
    char username[50];
    char password[50];
};

struct Admin admin = {"Admin", "admin"}; // Define the admin credentials

int adminLogin()
{
    char enteredUsername[50];
    char enteredPassword[50];
    blue();
    printf("\n********Bank Management System********\n");
    green();
    printf("\n\tEnter admin username: ");
    scanf("%s", enteredUsername);
    printf("\tEnter admin password: ");
    scanf("%s", enteredPassword);
    white();

    if (strcmp(enteredUsername, admin.username) == 0 && strcmp(enteredPassword, admin.password) == 0)
    {
        printf("Admin login successful.\n");
        return 1; // Return 1 for successful login
    }
    else
    {
        printf("Invalid admin credentials. Please try again.\n");
        return 0; // Return 0 for failed login
    }
}

// Function to display daily tasks in the work queue
void displayDailyTasks(struct CircularQueue *queue)
{
    if (isEmpty(queue))
    {
        printf("No tasks for today!\n");
        return;
    }

    printf("Daily Tasks:\n");

    int front = queue->front;
    int rear = queue->rear;

    while (front != rear)
    {
        printf("Task %d: %s\n", front + 1, queue->array[front].description, queue->array[front].priority);
        front = (front + 1) % queue->capacity;
    }

    printf("Task %d: %s\n", front + 1, queue->array[front].description);
}

void processDailyTasks(struct CircularQueue *queue)
{
    if (isEmpty(queue))
    {
        printf("No tasks for today!\n");
        return;
    }

    struct Task task = dequeue(queue);
    printf("Working on task: %s\n", task.description);
    if (isEmpty(queue))
    {
        printf("Daily tasks completed!\n");
    }
}

int main()
{
    struct Customer customers[100];               // Array to store customer records
    int customerCount = 0;                        // Number of customers
    struct TransactionStack bankTransactionStack; // Transaction history for the bank
    initializeTransactionStack(&bankTransactionStack);

    // Load customer data from file if available
    customerCount = loadCustomersFromFile(customers);
    int isAdminLoggedIn = 0; // Flag to track admin login status

    struct CircularQueue *workQueue = initQueue(5); // Initialize a circular queue to hold 5 tasks

    int choice;
    do
    {
        if (!isAdminLoggedIn)
        {
            isAdminLoggedIn = adminLogin(); // Prompt for admin login if not logged in
            continue;                       // Restart loop to re-check login status
        }

        printf("Inside admin panel... You are now logged in as an admin.\n");

    mainMenu:
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
        printf("\t10. Daily Task\n");
        printf("\t11. Transaction history\n");
        white();

        printf("Enter your choice: ");
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
        { // Deposit
            int accountNumber;
            double amount;
            printf("Enter account number: ");
            scanf("%d", &accountNumber);
            int customerIndex = findCustomerIndex(customers, customerCount, accountNumber);
            if (customerIndex != -1)
            {
                printf("Enter deposit amount: ");
                scanf("%lf", &amount);
                deposit(&customers[customerIndex], amount, &bankTransactionStack);
                printf("Deposit successful.\n");
            }
            else
            {
                printf("Account not found. Please enter a valid account number.\n");
            }
            break;
        }

        case 3:
        { // Withdraw
            int accountNumber;
            double amount;
            printf("Enter account number: ");
            scanf("%d", &accountNumber);
            int customerIndex = findCustomerIndex(customers, customerCount, accountNumber);
            if (customerIndex != -1)
            {
                printf("Enter withdrawal amount: ");
                scanf("%lf", &amount);
                withdraw(&customers[customerIndex], amount, &bankTransactionStack);
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
        { // Send Money
            int senderAccountNumber;
            double amount;
            printf("Enter sender's account number: ");
            scanf("%d", &senderAccountNumber);
            int senderCustomerIndex = findCustomerIndex(customers, customerCount, senderAccountNumber);
            if (senderCustomerIndex != -1)
            {
                int receiverAccountNumber;
                printf("Enter receiver's account number: ");
                scanf("%d", &receiverAccountNumber);
                int receiverCustomerIndex = findCustomerIndex(customers, customerCount, receiverAccountNumber);
                if (receiverCustomerIndex != -1)
                {
                    printf("Enter send amount: ");
                    scanf("%lf", &amount);
                    sendMoney(&customers[senderCustomerIndex], &customers[receiverCustomerIndex], amount, &bankTransactionStack);
                    printf("Send successful.\n");
                }
                else
                {
                    printf("Receiver account not found. Please enter a valid receiver account number.\n");
                }
            }
            else
            {
                printf("Sender account not found. Please enter a valid sender account number.\n");
            }
            break;
        }
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
        case 9:
            printf("Exiting the program. Goodbye!\n");
            break;
        case 10:
        {
            while (1)
            {
                green();
                printf("\n\t1.Add a Task\n");
                printf("\t2.Display all Tasks\n");
                printf("\t3.Finish a Task\n");
                printf("\t4.Exit to main menu\n");
                white();
                printf("4.->");

                int taskchoice;
                scanf("%d", &taskchoice);
                switch (taskchoice)
                {
                case 1:
                {
                    char description[100];
                    int priority;
                    printf("Enter description for the task: ");
                    scanf("%s", description);
                    struct Task newTask;
                    strcpy(newTask.description, description);
                    newTask.priority = priority;
                    enqueue(workQueue, newTask);
                }
                case 2:
                {
                    green();
                    displayDailyTasks(workQueue);
                    white();
                    break;
                }
                case 3:
                {
                    green();
                    processDailyTasks(workQueue);
                    white();
                    break;
                }
                case 4:
                {
                    goto mainMenu;
                }
                default:
                    printf("Invalid choice. Please try again.\n");
                }
            }

            break;
        }

        case 11:
        { // Transaction History
            green();
            displayLastTenTransactions(&bankTransactionStack);
            white();
            break;
        }

        default:
            printf("Invalid choice. Please try again.\n");
        }

        if (choice == 9)
        {
            saveCustomersToFile(customers, customerCount);
        }

        printf("Press Enter to continue...");
        getchar(); // Wait for Enter key press
        getchar(); // Wait for Enter key press again (to clear any remaining input buffer)
    } while (choice != 9);

    return 0;
}
