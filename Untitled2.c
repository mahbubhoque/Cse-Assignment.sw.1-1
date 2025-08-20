#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"

int unsuccessful028;

int is_exist028(char number028[], FILE *reference028){
    char line028[32];
    int found028=0;
    while(fgets(line028,sizeof(line028),reference028)){
        line028[strcspn(line028,"\n")]=0;
        if(strcmp(line028,number028)==0) found028++;
    }
    fclose(reference028);
    return found028;
}

// ---------- Admin Functions ----------
void generate_cards028(int count028,int minute028){
    FILE *fptr028;
    if(minute028==100) fptr028=fopen("unused_cards/100.txt","a+");
    else if(minute028==60) fptr028=fopen("unused_cards/60.txt","a+");
    else if(minute028==40) fptr028=fopen("unused_cards/40.txt","a+");
    else { printf(RED"Invalid minute\n"RESET); return; }
    srand(time(NULL));
    for(int i=0;i<count028;i++){
        char card028[21]="";
        for(int j=0;j<20;j++){
            int num028=rand()%10;
            char digit028[2]={num028+'0','\0'};
            strcat(card028,digit028);
        }
        fprintf(fptr028,"%s\n",card028);
    }
    fclose(fptr028);
    printf(GREEN"%d Cards of %d minutes created\n"RESET,count028,minute028);
}

void delete_card028(){
    char card028[21]; int minute028;
    printf("Enter minute type (40/60/100): "); scanf("%d",&minute028);
    printf("Enter card number: "); scanf("%20s",card028);
    char filename028[64];
    if(minute028==100) strcpy(filename028,"unused_cards/100.txt");
    else if(minute028==60) strcpy(filename028,"unused_cards/60.txt");
    else if(minute028==40) strcpy(filename028,"unused_cards/40.txt");
    else { printf(RED"Invalid\n"RESET); return; }
    FILE *src028=fopen(filename028,"r");
    FILE *tmp028=fopen("unused_cards/temp.txt","w");
    char line028[32]; int found028=0;
    while(fgets(line028,sizeof(line028),src028)){
        line028[strcspn(line028,"\n")]=0;
        if(strcmp(line028,card028)!=0) fprintf(tmp028,"%s\n",line028);
        else found028=1;
    }
    fclose(src028); fclose(tmp028);
    remove(filename028); rename("unused_cards/temp.txt",filename028);
    if(found028) printf(GREEN"Card deleted\n"RESET);
    else printf(RED"Card not found\n"RESET);
}

void unblock_number028(char number028[]){
    FILE *src028=fopen("blocked_numbers/numbers.txt","r");
    FILE *tmp028=fopen("blocked_numbers/temp.txt","w");
    char line028[12]; int found028=0;
    while(fgets(line028,sizeof(line028),src028)){
        line028[strcspn(line028,"\n")]=0;
        if(strcmp(line028,number028)!=0) fprintf(tmp028,"%s\n",line028);
        else found028=1;
    }
    fclose(src028); fclose(tmp028);
    remove("blocked_numbers/numbers.txt");
    rename("blocked_numbers/temp.txt","blocked_numbers/numbers.txt");
    if(found028) printf(GREEN"%s unblocked\n"RESET,number028);
    else printf(RED"%s not in block list\n"RESET,number028);
}

void record_transaction028(char *card028,char *mobile028,int minute028){
    FILE *history028=fopen("admin/history.txt","a+");
    if(!history028){ printf("Error\n"); return; }
    char date028[12],time028[10];
    time_t now028=time(NULL);
    struct tm *t028=localtime(&now028);
    strftime(date028,sizeof(date028),"%d-%m-%Y",t028);
    strftime(time028,sizeof(time028),"%H:%M",t028);
    fprintf(history028,"%s %s %s %d %s\n",card028,date028,time028,minute028,mobile028);
    fclose(history028);
}

void statistics028(){
    int sold40=0,sold60=0,sold100=0,rem40=0,rem60=0,rem100=0;
    FILE *f40=fopen("used_cards/40.txt","r"); FILE *f60=fopen("used_cards/60.txt","r"); FILE *f100=fopen("used_cards/100.txt","r");
    FILE *u40=fopen("unused_cards/40.txt","r"); FILE *u60=fopen("unused_cards/60.txt","r"); FILE *u100=fopen("unused_cards/100.txt","r");
    char line028[32];
    while(f40 && fgets(line028,sizeof(line028),f40)) sold40++;
    while(f60 && fgets(line028,sizeof(line028),f60)) sold60++;
    while(f100 && fgets(line028,sizeof(line028),f100)) sold100++;
    while(u40 && fgets(line028,sizeof(line028),u40)) rem40++;
    while(u60 && fgets(line028,sizeof(line028),u60)) rem60++;
    while(u100 && fgets(line028,sizeof(line028),u100)) rem100++;
    if(f40) fclose(f40); if(f60) fclose(f60); if(f100) fclose(f100);
    if(u40) fclose(u40); if(u60) fclose(u60); if(u100) fclose(u100);
    printf("\nTotal Sold: 40:%d 60:%d 100:%d\nRemaining: 40:%d 60:%d 100:%d\nAmount: 40:%d 60:%d 100:%d\n",
           sold40,sold60,sold100,rem40,rem60,rem100,sold40*50,sold60*70,sold100*120);
}

