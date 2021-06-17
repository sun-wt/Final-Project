#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define RED    "\x1b[;31;1m"
#define GREEN  "\x1b[;32;5m"
#define YELLOW "\x1b[;33;5m"
#define BLUE   "\x1b[;34;5m"
#define PURPLE "\x1b[;35;5m"
#define B_G    "\x1b[;36;5m"
#define NC     "\033[0m"

#define range(x) ( x >= 0 && x <= 109 )
#define special(x) ( x >= 42 && x <= 109 ) 

#define one(x) ( x >= 0 && x <= 9 )
#define two(x) ( x >= 10 && x <= 17 )
#define three(x) ( x >= 18 && x <= 25 )
#define four(x) ( x >= 26 && x <= 33 )
#define five(x) ( x >= 34 && x <= 41 )

#define Tower(x) ( x >= 42 && x <= 44 ) //1
#define Chapel(x) ( x >= 45 && x <= 47 ) //2
#define Smithy(x) ( x >= 48 && x <= 50 ) //3
#define Poor_house(x) ( x >= 51 && x <= 53 ) //4
#define Black_market(x) ( x >= 54 && x <= 56 ) //5
#define Crane(x) ( x >= 57 && x <= 59 ) //6
#define Carpenter(x) ( x >= 60 && x <= 62 ) //7
#define Quarry(x) ( x >= 63 && x <= 65 ) //8
#define Well(x) ( x >= 66 && x <= 68 ) //9
#define Aqueduct(x) ( x >= 69 && x <= 71 ) //10
#define Market_stand(x) ( x >= 72 && x <= 74 ) //11
#define Market_hall(x) ( x >= 75 && x <= 77 ) //12
#define Trading_post(x) ( x >= 78 && x <= 80 ) //13
#define Archive(x) ( x >= 81 && x <= 83 ) //14
#define Prefecture(x) ( x >= 84 && x <= 86 ) //15
#define Gold_mine(x) ( x >= 87 && x <= 89 ) //16
#define Library(x) ( x >= 90 && x <= 92 ) //17
#define Statue(x) ( x >= 93 && x <= 95 ) //18
#define Victory_column(x) ( x >= 96 && x <= 98 ) //19
#define Hero(x) ( x >= 99 && x <= 101 ) //20
#define Guild_hall(x) ( x >= 102 && x <= 103 ) //21
#define City_hall(x) ( x >= 104 && x <= 105 ) //22
#define Triumhal_arch(x) ( x >= 106 && x <= 107 ) //23
#define Palace(x) ( x >= 108 && x <= 109 ) //24


char *RP = "Role playcards: ";
int32_t Card[500]; //牌庫
int32_t Card_position = 0;
int32_t Card_Dispose[110]; //棄牌
int32_t Card_D_p = 0;

int32_t Card_Point[110] = 
{ 
    1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,2,2,
    2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,3,3,3,3,3,3,
    3,3,2,2,2,2,2,2,1,1,
    1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,1,1,1,2,
    2,2,1,1,1,2,2,2,1,1,
    1,1,1,1,2,2,2,1,1,1,
    3,3,3,3,3,3,4,4,4,5,
    5,5,0,0,0,0,0,0,0,0
};

int32_t Card_Paid[110] = 
{ 
    1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,3,3,
    3,3,3,3,3,3,4,4,4,4,
    4,4,4,4,5,5,5,5,5,5,
    5,5,3,3,3,3,3,3,1,1,
    1,2,2,2,2,2,2,2,2,2,
    3,3,3,4,4,4,2,2,2,3,
    3,3,2,2,2,4,4,4,2,2,
    2,1,1,1,3,3,3,1,1,1,
    5,5,5,3,3,3,4,4,4,5,
    5,5,6,6,6,6,6,6,6,6
};

int32_t Product_Paid[5][5] =
{
    {1,2,2,3,3},
    {1,1,2,2,3},
    {1,2,2,2,3},
    {1,1,1,2,2},
    {1,1,2,2,2}
};

typedef struct player
{
    char name[4];
    char role[15];
    int32_t handcard[12];
    int32_t handcard_number;
    int32_t table[12];
    int32_t table_number;
    int32_t type[12];
    int32_t product[12]; // 貨品內容
    int32_t product_position[12]; //貨品位置
    int32_t product_number;
    int32_t limit;
    int32_t sell;
    int32_t n;
    int32_t church[50];
    int32_t church_number;
    int32_t memory[5];
    int32_t memory_number;
    bool difficulty;
    int32_t score;
} Player;
Player ip[4] = {0};

bool state[5]; //可否使用
bool state_g[4]; //總督位


char role_name[5][15] =
{
    {"Builder"},
    {"Producer"},
    {"Trader"},
    {"Councillor"},
    {"Prospector"}
};

char role_introduce_1[5][30] = 
{
    {"Build a building.\n"},
    {"Produce a product.\n"},
    {"Produce a product.\n"},
    {"Pick 2 choose 1.\n"},
    {"Pick a card.\n"}
};

char role_introduce_2[5][30] = 
{
    {"Build 2 building.\n"},
    {"Produce 2 product.\n"},
    {"Produce 2 product.\n"},
    {"Pick 5 choose 1.\n"},
    {"Pick 2 card.\n"}
};

void Type( Player *ip ) //桌上牌的類型
{
    for(int i=0;i<12;i++)
    {
        ip->type[i] = 0;
    }
    for(int i=0;i<ip->table_number;i++)
    {
        if( Tower(ip->table[i]) )
        {
            ip->type[i] = 1;
        }
        if( Chapel(ip->table[i]) )
        {
            ip->type[i] = 2;
        }
        if( Smithy(ip->table[i]) )
        {
            ip->type[i] = 3;
        }
        if( Poor_house(ip->table[i]) )
        {
            ip->type[i] = 4;
        }
        if( Black_market(ip->table[i]) )
        {
            ip->type[i] = 5;
        }
        if( Crane(ip->table[i]) )
        {
            ip->type[i] = 6;
        }
        if( Carpenter(ip->table[i]) )
        {
            ip->type[i] = 7;
        }
        if( Quarry(ip->table[i]) )
        {
            ip->type[i] = 8;
        }
        if( Well(ip->table[i]) )
        {
            ip->type[i] = 9;
        }
        if( Aqueduct(ip->table[i]) )
        {
            ip->type[i] = 10;
        }
        if( Market_stand(ip->table[i]) )
        {
            ip->type[i] = 11;
        }
        if( Market_hall(ip->table[i]) )
        {
            ip->type[i] = 12;
        }
        if( Trading_post(ip->table[i]) )
        {
            ip->type[i] = 13;
        }
        if( Archive(ip->table[i]) )
        {
            ip->type[i] = 14;
        }
        if( Prefecture(ip->table[i]) )
        {
            ip->type[i] = 15;
        }
        if( Gold_mine(ip->table[i]) )
        {
            ip->type[i] = 16;
        }
        if( Library(ip->table[i]) )
        {
            ip->type[i] = 17;
        }
        if( Statue(ip->table[i]) )
        {
            ip->type[i] = 18;
        }
        if( Victory_column(ip->table[i]) )
        {
            ip->type[i] = 19;
        }
        if( Hero(ip->table[i]) )
        {
            ip->type[i] = 20;
        }
        if( Guild_hall(ip->table[i]) )
        {
            ip->type[i] = 21;
        }
        if( City_hall(ip->table[i]) )
        {
            ip->type[i] = 22;
        }
        if( Triumhal_arch(ip->table[i]) )
        {
            ip->type[i] = 23;
        }
        if( Palace(ip->table[i]) )
        {
            ip->type[i] = 24;
        }
    }
}

int32_t Check_type( int32_t x ) //確認單張牌的類型
{
    if( Tower(x) )
    {
        return 1;
    } 
    if( Chapel(x) )
    {
        return 2;
    }
    if( Smithy(x) )
    {
        return 3;
    }
    if( Poor_house(x) )
    {
        return 4;
    }
    if( Black_market(x) )
    {
        return 5;
    }
    if( Crane(x) )
    {
        return 6;
    }
    if( Carpenter(x) )
    {
        return 7;
    }
    if( Quarry(x) )
    {
        return 8;
    }
    if( Well(x) )
    {
        return 9;
    }
    if( Aqueduct(x) )
    {
        return 10;
    }
    if( Market_stand(x) )
    {
        return 11;
    }
    if( Market_hall(x) )
    {
        return 12;
    }
    if( Trading_post(x) )
    {
        return 13;
    }
    if( Archive(x) )
    {
        return 14;
    }
    if( Prefecture(x) )
    {
        return 15;
    }
    if( Gold_mine(x) )
    {
        return 16;
    }
    if( Library(x) )
    {
        return 17;
    }
    if( Statue(x) )
    {
        return 18;
    }
    if( Victory_column(x) )
    {
        return 19;
    }
    if( Hero(x) )
    {
        return 20;
    }
    if( Guild_hall(x) )
    {
        return 21;
    }
    if( City_hall(x) )
    {
        return 22;
    }
    if( Triumhal_arch(x) )
    {
        return 23;
    }
    if( Palace(x) )
    {
        return 24;
    }
    return 0;
}

int32_t Check_input( bool condition )
{
    int32_t input = 0;
    if( scanf( "%d" , & input ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    else if( condition ) //只能輸入 0 || 1
    {
        if( input != 0 || input != 1 )
        {
            printf( "You input a wrong option.\n" );
            return -1;
        }
    }
    return input;
}

void Introduce()
{
    printf( YELLOW"************************ SAN JUAN ************************\n"NC );
    printf( "Governor first ( Swap clockwise every round )\n" );
    printf( "Everyone will have 4 handcards in the begining.\n" );
    printf( "And limit of handcards is 7 ( In normal condition )\n" );
    printf( "If the number of your handcards exceed the limit,\n" );
    printf( "you have to give up one of them.\n" );
    printf( "There are 5 role card :\n" );
    for(int i=0;i<5;i++)
    {
        printf( "%d. %s\n" , i+1 , role_name[i] );
        printf( "$ normal    : %s" , role_introduce_1[i] );
        printf( "$ privilege : %s\n" , role_introduce_2[i] );
    }
    printf( "Decide whether to behave or exercise privilege\n" );
    printf( "( If you choose that role card )\n\n" );
    printf( "The game will end when 12 buildings are builded by anyone.\n" );
    printf( "Difficulty : \n" );
    printf( "[1]Normal [2]Forgetful\n" );
    printf( "Version Action : \n" );
    printf( "[1]Normal [2]Shorter\n" );
    printf("If you don't have so much time.\nYou can choose a shorter version.(8 cards end the game).\n" );
    printf( "Special Action : \n" );
    printf( "If you are so lazy to pick by yourself.\n" );
    printf( "Computer can choose for you.(Only for Builder's turn)\n" );
    printf( RED"Waring!Please do not enter any string or char.\n" );
    printf( "Because the game will be ended!\n"NC );
    printf( YELLOW"************************ SAN JUAN ************************\n"NC ); 
    printf( "\n" );   
}

void Shuffle( int32_t number )
{
    srand( time(NULL) );
    for(int i=0;i<number;i++)
    {
        Card[i] = i; // 一開始的染坊
        ip[i].limit = 7; //每個人的手牌數
        ip[i].sell = 0;
        ip[i].n = i;
        ip[i].product_number = 0;
        ip[i].church_number = 0;
        ip[i].memory_number = 0;
        ip[i].score = 0;
    }
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<12;j++)
        {
            ip[i].product_position[j] = -1;
        }
    }
    for(int i=number;i<110;i++)
    {
        Card[i] = (rand()%(110-number)) + number;
        for(int j=number;j<i;j++)
        {
            while( Card[i] == Card[j] )
            {
                i--;
                break;
            }
        }
        //printf("%d \n" ,Card[i]);
    }
    for(int i=0;i<number;i++)
    {
        memset( ip[i].handcard , 0 , 12 );
        for(int j=0;j<4;j++)
        {
            ip[i].handcard[j] = Card[i*4+j+number]; //發手牌
            ip[i].handcard_number = j + 1;
            //printf( "%3d " , ip[i].handcard[j] );
            Card_position++;
        }
        ip[i].table[0] = i;
        ip[i].table_number = 1;
        //printf( "\n" );
        Type( &ip[i] );
    }
    Card_position += number;
}

