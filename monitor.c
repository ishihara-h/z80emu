/* monitor.c
 * command example
 *  0[cr] 0.10[cr]
 *  0:0 1 2 3[cr] 
 *  0g
 *
 * Copyright (c) 2015 Hiroshi Ishihara 
 *
 * This program is free, do whatever you want with it.
 */

#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h> 
/* #include "monitor.h" */

int dump(x, y)
{
	printf("%04x", x);
        do {
            if ((x & 7) == 0)
                printf("\n%04x", x);
            printf(" %02x", 0);
            x++;
        } while (x <= y);
	printf("\n");
        return 0;
}

int main (void)
{
        uint16_t n = 0;
        uint16_t n1 = 0;
        uint16_t n2 = 0;
        int8_t mode = 0;
        char c = 0;
        char buf[80];
        char *p;

	while(1) {
		printf(">");
		fgets(buf, sizeof(buf), stdin);
		p = buf;
		while (1) {
			c = *p++;
			if ((c == '\0') || (c == '\n')) {
				dump(n1, n2);
				n1 = n2 + 1;
				n2 = n1 | 0x07;
				mode = 0;
				n = 0;
				break;
			} else if (c == '.') {
				mode = 1;
				n = 0;
			} else if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')) {
				if (c >= 'a')
					n = (n << 4) + c - 'a' + 10;
				else if (c >= 'A')
				    	n = (n << 4) + c - 'A' + 10;
				else
					n = (n << 4) + c - '0';
				if (mode == 0)
					n1 = n2 = n;
				else if (mode == 1)
					n2 = n;
				else
					printf("mode err1\n");
			} else if (c == ':')
				;
			else if (c == 'g')
				;
			else
				printf("err\n");
		}
	}
		return EXIT_SUCCESS;
}


