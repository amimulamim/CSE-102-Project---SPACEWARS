#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<windows.h>
#include<MMSystem.h>


#define maxenemynum 20
#define maxbulletvelocity 8
#define maxenemyvelocity 4

int l=1400, w=750;
int myx=100, myy=w/2;
int my_missile_x[10000000], my_missile_y[10000000], n=0, m1=1;
int i, j;
int resumestate=0;
int musflag=1;
int resetstate=0;
//int enemyshot=0;

int lives=5, point=0;
FILE *fp;
FILE *fscorename;

int my_missile_velocity=20;
int bulletvelocity=5;
int enemyvelocity=2;


int planewidth=150, myplaneht=40;
int eplanewidth=110, eplaneht=25;
int bottomht=50;

int backcordx=0,backcordy=0;

int eachbgwidth=100;

bool musicon=true;
char bg[6][20]= {"front.bmp","gameoverss.bmp","gamestartbg.bmp","gamebg6.bmp","skyfighter2.bmp","space.bmp"};


int bgind=0;
//char upbgimage[10][20]= {"bg1.bmp","bg2.bmp","bg3.bmp","bg4.bmp","bg5.bmp","bg6.bmp","bg7.bmp","bg8.bmp","bg9.bmp","bg10.bmp"};
//int bgspeed=6;

char button[10][15]= {"resume.bmp","start.bmp","help.bmp","sound_off.bmp","credits.bmp","sound_on.bmp"};
char button2[10][15];
char homemenu[25];
char play[25];
char highscore[25];
char ins[25];


char enemypic[10][20]= {"Enemy1.bmp","Enemy2.bmp","Enemy3.bmp","Enemy4.bmp","Enemy5.bmp","Enemy6.bmp","Enemy7.bmp","Enemy8.bmp","Enemy9.bmp","Enemy10.bmp"};

int gamestate=-1;

struct buttoncord
{
    int x;
    int y;
} bcord[5];
struct resumecord
{
    int x;
    int y;
} rcord[5];

struct background
{
    int x;
    int y;
};
background upbg[10],lowbg[10];

typedef struct
{
    char name[100];
    int score;

}players;


char playername[80];
int nameind=0;


int enemynumber=3;

struct coord
{
    int x;
    int y;
};

typedef struct
{
    int ex;
    int ey;
    int eind;
    bool eshow;
    coord e_bullet[100000];
    int enemyshot;
    int enemytime;
    int collide;


} enemies;

enemies e[20];

coord name_text,name_box;
coord rankbutton;

int hall_of_fame_called=0;
int name_file_opened=0;


int saved=0;
coord savebutton;
coord doexit;


int showrank=0;


int enemyflag=500;


FILE *fileofname;




void trimLeading(char * str)
{
    int index, i, j;

    index = 0;

    /* Find last index of whitespace character */
    while(str[index] == ' ' || str[index] == '\t' || str[index] == '\n')
    {
        index++;
    }


    if(index != 0)
    {
        /* Shift all trailing characters to its left */
        i = 0;
        while(str[i + index] != '\0')
        {
            str[i] = str[i + index];
            i++;
        }
        str[i] = '\0'; // Make sure that string is NULL terminated
    }
}

int firsttime=1;
int write_successfully=0;



