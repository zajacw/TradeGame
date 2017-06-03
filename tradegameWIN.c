#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct acc
{
       char login[20];
       char pass[20];
};

struct character
{
       int money;
       char place[40]; 
       int dif;
       int store[20];    
};

struct product
{
       char place[40];
       char name[30];
       int no;
       int sell;
       int buy;
};

int dangers(char[]);
int travelcost(char[], char[]);

int compare(const void *a, const void *b)  // funkcja porównuj¹ca by nie powtarzaæ miejsca w który jestem w menu -> q sort
{
    const char *ia = (const char *)a;
    const char *ib = (const char *)b;
    return strcmp(ia, ib);
}

int main()
{
  int amount, a, b, op, option, i=0, count;
  char c, string[1000], sort[6][30], placelist[][30]={"Macy's", "5th_Avenue", "Galeries_Lafayette", "The_Dubai_Mall", "Galleria_Vittorio_Emanuele_II", "Harrods"};
  FILE *f, *fp;
  struct acc account, check;
  struct character player;
  struct product product, p[8];
  
  do
  {
      fflush(stdin);  //czyszcz¹ bufory
      system("cls");
      printf("Main menu:\n");
      printf("1. Log in.\n2. Create new account.\n3. Quit.\n");
      scanf("%d",&op);
      
      switch (op)
      {
             case 3: break; //quit
             case 1:                //logging
             {
                  system("cls");
                  printf("Login: ");
                  scanf("%s",account.login);
                  printf("Password: ");
                  scanf("%s",account.pass);
                  f=fopen("accounts", "rb");
                  while(fread(&check, sizeof(struct acc), 1, f)>0)
                  {
                      if ((strcmp(account.login, check.login)==0)&&(strcmp(account.pass, check.pass)==0)) 
                      //strcmp funkcja porównujaca ciagi znakow. z pliku/wprowadzony
                      {
                           i=1;
                           fp=fopen(account.login, "rt");
                           fread(&player, sizeof(struct character), 1, fp);
                           fclose(f);
                           break;
                      }
                  }
                  if(i==1)
                  {
                      printf("Congratulations %s, you have successfully logged in.\n", account.login);  //game
                      system("PAUSE");
                      do // warunki wygranej lub przegranej
                      {
                           option=-1;
                           system("cls");
                           fp=fopen(account.login, "wt");
                           fwrite(&player,sizeof(struct character),1,fp); //zapis pliku gracza po przejsciu do menu glownego
                           fclose(fp);
                           if (player.money>=3000) //win
                           {
                                printf("Congratulations! You win having %d$.\nThank you for playing.\n");
                                op=0;
                                break;
                           }
                           else if (player.money<0) //loose
                           {
                                printf("Game over! You have run into debt.\n");
                                op=0;
                                break;
                           }
                           
                           printf("Player: %s    Place: %s    Money: %d$\n", account.login, player.place, player.money);
                           printf("Go to:\n");
                           printf("1. Shop.\n2. Airport.\n0 to return to main menu.\n");
                           scanf("%d",&option);
                           switch(option)
                           {
                                 case 1: //shopping
                                 {
                                      fflush(stdin); //czyœci bufor
                                      system("cls");
                                      fp=fopen("products.txt", "rt");
                                      count=0;
                                      while(!feof(fp)) // end of file sprawdza czy plik sie skonczyl
                                      {
                                            fgets(string, 1000, fp); // pobór z pliku do stringu
                                            sscanf(string, "%s %s %d %d %d", product.place, product.name, &product.no, &product.sell, &product.buy);
                                            // up: pobór produktu ze stringu
                                            if(strcmp(player.place, product.place)==0)
                                            {
                                                 p[count]=product; // zapis do tablicy p
                                                 count++;
                                            }
                                      }
                                      printf("Player: %s    Place: %s    Money: %d$\n", account.login, player.place, player.money);
                                      printf("Store:\n");
                                      for(i=0; i<count; i++) printf("\n%d %s sell_price:%d$ buy_price:%d$\n", p[i].no, p[i].name, p[i].sell, p[i].buy);
                                      // up: wypisuje produkt
                                      printf("Select the product number or 0 to go back.\n");
                                      scanf("%d", &op);
                                      if(op==0) break;
                                      else 
                                      {
                                           for(count=count; count>=0; count--)
                                           {
                                                  if(op==p[count].no) {a=1; b=count;}                                         
                                           }
                                           //kupno
                                           if (a==1)
                                           {
                                                  fflush(stdin);
                                                  if(player.store[op]==0)
                                                  {
                                                        printf("You do not have any of these products.\n");
                                                        printf("Do you want to buy %s?(Y/N)\n", p[b].name, p[b].buy);
                                                        c=getchar();
                                                        if ((c=='N')||(c=='n'));
                                                        else if ((c=='Y')||(c=='y'))
                                                        {
                                                             amount=-1;
                                                             //p[b] tablica z produktem
                                                             while((amount*p[b].buy<0)||((player.money-amount*p[b].buy)<0))
                                                             {
                                                                    printf("How many of these would you like to buy? ");
                                                                    scanf("%d", &amount);
                                                                    if((player.money-amount*p[b].buy)<0) printf("You do not have enough money.\n");
                                                                    else if (amount<0) printf("Only positive numbers enabled.\n");
                                                             }
                                                             player.money=player.money-amount*p[b].buy;
                                                             player.store[op]=player.store[op]+amount; //iloœæ moich produktów
                                                             printf("You have successfully bought %d of %s! -%d$\n", amount, p[b].name, amount*p[b].buy);
                                                        }
                                                  }
                                                  // mam produkt whats next ends: ou have succesfuly sold
                                                  else if(player.store[op]>0)
                                                  {
                                                        printf("You have %d of these products.\n", player.store[op]);
                                                        printf("Do you want to buy or sell %s? Or maybe not?(B/S/N)\n", p[b].name);
                                                        c=getchar();
                                                        if ((c=='N')||(c=='n'));
                                                        else if ((c=='B')||(c=='b'))
                                                        {
                                                             amount=-1;
                                                             while((amount*p[b].buy<0)||((player.money-amount*p[b].buy)<0))
                                                             {
                                                                    printf("How many of these would you like to buy? ");
                                                                    scanf("%d", &amount);
                                                                    if((player.money-amount*p[b].buy)<0) printf("You do not have enough money.\n");
                                                                    else if (amount<0) printf("Only positive numbers enabled.\n");
                                                             }
                                                             player.money=player.money-amount*p[b].buy;
                                                             player.store[op]=player.store[op]+amount;
                                                             printf("You have successfully bought %d of %s! -%d$\n", amount, p[b].name, amount*p[b].buy);
                                                        }
                                                        else if ((c=='S')||(c=='s'))
                                                        {
                                                             amount=-1;
                                                             while((amount>player.store[op])||amount<0)
                                                             {
                                                                    printf("How many of these would you like to sell? ");
                                                                    scanf("%d", &amount);
                                                                    if(amount>player.store[op]) printf("You do not have enough products.\n");
                                                                    else if (amount<0) printf("Only positive numbers enabled.\n");
                                                             }
                                                             player.money=player.money+amount*p[b].sell;
                                                             player.store[op]=player.store[op]-amount;
                                                             printf("You have successfully sold %d of %s! +%d$\n", amount, p[b].name, amount*p[b].sell);
                                                        }
                                                  }
                                           }
                                           else printf("Not a proper number.\n");
                                      } fclose(fp);
                                      system("PAUSE");
                                 } break;
                                 case 2:  //travelling
                                 {
                                      fflush(stdin);
                                      system("cls");
                                      printf("Player: %s    Place: %s    Money: %d$\n", account.login, player.place, player.money);
                                      printf("You are at the airport. Where do you want to travel:\n");
                                      b=0;
                                      for(a=0; a<6; a++)
                                      {
                                               
                                               if(strcmp(placelist[a], player.place)!=0)
                                               {
                                                       sprintf(sort[b], "%s", placelist[a]);
                                                       printf("%d. %s %d$\n", b+1, sort[b], travelcost(player.place, sort[b]));
                                                       b++;
                                               }
                                      }

                                      printf("0 to go back.\n");
                                      scanf("%d", &b);
                                      switch(b)
                                      {
                                               case 0: break;
                                               case 1: case 2: case 3: case 4: case 5:
                                                    {
                                                            printf("OK! Flying from %s to %s for %d$.\n", player.place, sort[b-1],travelcost(player.place, sort[b-1]));
                                                            player.money=player.money-travelcost(player.place, sort[b-1])-dangers(account.login);;
                                                            sprintf(player.place, "%s", sort[b-1]);
                                                            system("PAUSE");
                                                    } break;
                                               default: {printf("Oops! Wrong option!\n"); system("PAUSE");} break;
                                      }
                                 } break;
                                 case 0: break;
                                 default: {printf("Oops! wrong option, try one more time.\n"); system("PAUSE");} break;
                           }
                      }
                      while (option!=0);
                      system("PAUSE");
                  }
                  else {printf("Oops! That's the wrong login or password.\n"); system("PAUSE");}
                  fclose(f);
             } break;
             case 2:                 //creating new account
             {
                  system("cls");
                  printf("Enter your name (which will be your login also): ");
                  scanf("%s", account.login);
                  printf("Enter your password: ");
                  scanf("%s", account.pass);
                  f=fopen("accounts", "ab+"); // tworzenie nowego konta lub otwieranie go z pliku
                  fwrite(&account,sizeof(struct acc),1,f);
                  fclose(f);
                  do
                  {
                      printf("Select the difficulty: 1. Easy, 2. Medium, 3. Hard: ");
                      scanf("%d", &option);
                      switch(option)
                      {
                          case 1: {player.dif=1; player.money=2000;} break;
                          case 2: {player.dif=2; player.money=1000;} break;
                          case 3: {player.dif=3; player.money=200;} break;
                          default: {printf("Error! Wrong difficulty number.\n"); option=0;} break;
                       }
                  }
                  while(option==0);
                  f=fopen(account.login, "a+");
                  sprintf(player.place, "Macy's"); // zapis do struktury
                  for(count=0; count<20; count++) player.store[count]=0;// zeruje produkty na start
                  fwrite(&player,sizeof(struct character),1,f); // zapis gracza do pliku
                  fclose(f);
             } break;
             default:
             {
                     printf("This is not a proper function.\n");
                     system("PAUSE");
             }
      }
  } while(op!=3); //quit
  return 0;
}
//niebezpieczeñstwa
int dangers(char name[])
{
    FILE *f;
    int r;
    struct character player;
    srand(time(0));
    f=fopen(name, "rt");
    fread(&player, sizeof(struct character), 1, f);
    fclose(f);
    f=fopen(name, "wt");
    if(player.dif==1) r=0;
    else if(player.dif==2) r=rand()%10+1; // zakres losowania od 1 do 10
    else if(player.dif==3) r=rand()%4+1;
    switch(r)
    {
        case 0: return 0; break;
        case 1:
        {
             printf("A pickpocket stoles your wallet. -100$\n");
             return 100;
        } break;
        case 2:
        {
             printf("Your flight had a delay. -50$\n");
             return 50;
        } break;
        case 3:
        {
             printf("You broke a heel. -50$\n");
             return 50;
        } break;
        case 4:
        {
             printf("A shoe was fake. -100$\n");
             return 100;
        } break;
        case 5: case 6: case 7: case 8: case 9: case 10: 
        {
             printf("You were lucky. Nothing bad had happened.\n");
             return 0;
        } break;
        default: 
        {
                 printf("\nERROR\nCongratulations! You have broken a program...\n");
                 return 0;
        } break;
    }
    fwrite(&player,sizeof(struct character),1,f);
    fclose(f);
}

