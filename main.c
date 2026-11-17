#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char date[11];
    double amount;
    char category[50];
    char type[10];
} Transaction;

typedef struct {
    char category[50];
    int month;
    int year;
    double limit;
} Budget;

Transaction *transactions = NULL;
int transactionCount = 0;

Budget budgets[100];
int budgetCount = 0;

void addTransaction() {
    Transaction *temp = realloc(
        transactions, (transactionCount + 1) * sizeof(Transaction));

    if (temp == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    transactions = temp;
    Transaction *t = &transactions[transactionCount];

    printf("Enter date (YYYY-MM-DD): ");
    scanf("%10s", t->date);

    printf("Enter amount: ");
    if (scanf("%lf", &t->amount) != 1 || t->amount <= 0) {
        printf("Invalid amount.\n");
        return;
    }

    printf("Enter category: ");
    scanf(" %49[^\n]", t->category);

    printf("Enter type (income/expense): ");
    scanf("%9s", t->type);

    if (strcmp(t->type, "income") != 0 &&
        strcmp(t->type, "expense") != 0) {
        printf("Invalid transaction type.\n");
        return;
    }

    transactionCount++;
    printf("Transaction added successfully.\n");
}

void setBudget() {
    if (budgetCount >= 100) {
        printf("Budget limit reached.\n");
        return;
    }

    Budget *b = &budgets[budgetCount];

    printf("Enter category: ");
    scanf(" %49[^\n]", b->category);

    printf("Enter month (1-12): ");
    scanf("%d", &b->month);

    printf("Enter year: ");
    scanf("%d", &b->year);

    printf("Enter monthly budget: ");
    scanf("%lf", &b->limit);

    if (b->month < 1 || b->month > 12 || b->limit < 0) {
        printf("Invalid budget details.\n");
        return;
    }

    budgetCount++;
    printf("Budget set successfully.\n");
}

void monthlySummary() {
    int month, year;
    double income = 0, expense = 0;

    printf("Enter month (1-12) and year: ");
    scanf("%d %d", &month, &year);

    if (month < 1 || month > 12) {
        printf("Invalid month.\n");
        return;
    }

    for (int i = 0; i < transactionCount; i++) {
        int tm, ty;
        sscanf(transactions[i].date, "%4d-%2d-%2d",
               &ty, &tm, &(int){0});

        if (tm == month && ty == year) {
            if (strcmp(transactions[i].type, "income") == 0)
                income += transactions[i].amount;
            else
                expense += transactions[i].amount;
        }
    }

    printf("\nMonthly Summary: %02d/%d\n", month, year);
    printf("Total Income: %.2f\n", income);
    printf("Total Expense: %.2f\n", expense);
    printf("Net Savings: %.2f\n", income - expense);
}

void categorySpending() {
    int month, year;
    char category[50];
    double total = 0;

    printf("Enter category: ");
    scanf(" %49[^\n]", category);

    printf("Enter month and year: ");
    scanf("%d %d", &month, &year);

    for (int i = 0; i < transactionCount; i++) {
        int tm, ty, day;

        if (sscanf(transactions[i].date, "%4d-%2d-%2d",
                   &ty, &tm, &day) != 3)
            continue;

        if (tm == month && ty == year &&
            strcmp(transactions[i].category, category) == 0 &&
            strcmp(transactions[i].type, "expense") == 0) {
            total += transactions[i].amount;
        }
    }

    printf("Spending in %s: %.2f\n", category, total);
}

void checkBudgetAlerts() {
    for (int b = 0; b < budgetCount; b++) {
        double spent = 0;

        for (int i = 0; i < transactionCount; i++) {
            int tm, ty, day;

            if (sscanf(transactions[i].date, "%4d-%2d-%2d",
                       &ty, &tm, &day) != 3)
                continue;

            if (tm == budgets[b].month &&
                ty == budgets[b].year &&
                strcmp(transactions[i].category,
                       budgets[b].category) == 0 &&
                strcmp(transactions[i].type, "expense") == 0) {
                spent += transactions[i].amount;
            }
        }

        printf("\nCategory: %s (%02d/%d)\n",
               budgets[b].category,
               budgets[b].month, budgets[b].year);
        printf("Budget: %.2f | Spent: %.2f\n",
               budgets[b].limit, spent);

        if (spent > budgets[b].limit)
            printf("ALERT: Budget exceeded!\n");
        else if (spent >= budgets[b].limit * 0.8)
            printf("WARNING: You have used 80%% or more of your budget.\n");
        else
            printf("Status: Within budget.\n");
    }
}

void trendReport() {
    int n;

    printf("Enter number of recent months: ");
    scanf("%d", &n);

    if (n <= 0 || n > 120) {
        printf("Enter a number between 1 and 120.\n");
        return;
    }

    int month, year;
    printf("Enter ending month and year: ");
    scanf("%d %d", &month, &year);

    if (month < 1 || month > 12) {
        printf("Invalid month.\n");
        return;
    }

    printf("\nMonthly Trend Report\n");

    for (int k = n - 1; k >= 0; k--) {
        int m = month - k;
        int y = year;

        while (m <= 0) {
            m += 12;
            y--;
        }

        double income = 0, expense = 0;

        for (int i = 0; i < transactionCount; i++) {
            int tm, ty, day;

            if (sscanf(transactions[i].date, "%4d-%2d-%2d",
                       &ty, &tm, &day) != 3)
                continue;

            if (tm == m && ty == y) {
                if (strcmp(transactions[i].type, "income") == 0)
                    income += transactions[i].amount;
                else
                    expense += transactions[i].amount;
            }
        }

        printf("%02d/%d | Income: %.2f | Expense: %.2f | Savings: %.2f\n",
               m, y, income, expense, income - expense);
    }
}

void exportCSV() {
    FILE *file = fopen("transactions.csv", "w");

    if (file == NULL) {
        printf("Unable to create CSV file.\n");
        return;
    }

    fprintf(file, "Date,Amount,Category,Type\n");

    for (int i = 0; i < transactionCount; i++) {
        fprintf(file, "%s,%.2f,\"%s\",%s\n",
                transactions[i].date,
                transactions[i].amount,
                transactions[i].category,
                transactions[i].type);
    }

    fclose(file);
    printf("Transactions exported to transactions.csv\n");
}

void viewTransactions() {
    if (transactionCount == 0) {
        printf("No transactions available.\n");
        return;
    }

    for (int i = 0; i < transactionCount; i++) {
        printf("\nDate: %s\nAmount: %.2f\nCategory: %s\nType: %s\n",
               transactions[i].date,
               transactions[i].amount,
               transactions[i].category,
               transactions[i].type);
    }
}

int main() {
    int choice;

    do {
        printf("\n===== PERSONAL FINANCE TRACKER =====\n");
        printf("1. Add Transaction\n");
        printf("2. Set Monthly Budget\n");
        printf("3. Monthly Summary\n");
        printf("4. Category-wise Spending\n");
        printf("5. Check Budget Alerts\n");
        printf("6. Trend Report\n");
        printf("7. Export Transactions to CSV\n");
        printf("8. View All Transactions\n");
        printf("0. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
            break;

        switch (choice) {
            case 1: addTransaction(); break;
            case 2: setBudget(); break;
            case 3: monthlySummary(); break;
            case 4: categorySpending(); break;
            case 5: checkBudgetAlerts(); break;
            case 6: trendReport(); break;
            case 7: exportCSV(); break;
            case 8: viewTransactions(); break;
            case 0: printf("Exiting Finance Tracker.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);

    free(transactions);
    return 0;
}