void hall_of_fame()
{
    int no_of_data;

    hall_of_fame_called=1;
    FILE *database;
   database=fopen("hall_of_fame.txt","r");

    if(database==NULL){
        printf("database error\n");
        exit(1);
    }


    fscanf(database,"%d",&no_of_data);

    no_of_data++;



    players *player_data=(players*)malloc(no_of_data*sizeof(players));
    int readsuccessfully=0;

    for(int it=0; it<no_of_data-1; it++)
    {

        fscanf(database,"%d",&player_data[it].score);
        fgets(player_data[it].name,99,database);
        trimLeading(player_data[it].name);

        if(feof(database) || player_data[it].score<=0 )
        {   printf("eof\n");
            break;

        }
        readsuccessfully++;

    }



    fclose(database);
    if(no_of_data==readsuccessfully+1)
    {

         player_data[no_of_data-1].score=0;
    }
    else
    {
        no_of_data=readsuccessfully+1;
        player_data[no_of_data-1].score=0;

    }


    char nameofplayer[100];
    FILE *file1=fopen("name.txt","r");
    int si=0;
    if(file1==NULL)
    {
        printf("error reading name file\n");
    }
    else
    {
            do
            {
                char key=fgetc(file1);
                if(feof(file1))
                {
                    fclose(file1);
                    break;
                }


                if(key!='\b')
                {
                    nameofplayer[si]=key;

                    si++;
                    nameofplayer[si]='\0';
                }


                else if(key=='\b')
                {

                    if(si<=0)
                    {
                        si=0;
                    }
                    else
                    {
                        si--;
                        nameofplayer[si]='\0';
                    }
                }


            }
            while(1);

            trimLeading(nameofplayer);
            if(nameofplayer[0]=='\0')
            {
            strcpy(nameofplayer,"UNKNOWN PLAYER");
            }



        }
        trimLeading(nameofplayer);

        fclose(file1);




    for(int it=0; it<no_of_data; it++)
    {
        if(point>=player_data[it].score)
        {


            for(int jt=no_of_data-1; jt>it; jt--)
            {
                player_data[jt].score=player_data[jt-1].score;
                strcpy(player_data[jt].name,player_data[jt-1].name);
                trimLeading(player_data[jt].name);
            }
            player_data[it].score=point;
            strcpy(player_data[it].name,nameofplayer);
            trimLeading(player_data[it].name);


            break;
        }


    }





    FILE *newdata=fopen("hall_of_fame.txt","w");
    if(newdata==NULL){
        printf("error opening newdata file to write\n");
        exit(1);
    }

    fprintf(newdata,"%d\n",no_of_data);
    if(no_of_data==1)
    {
        player_data[0].score=point;
        strcpy(player_data[0].name,nameofplayer);

    }
    for(int it=0; it<no_of_data; it++)
    {
        trimLeading(player_data[it].name);
        if((player_data[it].score>0 && player_data[it].score<=100000)&&((player_data[it].name[0]>='A' && player_data[it].name[0]<='Z')||(player_data[it].name[0]>='a' && player_data[it].name[0]<='z') || (player_data[it].name[0]>='0' && player_data[it].name[0]<='9') ))
        {


        fprintf(newdata,"%d %s\n",player_data[it].score,player_data[it].name);

        }
        else{
            break;
        }
    }



    fclose(newdata);


    free(player_data);
}





void my_bullet_movement(void)
{
    for(i=0; i<n; i++)
    {
        my_missile_x[i]+=my_missile_velocity;
    }

}



int f2called=100;
void enemy_movement(void)
{


    if(point/f2called>0)
    {
        f2called*=2;
        if(enemynumber<maxenemynum)
        {
            enemynumber=enemynumber+1;
        }
    }

    int j=0;

    for (i=0; i<enemynumber; i++)
    {
        if(e[i].ex<=1-eplanewidth)
        {
            e[i].collide=0;
            e[i].enemyshot=0;
            e[i].enemytime=0;
            e[i].ex=l+10;
            e[i].ey=(rand()%(w-eplaneht-bottomht))+bottomht;
            for (j=0; j<enemynumber; j++)
            {

                if(j!=i && e[i].ex>=e[j].ex-2*eplanewidth && e[i].ex<=(e[j].ex+2*eplanewidth) && e[i].ey>=e[j].ey-2*eplaneht && e[i].ey<=(e[j].ey+2*eplaneht))
                {

                    e[i].ey=rand()%(w-eplaneht-bottomht)+bottomht;
                    j=0;



                }
            }

        }
        else
        {



            e[i].ex=e[i].ex-(enemyvelocity);
            if(point>enemyflag){
                if(enemyvelocity<maxenemyvelocity){
                    enemyvelocity++;

                }

                if(bulletvelocity<maxbulletvelocity)

                {
                    bulletvelocity++;
                }

                enemyflag*=2;
            }


        }
    }
}

void enemy_bullet_movement(void)
{
    for(int ind=0; ind<enemynumber; ind++)
    {
        for(i=0; i<m1; i++)
        {


            e[ind].e_bullet[i].x=e[ind].e_bullet[i].x-bulletvelocity;

        }
    }
}


void enemy_bullet_position(void)
{
    for (int ind=0; ind<enemynumber; ind++)
    {
        if(e[ind].ey>=myy-200 && e[ind].ey<=myy+200)
        {
            if(e[ind].e_bullet[m1-1].x<=5)
            {

                e[ind].e_bullet[m1-1].x=e[ind].ex;
                e[ind].e_bullet[m1-1].y=e[ind].ey+eplaneht-5;

            }
        }
    }
}




int gameover=0;
int gulikhaise=0;
int dhakkalgse=0;


int aftercollision=0;

int highest_score;
int ishighest=0;



