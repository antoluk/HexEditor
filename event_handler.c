//
// Created by antoluk on 5.4.23.
//
#include "event_handler.h"

int letter=1;

int event()
{
   if(kbhit())
   {
       int key=getch();

       switch (key) {
           case KEY_DOWN:

               if(cur.y<20) {
                   cur.y++;
                   ccur.y++;
               }else dump_offset+=16;
               inFile.y+=16;
               break;
           case KEY_UP:
           {
               if(cur.y>1)
               {
                   cur.y--;
                   ccur.y--;
               }else if (dump_offset>0)
               {
                   dump_offset-=16;
               }
               if(inFile.y>0)
               {
                   inFile.y-=16;
               }

           }
               break;
           case KEY_LEFT:
           {
               if(cur.x>12)
               {
                   if(letter==1)
                   {
                       ccur.x--;
                       cur.x--;
                       inFile.x--;
                       letter=2;
                   }else letter=1;
                   cur.x--;
               }
               break;
           }
           case KEY_RIGHT:
           {
               if(cur.x<58)
               {
                   if(letter==2)
                   {
                       ccur.x++;
                       cur.x++;
                       inFile.x++;
                       letter=1;
                   }else letter=2;
                   cur.x++;
               }
               break;
           }

           case 'q':
               return -1;
           default:

                   return key;

               break;
       }
   }
    return -2;
}

int kbhit()
{
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}