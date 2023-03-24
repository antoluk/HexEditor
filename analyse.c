//
// Created by antoluk on 8.3.23.
//

#include "analyse.h"
void analyse(char* ext)
{

    if(!strncmp("png",ext,3))
    {
        png_analyse();
    }
}