void collision(void)
{
    if(gulikhaise==1 || dhakkalgse==1)
    {

        aftercollision++;

    }

    for (int ind=0; ind<enemynumber; ind++)

    {
        if(e[ind].enemyshot==1 || e[ind].collide==1)
        {
            e[ind].enemytime++;
        }

        for(i=0; i<m1; i++)
        {

            if(e[ind].e_bullet[i].x<myx+planewidth && e[ind].e_bullet[i].x>myx+5 && e[ind].e_bullet[i].y>=myy+10 && e[ind].e_bullet[i].y<myy+myplaneht+10)
            {


                lives-=1;
                iSetColor(rand()%256,rand()%256,rand()%256);

                e[ind].e_bullet[i].x=-15;
                gulikhaise=1;
                iSetColor(rand()%255,rand()%255,rand()%255);

                if(musicon==true)
                PlaySound("fail.wav",NULL,SND_ASYNC);





            }
            else if(e[ind].ex<myx+planewidth-40 && e[ind].ex>myx-eplanewidth/2 && e[ind].ey>myy+myplaneht/2 && e[ind].ey<myy+myplaneht)
            {
                lives-=1;




                point++;
                dhakkalgse=1;
                e[ind].collide=1;
                e[ind].ex=myx-eplanewidth-10;
                if(musicon==true)
                PlaySound("fail.wav",NULL,SND_ASYNC);
            }
            else if(e[ind].ex<myx+planewidth && e[ind].ex>myx-eplanewidth+10 && e[ind].ey>myy-eplaneht+10 && e[ind].ey<myy+myplaneht/2)
            {
                lives-=1;




                point++;
                dhakkalgse=1;
                e[ind].collide=1;
                e[ind].ex=myx-eplanewidth-10;
                if(musicon==true)
                PlaySound("fail.wav",NULL,SND_ASYNC);
            }
            else if(e[ind].ex<myx+planewidth/2 && e[ind].ex>myx-eplanewidth+10 && e[ind].ey>myy-eplaneht && e[ind].ey<myy-eplaneht+10)

            {
                lives-=1;


                point++;
                dhakkalgse=1;
                e[ind].collide=1;
                e[ind].ex=myx-eplanewidth-10;

                if(musicon==true)
                PlaySound("fail.wav",NULL,SND_ASYNC);
            }
        }

    }

    if(lives<=0)
    {
        iPauseTimer(0);
        iPauseTimer(1);
        iPauseTimer(2);
        iPauseTimer(3);
        iPauseTimer(4);
           gameover=1;

        fp=fopen("highscore.txt","r");
        if(fp==NULL)
            printf("Cannot open file");
        else
        {
            fscanf(fp,"%d",&highest_score);
            fclose(fp);
            printf("prev highest score=%d\n",highest_score);
            if(point>=highest_score && ishighest==0)
            {

                FILE *high=fopen("highscore.txt","w");
                ishighest=1;
                fprintf(fp,"%d\n",point);
                highest_score=point;
                printf("thisishighestscore\n");
                fclose(high);
            }
            itoa(highest_score,highscore,10);

        }


    }

    for(int ind=0; ind<enemynumber; ind++)
    {
        for( i=0; i<n; i++)
        {
            if(my_missile_x[i]>(e[ind].ex-5) && my_missile_x[i]<(e[ind].ex+eplanewidth) && my_missile_y[i]>(e[ind].ey-10) && my_missile_y[i]<(e[ind].ey+eplaneht))
            {
                if(e[ind].enemyshot==0)
                    point+=50;


                my_missile_x[i]=l+1500;
                e[ind].enemyshot=1;
                if(musicon==true)
                PlaySound("dead.wav",NULL,SND_ASYNC);



            }
        }
    }

}





char score_point[20];
char love[4];


