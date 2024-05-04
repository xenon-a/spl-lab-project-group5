#include<stdio.h>
#include<string.h>

int user_index = -1;
int admin = 0;

struct User
{
    char name[256];
    char username[64];
    char pass[64];
    int id;
    float balance;
    // Date of account creation
};


void updatedb(struct User users[], int total)
{
    FILE *usersdb = fopen("usersdb.dat", "wb");

    fwrite(users, sizeof(struct User), total, usersdb);

    fclose(usersdb);
}


int get_user_count()
{
    FILE *usersdb = fopen("usersdb.dat", "rb");

    if (usersdb==NULL)
        return 0;

    fseek(usersdb, 0, SEEK_END);
    int count = ftell(usersdb) / sizeof(struct User);
    fclose(usersdb);

    return count;
}


void readFromFile(struct User *users, int count)
{
    FILE *usersdb = fopen("usersdb.dat", "rb");

    fread(users, sizeof(struct User), count, usersdb);

    fclose(usersdb);
}


int generate_id()
{
    int total = get_user_count();

    if (total<1)
        return 1;

    struct User users[total];

    readFromFile(users, total);

    return users[total-1].id + 1;
}


void show_db()
{
    int total = get_user_count();
    if (total<1){
        printf("\n\n--Database empty--\n\n");
    }
    else {
        struct User users[total];
        readFromFile(users, total);
        printf("\n\n");
        for (int i=0; i<total; i++){
            printf("Account owner: %s", users[i].name);
            printf("User ID: %d\n", users[i].id);
            printf("Username: %s\n", users[i].username);
            printf("Password: %s", users[i].pass);
            printf("Balance: %.2f BDT\n\n", users[i].balance);
        }
    }
}


void show_info()
{
    int total = get_user_count();
    struct User users[total];
    readFromFile(users, total);

    printf("\n\nOwner Name: %s", users[user_index].name);
    printf("Username: %s\n", users[user_index].username);
    printf("User ID: %d\n", users[user_index].id);
    printf("Balance: %.2f BDT\n\n", users[user_index].balance);
}

int get_user_index(char username[], int check)
{
    int users_count = get_user_count();

    if (users_count==0)
        return -1;

    struct User users[users_count];

    readFromFile(users, users_count);

    for (int i=0; i<users_count; i++){
        if (i==check)
            continue;
        if (strcmp(users[i].username, username)==0){
            return i;
        }
    }

    return -1;
}

void addUser(struct User user)
{
    FILE *usersdb = fopen("usersdb.dat", "ab");

    fwrite(&user, sizeof(struct User), 1, usersdb);

    fclose(usersdb);
}

int get_index_by_id()
{
    int id;
    printf("Enter the id of the user: \n");
    printf(">>> ");
    scanf("%d", &id);
    getchar();

    int total = get_user_count();
    struct User users[total];
    readFromFile(users, total);

    for (int i=0; i<total; i++){
        if (users[i].id == id){
            return i;
        }
    }

    printf("\n!!!No user with that ID was found in the database!!!\n\n");
    return -1;
}

int get_index_by_username()
{
    char username[64];
    printf("Enter the username of the user: \n");
    printf(">>> ");
    scanf("%s", username);

    int total = get_user_count();
    struct User users[total];
    readFromFile(users, total);

    for (int i=0; i<total; i++){
        if (strcmp(users[i].username, username)==0){
            return i;
        }
    }

    printf("\n!!!No user with that username was found in the database!!!\n\n");
    return -1;
}

void auto_delete_accounts()
{
    int total = get_user_count();
    struct User users[total];
    readFromFile(users, total);

    int delete_count = 0;

    for (int i=0; i<total; i++){
        if (users[i].balance < 200){
            for (int j=i; j<total-1; j++){
                users[j] = users[j+1];
            }
            delete_count++;
        }
    }
    updatedb(users, total-delete_count);

    printf("\n----Deleted accounts with Balance Less than BDT 200----\n\n");
}

void deleteUser()
{
    printf("\nDeleting account...\n");
    int total = get_user_count();
    struct User users[total];
    readFromFile(users, total);

    for (int x=user_index; x<total-1; x++){
        users[x] = users[x+1];
    }

    user_index = -1;

    updatedb(users, total-1);

    printf("---Deleted the account successfully!!\n\n");
}