void history028(){
    FILE *history028=fopen("admin/history.txt","r");
    if(!history028){ printf(YELLOW"No history\n"RESET); return; }
    char line028[100];
    printf("\nCardNo Date Time Min Mobile\n");
    while(fgets(line028,sizeof(line028),history028)) printf("%s",line028);
    fclose(history028);
}

// ---------- User Functions ----------
void check_balance028(){
    FILE *f028=fopen("user/balance.txt","r");
    int bal028=0; if(f028){ fscanf(f028,"%d",&bal028); fclose(f028); }
    printf("Your balance: %d minutes\n",bal028);
}

void recharge028(char *mobile028){
    unsuccessful028=3;
    char card028[21]; int minute028=0;
    printf("\nSelect package: 1.40min50 2.60min70 3.100min120 : "); int choice028; scanf("%d",&choice028);
    if(choice028==1) minute028=40; else if(choice028==2) minute028=60; else if(choice028==3) minute028=100;
    else { printf(RED"Invalid\n"RESET); return;}

    while(unsuccessful028){
        printf("Enter card number (%d attempts left): ",unsuccessful028); scanf("%20s",card028);
        char unused028[64]; if(minute028==40) strcpy(unused028,"unused_cards/40.txt");
        else if(minute028==60) strcpy(unused028,"unused_cards/60.txt");
        else strcpy(unused028,"unused_cards/100.txt");
        FILE *f028=fopen(unused028,"r+");
        int found028=is_exist028(card028,f028);
        if(found028){
            FILE *uf028=fopen(unused028,"r"); FILE *tmp028=fopen("unused_cards/temp.txt","w");
            char line028[32]; while(fgets(line028,sizeof(line028),uf028)){ line028[strcspn(line028,"\n")]=0;
                if(strcmp(line028,card028)!=0) fprintf(tmp028,"%s\n",line028); }
            fclose(uf028); fclose(tmp028); remove(unused028); rename("unused_cards/temp.txt",unused028);
            char used028[64]; if(minute028==40) strcpy(used028,"used_cards/40.txt");
            else if(minute028==60) strcpy(used028,"used_cards/60.txt"); else strcpy(used028,"used_cards/100.txt");
            FILE *fused028=fopen(used028,"a+"); fprintf(fused028,"%s\n",card028); fclose(fused028);
            FILE *balf028=fopen("user/balance.txt","r+"); int bal028=0; fscanf(balf028,"%d",&bal028); fclose(balf028);
            balf028=fopen("user/balance.txt","w"); fprintf(balf028,"%d",bal028+minute028); fclose(balf028);
            record_transaction028(card028,mobile028,minute028);
            printf(GREEN"Recharge successful! +%d minutes\n"RESET,minute028); return;
        } else { printf(RED"Card not found\n"RESET); unsuccessful028--;
            if(unsuccessful028==0){ FILE *b028=fopen("blocked_numbers/numbers.txt","a+"); fprintf(b028,"%s\n",mobile028); fclose(b028);
            printf(RED"You are blocked!\n"RESET); return; } }
    }
}

// ---------- Admin Menu ----------
void admin028(){
    int t=1;
    while(t){
        printf("\n1.New Card 2.Delete 3.Unblock 4.History 5.Stats 6.Exit\nChoice: "); int ch; scanf("%d",&ch);
        if(ch==1){ int c,m; printf("How many & minutes: "); scanf("%d %d",&c,&m); generate_cards028(c,m);}
        else if(ch==2) delete_card028();
        else if(ch==3){ char num028[12]; printf("Enter mobile: "); scanf("%11s",num028); unblock_number028(num028);}
        else if(ch==4) history028();
        else if(ch==5) statistics028();
        else if(ch==6) {t=0; printf(CYAN"Back to main\n"RESET);}
        else printf(RED"Invalid\n"RESET);
    }
}

// ---------- User Menu ----------
void user028(){
    char mobile028[12]; printf("Enter your mobile: "); scanf("%11s",mobile028);
    int t=1;
    while(t){
        FILE *blk028=fopen("blocked_numbers/numbers.txt","r");
        if(blk028 && is_exist028(mobile028,blk028)){ printf(RED"You are blocked!\n"RESET); break;}
        printf("\n1.Balance 2.Recharge 3.Exit\nChoice: "); int ch; scanf("%d",&ch);
        if(ch==1) check_balance028();
        else if(ch==2) recharge028(mobile028);
        else if(ch==3) t=0;
        else printf(RED"Invalid\n"RESET);
    }
}

// ---------- Main ----------
int main(){
    int t=1;
    while(t){
        printf("\n1.Admin 2.User 3.Exit\nChoice: "); int ch; scanf("%d",&ch);
        if(ch==1){ int pwd; printf("Password: "); scanf("%d",&pwd); if(pwd==2403028) admin028();
        else printf(RED"Wrong password\n"RESET);}
        else if(ch==2) user028();
        else if(ch==3){ t=0; printf(BLUE"Goodbye\n"RESET);}
        else printf(RED"Invalid\n"RESET);
    }
    return 0;
}