void iDraw()
{
    iClear();

    if(gamestate!=-1)
    {
        if(musicon)
        {
            iShowBMP(l/8,0,"soundison.bmp");
        }
        else if(musicon==false)
        {
            iShowBMP(l/8,0,"soundisoff.bmp");
        }
    }
    if(gamestate==-1)
    {

        if(musflag==1)
        {
            if(musicon==true)
                PlaySound("tense.wav", NULL,SND_ASYNC|SND_LOOP);
            else
                PlaySound(NULL,NULL,SND_SYNC);
            musflag=0;
        }
        iShowBMP(0,0,bg[bgind]);
        iShowBMP2(l/3-150,w/2+120,"logo2.bmp",0);


        for (int i=1; i<5; i++)
        {

            if(i!=3)
                iShowBMP2(bcord[i].x,bcord[i].y,button[i],0);
            else
            {
                if(musicon==true)
                    iShowBMP2(bcord[3].x,bcord[3].y,button[3],0);
                else
                {
                    iShowBMP2(bcord[3].x,bcord[3].y,button[5],0);

                }


            }
        }

        iShowBMP(rankbutton.x,rankbutton.y,"ranking.bmp");


        if(resumestate==2)
        {
            iShowBMP2(bcord[0].x,bcord[0].y,button[0],0);
        }


        iPauseTimer(0);
        iPauseTimer(1);
        iPauseTimer(2);
        iPauseTimer(3);

        //iShowBMP(l-150,bottomht-20,"exitgame.bmp");
        doexit.x=l-150;
        doexit.y=bottomht-50;

        iShowBMP(doexit.x,doexit.y,"exitgame.bmp");


    }


    else if(gamestate==0)
    {
        iResumeTimer(1);
        iResumeTimer(2);
        iResumeTimer(3);

        iSetColor(255,rand()%255,rand()%255);

        itoa(point,score_point,10);
        itoa(lives,love,10);
        iText(l/2,bottomht/2,"SCORE:",GLUT_BITMAP_TIMES_ROMAN_24);
        iText(l/2+100,bottomht/2,score_point,GLUT_BITMAP_TIMES_ROMAN_24);
        iText(l/4,bottomht/2,"LIVES:",GLUT_BITMAP_HELVETICA_18);
        iText(l/4+80,bottomht/2,love,GLUT_BITMAP_HELVETICA_18);
        iShowBMP(0,bottomht,"mohakash.bmp");
        if(resumestate==1)
        {
            iShowBMP2(l/4,w/2-100,"quit.bmp",0);
            iPauseTimer(0);
            iPauseTimer(1);
            iPauseTimer(2);
            iPauseTimer(3);

        }

        if(resumestate==0 && gameover==0)
        {
            if(gulikhaise==0 && dhakkalgse==0)
            {

                iShowBMP2(myx,myy,"Alien.bmp",0);


                for(i=0; i<n; i++)
                {
                    iFilledRectangle(my_missile_x[i],my_missile_y[i],5,5);
                }
            }
            else if(gulikhaise==1 || dhakkalgse==1)
            {


                iShowBMP2(myx,myy,"Alien.bmp",0);
                iShowBMP2(myx+35,myy+myplaneht-10,"planeagunnn.bmp",255);

                for(i=0; i<n; i++)
                {
                    iFilledRectangle(my_missile_x[i],my_missile_y[i],5,5);
                }

                if(aftercollision>=50)
                {

                    gulikhaise=0;
                    dhakkalgse=0;
                    aftercollision=0;
                }
            }
            iSetColor(0,0,255);


            for (i=0; i<enemynumber; i++)
            {

                if(e[i].enemytime>=50&& e[i].enemyshot==1)
                {
                    e[i].ex=l+10;
                    e[i].ey=rand()%(w-eplaneht-bottomht)+bottomht;

                    for (j=0; j<enemynumber; j++)
                    {

                        if(j!=i && e[i].ex>=e[j].ex-2*eplanewidth && e[i].ex<=(e[j].ex+2*eplanewidth) && e[i].ey>=e[j].ey-2*eplaneht && e[i].ey<=(e[j].ey+2*eplaneht))
                        {

                            e[i].ey=rand()%(w-eplaneht-bottomht)+bottomht;
                            j=0;




                        }
                    }


                    e[i].enemytime=0;
                    e[i].enemyshot=0;
                    e[i].collide=0;
                }

                if(e[i].enemytime>=50&& e[i].collide==1)
                {
                    e[i].ex=l+100;
                    e[i].ey=rand()%(w-eplaneht-bottomht)+bottomht;

                    for (j=0; j<enemynumber; j++)
                    {

                        if(j!=i && e[i].ex>=e[j].ex-2*eplanewidth && e[i].ex<=(e[j].ex+2*eplanewidth) && e[i].ey>=e[j].ey-2*eplaneht && e[i].ey<=(e[j].ey+2*eplaneht))
                        {

                            e[i].ey=rand()%(w-eplaneht-bottomht)+bottomht;

                            j=0;

                        }
                    }





                    e[i].enemytime=0;
                    e[i].enemyshot=0;
                    e[i].collide=0;
                }

                int no=rand()%10;


                iShowBMP2(e[i].ex,e[i].ey,enemypic[no],0);

                if((e[i].enemyshot==1 || e[i].collide==1) && e[i].enemytime<=50 && e[i].ex>0)
                {
                    iShowBMP2(e[i].ex+30,e[i].ey+12,"planeagunnn.bmp",255);

                }

            }

            iSetColor(255,100,0);
            for(int ind=0; ind<enemynumber; ind++)
            {
                for(i=0; i<m1; i++)
                {
                    iFilledRectangle(e[ind].e_bullet[i].x, e[ind].e_bullet[i].y-eplaneht/2-5,5,5);

                }
            }

        }
         if(resetstate==1)
            {


                n=0;
                m1=1;
                lives=5;
                point=0;
                f2called=100;
                saved=0;
                nameind=0;
                iResumeTimer(4);
                myx=100;
                myy=w/2;
                showrank=0;


                    bulletvelocity=5;
                    enemyvelocity=2;
                    name_box.x=3*l/4;
                    name_box.y=bottomht+200;
                    name_text.x=name_box.x+20;
                    name_text.y=name_box.y+5;

                    savebutton.x=l-230;
                    savebutton.y=bottomht-20;
                    playername[0]='\0';
                    hall_of_fame_called=0;
                    name_file_opened=0;





                for(i=0; i<enemynumber; i++)
                {
                    e[i].collide=0;
                    e[i].enemyshot=0;
                    e[i].enemytime=0;
                    e[i].ex=l+10;
                    e[i].ey=(rand()%(w-eplaneht-bottomht))+bottomht;

                    for (j=0; j<enemynumber; j++)
                    {

                        if(j!=i && e[i].ex>=e[j].ex-2*eplanewidth && e[i].ex<=(e[j].ex+2*eplanewidth) && e[i].ey>=e[j].ey-2*eplaneht && e[i].ey<=(e[j].ey+2*eplaneht))
                        {

                            e[i].ey=rand()%(w-eplaneht-bottomht)+bottomht;

                            j=0;

                        }
                    }


                }
                gameover=0;

                gulikhaise=0;
                dhakkalgse=0;
                enemynumber=3;

                aftercollision=0;
                ishighest=0;

                saved=0;
                nameind=0;
                resetstate=0;
                for (int ind=0; ind<enemynumber; ind++)
                {


                    e[ind].e_bullet[m1-1].x=e[ind].ex;
                    e[ind].e_bullet[m1-1].y=e[ind].ey+eplaneht-5;



                }
            }

        iSetColor(rand()%256,rand()%256,rand()%256);
        if(gameover==1)
        {
            iShowBMP(0,0,bg[1]);

            iSetColor(255,50,20);

            iText(l/3-250,w/2-100,"YOUR SCORE IS",GLUT_BITMAP_TIMES_ROMAN_24);
            iText(l/3+150,w/2-100,score_point,GLUT_BITMAP_TIMES_ROMAN_24);
            iText(l/3-250,w/2-200,"HIGHEST SCORE:",GLUT_BITMAP_TIMES_ROMAN_24);
            iText(l/3+150,w/2-200,highscore,GLUT_BITMAP_TIMES_ROMAN_24);

            iSetColor(255,rand()%200,rand()%50);
            iText(name_box.x-150,name_box.y+5,"Player's Name: ");
            iSetColor(255,255,255);
            iFilledRectangle(name_box.x,name_box.y,300,25);

            iSetColor(0,0,0);
            iText(name_text.x,name_text.y,playername);

            if(ishighest==1)
            {
                iText(l/2-200,w/2-300,"CONGRATULATIONS!!YOU HAVE SCORED HIGHEST");
            }

            iShowBMP(savebutton.x,savebutton.y,"savebutton.bmp");
            if(saved==1 && hall_of_fame_called==0)
            {
                fclose(fileofname);
                hall_of_fame_called=1;
                name_box.x=l+200;
                name_text.x=l+200;
                savebutton.x=l+200;
                hall_of_fame();

            }
            doexit.x=l-150;
            doexit.y=bottomht-20;

            iShowBMP(doexit.x,doexit.y,"exitgame.bmp");

        }
        if(gameover==0 || gamestate!=0)

        {
            doexit.x=l-150;
            doexit.y=bottomht-50;

            iShowBMP(doexit.x,doexit.y,"exitgame.bmp");
        }

    }


    else if(gamestate==1)
    {
        iShowBMP(0,0,"helpingbg1.bmp");
        iSetColor(rand()%255,rand()%255,rand()%255);

        iText(l/3,w-200,"Instructions to play this game: ",GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(255,255,0);
        iText(l/3,w-300,"Use Keyboard Up,Down,Left,Right arrow buttons to move your plane",GLUT_BITMAP_HELVETICA_18);
        iText(l/3,w-350,"Use backspace to pause the game",GLUT_BITMAP_HELVETICA_18);
        iText(l/3,w-400,"Use SPACE key to shoot enemy",GLUT_BITMAP_HELVETICA_18);


        iSetColor(0,255,0);
        iText(l/3,w-500,"Use mouse to click menu",GLUT_BITMAP_TIMES_ROMAN_24);

        doexit.x=l-150;
        doexit.y=bottomht-50;

        iShowBMP(doexit.x,doexit.y,"exitgame.bmp");

    }
    else if(gamestate==3){
            iShowBMP(0,0,"bgsolid.bmp");
                iSetColor(255,100,20);
        iText(l/3+50,w-250,"DEVELOPER:",GLUT_BITMAP_TIMES_ROMAN_24);
        iText(l/3,w-350,"AL MUHIT MUHTADI",GLUT_BITMAP_TIMES_ROMAN_24);
        iText(l/3-100,w-400,"ABDULLAH MUHAMMED AMIMUL EHSAN",GLUT_BITMAP_TIMES_ROMAN_24);

                iShowBMP(doexit.x,doexit.y,"exitgame.bmp");


    }

    if(gamestate==4){
            iSetColor(110,47,43);
            iShowBMP(0,0,"leaderboard_image.bmp");

        iText(950,700,"LEADERBOARD",GLUT_BITMAP_TIMES_ROMAN_24);

        iSetColor(0,50,255);
        iText(490,600,"NAME",GLUT_BITMAP_TIMES_ROMAN_24);
        iText(1120,600,"SCORE",GLUT_BITMAP_TIMES_ROMAN_24);



        FILE *frank=fopen("hall_of_fame.txt","r");
        if(frank==NULL){
            printf("error leaderboard\n");
            exit(1);
        }
            int num;
            char rankname[100],rankscore[10];
            int rankpoint;
            fscanf(frank,"%d",&num);
            int ranky=520;
            int delranky=100;
            iSetColor(117,22,63);

            if(num<5){
                    for(int z=0;z<num;z++){
                        fscanf(frank,"%d",&rankpoint);
                        if(feof(frank)){

                            break;
                        }
                        itoa(rankpoint,rankscore,10);
                        fgets(rankname,99,frank);

                        if(feof(frank)){

                            break;
                        }
                        trimLeading(rankname);
                        trimLeading(rankscore);

                        iText(450,ranky,rankname,GLUT_BITMAP_TIMES_ROMAN_24);
                        iText(1120,ranky,rankscore,GLUT_BITMAP_TIMES_ROMAN_24);
                        ranky=ranky-delranky;

                    }


            }

            else{


                for(int z=0;z<5;z++){
                        fscanf(frank,"%d",&rankpoint);
                        if(feof(frank)){

                            break;
                        }
                        itoa(rankpoint,rankscore,10);
                        fgets(rankname,99,frank);

                        if(feof(frank)){

                            break;
                        }


                   trimLeading(rankname);
                        trimLeading(rankscore);

                        iText(450,ranky,rankname,GLUT_BITMAP_TIMES_ROMAN_24);
                        iText(1120,ranky,rankscore,GLUT_BITMAP_TIMES_ROMAN_24);


                        ranky=ranky-delranky;
                    }

            }
            fclose(frank);
            showrank=0;


         iShowBMP(doexit.x,doexit.y,"exitgame.bmp");

    }

    if(gamestate!=(-1))
    {
        iShowBMP2(backcordx,backcordy,"back.bmp",0);

    }


}

void iMouseMove(int mx,int my)
{

}







void iKeyboard(unsigned char key)
{
    if(key==' ' && gamestate==0 && gameover==0)
    {
        iResumeTimer(0);
        n++;
        my_missile_x[n-1]=myx+planewidth;
        my_missile_y[n-1]=myy+myplaneht/2-10;
        if(musicon==true)
            PlaySound("gunshot.wav", NULL,SND_ASYNC);
    }
    if(key=='\b' && gamestate==0 && gameover==0)
    {
        resumestate=1;
    }


    if(gameover==1)
    {


        if(name_file_opened==0)
        {
            fileofname=fopen("name.txt","w");
            name_file_opened=1;
        }


        if(saved==0)
        {
            if(key!='\b')
            {
                if(((key>='A' && key<='Z')||(key>='a' && key<='z') || (key>='0' && key<='9')||(key==' ') ))
                {
                    playername[nameind]=key;
                    fputc(key,fileofname);

                    nameind++;
                    playername[nameind]='\0';

                    if(musicon==true)
                    {

                        PlaySound("select.wav",NULL,SND_ASYNC);
                    }
                    }
                }



            else if(key=='\b')
            {
                fputc(key,fileofname);
                if(musicon==true)
                {

                    PlaySound("select.wav",NULL,SND_ASYNC);
                }

                if(nameind<=0)
                {
                    nameind=0;
                }
                else
                {
                    nameind--;
                    playername[nameind]='\0';
                }
            }
        }





    }




}


void iMouse(int button,int state,int mx,int my)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        for ( i=0; i<5; i++)
        {
            if(i!=3 && i!=0)
            {

                if(mx>=bcord[i].x+1 && mx<=(bcord[i].x+149)&& my>=bcord[i].y+1 && my<=bcord[i].y+30)
                {

                    if(gamestate==-1)
                    {
                        gamestate=i-1;
                        if(i==1)
                        {
                            resetstate=1;
                            resumestate=0;
                        }
                        if(musicon==true)
                            PlaySound("select.wav", NULL,SND_ASYNC);
                    }
                }
            }
            else if(i==3)
            {
                if(mx>=bcord[i].x+1 && mx<=(bcord[i].x+149)&& my>=bcord[i].y+1 && my<=bcord[i].y+30)
                {
                    if(musicon==true)
                    {
                        musicon=false;
                        PlaySound("select.wav", NULL,SND_ASYNC);
                    }
                    else
                    {
                        musicon=true;

                    }
                    musflag=1;
                }

            }
            else
            {
                if(mx>=bcord[0].x+1 && mx<=(bcord[0].x+149)&& my>=bcord[0].y+1 && my<=bcord[0].y+30)
                {

                    gamestate=0;
                    resumestate=0;
                    if(musicon==true)
                        PlaySound("select.wav", NULL,SND_ASYNC);
                }
            }


        }

        for ( i=0; i<2; i++)
        {
            if(mx>=rcord[i].x && mx<=(rcord[i].x+175)&& my>=rcord[i].y && my<=rcord[i].y+65)
            {
                if(gamestate==0 && resumestate==1)
                {
                    gamestate=i-1;
                    resumestate=0;

                    if(musicon==true)
                        PlaySound("select.wav", NULL,SND_ASYNC);
                }
            }


        }


        if((gameover==1 || gamestate!=0) && mx>=doexit.x && mx<=(doexit.x+135) && my>=(doexit.y) && my<=(doexit.y+50) )
        {
            if(musicon==true)
                PlaySound("select.wav", NULL,SND_SYNC);
            exit(0);
        }

        if(musicon==true && mx>=l/8 && mx<=(l/8+50) && my<=50 &&my>0)
        {
            musicon=false;
            musflag=1;
        }

        else if(musicon==false && mx>=l/8 && mx<=(l/8+50) && my<=50 && my>0)
        {
            musicon=true;
            musflag=1;
        }

        if(gameover==0 && gamestate==0 && mx>=l-150 && mx<=(l-15) && my>=(bottomht-50) && my<=(bottomht))
        {
            resumestate=1;
            iShowBMP2(l/4,w/2-100,"quit.bmp",0);
            iPauseTimer(0);
            iPauseTimer(1);
            iPauseTimer(2);
            iPauseTimer(3);

            if(musicon==true)
                PlaySound("select.wav", NULL,SND_ASYNC);


        }

        if((gameover==1 || gamestate!=0) && mx>=savebutton.x && mx<=(savebutton.x+47) && my>=(savebutton.y) && my<=(savebutton.y+50)&& saved==0)
        {
            saved=1;
            if(musicon==true)
                PlaySound("select.wav", NULL,SND_ASYNC);
            savebutton.x=l+200;


        }
        if(gamestate!=-1 && mx>=backcordx && mx<=backcordx+50 && my>=backcordy && my<=backcordy+50)
        {
            if(gamestate==0)
            {
                resumestate=2;

            }
            gamestate=-1;
            if(musicon==true)
                PlaySound("select.wav", NULL,SND_SYNC);
        }
        if(gamestate==-1 && mx>=rankbutton.x && mx<=rankbutton.x+33 && my>=rankbutton.y && my<=rankbutton.y+33){
             if(musicon==true)
                PlaySound("select.wav", NULL,SND_ASYNC);
            showrank=1;
            gamestate=4;

             printf("showrank=%d\n",showrank);



        }

    }
    if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    {
        for ( i=0; i<5; i++)
        {
            if(i!=3 && i!=0)
            {

                if(mx>=bcord[i].x+1 && mx<=(bcord[i].x+149)&& my>=bcord[i].y+1 && my<=bcord[i].y+30)
                {

                    if(gamestate==-1)
                    {
                        gamestate=i-1;
                        if(i==1)
                        {
                            resetstate=1;
                            resumestate=0;
                        }
                        if(musicon==true)
                            PlaySound("select.wav", NULL,SND_ASYNC);
                    }

                }
            }
            else if(i==3)
            {
                if(mx>=bcord[i].x+1 && mx<=(bcord[i].x+149)&& my>=bcord[i].y+1 && my<=bcord[i].y+30)
                {
                    if(musicon==true)
                    {
                        musicon=false;

                        PlaySound("select.wav", NULL,SND_ASYNC);
                    }
                    else
                    {
                        musicon=true;

                    }
                    musflag=1;
                }

            }
            else
            {
                if(mx>=bcord[0].x+1 && mx<=(bcord[0].x+149)&& my>=bcord[0].y+1 && my<=bcord[0].y+30)
                {

                    gamestate=0;
                    resumestate=0;
                    PlaySound("select.wav", NULL,SND_ASYNC);
                }
            }


        }

        for ( i=0; i<2; i++)
        {
            if(mx>=rcord[i].x && mx<=(rcord[i].x+175)&& my>=rcord[i].y && my<=rcord[i].y+65)
            {
                if(gamestate==0 && resumestate==1)
                {
                    gamestate=i-1;
                    resumestate=0;
                    if(musicon==true)
                        PlaySound("select.wav", NULL,SND_ASYNC);
                }
            }


        }



        if((gameover==1 || gamestate!=0) && mx>=doexit.x && mx<=(doexit.x+135) && my>=(doexit.y) && my<=(doexit.y+50) )
        {
            if(musicon==true)
                PlaySound("select.wav", NULL,SND_SYNC);
            exit(0);
        }

        if(musicon && mx>=l/8 && mx<=(l/8+50) && my<=50)
        {
            musicon=false;
        }
        if(musicon==false && mx>=l/8 && mx<=(l/8+50) && my<=50)
        {
            musicon=true;
        }

        if(gameover==0 && gamestate==0 && mx>=l-150 && mx<=(l-15) && my>=(bottomht-50) && my<=(bottomht))
        {
            resumestate=1;
            iShowBMP2(l/4,w/2-100,"quit.bmp",0);
            iPauseTimer(0);
            iPauseTimer(1);
            iPauseTimer(2);
            iPauseTimer(3);

            if(musicon==true)
                PlaySound("select.wav", NULL,SND_ASYNC);


        }


        if((gameover==1 || gamestate!=0) && mx>=savebutton.x && mx<=(savebutton.x+47) && my>=(savebutton.y) && my<=(savebutton.y+50)&& saved==0)
        {
            saved=1;
            if(musicon==true)
                PlaySound("select.wav", NULL,SND_ASYNC);
            savebutton.x=l+200;


        }
        if(gamestate!=-1 && mx>=backcordx && mx<=backcordx+50 && my>=backcordy && my<=backcordy+50)
        {
            if(gamestate==0)
            {
                resumestate=2;

            }
            gamestate=-1;
            if(musicon==true)
                PlaySound("select.wav", NULL,SND_SYNC);
        }

        if(gamestate==-1 && mx>=rankbutton.x && mx<=rankbutton.x+33 && my>=rankbutton.y && my<=rankbutton.y+33){
             if(musicon==true)
                PlaySound("select.wav", NULL,SND_ASYNC);
            showrank=1;
            gamestate=4;
            printf("showrank=%d\n",showrank);



        }



    }
}