void updateAccount()
{
    int total = get_user_count();
    struct User users[total];
    readFromFile(users, total);
    updateMenu:
        printf("\n----Please choose what you want to update: ----\n\n");
        printf("1. Name\n");
        printf("2. Username\n");
        printf("3. Password\n");
        printf("4. Save changes and return\n");
        printf("5. Discard changes and return\n");
    int choice;
    printf(">>> ");
    scanf("%d", &choice);
    getchar();

    if (choice==1){
        char newname[256];
        printf("Enter new name: ");
        fgets(newname, 256, stdin);
        strcpy(users[user_index].name, newname);
        printf("\n---Name of the owner has been changed successfully!---\n\n");
        goto updateMenu;
    }
    else if (choice==2){
        char newusername[64];
        printf("Enter the new username: ");
        scanf("%s", newusername);
        getchar();
        if (get_user_index(newusername, user_index)!=-1){
            printf("\n!!!Username already taken!!!\n\n");
        }
        else {
            strcpy(users[user_index].username, newusername);
            printf("\n---Succesfully changed the username---\n\n");
        }
        goto updateMenu;
    }
    else if (choice==3){
        char newPass[64];
        char oldPass[64];
        char confirmPass[64];
        if (admin==1)
            goto choosepass;
        printf("Enter old password:\n");
        printf(">>> ");
        fgets(oldPass, 64, stdin);
        if (strcmp(oldPass, users[user_index].pass)!=0){
            printf("\n!!!Wrong Password!!!\n\n");
            goto updateMenu;
        }
        else {
            choosepass:
                printf("Enter New Password: ");
                printf(">>> ");
                fgets(newPass, 64, stdin);
                printf("Confirm password: ");
                printf(">>> ");
                fgets(confirmPass, 64, stdin);
            if (strcmp(newPass, confirmPass)!=0){
                printf("\n!!Passwords didn't match!! Try again!\n\n");
                goto choosepass;
            }
            else {
                strcpy(users[user_index].pass, newPass);
                printf("\n---Password has been changed successfully---\n\n");
            }
            goto updateMenu;
        }
    }
    else if (choice==4){
        printf("\n---Saving changes to this account---\n");
        updatedb(users, total);
        printf("---Changes saved successfully---\n\n");
    }
    else if (choice==5){
        printf("\n---Changes were not saved---\n\n");
    }
    else {
        printf("\nInvalid choice!!!\n\n");
        goto updateMenu;
    }
}

void addMoney()
{
    printf("Enter the amount of money you want to deposit: \n");
    printf(">>> ");
    float amount;
    scanf("%f", &amount);
    getchar();

    int total = get_user_count();
    struct User users[total];

    readFromFile(users, total);

    users[user_index].balance += amount;

    updatedb(users, total);

    printf("\n--Successfully added BDT %.3f to your account--\n\n", amount);
}


void takeMoney()
{
    printf("Enter the amount of money you want to withdraw: \n");
    printf(">>> ");
    float amount;
    scanf("%f", &amount);

    int total = get_user_count();
    struct User users[total];

    readFromFile(users, total);

    if (users[user_index].balance < amount)
        printf("\n---Not Enough Balance!!!---\n\n");
    else {
        users[user_index].balance -= amount;
        updatedb(users, total);

        printf("\n--Withdrawal of BDT %.3f from your account is successful!--\n\n", amount);
    }
}


void check_balance()
{
    int total = get_user_count();
    struct User users[total];
    readFromFile(users, total);

    printf("\n------------\n");
    printf("Account Owner: %s", users[user_index].name);
    printf("Current Balance: %.2f BDT", users[user_index].balance);
    printf("\n------------\n\n");
}


int login()
{
    printf("\n--------------Login---------------\n\n");
    char username[64];
    char pass[64];

    printf("Username: ");
    scanf("%s", username);
    getchar();

    int index = get_user_index(username, -1);

    if (index==-1){
        printf("\n---User not found! You can create an account from the main menu---\n\n");
        return -1;
    }
    else {
        int total = get_user_count();
        struct User users[total];
        readFromFile(users, total);

        printf("Enter the password: \n");
        int chances = 3;

        while (chances>0){
            printf(">>> ");
            fgets(pass, 64, stdin);

            if (strcmp(pass, users[index].pass)!=0){
                printf("\nIncorrect Password! ");
                chances--;
                if (chances<=0){
                    printf("!!!Access Denied!!\n\n");
                    return -1;
                }
                else {
                    printf("%d Attempts left!\n\n", chances);
                }
            }
            else {
                printf("\n\n--------Login Successful!--------\n\n");
                return index;
            }
        }
    }
}


