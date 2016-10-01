//
//  main.c
//  ARMHW
//
//  Created by callum taylor on 21/10/2015.
//  Copyright (c) 2015 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include "arm.h"

__attribute__((noinline))
int test_func()
{
    
    return 20;
}

int main (int argc, const char * argv[])
{
	return test_func();
}

