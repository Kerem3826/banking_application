#include<stdio.h>
#include<string.h>
#ifdef _WIN32
    #include <direct.h>  // For using _mkdir() function on Windows which creates directories (folders) to keep the files organised.
    #include <io.h>      // For using _access() function on Windows which enables us to access to a directory. In this case we use it for checking if a directory already exists.
    char bankingAppDir[6][64]={"Banking Application", // Making this directories global allows us to use them freely when creating files.
                                "Banking Application\\Accounts\\",
                                "Banking Application\\Receipts\\",
                                "Banking Application\\Receipts\\Account Creation Receipts\\",
                                "Banking Application\\Receipts\\Account Closure Receipts\\",
                                "Banking Application\\Receipts\\Money Transfer Receipts\\"}; // Creating directory names according to the structure.
#else
    #include <unistd.h>  // For using mkdir() which is the Unix-like systems (Linux, MacOS etc.) equivalent of _mkdir().
    #include <sys/stat.h> // For using access() which is the Unix-like systems (Linux, MacOS etc.) equivalent of _access().
    char bankingAppDirUnix[6][64]={"Banking Application",
                                    "Banking Application/Accounts",
                                    "Banking Application/Receipts",
                                    "Banking Application/Receipts/Account Creation Receipts/",
                                    "Banking Application/Receipts/Account Closure Receipts/",
                                    "Banking Application/Receipts/Money Transfer Receipts/"}; // Unix-like systems use "/" instead of "\\"
#endif
#include<time.h>
#define maxUserAmount 5000
#define maxStrLen 20

int accountNumber[maxUserAmount];
char trID[maxUserAmount][12]; // Maximum digit amount for a TrID is 11.
char name[maxUserAmount][maxStrLen];
char surname[maxUserAmount][maxStrLen];
char currency[maxUserAmount][maxStrLen];
double balance[maxUserAmount];