int32_t Shuffle_again( int32_t last )
{
    int32_t ramdom[Card_D_p];
    int temp = 0;
    for(int i=0;i<Card_D_p;i++)
    {
        ramdom[i] = (rand()%Card_D_p) + 0;
        for(int j=0;j<i;j++)
        {
            if( ramdom[i] == ramdom[j] )
            {
                i--;
                break;
            }
        }
    }
    for(int i=0;i<Card_D_p;i++)
    {
        Card[last] = Card_Dispose[ramdom[i]];
        Card_Dispose[ramdom[i]] = 0;
        last++;
    }
    Card_D_p = 0;
    return last;
}

void Print_surface( const Player ip[4] )
{
    printf( "\n" );
    for(int i=0;i<30;i++)
    {
        printf( "- " );
    }
    printf( "\n" );
    printf( "Player: %s" , ip[0].name );
    for(int i=0;i<=18;i++)
    {
        printf( " " );
    }
    printf("|");
    printf( "Player: " );
    for(int i=0;i<4;i++)
    {
        printf( "%c" , ip[1].name[i] );
    }
    printf( "\n" );
    //上第一行^
    if( strlen(ip[0].role) == 0 )
    {
        printf( "%s" , RP );
        for(int i=0;i<30-16;i++)
        {
            printf( " " );
        }
    }
    else
    {
        printf( "%s %s" , RP , ip[0].role );
        for(int i=0;i<30-16-strlen(ip[0].role)-1;i++)
        {
            printf( " " );
        }
    }
    printf("|");
    if( strlen(ip[2].role) == 0 )
    {
        printf( "%s\n" , RP );
    }
    else
    {
        printf( "%s %s\n" , RP , ip[1].role );
    }
    //上第二行^
    printf( "Governor: %d" , state_g[0] );
    for(int i=0;i<30-11;i++)
    {
        printf( " " );
    }
    printf("|");
    printf( "Governor: %d" , state_g[1] );
    printf( "\n" );
    //上第三行^
    printf( "Handcards: " );
    for(int i=0;i<30- strlen("Handcards: ");i++)
    {
        printf( " " );
    }
    printf("|");
    printf( "Handcards: " );
    printf( "\n" );
    //上第四行
    for(int i=0;i<6;i++)
    {
        if( ip[0].handcard[i] != 0 )
        {
            printf( "%03d " , ip[0].handcard[i]);
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "      " );
    printf("|");
    for(int i=0;i<6;i++)
    {
        if( ip[1].handcard[i] != 0 )
        {
            printf( "%03d " , ip[1].handcard[i]);
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "\n" );
    //上第五行
    for(int i=6;i<12;i++)
    {
        if( ip[0].handcard[i] != 0 )
        {
            printf( "%03d " , ip[0].handcard[i]);
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "      " );
    printf("|");
    for(int i=6;i<12;i++)
    {
        if( ip[1].handcard[i] != 0 )
        {
            printf( "%03d " , ip[1].handcard[i]);
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "\n" );
    //上第六行
    printf( "Table: " );
    for(int i=0;i<30- strlen("Table: ");i++)
    {
        printf( " " );
    }
    printf("|");
    printf( "Table: " );
    printf( "\n" );
    //上第七行
    printf( "%03d " , ip[0].table[0] );
    for(int i=1;i<6;i++)
    {
        if( ip[0].table[i] != 0 )
        {
            printf( "%03d " , ip[0].table[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "      " );
    printf("|");
    for(int i=0;i<6;i++)
    {
        if( ip[1].table[i] != 0 )
        {
            printf( "%03d " , ip[1].table[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "\n" );
    //上第八行
    for(int i=6;i<12;i++)
    {
        if( ip[0].table[i] != 0 )
        {
            printf( "%03d " , ip[0].table[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "      " );
    printf("|");
    for(int i=6;i<12;i++)
    {
        if( ip[1].table[i] != 0 )
        {
            printf( "%03d " , ip[1].table[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "\n" );
    //上第九行
    printf( "Product position: " );
    for(int i=0;i<30- strlen("Product position: ");i++)
    {
        printf( " " );
    }
    printf("|");
    printf( "Product position: " );
    printf( "\n" );
    //上第十行
    for(int i=0;i<6;i++)
    {
        if( ip[0].product_position[i] != -1 )
        {
            printf( "%03d " , ip[0].product_position[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "      " );
    printf("|");
    for(int i=0;i<6;i++)
    {
        if( ip[1].product_position[i] != -1 )
        {
            printf( "%03d " , ip[1].product_position[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "\n" );
    //上第十一行
    for(int i=6;i<12;i++)
    {
        if( ip[0].product_position[i] != -1 )
        {
            printf( "%03d " , ip[0].product_position[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "      " );
    printf("|");
    for(int i=6;i<12;i++)
    {
        if( ip[1].product_position[i] != -1 )
        {
            printf( "%03d " , ip[1].product_position[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "\n" );
    //上第十二行
    printf( "Under the Chapel: %03d" , ip[0].church_number );
    for(int i=0;i<30- strlen("Under the Chapel: ")-3;i++)
    {
        printf( " " );
    }
    printf("|");
    printf( "Under the Chapel: %03d" , ip[1].church_number );
    printf( "\n" );
    //上第十三行
    for(int i=0;i<30;i++)
    {
        printf( "- " );
    }
    printf("\n");
    //分隔線^
    for(int i=0;i<5;i++)
    {
        if( state[i] == 1 )
        {
            printf( "%s " , role_name[i] );
        }
        else
        {
            for(int i=0;i<=strlen(role_name[i]);i++)
            {
                printf( " " );
            }
        }
    }
    printf("\n");
    //中間^
    for(int i=0;i<30;i++)
    {
        printf("- ");
    }
    printf("\n");
    //分隔線^
    printf( "Player: " );
    for(int i=0;i<4;i++)
    {
        printf( "%c" , ip[2].name[i] );
    }
    for(int i=0;i<18;i++)
    {
        printf( " " );
    }
    printf("|");
    printf( "Player: " );
    for(int i=0;i<4;i++)
    {
        printf( "%c" , ip[3].name[i] );
    }
    printf( "\n" );
    //下第一行^
    if( strlen(ip[2].role) == 0 )
    {
        printf( "%s" , RP );
        for(int i=0;i<30-16;i++)
        {
            printf( " " );
        }
    }
    else
    {
        printf( "%s %s" , RP , ip[2].role );
        for(int i=0;i<30-16-strlen(ip[2].role)-1;i++)
        {
            printf( " " );
        }
    }
    printf("|");
    if( strlen(ip[3].role) == 0 )
    {
        printf( "%s\n" , RP );
    }
    else
    {
        printf( "%s %s\n" , RP , ip[3].role );
    }
    //下第二行^
    printf( "Governor: %d" , state_g[2] );
    for(int i=0;i<30-11;i++)
    {
        printf( " " );
    }
    printf("|");
    printf( "Governor: %d\n" , state_g[3] );
    //下第三行^
    printf( "Handcards: " );
    for(int i=0;i<30- strlen("Handcards: ");i++)
    {
        printf( " " );
    }
    printf("|");
    printf( "Handcards: " );
    printf( "\n" );
    //下第四行
    for(int i=0;i<6;i++)
    {
        if( ip[2].handcard[i] != 0 )
        {
            printf( "%03d " , ip[2].handcard[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "      " );
    printf("|");
    for(int i=0;i<6;i++)
    {
        if( ip[3].handcard[i] != 0 )
        {
            printf( "%03d " , ip[3].handcard[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "\n" );
    //下第五行
    for(int i=6;i<12;i++)
    {
        if( ip[2].handcard[i] != 0 )
        {
            printf( "%03d " , ip[2].handcard[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "      " );
    printf("|");
    for(int i=6;i<12;i++)
    {
        if( ip[3].handcard[i] != 0 )
        {
            printf( "%03d " , ip[3].handcard[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "\n" );
    //下第六行
    printf( "Table: " );
    for(int i=0;i<30- strlen("Table: ");i++)
    {
        printf( " " );
    }
    printf("|");
    printf( "Table: " );
    printf( "\n" );
    //下第七行
    for(int i=0;i<6;i++)
    {
        if( ip[2].table[i] != 0 )
        {
            printf( "%03d " , ip[2].table[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "      " );
    printf("|");
    for(int i=0;i<6;i++)
    {
        if( ip[3].table[i] != 0 )
        {
            printf( "%03d " , ip[3].table[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "\n" );
    //下第八行
    for(int i=6;i<12;i++)
    {
        if( ip[2].table[i] != 0 )
        {
            printf( "%03d " , ip[2].table[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "      " );
    printf("|");
    for(int i=6;i<12;i++)
    {
        if( ip[3].table[i] != 0 )
        {
            printf( "%03d " , ip[3].table[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "\n" );
    //下第九行
    printf( "Product position: " );
    for(int i=0;i<30- strlen("Product position: ");i++)
    {
        printf( " " );
    }
    printf("|");
    printf( "Product position: " );
    printf( "\n" );
    //下第十行
    for(int i=0;i<6;i++)
    {
        if( ip[2].product_position[i] != -1 )
        {
            printf( "%03d " , ip[2].product_position[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "      " );
    printf("|");
    for(int i=0;i<6;i++)
    {
        if( ip[3].product_position[i] != -1 )
        {
            printf( "%03d " , ip[3].product_position[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "\n" );
    //下第十一行
    for(int i=6;i<12;i++)
    {
        if( ip[2].product_position[i] != -1 )
        {
            printf( "%03d " , ip[2].product_position[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "      " );
    printf("|");
    for(int i=6;i<12;i++)
    {
        if( ip[3].product_position[i] !=-1 )
        {
            printf( "%03d " , ip[3].product_position[i] );
        }
        else
        {
            printf( "    " );
        }
    }
    printf( "\n" );
    //下第十二行
    printf( "Under the Chapel: %03d" , ip[2].church_number );
    for(int i=0;i<30- strlen("Under the Chapel: ")-3;i++)
    {
        printf( " " );
    }
    printf("|");
    printf( "Under the Chapel: %03d" , ip[3].church_number );
    printf( "\n" );
    //下第十三行
    for(int i=0;i<30;i++)
    {
        printf( "- " );
    }
    printf( "\n" );
    //分隔線^
    printf("\n\n\n");
}

void Name( int32_t number )
{
    strncpy( ip[0].name , "You" , 3 );
    if( number == 2 )
    {
        strncpy( ip[1].name , "Com1" , 4 );
        strncpy( ip[2].name , "None" , 4 );
        strncpy( ip[3].name , "None" , 4 );
    }
    else if( number == 3 )
    {
        strncpy( ip[1].name , "Com1" , 4 );
        strncpy( ip[2].name , "Com2" , 4 );
        strncpy( ip[3].name , "None" , 4 );
    }
    else if( number == 4 )
    {
        strncpy( ip[1].name , "Com1" , 4 );
        strncpy( ip[2].name , "Com2" , 4 );
        strncpy( ip[3].name , "Com3" , 4 );
    }
    else
    {
        printf("wrong!\n");
    }
}

void Q()
{
    int32_t num;
    printf( "If you need what the number mean (Y:1,N:0): " );
    if( scanf( "%d" , &num ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    while( num < 0 )
    {
        if( scanf( "%d" , &num ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
    }
    if( num == 1 )
    {
        FILE *fp = fopen( "table.txt" , "r" );
        fseek( fp , 0 , SEEK_END );
        int32_t size = ftell( fp );
        fseek( fp , 0 , SEEK_SET );
        char temp[size];
        fread( temp , size , 1 , fp );
        printf( "%s\n" , temp );
    }
    printf( "Check for cost (Y:1,N:0): " );
    if( scanf( "%d" , &num ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    if( num == 1 )
    {
        printf( "Which card do you want to know (0~110): " );
        if( scanf( "%d" , &num ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
        if( range(num) == 1 )
        {
            printf( "%d: %d\n" , num , Card_Paid[num] );
        }
        else
        {
            printf( "Wrong input.\n" );
        }
    }
    printf( "Check for point (Yes:1,No:0): " );
    if( scanf( "%d" , &num ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    if( num == 1 )
    {
        printf( "Which card do you want to know (0~110): " );
        if( scanf( "%d" , &num ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
        if( range(num) == 1 )
        {
            printf( "%d: %d\n" , num , Card_Point[num] );
        }
        else
        {
            printf( "Wrong input.\n" );
        }
    }
    printf( "If you need what functions of special building (Y:1,N:0): " );
    if( scanf( "%d" , &num ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    if( num == 1 )
    {
        FILE *fp = fopen( "function.txt" , "r" );
        fseek( fp , 0 , SEEK_END );
        int32_t size = ftell( fp );
        fseek( fp , 0 , SEEK_SET );
        char temp[size];
        fread( temp , size , 1 , fp );
        printf( "%s\n" , temp );
    }
}

void Change( Player *ip , int32_t count , int32_t option , bool flag )
{
    //option = 0 -> handcard
    //option = 1 -> product product_position
    //option = 2 -> memory
    if( option == 0 )
    {
        if( flag )
        {
            Card_Dispose[Card_D_p] = ip->handcard[count];
            Card_D_p++;
        }
        for(int i=count;i<ip->handcard_number;i++)
        {
            ip->handcard[i] = ip->handcard[i+1];
        }  
        ip->handcard[ip->handcard_number] = 0;
        ip->handcard_number--;
    }
    else if( option == 1 )
    {
        if( flag )
        {
            Card_Dispose[Card_D_p] = ip->product[count];
            Card_D_p++;
        }
        for(int i=count;i<ip->product_number;i++)
        {
            ip->product[i] = ip->product[i+1];
            ip->product_position[i] = ip->product_position[i+1];
        }
        ip->product[ip->product_number] = 0;
        ip->product_position[ip->product_number] = -1;
        ip->product_number--;
    }
    else if( option == 2 )
    {
        for(int i=count;i<ip->memory_number;i++)
        {
            ip->memory[i] = ip->memory[i+1];
        }
        ip->memory[ip->memory_number] = 0;
        ip->memory_number--;
    }
    Type( ip );
}

int32_t normal_build( Player *ip )
{
    bool option[ip->table_number];
    bool s1 = 0;
    int32_t sure = 0;
    int32_t wanted = 0;
    int32_t need = 0;
    int32_t target = 0;
    if( ip->handcard_number == 0 )
    {
        printf( "Oops! You don't have any handcards\n" );
        printf( "Please wait for next turn.\n" );
        return 0;
    }
    printf( "-------------------------\n" );
    printf( "Your card type codename :\n" );
    printf( "Handcards : " );
    for(int i=0;i<ip->handcard_number;i++)
    {
        printf( "%d " , Check_type(ip->handcard[i]) );
    }
    printf( "\nTable : " );
    for(int i=0;i<ip->table_number;i++)
    {
        printf( "%d " , ip->type[i] );
    }
    printf( "\n-------------------------\n" );
    printf( "Paid :\n" );
    printf( "Handcards : " );
    for(int i=0;i<ip->handcard_number;i++)
    {
        printf( "%d " , Card_Paid[ip->handcard[i]] );
    }
    printf( "\n-------------------------\n" );
    printf( "Which do you want to build from your hand card :\n" );
    for(int i=0;i<ip->handcard_number;i++)
    {
        option[i] = 1;
        for(int j=0;j<ip->table_number;j++)
        {
            if( Check_type(ip->handcard[i]) == ip->type[j] )
            {
                if( ip->type[j] )
                {
                    option[i] = 0;
                    break;
                }
            }
        }
        if( option[i] == 1 )
        {
            printf( "[%d]%d " , i , ip->handcard[i] );
        }
    }
    printf( "\n--> " ); //可以出的手牌
    if( scanf( "%d" , &wanted ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    printf( "For sure (Y:1,N:0) ? " );
    if( scanf( "%d" , &sure ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    while( sure != 1 )
    {
        for(int i=0;i<ip->handcard_number;i++)
        {
            if( option[i] == 1 )
            {
                printf( "[%d]%d " , i , ip->handcard[i] );
            }
        }
        printf( "\n--> " );
        if( scanf( "%d" , &wanted ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
        printf( "For sure (Y:1,N:0) ? " );
        if( scanf( "%d" , &sure ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
    }
    if( wanted >= 0 && wanted < ip->handcard_number )
    {
        if( option[wanted] )
        {
            target = ip->handcard[wanted];
            need = Card_Paid[target];
            s1 = 1;
        }
        else //不可以蓋的地方
        {
            printf( "Wrong instruction.\n" );
            printf( "Please wait for next turn.\n" );
            return 0;
        }
    }
    else //不在範圍
    {
        printf( "Wrong instruction.\n" );
        printf( "Please wait for next turn.\n" );
        return 0;
    }
    if( s1 )
    {
        bool black =0;
        for(int i=0;i<ip->table_number;i++)
        {
            if( ip->type[i] == 3 )
            {
                printf( PURPLE"Due to Smithy :\n"NC );
                printf( "You can paid less(-1)\n" );
                need--;
            }
            if( ip->type[i] == 8 )
            {
                if( special(target) )
                {
                    printf( PURPLE"Due to Quarry :\n"NC );
                    printf( "Because you build a special building.\n" );
                    printf( "You can paid less(-1)\n" );
                    need--;
                }
            }
            if( ip->type[i] == 5 )
            {
                printf( PURPLE"Due to Black_market :\n"NC );
                printf( "You can use product as handcard.\n" );
                black = 1;
            }
            if( ip->type[i] == 17 )
            {
                printf( PURPLE"Due to Library :\n"NC );
                printf( "You can paid less(-2)\n" );
                need -= 2;
            }
        }
        if( need <= 0 )
        {
            printf( "You get a free building.\n" );
            Change( ip , wanted , 0 , 0 );
            ip->table[ip->table_number] = target;
            ip->table_number++;
            need = 0;
            return 0;
        }
        printf( "You need %d cards.\n" , need );
        if( black== 1 && need >0 )
        {
            if( need < ip->handcard_number + ip->product_number )
            {
                Change( ip , wanted , 0 , 0 );
                printf( "Dispose :\n" );
                int32_t count = 0;
                int32_t sure_1 = 0;
                while( need != 0 )
                {
                    if( count < ip->handcard_number )
                    {
                        printf( "Card%d(Yes:1,No:0) :" , ip->handcard[count] );
                    } 
                    else 
                    {
                        printf( "Product of card%d(Yes:1,No:0) :" , ip->product_position[count] );
                    }
                    if( scanf( "%d" , &sure_1 ) == 0 )
                    {
                        printf( "You input a string or a char.\n" );
                        printf( "Game Over.\n" );
                        exit(0);
                    }
                    if( sure == 1 && count < ip->handcard_number )
                    {
                        Change( ip , count , 0 , 1 );
                        need--;
                        count--;
                    }
                    else if( sure == 1 && count >= ip->handcard_number )
                    {
                        Change( ip , count - ip->handcard_number , 1 , 1 );
                        need--;
                        count--;
                    }
                    if( count == ip->handcard_number + ip->product_number )
                    {
                        count = 0;
                    }
                    count++;
                }
                ip->table[ip->table_number] = target;
                ip->table_number++;
                for(int i=0;i<ip->table_number;i++)
                {
                    if( ip->type[i] == 4 )
                    {
                        if( ip->handcard_number <= 1 )
                        {
                            printf( PURPLE"Due to Poor_house :\n"NC );
                            printf( "Because your handcard <= one.\n" );
                            printf( "You can add a handcard.\n" );
                            ip->handcard[ip->handcard_number] = Card[Card_position];
                            ip->handcard_number++;
                            Card_position++;
                        }
                    }
                    if( ip->type[i] == 7 )
                    {
                        if( special(target) )
                        {
                            printf( PURPLE"Due to Carpenter :\n"NC );
                            printf( "Because you build a special building.\n" );
                            printf( "You can add a handcard.\n" );
                            ip->handcard[ip->handcard_number] = Card[Card_position];
                            ip->handcard_number++;
                            Card_position++;
                        }
                    }
                }
            }
            else
            {
                printf( "Oops!You don't have enough money.(%d)\n\n" , need );
                return 0;
            }
        }
        else if( need > 0 )
        {
            if( need < ip->handcard_number )
            {
                Change( ip , wanted , 0 , 0 );
                printf( "Dispose :\n" );
                int32_t count = 0;
                int32_t sure_1 = 0;
                while( need != 0 )
                {
                    printf( "Card%d(Yes:1,No:0) :" , ip->handcard[count] ); 
                    if( scanf( "%d" , &sure_1 ) == 0 )
                    {
                        printf( "You input a string or a char.\n" );
                        printf( "Game Over.\n" );
                        exit(0);
                    }
                    if( sure_1 == 1 )
                    {
                        Change( ip , count , 0 , 1 );
                        need--;
                    }
                    else
                    {
                        count++;
                    }
                    if( count == ip->handcard_number)
                    {
                        count = 0;
                    }
                }
                ip->table[ip->table_number] = target;
                ip->table_number++;
                for(int i=0;i<ip->table_number;i++)
                {
                    if( ip->type[i] == 4 )
                    {
                        if( ip->handcard_number <= 1 )
                        {
                            printf( PURPLE"Due to Poor_house :\n"NC );
                            printf( "Because your handcard <= one.\n" );
                            printf( "You can add a handcard.\n" );
                            ip->handcard[ip->handcard_number] = Card[Card_position];
                            ip->handcard_number++;
                            Card_position++;
                        }
                    }
                    if( ip->type[i] == 7 )
                    {
                        if( special(target) )
                        {
                            printf( PURPLE"Due to Carpenter :\n"NC );
                            printf( "Because you build a special building.\n" );
                            printf( "You can add a handcard.\n" );
                            ip->handcard[ip->handcard_number] = Card[Card_position];
                            ip->handcard_number++;
                            Card_position++;
                        }
                    }
                }
            }
            else
            {
                printf( "Oops!You don't have enough money.(%d)\n\n" , need );
                return 0;
            }
        }
    }
    return 1;
}

int32_t crane_build( Player *ip )
{
    bool option[ip->table_number];
    bool s1 = 0;
    int32_t sure = 0;
    int32_t wanted = 0;
    int32_t need = 0;
    int32_t target = 0;
    int32_t replaced = 0;
    if( ip->handcard_number == 0 )
    {
        printf( "Oops! You don't have any handcards\n" );
        printf( "Please wait for next turn.\n" );
        return 0;
    }
    printf( "-------------------------\n" );
    printf( "Your card type codename :\n" );
    printf( "Handcards : " );
    for(int i=0;i<ip->handcard_number;i++)
    {
        printf( "%d " , Check_type(ip->handcard[i]) );
    }
    printf( "\nTable : " );
    for(int i=0;i<ip->table_number;i++)
    {
        printf( "%d " , ip->type[i] );
    }
    printf( "\n-------------------------\n" );
    printf( "Paid :\n" );
    printf( "Handcards : " );
    for(int i=0;i<ip->handcard_number;i++)
    {
        printf( "%d " , Card_Paid[ip->handcard[i]] );
    }
    printf( "\nTable : " );
    for(int i=0;i<ip->table_number;i++)
    {
        printf( "%d " , Card_Paid[ip->table[i]] );
    }
    printf( "\n-------------------------\n" );
    printf( "Which do you want to build from your hand card :\n" );
    int32_t times = 0;
    for(int i=0;i<ip->handcard_number;i++)
    {
        option[i] = 1;
        for(int j=0;j<ip->table_number;j++)
        {
            if( Check_type(ip->handcard[i]) == ip->type[j] )
            {
                if( ip->type[j] )
                {
                    times++;
                    if( times >= 2 )
                    {
                        option[i] = 0;
                    }
                    break;
                }
            }
        }
        if( option[i] == 1 )
        {
            printf( "[%d]%d " , i , ip->handcard[i] );
        }
        times = 0;
    }
    printf( "\n--> " ); //可以出的手牌
    if( scanf( "%d" , &wanted ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    printf( "For sure (Y:1,N:0) ? " );
    if( scanf( "%d" , &sure ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    while( sure != 1 )
    {
        for(int i=0;i<ip->handcard_number;i++)
        {
            if( option[i] == 1 )
            {
                printf( "[%d]%d " , i , ip->handcard[i] );
            }
        }
        printf( "\n--> " );
        if( scanf( "%d" , &wanted ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
        printf( "For sure (Y:1,N:0) ? " );
        if( scanf( "%d" , &sure ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
    }
    if( wanted >= 0 && wanted < ip->handcard_number )
    {
        if( option[wanted] )
        {
            target = ip->handcard[wanted];
            need = Card_Paid[target];
            printf( "Original Paid : %d\n" , need );
            s1 = 1;
        }
        else //不可以蓋的地方
        {
            printf( "Wrong instruction.\n" );
            printf( "Please wait for next turn.\n" );
            return 0;
        }
    }
    else //不在範圍
    {
        printf( "Wrong instruction.\n" );
        printf( "Please wait for next turn.\n" );
        return 0;
    }
    bool stop = 0;
    int32_t s2 = 0;
    for(int i=0;i<ip->table_number;i++)
    {
        if( Check_type(target) )
        {
            if( ip->type[i] == Check_type(target) )
            {
                printf( "You can only replace Card%d , paid : %d.\n" , ip->table[i] , Card_Paid[ip->table_number] );
                printf( "Continue (Y:1,N:0) ?" );
                if( scanf( "%d" , &s2 ) == 0 )
                {
                    printf( "You input a string or a char.\n" );
                    printf( "Game Over.\n" );
                    exit(0);
                }
                if( s2 == 1 )
                {
                    stop = 1;
                    replaced = i;//取代的位置
                    need -= Card_Paid[ip->table[i]];
                }
                else
                {
                    printf( "Please wait for next turn.\n" );
                    return 0;
                }
            }
            break;
        }
    }
    if( stop == 0 )
    {
        printf( "Where do you want to replace ? \n" );
        for(int i=0;i<ip->table_number;i++)
        {
            printf( "[%d]You can replace Card%d\n", i , ip->table[i] );
        }
        printf( "--> " );
        if( scanf( "%d" , &replaced ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
        if( replaced < 0 || replaced >= ip->table_number )
        {
            printf( "Wrong instruction.\n" );
            printf( "Please wait for next turn.\n" );
            return 0;
        }
        else
        {
            need -= Card_Paid[ip->table[replaced]];
        }
    }
    if( s1 )
    {
        bool black =0;
        for(int i=0;i<ip->table_number;i++)
        {
            if( ip->type[i] == 3 )
            {
                printf( PURPLE"Due to Smithy :\n"NC );
                printf( "You can paid less(-1)\n" );
                need--;
            }
            if( ip->type[i] == 8 )
            {
                if( special(target) )
                {
                    printf( PURPLE"Due to Quarry :\n"NC );
                    printf( "Because you build a special building.\n" );
                    printf( "You can paid less(-1)\n" );
                    need--;
                }
            }
            if( ip->type[i] == 5 )
            {
                printf( PURPLE"Due to Black_market :\n"NC );
                printf( "You can use product as handcard.\n" );
                black = 1;
            }
            if( ip->type[i] == 17 )
            {
                printf( PURPLE"Due to Library :\n"NC );
                printf( "You can paid less(-2)\n" );
                need -= 2;
            }
        }

        if( need <= 0 )
        {
            printf( "You get a free building.\n" );
            Card_Dispose[Card_D_p] = ip->table[replaced];
            Card_D_p++;
            ip->table[replaced] = target;
            Type( ip );
            need = 0;
            return 0;
        }

        printf( "You need %d(%d-%d) cards.\n" , need , need+Card_Paid[ip->table[replaced]] , Card_Paid[ip->table[replaced]] );
        if( black== 1 )
        {
            if( need < ip->handcard_number + ip->product_number )
            {
                Change( ip , wanted , 0 , 0 );
                printf( "Dispose :\n" );
                int32_t count = 0;
                int32_t sure_1 = 0;
                while( need != 0 )
                {
                    if( count < ip->handcard_number )
                    {
                        printf( "Card%d(Yes:1,No:0) :" , ip->handcard[count] );
                    } 
                    else 
                    {
                        printf( "Product of card%d(Yes:1,No:0) :" , ip->product_position[count] );
                    }
                    if( scanf( "%d" , &sure_1 ) == 0 )
                    {
                        printf( "You input a string or a char.\n" );
                        printf( "Game Over.\n" );
                        exit(0);
                    }
                    if( sure == 1 && count < ip->handcard_number )
                    {
                        Change( ip , count , 0 , 1 );
                        need--;
                        count--;
                    }
                    else if( sure == 1 && count >= ip->handcard_number )
                    {
                        Change( ip , count - ip->handcard_number , 1 , 1 );
                        need--;
                        count--;
                    }
                    if( count == ip->handcard_number + ip->product_number )
                    {
                        count = 0;
                    }
                    count++;
                }
                Card_Dispose[Card_D_p] = ip->table[replaced];
                Card_D_p++;
                ip->table[replaced] = target;
                Type( ip );
                for(int i=0;i<ip->table_number;i++)
                {
                    if( ip->type[i] == 4 )
                    {
                        if( ip->handcard_number <= 1 )
                        {
                            printf( PURPLE"Due to Poor_house :\n"NC );
                            printf( "Because your handcard <= one.\n" );
                            printf( "You can add a handcard.\n" );
                            ip->handcard[ip->handcard_number] = Card[Card_position];
                            ip->handcard_number++;
                            Card_position++;
                        }
                    }
                    if( ip->type[i] == 7 )
                    {
                        if( special(target) )
                        {
                            printf( PURPLE"Due to Carpenter :\n"NC );
                            printf( "Because you build a special building.\n" );
                            printf( "You can add a handcard.\n" );
                            ip->handcard[ip->handcard_number] = Card[Card_position];
                            ip->handcard_number++;
                            Card_position++;
                        }
                    }
                }
            }
            else
            {
                printf( "Oops!You don't have enough money.(%d)\n\n" , need );
                return 0;
            }
        }
        else
        {
            if( need < ip->handcard_number )
            {
                Change( ip , wanted , 0 , 0 );
                printf( "Dispose :\n" );
                int32_t count = 0;
                int32_t sure_1 = 0;
                while( need != 0 )
                {
                    printf( "Card%d(Yes:1,No:0) :" , ip->handcard[count] ); 
                    if( scanf( "%d" , &sure_1 ) == 0 )
                    {
                        printf( "You input a string or a char.\n" );
                        printf( "Game Over.\n" );
                        exit(0);
                    }
                    if( sure_1 == 1 )
                    {
                        Change( ip , count , 0 , 1 );
                        need--;
                    }
                    else
                    {
                        count++;
                    }
                    if( count == ip->handcard_number)
                    {
                        count = 0;
                    }
                }
                Card_Dispose[Card_D_p] = ip->table[replaced];
                Card_D_p++;
                ip->table[replaced] = target;
                Type( ip );
                for(int i=0;i<ip->table_number;i++)
                {
                    if( ip->type[i] == 4 )
                    {
                        if( ip->handcard_number <= 1 )
                        {
                            printf( PURPLE"Due to Poor_house :\n"NC );
                            printf( "Because your handcard <= one.\n" );
                            printf( "You can add a handcard.\n" );
                            ip->handcard[ip->handcard_number] = Card[Card_position];
                            ip->handcard_number++;
                            Card_position++;
                        }
                    }
                    if( ip->type[i] == 7 )
                    {
                        if( special(target) )
                        {
                            printf( PURPLE"Due to Carpenter :\n"NC );
                            printf( "Because you build a special building.\n" );
                            printf( "You can add a handcard.\n" );
                            ip->handcard[ip->handcard_number] = Card[Card_position];
                            ip->handcard_number++;
                            Card_position++;
                        }
                    }
                }
            }
            else
            {
                printf( "Oops!You don't have enough money.(%d)\n\n" , need );
                return 0;
            }
        }
    }
    return 1;
}

int32_t Your_Builder( Player *ip )
{
    Type( ip );
    int32_t wanted;
    int32_t target;
    int32_t sure = 0; // 0->不蓋 1->蓋 
    bool crane = 0;
    bool Switch = 0;
    int32_t need;
    int32_t position;
    printf( "\nDo you want to build (Y:1,N:any integer):" );
    if( scanf( "%d" , &sure ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    for(int i=0;i<ip->table_number;i++)
    {
        if( ip->type[i] == 6 )
        {
            printf( "Do you want to use the effect of \"Crane\" (Yes:1):" );
            if( scanf( "%d" , &sure ) == 0 )
            {
                printf( "You input a string or a char.\n" );
                printf( "Game Over.\n" );
                exit(0);
            }
            if( sure == 1 )
            {
                crane = 1;
            }
        }
    }
  
    if( crane ) //起重機
    {
        crane_build( ip );
    }
    else if( sure == 1 ) //普通蓋法
    {
        normal_build( ip );
    }
    else //不蓋
    {
        printf( "Please wait for next turn.\n" );
        return 0;
    }
    if( Check_type(target) == 1 )
    {
        ip->limit = 12;
    }
    return 1;
}

int32_t Your_Producer( Player *ip )
{
    Type( ip );
    int32_t sure = 0;
    bool Switch = 0; //是否有地方存放貨品
    bool option[ip->table_number]; //是否可以存貨物
    int32_t wanted = 0;

    printf( "\nDo you want to produce a product (Y:1,N:0) :" );
    if( scanf( "%d" , &sure ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    if( sure == 1 )
    {
        for(int i =0;i<ip->table_number;i++)
        {
            option[i] = 0;
            if( special(ip->table[i]) != 1 )
            {
                option[i] = 1; //可以放貨
                for(int j=0;j<ip->product_number;j++)
                {
                    if( ip->product_position[j] == ip->table[i] )
                    {
                        option[i] = 0; 
                        break;
                    }
                }
            }
        }
        for(int i=0;i<ip->table_number;i++)
        {
            if( option[i] )
            {
                Switch = 1;
                break;
            }
        }
    }
    else
    {
        printf( "Please wait for next turn.\n" );
        return 0;
    }
    if( Switch == 1 )
    {
        printf( "Where do you want to product ? \n" );
        for(int i =0;i<ip->table_number;i++)
        {
            if( option[i] == 1 )
            {
                printf( "[%d]%d " , i , ip->table[i] );
            }
        }
        printf( "--> " );
        if( scanf( "%d" , &wanted ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
        if( wanted >= 0 && wanted < ip->table_number ) 
        {
            if( option[wanted] == 1 )
            {
                ip->product[ip->product_number] = Card[Card_position];
                ip->product_position[ip->product_number] = ip->table[wanted];
                //printf(" %d" , ip->product_position[ip->product_number] );
                //printf(" %d\n" , ip->product[ip->product_number] );
                ip->product_number++;
                Card_position++;
            }
            else
            {
                printf( "Please wait for next turn.\n" );
                return 0;
            }
        }
        else
        {
            printf( "Please wait for next turn.\n" );
            return 0;
        }
    }
    else
    {
        printf( "Please wait for next turn.\n" );
        return 0;
    }
    return 1;
}

int32_t Your_Trader( Player *ip )
{
    Type( ip );
    int32_t sure = 0;
    bool Switch = 0;
    int32_t wanted = 0;
    int32_t gacha = 0;

    printf( "\nDo you want to sell (Y:1,N:0) :" );
    if( scanf( "%d" , &sure ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    if( sure == 1 )
    {
        if( ip->product_number )
        {
            Switch = 1;
        }
    }
    else
    {
        printf( "Please wait for next turn.\n" );
        return 0;
    }
    if( Switch )
    {
        printf( "Where do you want to sell :\n" );
        for(int i=0;i<ip->product_number;i++)
        {
            printf( "[%d]%d " , i , ip->product_position[i] );
        }
        printf( "--> " );
        if( scanf( "%d" , &wanted ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
        //printf(" %d" , ip->product_position[wanted] );
        if( wanted >= 0 && wanted < ip->product_number )
        {
            int32_t R_paid = (rand()%(5)) + 0;
            if( one(ip->product_position[wanted]) )
            {
                gacha = Product_Paid[R_paid][0];
            }
            else if( two(ip->product_position[wanted]) )
            {
                gacha = Product_Paid[R_paid][1];
            }
            else if( three(ip->product_position[wanted]) )
            {
                gacha = Product_Paid[R_paid][2];
            }
            else if( four(ip->product_position[wanted]) )
            {
                gacha = Product_Paid[R_paid][3];
            }
            else if( five(ip->product_position[wanted]) )
            {
                gacha = Product_Paid[R_paid][4];
            }
            Change( ip , wanted , 1 , 1 );
            for(int i=0;i<ip->table_number;i++)
            {
                if( ip->type[i] == 11 )
                {
                    if( ip->sell >= 2 )
                    {
                        printf( PURPLE"Due to Market_stand :\n"NC );
                        printf( "Because you had sell over 2 products.\n" );
                        printf( "You can add a handcard.\n"NC );
                        ip->handcard[ip->handcard_number] = Card[Card_position];
                        ip->handcard_number++;
                        Card_position++;
                    }
                }
            }
            for(int i=0;i<gacha;i++)
            {
                ip->handcard[ip->handcard_number] = Card[Card_position];
                ip->handcard_number++;
                Card_position++;
            }
            ip->sell++;
            //printf( "%d\n" , gacha );
        }
        else
        {
            printf( "Wrong.\nPlease wait for next turn.\n" );
            return 0;
        }
    }
    else
    {
        printf( "You don't have product to sell.\n" );
        return 0;
    }
    return 1;
}

int32_t Your_Councillor( Player *ip , bool option )
{
    Type( ip );
    int32_t sure = 0;
    int32_t wanted = -1;
    int32_t number = 0;
    int32_t decide = 0;
    bool s = 0;

    if( option == 1 )
    {
        printf( "\nDo you want to use privilege (Y:1,N:0): " );
        if( scanf( "%d" , &sure ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
    }
    if( sure + option == 2 )
    {
        number = 5;
    }
    else if( sure == 0 )
    {
        number = 2;
    }
    bool s1 = 0;
    bool s2 = 0;
    bool s3 = 0;
    for(int i=0;i<ip->table_number;i++)
    {
        if( ip->type[i] == 14 )
        {
            s1 = 1;
        }
        else if( ip->type[i] == 15 )
        {
            s2 = 1;
        }
        else if( ip->type[i] == 17 )
        {
            s3 = 1;
        }
    }
    
    if( s1 + s2 + s3 > 1 )
    {
        printf( "Which do you want to choose.\n" );
        if( s1 )
        {
            printf( "[0]Archive's " );
        }
        if( s2 )
        {
            printf( "[1]Archive's " );
        }
        if( s3 )
        {
            printf( "[2]Prefecture's " );
        }
        printf( "[others]normal\n" );
        if( scanf( "%d" , &decide ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
        if( decide != 0 || decide != 1 || decide != 2 )
        {
           s1 = 0 , s2 = 0 , s3 = 0;
        }
        else 
        {
            if( decide == 0 && s1 == 1 )
            {
                s2 = 0 , s3 = 0; 
            }
            else if( decide == 1 && s2 == 1 )
            {
                s1 = 0 , s3 = 0; 
            }
            else if( decide == 2 && s3 == 1 )
            {
                s1 = 0 , s2 = 0; 
            }
            else
            {
                s1 = 0 , s2 = 0 , s3 = 0;
            }
        }
    }
    if( s1 == 1 )
    {
        printf( PURPLE"Due to Archive :\n"NC );
        printf( "You can add card to handcard first and then select.\n" );
        for(int i=0;i<ip->handcard_number;i++)
        {
            printf( "[%d]%d " , i , ip->handcard[i] );
        }
        for(int i=0;i<number;i++)
        {
            ip->handcard[ip->handcard_number] = Card[Card_position];
            printf( "[%d]%d " , i , ip->handcard[ip->handcard_number] );
            ip->handcard_number++;
            Card_position++;
        }
        printf( "Dispose %d times : " , number - 1 );
        int32_t need = number - 1;
        int32_t count = 0;
        int32_t sure_1 = 0;     
        while( need )
        {
            printf( "Card%d(Y:1,N:0) :" , ip->handcard[count] );
            if( scanf( "%d" , &sure_1 ) == 0 )
            {
                printf( "You input a string or a char.\n" );
                printf( "Game Over.\n" );
                exit(0);
            }
            if( sure_1 == 1 )
            {
                Change( ip , count , 0 , 1 );
                need--;
                count--;
            }
            if( count >= ip->handcard_number )
            {
                count = 0;
            }
            else
            {
                count++;
            }
        }
    }
    else if( s2 == 1 ) 
    {
        printf( PURPLE"Due to Prefecture :\n"NC );
        int32_t count = 0;
        int32_t sure_2 = 0;
        printf( "You can choose 2 of %d to be your handcard.\n" , number );
        for(int i=0;i<number;i++)
        {
            printf( "[%d]%d " , i , Card[Card_position] );
            ip->memory[i] = Card[Card_position];
            ip->memory_number++;
            Card_position++;
            
        }
        if( number == 2 )
        {
            printf( " will be your handcards.\n" );
            ip->handcard[ip->handcard_number] = ip->memory[0];
            ip->handcard_number++;
            ip->handcard[ip->handcard_number] = ip->memory[1];
            ip->handcard_number++;
        }
        else
        {
            int32_t need = 2;
            printf( "\n" );
            while( need )
            {
                printf( "Card%d(Y:1,N:0) :" , ip->memory[count] );
                if( scanf( "%d" , &sure_2 ) == 0 )
                {
                    printf( "You input a string or a char.\n" );
                    printf( "Game Over.\n" );
                    exit(0);
                }
                if( sure_2 == 1 )
                {
                    ip->handcard[ip->handcard_number] = ip->memory[count];
                    Change( ip , count , 2 , 0 );
                    ip->handcard_number++;
                    need--;
                    count--;
                }
                if( count ==  ip->memory_number )
                {
                    count = 0;
                }
                else
                {
                    count++;
                }
            }
            for(int i=0;i<ip->memory_number;i++)
            {
                Card_Dispose[Card_D_p] = ip->memory[i];
                Card_D_p++;
                ip->memory[i] = 0;
            }        
        }        
    }
    else 
    {
        if( s3 )
        {
            printf( PURPLE"Due to Library : \n"NC );
            printf( "You can upgrade your option to 8.\n" );
            number = 8;
        }
        printf( "You can choose 1 of %d to be your handcard.\n" , number );
        int32_t memory[5];
        for(int i=0;i<number;i++)
        {
            memory[i] = Card[Card_position];
            printf( "[%d]%d " , i , Card[Card_position] );
            Card_position++;
        }
        printf( "\n--> " );
        if( scanf( "%d" , &wanted ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
        if( wanted >=0 && wanted <= number-1 )
        {
            ip->handcard[ip->handcard_number] = memory[wanted];
            ip->handcard_number++;
        }
        else
        {
            printf( "Wrong.\nPlease wait for next turn.\n" );
            return 0;
        }
        for(int i=0;i<number;i++)
        {
            if( i != wanted )
            {
                Card_Dispose[Card_D_p] = memory[i];
                Card_D_p++;
            }
        }
    }  
    return 1;
}

int32_t Your_Prospector( Player *ip , int32_t who )
{
    Type( ip );
    bool golden = 0;
    if( who == ip-> n )
    {
        int32_t sure;
        printf( "You can add a handcard (Y:1,N:0) :" );
        if( scanf( "%d" , &sure ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
        if( sure == 1 )
        {
            ip->handcard[ip->handcard_number] = Card[Card_position];
            ip->handcard_number++;
            Card_position++;
        }
        else
        {
            printf( "Please wait for next turn.\n" );
            return 0;
        }
    }
    for(int i=0;i<ip->table_number;i++)
    {
        if( ip->type[i] == 16 )
        {
            golden = 1;
        }
    }
    if( golden )
    {
        printf( PURPLE"Due to Gold_mine : \n"NC );
        printf( "You can pick 1 of 4 if the cost are all different.\n" );
        int32_t buf[4][2];
        for(int i=0;i<4;i++)
        {
            buf[i][0] = Card[Card_position];
            buf[i][1] = Card_Paid[buf[i][0]];
            Card_position++;
            printf( "[%d] Card%d : need %d.\n" , i+1 , buf[i][0] , buf[i][1] );
        }
        bool diffrent = 1;
        for(int i=0;i<4;i++)
        {
            for(int j=i+1;j<4;j++)
            {
                if( buf[i][1] == buf[j][1] )
                {
                    diffrent = 0;
                }
            }
        }
        if( diffrent )
        {
            printf( "You can pick 1 of 4.\n" );
            printf( "--> " );
            int32_t wanted = 0;
            if( scanf( "%d" , &wanted ) == 0 )
            {
                printf( "You input a string or a char.\n" );
                printf( "Game Over.\n" );
                exit(0);
            }
            if( wanted >= 0 && wanted < 4 )
            {
                for(int i=0;i<4;i++)
                {
                    if( i != wanted )
                    {
                        Card_Dispose[Card_D_p] = buf[i][0];
                        Card_D_p++;
                    }
                    else
                    {
                        ip->handcard[ip->handcard_number] = buf[i][0];
                        ip->handcard_number++;
                    }
                }
            }
            else
            {
                printf( "You lose the chance for this.\n" );
                return 0;
            }
        }
        else
        {
            printf( "You are unfortunate...\n" );
            for(int i=0;i<4;i++)
            {
                Card_Dispose[Card_D_p] = buf[i][0];
                Card_D_p++;
            }
        }
    }
    return 1;
}

int32_t Other_Builder( Player *ip )
{
    Type( ip );
    bool crane = 0;
    bool other = 1;
    int32_t replaced = (rand()%(ip->table_number)) + 0;
    int32_t wanted = 0;
    int32_t target = 0;
    int32_t need = 0;
    int32_t dispose = 0;
    int32_t range = 0 ;
    bool black = 0;
    if( ip->handcard_number )
    {
        wanted = (rand()%(ip->handcard_number)) + 0;
        target =ip->handcard[wanted];
        need = Card_Paid[ip->handcard[wanted]];
        for(int i=0;i<ip->table_number;i++)
        {
            if( ip->type[i] == 5 )
            {
                range += ip->product_number;
                black = 1;
            }
            if( ip->type[i] == 6 )
            {
                crane = 1;
                need -= Card_Paid[ip->table[replaced]];
            }
        }
        if( range + ip->handcard_number > need )
        {
            Change( ip , wanted , 0 , 0 );
            range += ip->handcard_number;
            for(int i=0;i<ip->table_number;i++)
            {
                if( ip->type[i] == 1 )
                {
                    ip->limit = 12;
                }
                if( ip->type[i] == 3 )
                {
                    need--;
                }
                if( ip->type[i] == 4 )
                {
                    if( ip->handcard_number - need <= 1 )
                    {
                        need--;
                    }
                }
                if( ip->type[i] == 7 )
                {
                    if( special(target) )
                    {
                        ip->handcard[ip->handcard_number] = Card[Card_position];
                        ip->handcard_number++;
                        Card_position++;
                    }
                }
                if( ip->type[i] == 8 )
                {
                    if( special(target) )
                    {
                        need--;
                    }
                }
                if( ip->type[i] == 17 )
                {
                    need -= 2;
                }
            }
            for(int i=0;i<ip->table_number;i++)
            {
                if( Check_type(target) == ip->type[i] && ip->type[i] != 0 )
                {
                    crane = 0;
                    other = 0;
                    printf( "$Com%d choose not to build.\n" , ip->n );
                    return 0;
                }
            }
            
            if( crane )
            {            
                for(int i=0;i<ip->table_number;i++)
                {
                    if( ip->product_position[i] == ip->table[replaced])
                    {
                        Change( ip , i ,  1 , 0 );
                    }
                }
                
                printf( "$Com%d choose to build %d replace %d.\nDispose : " , ip->n , target , ip->table[replaced] );
                if( need == 0 )
                {
                    printf( "None.\n" );
                }
                for(int i=0;i<need;i++)
                {
                    dispose = (rand()%(range)) + 0;
                    if( dispose >= ip->handcard_number )
                    {
                        dispose -= ip->handcard_number;
                        printf( "%d " , ip->product_position[dispose] );
                        Change( ip , dispose ,  1 , 1 );
                    }
                    else
                    {
                        printf( "%d " , ip->handcard[dispose] );
                        Change( ip , dispose ,  0 , 1 );
                    }
                    range--;
                }
                printf( "\n" );
                Card_Dispose[Card_D_p] = ip->table[replaced];
                Card_D_p++;
                ip->table[replaced] = target;
                Type( ip );
            }
            else
            {
                printf( "$Com%d choose to build %d.\nDispose : " , ip->n , target );
                if( need == 0 )
                {
                    printf( "None.\n" );
                }
                for(int i=0;i<need;i++)
                {
                    dispose = (rand()%(range)) + 0;
                    if( dispose >= ip->handcard_number )
                    {
                        dispose -= ip->handcard_number;
                        printf( "%d " , ip->product_position[dispose] );
                        Change( ip , dispose ,  1 , 1 );
                    }
                    else
                    {
                        printf( "%d " , ip->handcard[dispose] );
                        Change( ip , dispose ,  0 , 1 );
                    }
                    range--;
                }
                printf( "\n" );
                ip->table[ip->table_number] = target;
                ip->table_number++;
            }
        }
        else 
        {
            printf( "$Com%d choose not to build.\n" , ip->n );
            return 0;
        }
    }
    else
    {
        printf( "$Com%d choose not to build.\n" , ip->n );
        return 0;
    }  
    return 1;
}

int32_t Other_Producer( Player *ip )
{
    Type( ip );
    int32_t option[ip->table_number];
    int32_t count = 0;
    int32_t wanted = 0;
    int32_t position = 0;
    int32_t target = 0;
    for(int i=0;i<ip->table_number;i++)
    {
        if( special(ip->table[i]) )
        {
            option[i] = 0;
        }
        else
        {
            option[i] = 1;  
            count++; 
        }
    }
    int32_t valid[count];
    count = 0;
    for(int i=0;i<ip->table_number;i++)
    {
        if( option[i] )
        {
            valid[count] = ip->table[i];
            count++; 
        }
    }
    if( count == 0 )
    {
        printf( "$Com%d choose not to produce.\n" , ip->n );
        return 0;
    }
    else
    {
        wanted = (rand()%(count)) + 0;
        bool cancel = 0;
        for(int i=0;i<ip->table_number;i++)
        {
            if( valid[wanted] == ip->table[i] )
            {
                position = i;
                target = ip->table[i];
            }
        }
        //printf( "%d " , ip->product_position[i] );
        for(int i=0;i<ip->product_number;i++)
        {
            //printf( "%d " , ip->product_position[i] );
            if( ip->product_position[i] == target )
            {
                cancel = 1;
                break;
            }
        }
        if( cancel )
        {
            printf( "$Com%d choose not to produce.\n" , ip->n );
            return 0;
        }
        else
        {
            ip->product_position[ip->product_number] = target;
            ip->product[ip->product_number] = Card[Card_position];
            printf( "$Com%d" , ip->n );
            printf( " choose to produce at %d.\n" , target );
            ip->product_number++;
            Card_position++;
        } 
    }
    return 1;
}

int32_t Other_Trader( Player *ip )
{
    Type( ip );
    int32_t gacha = 0;
    for(int i=0;i<ip->table_number;i++)
    {
        if( ip->type[i] == 11 )
        {
            if( ip->sell >= 2 )
            {
                ip->handcard[ip->handcard_number] = Card[Card_position];
                ip->handcard_number++;
                Card_position++;
            }
        }
    }
    if( ip->product_number )
    {
        int32_t wanted = (rand()%(ip->product_number)) + 0; 
        int32_t R_paid = (rand()%(5)) + 0;  
        if( one(ip->product_position[wanted]) )
        {
            gacha = Product_Paid[R_paid][0];
        }
        else if( two(ip->product_position[wanted]) )
        {
            gacha = Product_Paid[R_paid][1];
        }
        else if( three(ip->product_position[wanted]) )
        {
            gacha = Product_Paid[R_paid][2];
        }
        else if( four(ip->product_position[wanted]) )
        {
            gacha = Product_Paid[R_paid][3];
        }
        else if( five(ip->product_position[wanted]) )
        {
            gacha = Product_Paid[R_paid][4];
        }
        printf( "$Com%d choose to sell the product of %d .\n" , ip->n , ip->product_position[wanted]);
        Change( ip , wanted , 1 , 1 );
        for(int i=0;i<gacha;i++)
        {
            ip->handcard[ip->handcard_number] = Card[Card_position];
            ip->handcard_number++;
            Card_position++;
        }
        ip->sell++;
    }
    else
    {
        printf( "$Com%d choose not to sell.\n" , ip->n );
        return 0;
    }
    return 1;
}

void Other_Councillor( Player *ip , int32_t who )
{
    Type( ip );
    int32_t option = 0;
    int32_t range = 0;
    int32_t wanted_1 = 0;
    int32_t wanted_2 = 0;
    if( ip->n == who )
    {
        range = 5;
    }
    else
    {
        range = 2;
    }
    for(int i=0;i<ip->table_number;i++)
    {
        if( ip->type[i] == 14 )
        {
            option = 1;
        }
        else if( ip->type[i] == 15 )
        {
            option = 2;
        }
        else if( ip->type[i] == 17 )
        {
            option = 0;
            range = 8;
        }
    }
    if( option == 0 ) 
    {
        wanted_1 = (rand()%(range)) + 0;
        //printf( "%d\n" , wanted_1 );
        for(int i=0;i<range;i++)
        {
            if( i == wanted_1 )
            {
                ip->handcard[ip->handcard_number] = Card[Card_position];
                ip->handcard_number++;
            }
            else
            {
                Card_Dispose[Card_D_p] = Card[Card_position];
                Card_D_p++;
            }
            //printf( "%d " , Card[Card_position] );
            Card_position++;
        }
        printf( "$Com%d choose %d of 1.\n" , ip->n , range );
    }
    else if( option == 1 ) 
    {
        for(int i=0;i<range;i++)
        {
            ip->handcard[ip->handcard_number] = Card[Card_position];
            ip->handcard_number++;
            Card_position++;
        }
        for(int i=0;i<range-1;i++)
        {
            wanted_1 = (rand()%(ip->handcard_number)) + 0;
            Change(ip , wanted_1 , 0 , 1 );
        }
        printf( "$Com%d choose (%d+hancard) of 1.\n" , ip->n , range );
    }
    else if( option == 2 )
    {
        wanted_1 = (rand()%(range)) + 0;
        wanted_2 = (rand()%(range)) + 0;
        while( wanted_2 == wanted_1 )
        {
            wanted_2 = (rand()%(range)) + 0;
        }
        for(int i=0;i<range;i++)
        {
            if( i == wanted_1 || i == wanted_2 )
            {
                ip->handcard[ip->handcard_number] = Card[Card_position];
                ip->handcard_number++;
            }
            else
            {
                Card_Dispose[Card_D_p] = Card[Card_position];
                Card_D_p++;
            }
            //printf( "%d " , Card[Card_position] );
            Card_position++;
        }
        printf( "$Com%d choose %d of 2.\n" , ip->n , range );
    }
}

void Other_Prospector( Player *ip , int32_t who )
{
    Type( ip );
    bool golden = 0;
    if( who == ip->n )
    {
        printf( "$Com%d pick a card.\n" , ip->n );
        ip->handcard[ip->handcard_number] = Card[Card_position];
        ip->handcard_number++;
        Card_position++;
    }
    for(int i=0;i<ip->table_number;i++)
    {
        if( ip->type[i] == 16 )
        {
            golden = 1;
        }
    }
    if( golden )
    {
        int32_t buf[4][2];
        for(int i=0;i<4;i++)
        {
            buf[i][0] = Card[Card_position];
            buf[i][1] = Card_Paid[buf[i][0]];
            Card_position++;
        }
        bool diffrent = 1;
        for(int i=0;i<4;i++)
        {
            for(int j=i+1;j<4;j++)
            {
                if( buf[i][1] == buf[j][1] )
                {
                    diffrent = 0;
                }
            }
        }
        if( diffrent )
        {
            int32_t wanted = (rand()%(4)) + 0;
            for(int i=0;i<4;i++)
            {
                if( i != wanted )
                {
                    Card_Dispose[Card_D_p] = buf[i][0];
                    Card_D_p++;
                }
                else
                {
                    ip->handcard[ip->handcard_number] = buf[i][0];
                    ip->handcard_number++;
                }
            }
        }
        else
        {
            for(int i=0;i<4;i++)
            {
                Card_Dispose[Card_D_p] = buf[i][0];
                Card_D_p++;
            }
        }
    }
}

int32_t Your_behave( Player *ip , int32_t choose , Player *who , Player *Governor )
{
    Type( ip );
    if( choose == 0 )
    {
        int32_t Auto = 0;
        printf( "Computer choose for you (Y:1,N:any integer) ? " );
        if( scanf( "%d" , &Auto ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
        if( Auto == 1 )
        {
            Other_Builder( ip );
        }
        else
        {
            Your_Builder( ip );
        }
        if( ip->table_number >= 12 )
        {
            return 1;
        }
        printf( "\n" );
    }
    else if( choose == 1 )
    {
        int32_t times = 0;
        int temp = ip->product_number;
        bool library = 0;
        for(int i=0;i<ip->table_number;i++)
        {
            if( ip->type[i] == 17 ) //produce
            {
                printf( PURPLE"Due to Library :\n"NC );
                printf( "You can produce at most 3 produces.\n" );
                library = 1;
            }
            if( ip->type[i] == 9 )
            {
                if( ip->product_number >= 2 )
                {
                    printf( PURPLE"Due to Well :\n"NC );
                    printf( "Because you have products >= 2.\n" );
                    printf( "You can add a handcard.\n" );
                    ip->handcard[ip->handcard_number] = Card[Card_position];
                    ip->handcard_number++;
                    Card_position++;
                }
            }
            if( ip->type[i] == 10 )
            {
                printf( PURPLE"Due to Aqueduct :\n"NC );
                printf( "You can produce again.\n" );
                Your_Producer( ip );
                times++;
            }
        }
        Your_Producer( ip );
        times++;
        if( library )
        {
            for(int i=times;i<2;i++)
            {
                Your_Producer( ip );
            }
        }
        printf( "\n" );
    }
    else if( choose == 2 )
    {
        int32_t times = 0;
        int temp = ip->product_number;
        bool library = 0;
        for(int i=0;i<ip->table_number;i++)
        {
            if( ip->type[i] == 12 )
            {
                printf( PURPLE"Due to Market_hall :\n"NC );
                printf( "You can add a handcard.\n" );
                ip->handcard[ip->handcard_number] = Card[Card_position];
                ip->handcard_number++;
                Card_position++;
            }
            if( ip->type[i] == 17 ) //produce
            {
                printf( PURPLE"Due to Library :\n"NC );
                printf( "You can sell at most 3 produces.\n" );
                library = 1;
            }
            if( ip->type[i] == 13 )
            {
                printf( PURPLE"Due to Trading_post :\n"NC );
                printf( "You can sell again.\n" );
                Your_Trader( ip );
                times++;
            }
        }
        Your_Trader( ip );
        times++;
        if( library )
        {
            for(int i=times;i<2;i++)
            {
                Your_Trader( ip );
            }
        }
        printf( "\n" );
    }
    else if( choose == 3 )
    {
        Your_Councillor( ip , 0 );
        printf( "\n" );
    }
    else if( choose == 4 )
    {
        Your_Prospector( ip , who->n );
    }
    return 0;
}

int32_t Other_behave( Player *ip , int32_t choose , Player *who , Player *Governor )
{
    Type( ip );
    bool daze = 0;
    if( ip->difficulty == 0 )
    {
        int32_t ramdom = (rand()%10) + 0;
        if( ramdom == 3 )
        {
            daze = 1;
        }
    }
    if( choose != 4 )
    {
        sleep(1);
    }
    if( daze == 0 )
    {
        if( choose == 0 )
        {
            Other_Builder( ip );
            if( ip->table_number >= 12 )
            {
                return 1;
            }
        }
        else if( choose == 1 )
        {
            int32_t times = 0;
            int32_t temp = ip->product_number;
            bool library = 0;
            for(int i=0;i<ip->table_number;i++)
            {
                if( ip->type[i] == 9 )
                {
                    if( ip->product_number >= 2 )
                    {
                        ip->handcard[ip->handcard_number] = Card[Card_position];
                        ip->handcard_number++;
                        Card_position++;
                    }
                }
                if( ip->type[i] == 10 )
                {
                    Other_Producer( ip );
                    times++;
                }
                if( ip->type[i] == 17 )
                {
                    library = 1;
                }
            }
            Other_Producer( ip );
            times++;
            if( library )
            {
                for(int i=times;i<2;i++)
                {
                    Other_Producer( ip );   
                }
            }
        }
        else if( choose == 2 )
        {
            int32_t times = 0;
            int32_t temp = ip->product_number;
            bool library = 0;
            for(int i=0;i<ip->table_number;i++)
            {
                if( ip->type[i] == 12 )
                {
                    ip->handcard[ip->handcard_number] = Card[Card_position];
                    ip->handcard_number++;
                    Card_position++;
                }
                if( ip->type[i] == 13 )
                {
                    Other_Trader( ip );
                    times++;
                }
                if( ip->type[i] == 17 )
                {
                    library = 1;
                }
            }
            Other_Trader( ip );
            times++;
            if( library )
            {
                for(int i=times;i<2;i++)
                {
                    Other_Trader( ip );   
                }
            }
        }
        else if( choose == 3 )
        {
            Other_Councillor( ip , who->n );
        }
        else
        {
            for(int i=0;i<ip->table_number;i++)
            {
                if( ip->type[i] == 17 )
                {
                    for(int i=0;i<2;i++)
                    {
                        ip->handcard[ip->handcard_number] = Card[Card_position];
                        ip->handcard_number++;
                        Card_position++;
                    }
                }
            }
            Other_Prospector( ip , who->n );
        }
        sleep(1);
    }
    else
    {
        printf( "Com%d is dazing , forgot to action ...\n" , ip->n );
    }
    return 0;
}

void Check_for_every_turn( int32_t number )
{
    Type( ip );
    printf( BLUE"The round is over.\nChecking......\n"NC );
    sleep(2);
    bool y_c = 0;
    for(int i=0;i<number;i++)
    {
        for(int j=0;j<ip->table_number;j++)
        {
            if( ip[i].type[j] == 1 )
            {
                ip[i].limit = 12;
            }
            if( ip[i].type[j] == 2 )
            {
                if( i == 0 )
                {
                    if( ip[i].handcard_number > 0 )
                    {
                        y_c = 1;
                    }
                }
                else
                {
                    if( ip[i].handcard_number > 0 )
                    {
                        ip[i].church[ip[i].church_number] = ip->handcard[0];
                        ip->church_number++;
                        Change( &ip[i] , 0 , 0 , 0 );
                    }
                }
            }
        }
    }
    if( ip[0].limit < ip[0].handcard_number )
    {
        int32_t need = ip[0].handcard_number-ip[0].limit;
        printf( "You need to dispose %d cards\n" , need );
        printf( "Dispose :\n" );
        int32_t count = 0;
        int32_t sure = 0;
        while( need != 0 )
        {
            printf( "Card%d(Yes:1,No:0) :" , ip->handcard[count] ); 
            if( scanf( "%d" , &sure ) == 0 )
            {
                printf( "You input a string or a char.\n" );
                printf( "Game Over.\n" );
                exit(0);
            }
            if( sure == 1 )
            {
                Change( ip , count , 0 , 1 );
                need--;
            }
            else
            {
                count++;
            }
            if( count == ip->handcard_number )
            {
                count = 0;
            }
        }
    }
    if( y_c )
    {
        printf( PURPLE"Due to Chapel : \n"NC );
        printf( "You can put a handcard under your chapel.\n" );
        int32_t wanted = 0;
        for(int i=0;i<ip[0].handcard_number;i++)
        {
            printf( "[%d]%d " , i , ip[0].handcard[i] );
        }
        printf( "\n" );
        if( scanf( "%d" , &wanted ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );  
            exit(0);
        }
        if( wanted >= 0 && wanted < ip->handcard_number )
        {
            ip[0].church[ip[0].church_number] = ip[0].handcard[wanted];
            Change( &ip[0] , wanted , 0 , 0 );
            ip[0].church_number++;
        }
        else
        {
            printf( "You lose the chance for this.\n" );
        }
    }
    printf( "You done.\n" );
    for(int i=1;i<number;i++)
    {
        if( ip[i].limit < ip[i].handcard_number )
        {
            for(int j=0;j<ip[i].handcard_number-ip[i].limit;j++)
            {
                int32_t ramdom = (rand()%(ip[i].handcard_number)) + 0;
                Change( &ip[i] , ramdom , 0 , 1 );
            }
        }
        printf( "Com%d done.\n" , i );
    }                                  
    printf( BLUE"Done\n"NC );
}

int32_t Your_Role( Player *ip )
{
    Type( ip );
    int32_t choose = -1;
    int32_t sure = 0;
    while( choose == -1 )
    {
        printf("Which role card do you want to choose ? \n");
        for(int i=0;i<5;i++)
        {
            if( state[i] == 1 )
            {
                printf( "%d.%s\n" , i , role_name[i] );
            }
        }
        printf( "--> " );
        
        if( scanf( "%d" , &choose ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );  
            exit(0);
        }
        if( state[choose] == 0 )
        {
            choose = -1;
        }
    }
    
    state[choose] = 0;
    strncpy( ip->role , role_name[choose] , strlen(role_name[choose]) );
    printf( "Information needed (Y:1,N:0) --> " );
    if( scanf( "%d" , &sure ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    if(  sure == 1 )
    {
        Q();
    }
    if( choose == 0 )
    {
        printf( "It's Builder\n" );
        for(int i=0;i<2;i++)
        {
            int32_t Auto = 0;
            printf( "Computer choose for you (Y:1,N:any integer) ? " );
            if( scanf( "%d" , &Auto ) == 0 )
            {
                printf( "You input a string or a char.\n" );
                printf( "Game Over.\n" );
                exit(0);
            }
            if( Auto == 1 )
            {
                Other_Builder( ip );
            }
            else
            {
                Your_Builder( ip );
            }
            if( ip->table_number >= 12 )
            {
                return 6;
            }
        }
    }
    else if( choose == 1 )
    {
        int32_t times = 0;
        int temp = ip->product_number;
        bool library = 0;
        for(int i=0;i<ip->table_number;i++)
        {
            if( ip->type[i] == 17 ) //produce
            {
                printf( PURPLE"Due to Library :\n"NC );
                printf( "You can produce at most 3 produces.\n" );
                library = 1;
            }
            if( ip->type[i] == 9 )
            {
                if( ip->product_number >= 2 )
                {
                    printf( PURPLE"Due to Well :\n"NC );
                    printf( "Because you have products >= 2.\n" );
                    printf( "You can add a handcard.\n" );
                    ip->handcard[ip->handcard_number] = Card[Card_position];
                    ip->handcard_number++;
                    Card_position++;
                }
            }
            if( ip->type[i] == 10 )
            {
                printf( PURPLE"Due to Aqueduct :\n"NC );
                printf( "You can produce again.\n" );
                Your_Producer( ip );
                times++;
            }
        }
        for(int i=0;i<2;i++)
        {
            Your_Producer( ip );
            times++;
        }
        if( library )
        {
            for(int i=times;i<2;i++)
            {
                Your_Producer( ip );
            }
        }
        printf( "\n" );
    }
    else if( choose == 2 )
    {
        int32_t times = 0;
        int temp = ip->product_number;
        bool library = 0;
        for(int i=0;i<ip->table_number;i++)
        {
            if( ip->type[i] == 17 ) //produce
            {
                printf( PURPLE"Due to Library :\n"NC );
                printf( "You can sell at most 3 produces.\n" );
                library = 1;
            }
            if( ip->type[i] == 12 )
            {
                printf( PURPLE"Due to Market_hall :\n"NC );
                printf( "You can add a handcard.\n" );
                ip->handcard[ip->handcard_number] = Card[Card_position];
                ip->handcard_number++;
                Card_position++;
            }
            if( ip->type[i] == 13 )
            {
                printf( PURPLE"Due to Trading_post :\n"NC );
                printf( "You can sell again.\n" );
                Your_Trader( ip );
                times++;
            }
        }
        for(int i=0;i<2;i++)
        {
            Your_Trader( ip );
            times++;
        }
        if( library )
        {
            for(int i=times;i<2;i++)
            {
                Your_Trader( ip );
            }
        }
        printf( "\n" );
    }
    else if( choose == 3 )
    {
        printf( "It's Councillor\n" );
        Your_Councillor( ip , 1 );
    }
    else if( choose == 4 )
    {
        printf( "It's Prospector\n" );
        for(int i=0;i<ip->table_number;i++)
        {
            if( ip->type[i] == 17 )
            {
                printf( PURPLE"Due to Library :\n"NC );
                printf( "You can pick 2 cards.\n" );
                for(int i=0;i<2;i++)
                {
                    ip->handcard[ip->handcard_number] = Card[Card_position];
                    ip->handcard_number++;
                    Card_position++;
                }
            }
        }
        Your_Prospector( ip , 0 );
    }
    return choose;
}

int32_t Other_Role( Player *ip )
{
    Type( ip );
    int32_t count = 0;
    for(int i=0;i<5;i++)
    {
        if( state[i] == 1 )
        {
            count++;
        }
    }
    int32_t wanted = (rand()%(count)) + 1;
    int32_t  choose;
    for(int i=0;i<5;i++)
    {
        if( state[i] == 1 )
        {
            wanted--;
        }
        if( wanted == 0 )
        {
            choose = i;
            break;
        }
    }
    state[choose] = 0;
    strncpy( ip->role , role_name[choose] , strlen(role_name[choose]) );
    bool daze = 0;
    if( ip->difficulty == 0 )
    {
        int32_t ramdom = (rand()%10) + 0;
        if( ramdom == 3 )
        {
            daze = 1;
        }
    }
    if( daze == 0 )
    {
        if( choose == 0 )
        {
            printf( "It's Builder\n" );
            Other_Builder( ip );
            Other_Builder( ip );
        }
        else if( choose == 1 )
        {
            printf( "It's Producer\n" );
            int32_t times = 0;
            int32_t temp = ip->product_number;
            bool library = 0;
            for(int i=0;i<ip->table_number;i++)
            {
                if( ip->type[i] == 9 )
                {
                    if( ip->product_number >= 2 )
                    {
                        ip->handcard[ip->handcard_number] = Card[Card_position];
                        ip->handcard_number++;
                        Card_position++;
                    }
                }
                if( ip->type[i] == 10 )
                {
                    Other_Producer( ip );
                    times++;
                }
                if( ip->type[i] == 17 )
                {
                    library = 1;
                }
            }
            Other_Producer( ip );
            times++;
            Other_Producer( ip );
            times++;
            if( library )
            {
                for(int i=times;i<2;i++)
                {
                    Other_Producer( ip );   
                }
            }
        }
        else if( choose == 2 )
        {
            printf( "It's Trader\n" );
            int32_t times = 0;
            int32_t temp = ip->product_number;
            bool library = 0;
            for(int i=0;i<ip->table_number;i++)
            {
                if( ip->type[i] == 12 )
                {
                    ip->handcard[ip->handcard_number] = Card[Card_position];
                    ip->handcard_number++;
                    Card_position++;
                }
                if( ip->type[i] == 13 )
                {
                    Other_Trader( ip );
                    times++;
                }
                if( ip->type[i] == 17 )
                {
                    library = 1;
                }
            }
            Other_Trader( ip );
            times++;
            Other_Trader( ip );
            times++;
            if( library )
            {
                for(int i=times;i<2;i++)
                {
                    Other_Trader( ip );   
                }
            }
        }
        else if( choose == 3 )
        {
            printf( "It's Councillor\n" );
            Other_Councillor( ip , ip->n );
        }
        else
        {
            printf( "It's Prospector\n\n" );
            for(int i=0;i<ip->table_number;i++)
            {
                if( ip->type[i] == 17 )
                {
                    for(int i=0;i<2;i++)
                    {
                        ip->handcard[ip->handcard_number] = Card[Card_position];
                        ip->handcard_number++;
                        Card_position++;
                    }
                }
            }
            Other_Prospector( ip , ip->n );
        }
    }
    else
    {
        printf( "Com%d is dazing , forgot to action ...\n" , ip->n );
    }
    return choose;
}

int32_t Champion( int32_t number )
{
    Type( ip );
    printf( "Game ended successly.\n" );
    for(int i=0;i<number;i++)
    {
        bool s1 = 0; //同業會館
        bool s2 = 0; //市政廳
        bool s3 = 0; //凱旋門
        bool s4 = 0; //宮殿
        for(int j=0;j<ip[i].table_number;j++)
        {
            ip[i].score += Card_Point[ip[i].table[j]];
            ip[i].score += ip[i].church_number;
            if( ip[i].type[j] == 21 )
            {
                s1 = 1;
            }
            if( ip[i].type[j] == 22 )
            {
                s2 = 1;
            }
            if( ip[i].type[j] == 23 )
            {
                s3 = 1;
            }
            if( ip[i].type[j] == 24 )
            {
                s4 = 1;
            }
        }
        for(int j=0;j<ip[i].church_number;j++)
        {
            ip[i].score++;
        }
        if( s1 )
        {
            for(int j=0;j<ip[i].table_number;j++)
            {
                if( special(ip[i].table[i]) == 0 )
                {
                    ip[i].score += 2;
                }
            }
        }
        if( s2 )
        {
            for(int j=0;j<ip[i].table_number;j++)
            {
                if( special(ip[i].table[i]) )
                {
                    ip[i].score ++;
                }
            }
        }
        if( s3 )
        {
            bool m1 = 0;
            bool m2 = 0;
            bool m3 = 0;
            for(int j=0;j<ip[i].table_number;j++)
            {
                if( ip[i].type[j] == 18 )
                {
                    m1 = 1;
                }
                if( ip[i].type[j] == 19 )
                {
                    m2 = 1;
                }
                if( ip[i].type[j] == 20 )
                {
                    m3 = 1;
                }

            }
            if( m1 + m2 + m3 == 3 )
            {
                ip[i].score += 8;
            }
            else if( m1 + m2 + m3 == 2 )
            {
                ip[i].score += 6;
            }
            else if( m1 + m2 + m3 == 1 )
            {
                ip[i].score += 4;
            }
        }
        if( s4 )
        {
            ip[i].score /= 4;
        }
    }
    int32_t max = 0;
    int32_t champion = 0;
    for(int i=0;i<number;i++)
    {
        printf( "Player%d : %d points.\n" , i+1 , ip[i].score );
        if( ip[i].score > max )
        {
            max = ip[i].score;
            champion = i;
        }
    }
    return champion;
}

int main() 
{
    int32_t number;
    int32_t mod;
    Introduce();
    printf("Please enter 2-4 players: ");
    if( scanf( "%d" , &number ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    while( number < 2 || number > 4 )
    {
        printf( "You input a wrong option.\n" );
        printf("Please enter 2-4 players: ");
        if( scanf( "%d" , &number ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
    }
    printf("If you don't have so much time, you can choose a shorter version.(8 cards end the game).\n" );
    printf( "[ Normal:any integer , Shorter:1 ] version : " );
    if( scanf( "%d" , &mod ) == 0 )
    {
        printf( "You input a string or a char.\n" );
        printf( "Game Over.\n" );
        exit(0);
    }
    if( mod == 1 )
    {
        mod = 8;
    }
    else
    {
        mod = 12;
    }

    printf( "Computer dificulty :\n" );
    
    for(int i=1;i<number;i++)
    {
        int32_t option = 0;
        printf( "Com%d [ Normal:any integer , Forgetful:1 ] : " , i );
        if( scanf( "%d" ,&option ) == 0 )
        {
            printf( "You input a string or a char.\n" );
            printf( "Game Over.\n" );
            exit(0);
        }
        if( option == 1 )
        {
            ip[i].difficulty = 0;
        }
        else
        {
            ip[i].difficulty = 1;
        }
    }
    Name( number );

    srand( time(NULL) );
    int32_t Governor = (rand()%(number-1)) + 0;
    memset( state_g , 0 , 4 );
    state_g[Governor] = 1;
    memset( state , 1 , 5 );
    Shuffle( number );
    
    Print_surface( ip );
    int32_t max = 0;
    int32_t behave = 0;
    int32_t last = 110;
    int32_t temp = last;
    while( max < mod )
    {
        printf( B_G"******************* START THIS ROUND!  *******************\n"NC );
        for(int i=0;i<Card_D_p;i++)
        {
            //printf( "%d " , Card_Dispose[i] );
        }
        if( Card_D_p >= 18 )
        {
            temp = Shuffle_again( last );
            last = temp;
            for(int i=0;i<last;i++)
            {
                //printf( "[%d] %d\n" , i ,Card[i] );
            }
        }
        printf( "\n" );
        if( Governor == 0 )
        {
            bool stop = 0;
            behave = Your_Role( &ip[0] );
            if( behave == 6 )
            {
                break;
            }
            for(int j=1;j<number;j++)
            {
                if( Other_behave( &ip[j] , behave , &ip[0] , &ip[Governor] ) )
                {
                    stop = 1;
                    break;
                }
            }
            if( stop )
            {
                break;
            }
            Print_surface( ip );
            for(int i=1;i<number;i++)
            {
                behave = Other_Role( &ip[i] );
                if( behave == 6 )
                {
                    stop = 1;
                    break;
                }
                if( Your_behave( &ip[0] , behave , &ip[i] , &ip[Governor] ) )
                {
                    stop = 1;
                    break;
                }
                for(int j=1;j<number;j++)
                {
                    if( j != i )
                    {
                        if( Other_behave( &ip[j] , behave , &ip[i] , &ip[Governor] ) )
                        {
                            stop = 1;
                            break;
                        }
                    }
                }
                if( stop )
                {
                    break;
                }
                Print_surface( ip );                 
            }
            if( stop )
            {
                break;
            }
            Check_for_every_turn( number );
            Governor++;
        }
        else if( Governor == 1 )
        {
            bool stop = 0;
            for(int i=1;i<number;i++)
            {
                behave = Other_Role( &ip[i] );
                if( behave == 6 )
                {
                    stop = 1;
                    break;
                }
                if( Your_behave( &ip[0] , behave , &ip[i] , &ip[Governor] ) )
                {
                    stop = 1;
                    break;
                }
                for(int j=1;j<number;j++)
                {
                    if( j != i )
                    {
                        if( Other_behave( &ip[j] , behave , &ip[i] , &ip[Governor] ) )
                        {
                            stop = 1;
                            break;
                        }
                    }
                }
                if( stop )
                {
                    break;
                }
                Print_surface( ip );
            }
            if( stop )
            {
                break;
            }
            behave = Your_Role( &ip[0] );
            if( behave == 6 )
            {
                break;
            }
            for(int j=1;j<number;j++)
            {
                if( Other_behave( &ip[j] , behave , &ip[0] , &ip[Governor] ) )
                {
                    stop = 1;
                    break;
                }
            }
            if( stop )
            {
                break;
            }
            Print_surface( ip );
            Check_for_every_turn( number );
            Governor++;
        }
        else if( Governor == 2 )
        {
            bool stop = 0;
            for(int i=2;i<number;i++)
            {
                behave = Other_Role( &ip[i] );
                if( behave == 6 )
                {
                    stop = 1;
                    break;
                }
                if( Your_behave( &ip[0] , behave , &ip[i] , &ip[Governor] ) )
                {
                    stop = 1;
                    break;
                }
                for(int j=1;j<number;j++)
                {
                    if( j != i )
                    {
                        if( Other_behave( &ip[j] , behave , &ip[i] , &ip[Governor] ) )
                        {
                            stop = 1;
                            break;
                        }
                    }
                }
                if( stop )
                {
                    break;
                }
                Print_surface( ip );
            }
            if( stop )
            {
                break;
            }
            behave = Your_Role( &ip[0] );
            if( behave == 6 )
            {
                break;
            }
            for(int j=1;j<number;j++)
            {
                if( Other_behave( &ip[j] , behave , &ip[0] , &ip[Governor] ) )
                {
                    stop = 1;
                    break;
                }
            }
            if( stop )
            {
                break;
            }
            Print_surface( ip );
            behave = Other_Role( &ip[1] );
            if( behave == 6 )
            {
                break;
            }
            Your_behave( &ip[0] , behave , &ip[1] , &ip[Governor] );
            for(int j=2;j<number;j++)
            {
                if( Other_behave( &ip[j] , behave , &ip[1] , &ip[Governor] ) )
                {
                    stop = 1;
                    break;
                }
            }
            if( stop )
            {
                break;
            }
            Print_surface( ip );
            Check_for_every_turn( number );
            Governor++;
        }
        else 
        {
            bool stop = 0;
            behave = Other_Role( &ip[3] );
            if( behave == 6 )
            {
                break;
            }
            Your_behave( &ip[0] , behave , &ip[3] , &ip[Governor] );
            for(int j=1;j<3;j++)
            {
                if( Other_behave( &ip[j] , behave , &ip[3] , &ip[Governor] ) )
                {
                    stop = 1;
                    break;
                }
            }
            if( stop )
            {
                break;
            }
            Print_surface( ip );
            behave = Your_Role( &ip[0] );
            if( behave == 6 )
            {
                break;
            }
            for(int j=1;j<number;j++)
            {
                if( Other_behave( &ip[j] , behave , &ip[0] , &ip[Governor] ) )
                {
                    stop = 1;
                    break;
                }
            }
            if( stop )
            {
                break;
            }
            Print_surface( ip );
            for(int i=1;i<3;i++)
            {
                behave = Other_Role( &ip[i] );
                if( behave == 6 )
                {
                    stop = 1;
                    break;
                }
                if( Your_behave( &ip[0] , behave , &ip[i] , &ip[Governor] ) )
                {
                    stop = 1;
                    break;
                }
                for(int j=1;j<number;j++)
                {
                    if( j != i )
                    {
                        if( Other_behave( &ip[j] , behave , &ip[i] , &ip[Governor] ) )
                        {
                            stop = 1;
                            break;
                        }
                    }
                }
                if( stop )
                {
                    break;
                }
                Print_surface( ip );
            }      
            if( stop )
            {
                break;
            }
            Check_for_every_turn( number );  
            Governor = 0;
        }
        memset( state_g , 0 , number );
        state_g[Governor] = 1;
        memset( state , 1 , 5 );
        for(int i=0;i<number;i++)
        {
            for(int j=0;j<strlen(ip[i].role);j++)
            {
                ip[i].role[j] = ' ';
            }
        }
        for(int i=0;i<number;i++)
        {
            if( ip[i].table_number > max )
            {
                max = ip[i].table_number;
            }
        }
    }
    printf( "Result is .....\n" );
    sleep(1);
    printf( RED"Champion : Player %d!\n"NC , Champion(number) );
    return 0;
}