void iSpecialKeyboard(unsigned char key)
{
    if(gameover==1 && key==GLUT_KEY_END)
    {
        exit(0);
    }

    if(gameover==0 && gamestate==0 && resumestate==0)
    {
        if(key==GLUT_KEY_LEFT && myx>=5)
        {
            myx-=10;

        }
        if(key==GLUT_KEY_RIGHT && myx<=l-planewidth)
        {
            myx+=10;

        }

        if(key==GLUT_KEY_UP && myy<=w-myplaneht-10)
        {
            myy+=10;
        }
        if(key==GLUT_KEY_DOWN && myy>=bottomht)
        {
            myy-=10;
        }
    }

}





void setall()
{

    int sum=0;


    name_box.x=3*l/4;
    name_box.y=bottomht+200;
    name_text.x=name_box.x+20;
    name_text.y=name_box.y+5;

    savebutton.x=l-230;
    savebutton.y=bottomht-20;

    name_file_opened=0;
    hall_of_fame_called=0;
    showrank=0;




    int i;
    int summ=w/6;
    int increase=0;
    rankbutton.y=(w/9)-20;
    rankbutton.x=l/8+50;
    for (i=4; i>=0; i--)
    {
        bcord[i].x=l/8;
        bcord[i].y=summ;
        summ+=45;
    }

    for(i=0; i<2; i++)
    {
        rcord[i].x=l/4+60+increase;
        rcord[i].y=w/2-100;
        increase+=205;


    }
    for(j=0; j<100000; j++)
    {
        my_missile_x[j]=myx;
        my_missile_y[j]=myy;
    }

    enemy_movement();
    for(int ind=0; ind<enemynumber; ind++)

    {
        e[ind].collide=0;
        e[ind].enemyshot=0;
        e[ind].enemytime=0;

        for(j=0; j<10; j++)
        {
            e[ind].e_bullet[j].x=e[ind].ex;
            e[ind].e_bullet[j].y=e[ind].ey+5;
        }
    }


}



int main()
{

    setall();

    iSetTimer(1,my_bullet_movement);
    iPauseTimer(0);
    iSetTimer(10,enemy_movement);
    iSetTimer(1,enemy_bullet_movement);
    iSetTimer(2000,enemy_bullet_position);
    iSetTimer(1,collision);

    iInitialize(l,w,"SPACE WARS");


    return 0;
}