int numberOfAccounts=0;
////////////////////////////////////////////////////////// PROTOTYPES /////////////////////////////////////////////////////////////////
int mainMenu(); // Main menu function which displays the main menu of the program.
int createAccount(); // Account creation function which takes inputs and saves them to the corresponding arrays.
int trIDControl(char* trID); // Function to control if TrID is 11 digits and the %10 of the sum of first 10 digits gives the last digit.
int accountSaving(); // Function to save accounts to a txt file to create a "Local database".
int accountLoading(); // Function to read account information from the "Local database" and assign them to the corresponding arrays.
int moneyTransfer(); // Function to edit values of the customers' balances (For sending and receiving money).
double exchangeRate(double n, int m); // Function to calculate the exchange rate between currencies.
int accountUpdate(); // Function to edit the account information. Following 4 functions are sub-functions of this functions.
int changeTrID(int accNumber); // 1
int changeName(int accNumber); // 2
int changeSurname(int accNumber); // 3
int changeCurrency(int accNumber); // 4, Following 6 functions are sub-functions of this function to edit the customers' balance accordingly if the currency of the account changes.
int balanceConvertTryToUsd(int accNumber); // 1
int balanceConvertUsdToTry(int accNumber); // 2
int balanceConvertUsdToEur(int accNumber); // 3
int balanceConvertEurToUsd(int accNumber); // 4
int balanceConvertEurToTry(int accNumber); // 5
int balanceConvertTryToEur(int accNumber); // 6
int changeBalance(int accNumber); // Function to edit balances of customers without changing currency.
int accountInquiry(); // Function to query a specific account's details. Following 5 functions are sub-functions of this function to query an account by various ways.
int accountNumberQuery(); // 1
int nameQuery(); // 2
int surnameQuery(); // 3
int nameAndSurnameQuery(); // 4
int trIDQuery(); // 5
int accountListing(); // Function to list all the existing accounts in the "Local database".
int accountClosure(); // Function to close a number of accounts and delete the information about them. The following function is a sub-function of this function.
int closeAccount(int amount); // Function to close given number of accounts and rearrange the arrays of informations of customers.
char* directory(); // Function to create or find (if exists) organised directories to save txt files (Account data and receipts) into them.
char* receiptAccountCreation(int accNumber); // Function to create a receipt about account creation and save it to the proper directory.
char* receiptAccountClosure(int accNumber); // Function to create a receipt about account closure and save it to the proper directory.
char* receiptMoneyTransfer(int senderAccNumber, double sent, int recipientAccNumber, double received); // Function to create a receipt about money transfer and save it to the proper directory.
int showReceipt(char* fileName); // Function to read data from the created receipt files and show them to the user if wanted.
int pause(); // Function to wait an input from the user to continue operations.
//////////////////////////////////////////////////////////////// MAIN /////////////////////////////////////////////////////////////////
int main(){
    accountLoading(); // We read data of the already existing accounts (if there are any) and feed them into the program.
    while(1){
        switch(mainMenu()){ // Main menu function returns the corresponding value.
            case 1:
                createAccount();
                accountSaving(); // We save created accounts to the proper txt file.
                pause();
                break;
            case 2:
                moneyTransfer();
                accountSaving(); // We save new, edited information about balances of accounts to the proper txt file.
                pause();
                break;
            case 3:
                accountUpdate(); // We save new, edited information about accounts to the proper txt file.
                accountSaving();
                pause();
                break;
            case 4:
                accountInquiry();
                pause();
                break;
            case 5:
                accountListing();
                pause();
                break;
            case 6:
                accountClosure(); // We already saved accounts in the function itself but we can double-check in case of an error.
                accountSaving(); // <-|
                pause();
                break;
            case 7:
                    printf("Exiting...\n");
                    return 0;
            default:
                    printf("Invalid choice. Please try again.\n");
                    break;
        }
    }

    return 0;
}
////////////////////////////////////////////////////////////// FUNCTIONS //////////////////////////////////////////////////////////////
int mainMenu(){
    int choice;
    while(1){
        printf("\nWelcome to the YilmazBank! What would you like to do?\n");
        printf("1) Create Account\n");
        printf("2) Money Transfer\n");
        printf("3) Account Update\n");
        printf("4) Account Inquiry\n");
        printf("5) Account Listing\n");
        printf("6) Account Closure\n");
        printf("7) Exit\n");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n'){ // We delete all the invalid input characters (until the new line character) from memory.
                continue;
            }
        }

        switch (choice)
        {
        case 1:
            return choice;
        case 2:
            return choice;
        case 3:
            return choice;
        case 4:
            return choice;
        case 5:
            return choice;
        case 6:
            return choice;
        case 7:
            return choice;
        default:
            printf("Invalid choice. Please try again.");
            break;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int createAccount(){

    if(numberOfAccounts<maxUserAmount){
        int choice;
        
        accountNumber[numberOfAccounts]=numberOfAccounts+1; // Account numbers are given based on account creation order. Since array elements start from 0, we add 1 to it.
        while(1){
            printf("Please enter the TrID:\n");
            scanf(" %[^\n]%*c",&trID[numberOfAccounts] );
            if(trIDControl(trID[numberOfAccounts])==1){ // We control if the given TrID is valid.
            break;
            }
            else{
                continue;
            }
        }
        printf("Please enter the name:\n");
        scanf(" %[^\n]%*c",name[numberOfAccounts]);
        printf("Please enter the surname:\n");
        scanf(" %[^\n]%*c",surname[numberOfAccounts]);

        while(1){
        printf("Please select the currency:\n");
        printf("1) TRY\n");
        printf("2) USD\n");
        printf("3) EUR\n");
        scanf("%d",&choice);
        switch (choice)
        {
        case 1:
            strcpy(currency[numberOfAccounts],"TRY");
            break;
        case 2:
            strcpy(currency[numberOfAccounts],"USD");
            break;
        case 3:
            strcpy(currency[numberOfAccounts],"EUR");
            break;
        default:
            printf("Invalid choice. Please try again.");
            continue;
        }
        break;
        }
        while(1){
            printf("Please enter the initial balance:\n");
            if (scanf(" %lf",&balance[numberOfAccounts]) != 1){
                printf("Invalid input. Please enter a number.\n");
                while(getchar() != '\n'){ // We delete all the invalid input characters (until the new line character) from memory.
                    continue;
                }
            }
            else{
                break;
            }
        }
        printf("Successfully created account! The account number is: %d.\n",accountNumber[numberOfAccounts]);
        showReceipt(receiptAccountCreation(accountNumber[numberOfAccounts])); // Asks user if they want to see the receipt and informs them about the saving location of the txt file.
        numberOfAccounts++; // We increase the value of the variable that holds the number of accounts and save it into the "Local database" later.
        return 0;
    }
    else {
        printf("Can't create new account. Reached the maximum user amount.\n");
        return 0;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int trIDControl(char* trID){
    int sum=0;
    int i;
    int last;
    if(strlen(trID)!=11){ // Checks if the length of TrID is correct.

        printf("Please enter a valid TrID.\n");
        return 0;
    }
    for(i=0;i<10;i++){
        sum+=trID[i]-'0'; // Since ASCII numbers of numbers 0-9 are back to back, we can subtract the ASCII value of character '0' to get values in integer.
    }
    last=trID[10] -'0'; // Same thing here.
    if(sum%10!=last){
        printf("Please enter a valid TrID.\n");
        return 0;
    }
    else{
        return 1;
    }

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int accountSaving(){
    FILE *fp;
    int i;
    char fileName[128];

    directory();
    sprintf(fileName,"%sAccounts.txt",bankingAppDir[1]);

    fp=fopen(fileName,"w");
        if(fp==NULL){
            printf("Error! Couldn't open the file to save accounts.");
            return 0;
        }
        fprintf(fp,"%d",numberOfAccounts);
        for (i=0;i<numberOfAccounts;i++){ // We write the data that is in the program when it's running into the "Local database" with a proper format for reading it later.
            fprintf(fp, "\n%d",accountNumber[i]);
            fprintf(fp, "\n%s",trID[i]);
            fprintf(fp, "\n%s",name[i]);
            fprintf(fp, "\n%s",surname[i]);
            fprintf(fp, "\n%s",currency[i]);
            fprintf(fp, "\n%lf",balance[i]);
            fprintf(fp, "\n--------------------");
        }
    fclose(fp);
    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int accountLoading(){

    char fileName[128];

    directory();
    sprintf(fileName,"%sAccounts.txt",bankingAppDir[1]);

    FILE *fp2;
    fp2=fopen(fileName,"r");
    char line[21];

    if(fp2==NULL){ // If there aren't any saved accounts yet, it displays a message and leaves the memory locations reserved for account informations empty.
        printf("No registered accounts yet.\n");
        return 0;
    }

    fscanf(fp2,"%d",&numberOfAccounts); // First we read the number of accounts.
    
    memset(accountNumber, 0, sizeof(accountNumber)); // This line and following 5 lines are to get rid of any unwanted data in the corresponding memory locations to read our data from the file more safely.
    memset(trID, 0, sizeof(trID));
    memset(name, 0, sizeof(name));
    memset(surname, 0, sizeof(surname));
    memset(currency, 0, sizeof(currency));
    memset(balance, 0, sizeof(balance));

    int i=0;
    for(i=0;i<numberOfAccounts;i++){  // We read our data from the file with the same format we wrote it to a file. Basically we feed our data into the program.
        fscanf(fp2, "\n%d",&accountNumber[i]);
        fscanf(fp2, "\n %[^\n]%*c",&trID[i]);
        fscanf(fp2, "\n %[^\n]%*c",name[i]);
        fscanf(fp2, "\n %[^\n]%*c",surname[i]);
        fscanf(fp2, "\n %[^\n]%*c",currency[i]);
        fscanf(fp2, "\n%lf",&balance[i]);
        fgets(line,128,fp2); // Used to get rid of unwanted spaces that might disturb the reading.
        fgets(line,sizeof(line),fp2); // Used to get the seperator line and continue reading.
        if (i >= numberOfAccounts) {
            break; 
        }
    }
    fclose(fp2);
    printf("%d accounts have been loaded successfully.",numberOfAccounts);
    return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int moneyTransfer(){
    int sender;
    double amount;
    int recipient;
    while (1){
        printf("Please enter the account number you want to transfer money from:\n");
        printf("(To exit, type '0')\n");
        scanf("%d",&sender);
        if(sender==0){
            break;
        }
        else if(sender<0 || sender>maxUserAmount){
            printf("Invalid choice. Please try again.");
            continue;
        }
        printf("Please enter the amount you want to transfer:\n");
        scanf("%lf",&amount);
        if(amount>balance[sender-1]){
            printf("Insufficient balance. Please enter a sufficient amount.");
            continue;
        }
        else{
            printf("Please enter the account number you want to transfer money to:\n");
            scanf("%d",&recipient);
            balance[sender-1]-=amount;
            if(strcmp(currency[sender-1],currency[recipient-1])==0){ // This line checks if the currencies of sender and recipient are the same, and if so it doesn't have use exchangeRate function.
                balance[recipient-1]+=amount;
                printf("Money transfer completed.\n");
                showReceipt(receiptMoneyTransfer(sender,amount,recipient,amount));
            }
            else{ // This code block is separated to 6 parts like this to enhance readability.
                double newAmount;
                if(strcmp(currency[sender-1],"TRY")==0 && strcmp(currency[recipient-1],"USD")==0){
                    newAmount=exchangeRate(amount, 1); // 1 is TRY to USD exchange rate.
                    balance[recipient-1]+=newAmount;
                }
                else if(strcmp(currency[sender-1],"USD")==0 && strcmp(currency[recipient-1],"TRY")==0){
                    newAmount=exchangeRate(amount, 2); // 2 is USD to TRY exchange rate.
                    balance[recipient-1]+=newAmount;
                }
                else if(strcmp(currency[sender-1],"USD")==0 && strcmp(currency[recipient-1],"EUR")==0){
                    newAmount=exchangeRate(amount, 3); // 3 is USD to EUR exchange rate.
                    balance[recipient-1]+=newAmount;
                }
                else if(strcmp(currency[sender-1],"EUR")==0 && strcmp(currency[recipient-1],"USD")==0){
                    newAmount=exchangeRate(amount, 4); // 4 is EUR to USD exchange rate.
                    balance[recipient-1]+=newAmount;
                }
                else if(strcmp(currency[sender-1],"EUR")==0 && strcmp(currency[recipient-1],"TRY")==0){
                    newAmount=exchangeRate(amount, 5); // 5 is EUR to TRY exchange rate.
                    balance[recipient-1]+=newAmount;
                }
                else if(strcmp(currency[sender-1],"TRY")==0 && strcmp(currency[recipient-1],"EUR")==0){
                newAmount=exchangeRate(amount, 6); // 6 is TRY to EUR exchange rate.
                    balance[recipient-1]+=newAmount;
                }
                printf("Money transfer completed.\n");
                showReceipt(receiptMoneyTransfer(sender,amount,recipient,newAmount));
            }
            return 1;
        }
    }
    return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double exchangeRate(double n, int m){
    double newAmount;
    switch (m)
    {
        case 1: // TRY to USD
            newAmount=n*0.028;
            return newAmount;
        case 2: // USD to TRY
            newAmount=n*35.14;
            return newAmount;
        case 3: // USD to EUR
            newAmount=n*0.96;
            return newAmount;
        case 4: // EUR to USD
            newAmount=n*1.04;
            return newAmount;
        case 5: // EUR to TRY
            newAmount=n*36.63;
            return newAmount;
        case 6: // TRY to EUR
            newAmount=n*0.027;
            return newAmount;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int accountUpdate(){
    int choice;
    int choice2;
    double newBalance;
    int accNumber;
    printf("Enter the account number of the account you want to update:\n");
    scanf("%d",&accNumber);
    while(1){
        printf("What do you want to do?\n");
        printf("1) Change TrID\n");
        printf("2) Change Name\n");
        printf("3) Change Surname\n");
        printf("4) Change Currency\n");
        printf("5) Change Balance\n");
        printf("6) Exit\n");
        scanf("%d",&choice); // We take the input and send the account number into the corresponding sub-function.
        switch (choice)
        {
        case 1:
            return changeTrID(accNumber);
        case 2:
            return changeName(accNumber);
        case 3:
            return changeSurname(accNumber);
        case 4:
            return changeCurrency(accNumber);
        case 5:
            return changeBalance(accNumber);
        case 6:
            return 0;
        default:
            continue;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int changeTrID(int accNumber){
        char newID[12];
        char choice;
        printf("The current TrID in the account is %s\n",trID[accNumber-1]);
        while(1){
            printf("Please enter the new TrID:\n");
            scanf(" %[^\n]%*c",&newID);
            if(trIDControl(newID)==1){ // We control if the given TrID is valid.
            break;
            }
            else{
                continue;
            }
        }
        printf("You set %s as the new TrID of the customer %s %s (Account number %d), are you sure you want to continue?\n", newID, name[accNumber-1], surname[accNumber-1], accNumber);
        while(1){
            printf("y/n\n");
            scanf("%c",&choice);
            if(choice=='y'){
                strcpy(trID[accNumber-1],newID);
                printf("TrID successfully changed.\n");
                return 1;
            }
            else if(choice=='n'){
                printf("TrID change cancelled.\n");
                break;
            }
            else{
                printf("Invalid choice. Please try again.\n");
                continue;
            }
        }
        return 0;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int changeName(int accNumber){
        char newName[maxStrLen];
        char choice;
        printf("The current name in the account is %s\n",name[accNumber-1]);
        printf("Please enter the new name:\n");
        scanf(" %[^\n]%*c",newName);
        printf("You set %s as the new name of the customer %s %s (Account number %d), are you sure you want to continue?\n", newName, name[accNumber-1], surname[accNumber-1], accNumber);
        while(1){
            printf("y/n\n");
            scanf("%c",&choice);
            if(choice=='y'){
                strcpy (name[accNumber-1],newName);
                printf("Name successfully changed.\n");
                return 1;
            }
            else if(choice=='n'){
                printf("Name change cancelled.\n");
                break;
            }
            else{
                printf("Invalid choice. Please try again.\n");
                continue;
            }
        }
        return 0;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int changeSurname(int accNumber){
        char newSurname[maxStrLen];
        char choice;
        printf("The current surname in the account is %s\n",surname[accNumber-1]);
        printf("Please enter the new surname:\n");
        scanf(" %[^\n]%*c",newSurname);
        printf("You set %s as the new surname of the customer %s %s (Account number %d), are you sure you want to continue?\n", newSurname, name[accNumber-1], surname[accNumber-1], accNumber);
        while(1){
            printf("y/n\n");
            scanf("%c",&choice);
            if(choice=='y'){
                strcpy (surname[accNumber-1],newSurname);
                printf("Surname successfully changed.\n");
                return 1;
            }
            else if(choice=='n'){
                printf("Surname change cancelled.\n");
                break;
            }
            else{
                printf("Invalid choice. Please try again.\n");
                continue;
            }
        }
        return 0;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int changeCurrency(int accNumber){
        double newBalance;
        int choice;
        char choiceyn;
        printf("The current currency of the account is %s\n",currency[accNumber-1]);
        if(strcmp(currency[accNumber-1],"TRY")==0){ // Opens a menu according to the current currecy (TRY) of the customer.
            while(1){
                printf("Please select the new type of currency:\n");
                printf("1) USD\n");
                printf("2) EUR\n");
                printf("3) Exit\n");
                scanf("%d",&choice);
                switch (choice)
                {
                    case 1: // TRY to USD
                        if(balanceConvertTryToUsd(accNumber)==1){ // Sends the account number of the customer and checks if the balance conversion function worked properly (if so, it would return 1).
                            return 1;
                        }
                        else{
                            continue;
                        }
                    case 2: // TRY to EUR
                        if(balanceConvertTryToEur(accNumber)==1){
                            return 1;
                        }
                        else{
                            continue;
                        }
                    case 3:
                        printf("Currency change cancelled.\n");
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                    continue;
                }
                break;
            }
            return 0;
        }
        else if(strcmp(currency[accNumber-1],"USD")==0){ // Opens a menu according to the current currecy (USD) of the customer.
            while(1){
                printf("Please select the new type of currency:\n");
                printf("1) TRY\n");
                printf("2) EUR\n");
                printf("3) Exit\n");
                scanf("%d",&choice);
                switch (choice)
                {
                    case 1: // USD to TRY
                        if(balanceConvertUsdToTry(accNumber)==1){ // Sends the account number of the customer and checks if the balance conversion function worked properly (if so, it would return 1).
                            return 1;
                        }
                        else{
                            continue;
                        }
                    case 2: // USD to EUR
                        if(balanceConvertUsdToEur(accNumber)==1){
                            return 1;
                        }
                        else{
                            continue;
                        }
                    case 3:
                        printf("Currency change cancelled.\n");
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                    continue;
                }
                break;
            }
            return 0;
        }
        else if(strcmp(currency[accNumber-1],"EUR")==0){ // Opens a menu according to the current currecy (EUR) of the customer.
            while(1){
                printf("Please select the new type of currency:\n");
                printf("1) TRY\n");
                printf("2) USD\n");
                printf("3) Exit\n");
                scanf("%d",&choice);
                switch (choice)
                {
                    case 1: // EUR to TRY
                        if(balanceConvertEurToTry(accNumber)==1){ // Sends the account number of the customer and checks if the balance conversion function worked properly (if so, it would return 1).
                            return 1;
                        }
                        else{
                            continue;
                        }
                    continue;
                    case 2: // EUR to USD
                        if(balanceConvertEurToUsd(accNumber)==1){
                            return 1;
                        }
                        else{
                            continue;
                        }
                    continue;
                    case 3:
                        printf("Currency change cancelled.\n");
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                    continue;
                }
                break;
            }
            return 0;
        }
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int balanceConvertTryToUsd(int accNumber){
            double newBalance;
            char choiceyn;

            printf("The balance of the account is %.2lf %s and after the conversion it will be %.2lf USD.\n", balance[accNumber-1], currency[accNumber-1], exchangeRate(balance[accNumber-1],1)); //TRY to USD
            printf("Are you sure you want to continue?\n");
            while(1){
                printf("y/n\n");
                scanf("%c",&choiceyn);
                if(choiceyn=='y'){
                    strcpy(currency[accNumber-1],"USD");
                    newBalance=exchangeRate(balance[accNumber-1],1); //TRY to USD
                    balance[accNumber-1]=newBalance;
                    printf("Currency and balance successfully changed.\n");
                    return 1;
                }
                else if(choiceyn=='n'){
                    printf("Currency change cancelled.\n");
                    return 0;
                }
                else{
                    printf("Invalid choice. Please try again.\n");
                    continue;
                }
            }
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int balanceConvertUsdToTry(int accNumber){
            double newBalance;
            char choiceyn;

            printf("The balance of the account is %.2lf %s and after the conversion it will be %.2lf TRY.\n", balance[accNumber-1], currency[accNumber-1], exchangeRate(balance[accNumber-1],2)); //USD to TRY
            printf("Are you sure you want to continue?\n");
            while(1){
                printf("y/n\n");
                scanf("%c",&choiceyn);
                if(choiceyn=='y'){
                    strcpy(currency[accNumber-1],"TRY");
                    newBalance=exchangeRate(balance[accNumber-1],2); //USD to TRY
                    balance[accNumber-1]=newBalance;
                    printf("Currency and balance successfully changed.\n");
                    return 1;
                }
                else if(choiceyn=='n'){
                    printf("Currency change cancelled.\n");
                    return 0;
                }
                else{
                    printf("Invalid choice. Please try again.\n");
                    continue;
                }
            }
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int balanceConvertUsdToEur(int accNumber){
            double newBalance;
            char choiceyn;

            printf("The balance of the account is %.2lf %s and after the conversion it will be %.2lf EUR.\n", balance[accNumber-1], currency[accNumber-1], exchangeRate(balance[accNumber-1],3)); //USD to EUR
            printf("Are you sure you want to continue?\n");
            while(1){
                printf("y/n\n");
                scanf("%c",&choiceyn);
                if(choiceyn=='y'){
                    strcpy(currency[accNumber-1],"EUR");
                    newBalance=exchangeRate(balance[accNumber-1],3); // USD to EUR
                    balance[accNumber-1]=newBalance;
                    printf("Currency and balance successfully changed.\n");
                    return 1;
                }
                else if(choiceyn=='n'){
                    printf("Currency change cancelled.\n");
                    return 0;
                }
                else{
                    printf("Invalid choice. Please try again.\n");
                    continue;
                }
            }
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int balanceConvertEurToUsd(int accNumber){
            double newBalance;
            char choiceyn;

            printf("The balance of the account is %.2lf %s and after the conversion it will be %.2lf USD.\n", balance[accNumber-1], currency[accNumber-1], exchangeRate(balance[accNumber-1],4)); //EUR to USD
            printf("Are you sure you want to continue?\n");
            while(1){
                printf("y/n\n");
                scanf("%c",&choiceyn);
                if(choiceyn=='y'){
                    strcpy(currency[accNumber-1],"USD");
                    newBalance=exchangeRate(balance[accNumber-1],4); // EUR to USD
                    balance[accNumber-1]=newBalance;
                    printf("Currency and balance successfully changed.\n");
                    return 1;
                }
                else if(choiceyn=='n'){
                    printf("Currency change cancelled.\n");
                    return 0;
                }
                else{
                    printf("Invalid choice. Please try again.\n");
                    continue;
                }
            }
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int balanceConvertEurToTry(int accNumber){
            double newBalance;
            char choiceyn;

            printf("The balance of the account is %.2lf %s and after the conversion it will be %.2lf TRY.\n", balance[accNumber-1], currency[accNumber-1], exchangeRate(balance[accNumber-1],5)); //EUR to TRY
            printf("Are you sure you want to continue?\n");
            while(1){
                printf("y/n\n");
                scanf("%c",&choiceyn);
                if(choiceyn=='y'){
                    strcpy(currency[accNumber-1],"TRY");
                    newBalance=exchangeRate(balance[accNumber-1],5); //EUR to TRY
                    balance[accNumber-1]=newBalance;
                    printf("Currency and balance successfully changed.\n");
                    return 1;
                }
                else if(choiceyn=='n'){
                    printf("Currency change cancelled.\n");
                    return 0;
                }
                else{
                    printf("Invalid choice. Please try again.\n");
                    continue;
                }
            }
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int balanceConvertTryToEur(int accNumber){
            double newBalance;
            char choiceyn;

            printf("The balance of the account is %.2lf %s and after the conversion it will be %.2lf EUR.\n", balance[accNumber-1], currency[accNumber-1], exchangeRate(balance[accNumber-1],6)); //TRY to EUR
            printf("Are you sure you want to continue?\n");
            while(1){
                printf("y/n\n");
                scanf("%c",&choiceyn);
                if(choiceyn=='y'){
                    strcpy(currency[accNumber-1],"EUR");
                    newBalance=exchangeRate(balance[accNumber-1],6); //TRY to EUR
                    balance[accNumber-1]=newBalance;
                    printf("Currency and balance successfully changed.\n");
                    return 1;
                }
                else if(choiceyn=='n'){
                    printf("Currency change cancelled.\n");
                    return 0;
                }
                else{
                    printf("Invalid choice. Please try again.\n");
                    continue;
                }
            }
            
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int changeBalance(int accNumber){
        double newBalance;
        char choice;
        
        printf("The current balance of the account is %.2lf %s\n", balance[accNumber-1], currency[accNumber-1]);
        printf("Please enter the new balance:\n");
        scanf(" %lf",&newBalance);
        printf("You set %.2lf %s as the new balance of the customer %s %s (Account number %d), are you sure you want to continue?\n", newBalance, currency[accNumber-1], name[accNumber-1], surname[accNumber-1], accNumber);
        while(1){
            printf("y/n\n");
            scanf("%c",&choice);
            if(choice=='y'){
                balance[accNumber-1]=newBalance;
                printf("Balance successfully changed.\n");
                return 1;
            }
            else if(choice=='n'){
                printf("Balance change cancelled.\n");
                break;
            }
            else{
                printf("Invalid choice. Please try again.\n");
                continue;
            }
        }
        return 0;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int accountInquiry(){
    int choice;
    while (1){
        printf("What do you want to query the account by?\n");
        printf("1) Account Number\n");
        printf("2) Name\n");
        printf("3) Surname\n");
        printf("4) Name and Surname\n");
        printf("5) TrID\n");
        printf("6) Exit\n");
        scanf("%d",&choice); 
        switch (choice) // Sends the program to the according function of user's choice.
        {
        case 1:
            return accountNumberQuery();
            break;
        case 2:
            return nameQuery();
            break;
        case 3:
            return surnameQuery();
            break;
        case 4:
            return nameAndSurnameQuery();
            break;
        case 5:
            return trIDQuery();
            break;
        case 6:
            return 0;
            break;
        default:
            printf("Invalid choice. Please try again.");
            break;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int accountNumberQuery(){ // Queries accounts by their account number, which are unique to the account. Therefore it is easy to find the direct target.
        int i=0;
        int accNumber;

        if(numberOfAccounts==0){
            printf("No registered accounts yet.\n");
            return 0;
        }
        while (1){
            printf("Please enter the account number of the account you want to query:\n");
            printf("(To exit, type '0')\n");
            scanf("%d",&accNumber);
            if(accNumber==0){
                return 1;
            }
            else{
                while(accountNumber[i]!=accNumber){
                    i++;
                }
                if(accountNumber[i]==accNumber){
                    printf("Here are the details about the account:\n");
                    printf("Account number: %d, TrID: %s, Name: %s, Surname: %s, Currency: %s, Balance: %.2lf %s\n", accountNumber[i], trID[i], name[i], surname[i], currency[i], balance[i], currency[i]);
                    return 1;
                }
                else{
                    printf("Something went wrong when trying to find the account. Please try again.");
                    continue;
                }
            }
        }
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int nameQuery(){ // Queries accounts by their names and lists all the accounts that have the given name. After user makes a choice about which of the listed accounts he/she wants to query, it shows the information about the account.
        int i=0;
        int j=1;
        int matchedIndex[maxUserAmount];
        int matchAmount=0;
        int control=0;
        int index;
        int selection;
        char accName[maxStrLen];

        if(numberOfAccounts==0){
            printf("No registered accounts yet.\n");
            return 0;
        }

        while (1){
            printf("Please enter the name in the account you want to query:\n");
            printf("(To exit, type '0')\n");
            scanf(" %[^\n]%*c", accName);
            if(strcmp(accName,"0")==0){
                return 1;
            }
            for(i=0;i<numberOfAccounts;i++){
                if(strcmp(accName,name[i])==0){
                    control++;
                    break;
                }
                else if(i==numberOfAccounts-1){
                    printf("Couldn't find any accounts with the name %s.\n", accName);
                    break;
                }
                else{
                    continue;
                }
            }
            if(control==0){
                continue;
            }
            else{
                printf("Here is the list of accounts that have the name %s:\n",accName);
                for(i=0;i<numberOfAccounts;i++){
                    if(strcmp(accName,name[i])==0){
                        printf("%d) %s %s\n", j, name[i], surname[i]);
                        matchedIndex[matchAmount]=i;
                        j++;
                        matchAmount++;
                    }
                }
                break;
            }
        }
        while (1){
            printf("Which account do you want to choose?\n");
            scanf("%d",&index);
            if(index<1 || index>matchAmount){
                printf("Invalid choice. Please try again.");
                continue;
            }
            selection=matchedIndex[index-1];
            printf("Here are the details about the account:\n");
            printf("Account number: %d, TrID: %s, Name: %s, Surname: %s, Currency: %s, Balance: %.2lf %s\n", accountNumber[selection], trID[selection], name[selection], surname[selection], currency[selection], balance[selection], currency[selection]);
            return 1;
        }
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int surnameQuery(){ // Queries accounts by their surnames and lists all the accounts that have the given surname. After user makes a choice about which of the listed accounts he/she wants to query, it shows the information about the account.
        int i=0;
        int j=1;
        int matchedIndex[maxUserAmount];
        int matchAmount=0;
        int control=0;
        int index;
        int selection;
        char accSurname[maxStrLen];

        if(numberOfAccounts==0){
            printf("No registered accounts yet.\n");
            return 0;
        }

        while (1){
            printf("Please enter the surname in the account you want to query:\n");
            printf("(To exit, type '0')\n");
            scanf(" %[^\n]%*c", accSurname);
            if(strcmp(accSurname,"0")==0){
                return 1;
            }
            for(i=0;i<numberOfAccounts;i++){
                if(strcmp(accSurname,surname[i])==0){
                    control++;
                    break;
                }
                else if(i==numberOfAccounts-1){
                    printf("Couldn't find any accounts with the surname %s.\n", accSurname);
                    break;
                }
                else{
                    continue;
                }
            }
            if(control==0){
                continue;
            }
            else{
                printf("Here is the list of accounts that have the surname %s:\n",accSurname);
                for(i=0;i<numberOfAccounts;i++){
                    if(strcmp(accSurname,surname[i])==0){
                        printf("%d) %s %s\n", j, name[i], surname[i]);
                        matchedIndex[matchAmount]=i;
                        j++;
                        matchAmount++;
                    }
                }
                break;
            }
        }
        while (1){
            printf("Which account do you want to choose?\n");
            scanf("%d",&index);
            if(index<1 || index>matchAmount){
                printf("Invalid choice. Please try again.");
                continue;
            }
            selection=matchedIndex[index-1];
            printf("Here are the details about the account:\n");
            printf("Account number: %d, TrID: %s, Name: %s, Surname: %s, Currency: %s, Balance: %.2lf %s\n", accountNumber[selection], trID[selection], name[selection], surname[selection], currency[selection], balance[selection], currency[selection]);
            return 1;
        }
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int nameAndSurnameQuery(){ // A more specific function to query accounts that have the given name AND surname. It then lists all the accounts that have the given name and surname. After user makes a choice about which of the listed accounts he/she wants to query, it shows the information about the account.
        
        int i=0;
        int j=1;
        int matchedIndex[maxUserAmount];
        int matchAmount=0;
        int control=0;
        int index;
        int selection;
        char accName[maxStrLen];
        char accSurname[maxStrLen];

        if(numberOfAccounts==0){
            printf("No registered accounts yet.\n");
            return 0;
        }

        while (1){
            printf("Please enter the name and surname in the account you want to query:\n");
            printf("(To exit, type '0')\n");
            printf("Name:\n");
            scanf(" %[^\n]%*c", accName);
            if(strcmp(accName,"0")==0){
                return 1;
            }
            printf("Surname:\n");
            scanf(" %[^\n]%*c", accSurname);
            if(strcmp(accSurname,"0")==0){
                return 1;
            }

            for(i=0;i<numberOfAccounts;i++){
                if(strcmp(accName,name[i])==0 && strcmp(accSurname,surname[i])==0){
                    control++;
                    break;
                }
                else if(i==numberOfAccounts-1){
                    printf("Couldn't find any accounts with the name and surname %s %s.\n", accName, accSurname);
                    break;
                }
                else{
                    continue;
                }
            }
            if(control==0){
                continue;
            }
            else{
                printf("Here is the list of accounts that have the name and surname %s %s:\n",accName, accSurname);
                for(i=0;i<numberOfAccounts;i++){
                    if(strcmp(accName,name[i])==0 && strcmp(accSurname,surname[i])==0){
                        printf("%d) %s %s\n", j, name[i], surname[i]);
                        matchedIndex[matchAmount]=i;
                        j++;
                        matchAmount++;
                    }
                }
                break;
            }
        }
        while (1){
            printf("Which account do you want to choose?\n");
            scanf("%d",&index);
            if(index<1 || index>matchAmount){
                printf("Invalid choice. Please try again.");
                continue;
            }
            selection=matchedIndex[index-1];
            printf("Here are the details about the account:\n");
            printf("Account number: %d, TrID: %s, Name: %s, Surname: %s, Currency: %s, Balance: %.2lf %s\n", accountNumber[selection], trID[selection], name[selection], surname[selection], currency[selection], balance[selection], currency[selection]);
            return 1;
        }
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int trIDQuery(){ // Queries accounts by their TrID, which are unique to the customers. However it is possible that a customer has more than 1 account so it still lists the accounts that have the given TrID.
        int i=0;
        int j=1;
        int matchedIndex[maxUserAmount];
        int matchAmount=0;
        int control=0;
        int index;
        int selection;
        char accTrID[12];

        if(numberOfAccounts==0){
            printf("No registered accounts yet.\n");
            return 0;
        }
        while (1){
            printf("Please enter the TrID in the account you want to query:\n");
            printf("(To exit, type '0')\n");
            scanf(" %[^\n]%*c", accTrID);
            if(strcmp(accTrID,"0")==0){
                return 1;
            }
            for(i=0;i<numberOfAccounts;i++){
                if(strcmp(accTrID,trID[i])==0){
                    control++;
                    break;
                }
                else if(i==numberOfAccounts-1){
                    printf("Couldn't find any accounts with the TrID %s.\n", accTrID);
                    break;
                }
                else{
                    continue;
                }
            }
            if(control==0){
                continue;
            }
            else{
                printf("Here is the list of accounts that have the trID %s:\n",accTrID);
                for(i=0;i<numberOfAccounts;i++){
                    if(strcmp(accTrID,trID[i])==0){
                        printf("%d) %s %s %lf %s\n", j, name[i], surname[i], balance[i], currency[i]);
                        matchedIndex[matchAmount]=i;
                        j++;
                        matchAmount++;
                    }
                }
                break;
            }
        }
        while (1){
            printf("Which account do you want to choose?\n");
            scanf("%d",&index);
            if(index<1 || index>matchAmount){
                printf("Invalid choice. Please try again.");
                continue;
            }
            selection=matchedIndex[index-1];
            printf("Here are the details about the account:\n");
            printf("Account number: %d, TrID: %s, Name: %s, Surname: %s, Currency: %s, Balance: %.2lf %s\n", accountNumber[selection], trID[selection], name[selection], surname[selection], currency[selection], balance[selection], currency[selection]);
            return 1;
        }
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int accountListing(){ // Lists all the existing accounts and their information.
    int i=0;
    if(numberOfAccounts==0){
        printf("No registered accounts yet.\n");
        return 0;
    }
    printf("Here is the list of all the registered accounts:\n");
    for(i=0;i<numberOfAccounts;i++){
        printf("Account number: %-4d TrID: %-11s Name: %-20s Surname: %-20s Currency: %s Balance: %.2lf %s\n", accountNumber[i], trID[i], name[i], surname[i], currency[i], balance[i], currency[i]);
    }
    return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int accountClosure(){ // Asks the user the amount of accounts they want to close and sends this information to the sub-function.
    int choice;
    int amount;
    while(1){
        printf("How many accounts do you want to close?\n");
        printf("1) A certain amount\n");
        printf("2) All\n");
        printf("3) Exit\n");
        scanf("%d",&choice);
        switch (choice)
        {
        case 1:
            while(1){
                printf("How many accounts do you want to close?\n");
                printf("(To exit, type '0')\n");
                scanf("%d",&amount);
                if(amount==0){
                    break;
                }
                return closeAccount(amount);
            }
            break;
        case 2:
            while(1){
                char choiceyn;
                printf("Are you sure you want to close all accounts?\n");
                printf("y/n\n");
                scanf(" %c",&choiceyn);
                if(choiceyn=='y'){
                    return closeAccount(maxUserAmount);
                }
                else if(choiceyn=='n'){
                    printf("Account closure cancelled.\n");
                    return 0;
                }
                else{
                    printf("Invalid choice. Please try again.");
                    continue;
                }
            }
            break;
        case 3:
        return 0;
            break;
        default:
            printf("Invalid choice. Please try again.");
            break;
        }

    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int closeAccount(int amount){ // Closes accounts by overwriting data. Since accounts are ordered by account numbers, it shifts all the accounts after the wanted account to the left by 1.
    int i;
    int j;
    int index;
    int hold=0;
    
    for(i=0;i<amount;i++){
        if(amount==maxUserAmount){
            index=1;
        }
        else{
            printf("Enter the account number of the %d'th account you want to close:\n",i+1);
            printf("(To exit, type '0')\n");
            scanf("%d",&index);
            showReceipt(receiptAccountClosure(index));
            if(index>=hold){
            index-=i; // Since we don't take inputs simultaneously, this helps us keeping the index as the user's intended numbers by shifting the index by the amount of times we closed accounts in a single function.
        	hold=index+i;
			}
        	hold=index;
			if(index-1==-1){
                return 0;
            }
        }
        for(j=index-1;j<maxUserAmount;j++){
            if(j!=maxUserAmount-1){
                strcpy(trID[j],trID[j+1]);
                strcpy(name[j],name[j+1]);
                strcpy(surname[j],surname[j+1]);
                strcpy(currency[j],currency[j+1]);
                balance[j]=balance[j+1];
            }
            else{
                accountNumber[j]=0;
                strcpy(trID[j],"0");
                strcpy(name[j],"0");
                strcpy(surname[j],"0");
                strcpy(currency[j],"0");
                balance[j]=0;
            }
        }
    numberOfAccounts--;
    }
    if(amount==maxUserAmount){
        numberOfAccounts=0;
    }
    accountSaving();
    printf("Account closure successful.\n");
    return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char* directory(){ // Creates directories in the documents section of the computer.
    int i;
    #ifdef _WIN32
        for (i = 0; i < 6; ++i) {
            if (_access(bankingAppDir[i], 0) != 0) { // We check if directories exist.
                if (_mkdir(bankingAppDir[i]) != 0) { // We create directories if they don't exist.
                    printf("Error! Couldn't create the directory %s.\n", bankingAppDir[i]); // If _mkdir() function returns a value other than 0 (which means it couldn't create the file) we display an error message.
                }
            }
        }
        return* bankingAppDir;
    #else
        for (i = 0; i < 6; ++i) {
            if (access(bankingAppDirUnix[i], F_OK) != 0) { // We check if directories exist.
                if (mkdir(bankingAppDirUnix[i], 0755) != 0) { // We create directories if they don't exist.
                    printf("Error! Couldn't create the directory %s.\n", bankingAppDirUnix[i]);  // If mkdir() function returns a value other than 0 (which means it couldn't create the file) we display an error message.
                }
            }
        }
        return* bankingAppDirUnix;
    #endif
    
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char* receiptAccountCreation(int accNumber){ // Creates a formatted receipt about account creation and saves it to the proper directory.
    time_t t = time(NULL);
    struct tm date = *localtime(&t);
    static char fileName[128];
    

    directory(); // Checks the directories, creates if they don't exist.
    sprintf(fileName,"%s%s_%s_TrID(%s)_Account_Creation_Receipt_%d-%02d-%02d_%02d.%02d.%02d.txt", bankingAppDir[3], // 3 is the account creation directory.
                                                                                                    name[accNumber-1], surname[accNumber-1],trID[accNumber-1],
                                                                                                    date.tm_year+1900, date.tm_mon+1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);

    FILE *fp;
    fp=fopen(fileName,"w");
    
    if(fp==NULL){
            printf("Error! Couldn't create the receipt file.");
            return 0;
        }

    fprintf(fp,"----------------------------------------\n");
    fprintf(fp,"| Date: %d-%02d-%02d                     |\n", date.tm_year+1900, date.tm_mon+1, date.tm_mday);
    fprintf(fp,"| Time: %02d:%02d:%02d                       |\n", date.tm_hour, date.tm_min, date.tm_sec);
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"|       Account Creation Receipt       |\n");
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"| Created Account Info:                |\n");
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"| Account Number: %-21d|\n",accNumber);
    fprintf(fp,"| TrID: %-31s|\n",trID[accNumber-1]);
    fprintf(fp,"| Name: %-31s|\n",name[accNumber-1]);
    fprintf(fp,"| Surname: %-28s|\n",surname[accNumber-1]);
    fprintf(fp,"| Currency: %-27s|\n",currency[accNumber-1]);
    fprintf(fp,"| Balance: %-28.2lf|\n",balance[accNumber-1]);
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"----------------------------------------\n");
    

    fclose(fp);
    
    return fileName;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char* receiptAccountClosure(int accNumber){ // Creates a formatted receipt about account closure and saves it to the proper directory.
    time_t t = time(NULL);
    struct tm date = *localtime(&t);
    static char fileName[128];
    
    directory(); // Checks the directories, creates if they don't exist.
    sprintf(fileName,"%s%s_%s_TrID(%s)_Account_Closure_Receipt_%d-%02d-%02d_%02d.%02d.%02d.txt", bankingAppDir[4], // 4 is the account closure directory.
                                                                                                    name[accNumber-1], surname[accNumber-1],trID[accNumber-1], 
                                                                                                    date.tm_year+1900, date.tm_mon+1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
    FILE *fp;
    fp=fopen(fileName,"w");
    
    if(fp==NULL){
            printf("Error! Couldn't create the receipt file.");
            return 0;
        }

    fprintf(fp,"----------------------------------------\n");
    fprintf(fp,"| Date: %d-%02d-%02d                     |\n", date.tm_year+1900, date.tm_mon+1, date.tm_mday);
    fprintf(fp,"| Time: %02d:%02d:%02d                       |\n", date.tm_hour, date.tm_min, date.tm_sec);
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"|        Account Closure Receipt       |\n");
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"| Closed Account Info:                 |\n");
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"| Account Number: %-21d|\n",accNumber);
    fprintf(fp,"| TrID: %-31s|\n",trID[accNumber-1]);
    fprintf(fp,"| Name: %-31s|\n",name[accNumber-1]);
    fprintf(fp,"| Surname: %-28s|\n",surname[accNumber-1]);
    fprintf(fp,"| Currency: %-27s|\n",currency[accNumber-1]);
    fprintf(fp,"| Balance: %-28.2lf|\n",balance[accNumber-1]);
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"| Notice: You can withdraw your        |\n");
    fprintf(fp,"|         remaining balance within     |\n");
    fprintf(fp,"|         15 business days by visiting |\n");
    fprintf(fp,"|         our nearest branch to you.   |\n");
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"----------------------------------------\n");
    

    fclose(fp);
    
    return fileName;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char* receiptMoneyTransfer(int senderAccNumber, double sent, int recipientAccNumber, double received){ // Creates a formatted receipt about money transfer and saves it to the proper directory.
    time_t t = time(NULL);
    struct tm date = *localtime(&t);
    static char fileName[128];
    
    directory(); // Checks the directories, creates if they don't exist.
    sprintf(fileName,"%s%s_%s_TrID(%s)_To_%s_%s_TrID(%s)_Money_Transfer_Receipt_%d-%02d-%02d_%02d.%02d.%02d.txt", bankingAppDir[5], // 5 is the money transfer directory.
                                                                                                                        name[senderAccNumber-1], surname[senderAccNumber-1],trID[senderAccNumber-1],
                                                                                                                        name[recipientAccNumber-1], surname[recipientAccNumber-1],trID[recipientAccNumber-1],
                                                                                                                        date.tm_year+1900, date.tm_mon+1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
    FILE *fp;
    fp=fopen(fileName,"w");
    
    if(fp==NULL){
            printf("Error! Couldn't create the receipt file.");
            return 0;
        }

    fprintf(fp,"----------------------------------------\n");
    fprintf(fp,"| Date: %d-%02d-%02d                     |\n", date.tm_year+1900, date.tm_mon+1, date.tm_mday);
    fprintf(fp,"| Time: %02d:%02d:%02d                       |\n", date.tm_hour, date.tm_min, date.tm_sec);
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"|        Money Transfer Receipt        |\n");
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"| Transaction Summary:                 |\n");
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"| From:                                |\n");
    fprintf(fp,"|  %-20s                |\n", name[senderAccNumber-1]);
    fprintf(fp,"|  %-20s                |\n", surname[senderAccNumber-1]);
    fprintf(fp,"|  TrID: %-11s                   |\n", trID[senderAccNumber-1]);
    fprintf(fp,"| Sent:                                |\n");
    fprintf(fp,"|  %s %-32.2lf|\n", currency[senderAccNumber-1], sent);
    fprintf(fp,"| To:                                  |\n");
    fprintf(fp,"|  %-20s                |\n", name[recipientAccNumber-1]);
    fprintf(fp,"|  %-20s                |\n", surname[recipientAccNumber-1]);
    fprintf(fp,"|  TrID: %-11s                  |\n", trID[recipientAccNumber-1]);
    fprintf(fp,"| Received:                            |\n");
    fprintf(fp,"|  %s %-32.2lf|\n", currency[recipientAccNumber-1], received);
    if(sent!=received){
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"| (Converted %s into %s)             |\n", currency[senderAccNumber-1], currency[recipientAccNumber-1]);
    }
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"|   ////////////////////////////////   |\n");
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"|               Details                |\n");
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"| Sender Account Info:                 |\n");
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"| Account Number: %-21d|\n",senderAccNumber);
    fprintf(fp,"| TrID: %-31s|\n",trID[senderAccNumber-1]);
    fprintf(fp,"| Name: %-31s|\n",name[senderAccNumber-1]);
    fprintf(fp,"| Surname: %-28s|\n",surname[senderAccNumber-1]);
    fprintf(fp,"| Currency: %-27s|\n",currency[senderAccNumber-1]);
    fprintf(fp,"| Balance Before Transaction:          |\n");
    fprintf(fp,"|   %s %-31.2lf|\n", currency[senderAccNumber-1], balance[senderAccNumber-1]+sent);
    fprintf(fp,"| Balance After Transaction:           |\n");
    fprintf(fp,"|   %s %-31.2lf|\n", currency[senderAccNumber-1], balance[senderAccNumber-1]);
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"| Recipient Account Info:              |\n");
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"| Account Number: %-21d|\n",recipientAccNumber);
    fprintf(fp,"| TrID: %-31s|\n",trID[recipientAccNumber-1]);
    fprintf(fp,"| Name: %-31s|\n",name[recipientAccNumber-1]);
    fprintf(fp,"| Surname: %-28s|\n",surname[recipientAccNumber-1]);
    fprintf(fp,"| Currency: %-27s|\n",currency[recipientAccNumber-1]);
    fprintf(fp,"| Balance Before Transaction:          |\n");
    fprintf(fp,"|   %s %-31.2lf|\n", currency[recipientAccNumber-1], balance[recipientAccNumber-1]-received);
    fprintf(fp,"| Balance After Transaction:           |\n");
    fprintf(fp,"|   %s %-31.2lf|\n", currency[recipientAccNumber-1], balance[recipientAccNumber-1]);
    fprintf(fp,"|                                      |\n");
    fprintf(fp,"----------------------------------------\n");
    

    fclose(fp);
    
    return fileName;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int showReceipt(char* fileName){ // Function to show and (or) inform the user about the directory of the receipt file.
    FILE *fp;
    fp=fopen(fileName,"r");
    char choiceyn;
    char row[41];

    if(fp==NULL){
            printf("Error! Couldn't open the receipt.\n");
            return 0;
        }

    while(1){
        printf("Do you want to see the receipt?\n");
        printf("y/n\n");
        scanf(" %c",&choiceyn);
        if(choiceyn=='y'){
            while(fgets(row,41,fp)){;
             printf("%s",row); 
            }
            printf("The receipt is saved as %s to your device, you can check it anytime you want.\n",fileName);
            break;
        }
        else if(choiceyn=='n'){
            printf("The receipt is saved as %s to your device, you can check it anytime you want.\n",fileName);
            break;
        }
        else{
            printf("Invalid choice. Please try again.\n");
            continue;
        }
    }
    fclose(fp);
    return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int pause(){ // To wait for user to continue.
    printf("Press any key to continue...\n");
    getchar();
    getchar();
    return 1;
}