void signup()
{
    printf("\n-----------Sign up-----------\n\n");
    struct User newUser;

    printf("Enter your name: ");
    fgets(newUser.name, 256, stdin);

    chooseuser:
        printf("Choose your username: ");
        scanf("%s", newUser.username);
        getchar();

    int index = get_user_index(newUser.username, -1);

    if (index!=-1){
        printf("\n!!Username already exists!\n\n");
        int choice;
        userexists:
            printf("1. Choose different username\n");
            printf("2. Return to main menu\n");
            printf(">>> ");
            scanf("%d", &choice);
        if (choice==1)
            goto chooseuser;
        else if (choice == 2)
            return;
        else
            printf("\n!!Invalid choice!!!\n\n");
            goto userexists;
    }

    choosepass:
        printf("Enter new password: ");
        fgets(newUser.pass, 64, stdin);

        char confirm[64];
        printf("Confirm password: ");
        fgets(confirm, 64, stdin);

    if (strcmp(newUser.pass, confirm)!=0){
        printf("\nPasswords didn't match!!! Try again.\n\n");
        goto choosepass;
    }

    newUser.id = generate_id();
    newUser.balance = 0.00;

    addUser(newUser);

    printf("\n-------------Account creation Successful!----------------\n\n");
}

int main()
{
    int choice;
    printf("\n\n\n---------------Welcome to Simple Bank-------------\n\n");
    printf("Please enter the number corresponding to your desired action: \n\n");

    while (1)
    {
        if (user_index == -1 && admin == 0){
            printf("---------------Main Menu--------------\n");
            printf("1. Login\n");
            printf("2. Create account\n");
            printf("3. Exit\n");
            printf("4. Admin Panel\n");
            printf(">>> ");
            scanf("%d", &choice);
            getchar();

            if (choice==1)
                user_index = login();
            else if (choice==2)
                signup();
            else if (choice == 3)
                break;
            else if (choice==4){
                int confirm;
                printf("--------997977--------\n");
                printf("Enter the above number to get access to the Admin Panel: ");
                scanf("%d", &confirm);
                getchar();
                if (confirm != 997977){
                    printf("\n\n!!!Incorrect!!!\n\n");
                    admin = 0;
                }
                else {
                    admin = 1;
                    user_index = -1;
                }
            }
            else
                printf("\n!!Invalid choice!!\n\n");
        }
        else if (user_index != -1 && admin == 0) {
            printf("-------------Account Actions-----------\n");
            printf("1. Check Balance\n");
            printf("2. Add money\n");
            printf("3. Withdraw money\n");
            printf("4. Show Account informations\n");
            printf("5. Update Account informations\n");
            printf("6. Logout\n");
            printf("7. Delete Account\n");

            printf(">>> ");
            scanf("%d", &choice);
            getchar();

            if (choice==1){
                check_balance();
            }
            else if (choice==2){
                addMoney();
            }
            else if (choice==3){
                takeMoney();
            }
            else if (choice==4){
                show_info();
            }
            else if (choice==5){
                updateAccount();
            }
            else if (choice==6){
                printf("--Logging out--\n");
                user_index = -1;
            }
            else if (choice==7){
                printf("\n--Are you sure?--\n");
                printf("1. Yes\n");
                printf("2. No\n");
                int deletion_choice;
                printf(">>> ");
                scanf("%d", &deletion_choice);
                getchar();
                if (deletion_choice==1){
                    deleteUser();
                }
            }
            else {
                printf("\n!!!Invalid Choice!!!\n\n");
            }
        }
        else {
            printf("\n---------Admin Panel----------\n");
            printf("1. Show Database\n");
            printf("2. Delete account by ID\n");
            printf("3. Delete account by username\n");
            printf("4. Auto Delete accounts with balance less than 200 Tk\n");
            printf("5. Update account by ID\n");
            printf("6. Update account by username\n");
            printf("7. Close admin panel\n");

            printf(">>> ");
            scanf("%d", &choice);

            if (choice==1){
                show_db();
            }
            else if (choice==2){
                user_index = get_index_by_id();
                if (user_index!=-1)
                    deleteUser();
                user_index = -1;
            }
            else if (choice==3){
                user_index = get_index_by_username();
                if (user_index!=-1)
                    deleteUser();
                user_index = -1;
            }
            else if (choice==4){
                auto_delete_accounts();
            }
            else if (choice==5){
                user_index = get_index_by_id();
                if (user_index!=-1)
                    updateAccount();
                user_index = -1;
            }
            else if (choice==6){
                user_index = get_index_by_username();
                if (user_index!=-1)
                    updateAccount();
                user_index = -1;
            }
            else if (choice==7){
                printf("Closing admin panel...\n\n");
                admin = 0;
                user_index = -1;
            }
            else {
                printf("\nInvalid choice!!!\n\n");
            }
        }
    }

    printf("Thank you for using our service! Exiting...");

    return 0;
}