int travelcost(char place1[], char place2[])
{
    int i,a,b;
    char place[][30]={"Macy's", "5th_Avenue", "Galeries_Lafayette", "The_Dubai_Mall", "Galleria_Vittorio_Emanuele_II", "Harrods"};
    for(i=0; i<6; i++) if (strcmp(place1, place[i])==0) break;
    a=i;
    for(i=0; i<6; i++) if (strcmp(place2, place[i])==0) break;
    b=i;
    if(a==0) switch(b)
             {
                      case 1: return 1; break;
                      case 2: return 4; break;
                      case 3: return 4; break;
                      case 4: return 4; break;
                      case 5: return 5; break;
             }
    else if(a==1) switch(b)
             {
                      case 0: return 1; break;
                      case 2: return 3; break;
                      case 3: return 4; break;
                      case 4: return 2; break;
                      case 5: return 5; break;
             }
    else if(a==2) switch(b)
             {
                      case 0: return 4; break;
                      case 1: return 3; break;
                      case 3: return 2; break;
                      case 4: return 2; break;
                      case 5: return 3; break;
             }
    else if(a==3) switch(b)
             {
                      case 0: return 4; break;
                      case 1: return 4; break;
                      case 2: return 2; break;
                      case 4: return 3; break;
                      case 5: return 4; break;
             }
    else if(a==4) switch(b)
             {
                      case 0: return 4; break;
                      case 1: return 2; break;
                      case 2: return 2; break;
                      case 3: return 3; break;
                      case 5: return 3; break;
             }
    else if(a==5) switch(b)
             {
                      case 0: return 5; break;
                      case 1: return 5; break;
                      case 2: return 3; break;
                      case 3: return 4; break;
                      case 4: return 3; break;
             }
}
