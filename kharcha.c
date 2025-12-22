#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

struct Item {
    char d[20];
    float amt;
    char cat[50];
    char why[50];
};

struct Item list[1000];
int n = 0;
float bud = 0.0;
char cats[50][50];
int n_cats = 0;

int get_days_left();
void print_header();
void save();
void load();
void add();
void del();
void clear_all();
void trend();
void search();
void new_cat();
void set_bud();
float spent();

int get_days_left() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    int days_in_mon[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int y = tm->tm_year + 1900;
    if((y%4==0 && y%100!=0) || y%400==0) days_in_mon[2] = 29;    
    return days_in_mon[tm->tm_mon + 1] - tm->tm_mday;
}

void print_header() {
    system("cls"); 
    float s = spent();
    float left = bud - s;
    int days = get_days_left();
    
    printf(CYAN "========================================\n");
    printf("      EXPENSE TRACKER DASHBOARD\n");
    printf("========================================\n" RESET);
    
    printf("BUDGET:    " YELLOW "%.2f\n" RESET, bud);
    printf("SPENT:     " RED "%.2f\n" RESET, s);
    printf("REMAINING: " GREEN "%.2f\n" RESET, left);
    printf("DAYS LEFT: " CYAN "%d days\n" RESET, days);
    
    if(days > 0) 
        printf("SAFE/DAY:  " GREEN "%.2f\n" RESET, left/days);
    else 
        printf("SAFE/DAY:  0.00\n");
        
    printf(CYAN "----------------------------------------\n" RESET);
}

int main() {
    load();
    while(1) {
        print_header();
        
        printf("1. Add Transaction\n");
        printf("2. Remove Transaction\n");
        printf("3. Clear All Data\n");
        printf("4. Trend Chart\n");
        printf("5. Search\n");
        printf("6. New Category\n");
        printf("7. Set Budget\n");
        printf("8. Exit\n");
        
        printf(CYAN "----------------------------------------\n" RESET);
        printf("Select Option: ");
        
        int opt;
        scanf("%d", &opt);
        switch(opt) {
            case 1: add(); break;
            case 2: del(); break;
            case 3: clear_all(); break;
            case 4: trend(); break;
            case 5: search(); break;
            case 6: new_cat(); break;
            case 7: set_bud(); break;
            case 8: exit(1);
        }
    }
    return 0;
}

void save() {
    FILE *fp = fopen("expenses_data.txt", "w");
    if(fp == NULL) return;
    fprintf(fp, "%f\n", bud);
    fprintf(fp, "%d\n", n_cats);
    for(int i=0; i<n_cats; i++) fprintf(fp, "%s\n", cats[i]);
    fprintf(fp, "%d\n", n);
    for(int i=0; i<n; i++) {
        fprintf(fp, "%s|%f|%s|%s\n", list[i].d, list[i].amt, list[i].cat, list[i].why);
    }
    fclose(fp);
}

void load() {
    FILE *fp = fopen("expenses_data.txt", "r");
    if(fp == NULL) {
        strcpy(cats[0], "Food");
        strcpy(cats[1], "Travel");
        n_cats = 2;
        return;
    }
    fscanf(fp, "%f\n", &bud);
    fscanf(fp, "%d\n", &n_cats);
    for(int i=0; i<n_cats; i++) fscanf(fp, "%s\n", cats[i]);
    fscanf(fp, "%d\n", &n);
    for(int i=0; i<n; i++) {
        fscanf(fp, "%[^|]|%f|%[^|]|%[^\n]\n", list[i].d, &list[i].amt, list[i].cat, list[i].why);
    }
    fclose(fp);
}

void add() {
    if(n >= 1000) return;
    printf(GREEN "\n--- ADD NEW ---\n" RESET);
    printf("Amount: ");
    scanf("%f", &list[n].amt);
    
    printf("Categories:\n");
    for(int i=0; i<n_cats; i++) printf("%d. %s\n", i+1, cats[i]);
    int c;
    printf("Pick num: ");
    scanf("%d", &c);
    if(c > 0 && c <= n_cats) strcpy(list[n].cat, cats[c-1]);
    else strcpy(list[n].cat, "Other");
    
    printf("Reason: ");
    char trash; scanf("%c", &trash); 
    gets(list[n].why); 
    
    printf("Date (YYYY-MM-DD) or Enter for today: ");
    char temp[20];
    gets(temp);
    if(strlen(temp) < 5) {
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        sprintf(list[n].d, "%04d-%02d-%02d", tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
    } else {
        strcpy(list[n].d, temp);
    }
    n++;
    save();
}

void del() {
    printf(RED "\n--- REMOVE ---\n" RESET);
    for(int i=0; i<n; i++) {
        printf("%d: %s | %.2f | %s\n", i+1, list[i].d, list[i].amt, list[i].why);
    }
    int id;
    printf("ID to remove (0 to cancel): ");
    scanf("%d", &id);
    if(id > 0 && id <= n) {
        for(int i=id-1; i<n-1; i++) list[i] = list[i+1];
        n--;
        save();
        printf("Deleted.\n");
    }
}

void clear_all() {
    char ch;
    printf(RED "Delete EVERYTHING? (y/n): " RESET);
    scanf(" %c", &ch);
    if(ch == 'y') {
        n = 0;
        save();
        printf("All gone.\n");
    }
}

void trend() {
    printf(CYAN "\n--- TREND CHART ---\n" RESET);
    for(int i=0; i<n; i++) {
        printf("%s | ", list[i].d);
       
        printf(YELLOW); int stars = (int)(list[i].amt / 10);
        for(int j=0; j<stars; j++) printf("*");
        printf(RESET " %.2f\n", list[i].amt);
    }
    printf("\nPress Enter...");
    getchar(); getchar();
}

void search() {
    char key[50];
    printf(CYAN "\nEnter date or category: " RESET);
    char trash; scanf("%c", &trash);
    gets(key);
    for(int i=0; i<n; i++) {
        if(strcmp(list[i].d, key) == 0 || strcmp(list[i].cat, key) == 0) {
            printf("%s | %.2f | %s | %s\n", list[i].d, list[i].amt, list[i].cat, list[i].why);
        }
    }
    getchar();
}

void new_cat() {
    printf("New cat name: ");
    char trash; scanf("%c", &trash);
    gets(cats[n_cats]);
    n_cats++;
    save();
}

void set_bud() {
    printf("Old budget: %.2f\nNew: ", bud);
    scanf("%f", &bud);
    save();
}

float spent() {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char cur_m[10];
    sprintf(cur_m, "%04d-%02d", tm->tm_year+1900, tm->tm_mon+1);
    float sum = 0;
    for(int i=0; i<n; i++) {
        if(strncmp(list[i].d, cur_m, 7) == 0) sum += list[i].amt;
    }
    return sum;
}