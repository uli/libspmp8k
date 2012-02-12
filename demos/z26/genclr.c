/*
** z26 is Copyright 1997-2011 by John Saeger and contributors.  
** z26 is released subject to the terms and conditions of the 
** GNU General Public License Version 2 (GPL).	z26 comes with no warranty.
** Please see COPYING.TXT for details.
*/

/*
PF_BIT = 1
BL_BIT = 2
P1_BIT = 4
M1_BIT = 8
P0_BIT = 16
M0_BIT = 32
DL_BIT = 64
*/

void main(void)
{

	int i,n;

/* Table to translate a display entry (6 bits which are pf,ball,p1,m1,p0,m0)
   into a luminance register value . 2 of these,in the second, the
   playfield has priority ! */

	printf(";\n");
	printf("TIADisplayToColour label byte\n");
	for (i = 0;i < 64;i++)
	{
		n = 0;			/* background */
		if (i & 1)  n = 1;	/* playfield */
		if (i & 2)  n = 4;	/* ball */
		if (i & 12) n = 2;	/* p1,m1 */
		if (i & 48) n = 3;	/* p0,m0 */

		if (i % 16 == 0) 
			printf(" db ");
		printf("%d",n);
		if (i % 16 != 15) 
			printf(","); 
		else 
			printf("\n");
	}

	printf(";\n");
	printf("TIADisplayToColour2 label byte\n");
	for (i = 0;i < 64;i++)
	{
		n = 0;			/* background */
		if (i & 12) n = 2;	/* p1,m1 */
		if (i & 48) n = 3;	/* p0,m0 */
		if (i & 2)  n = 4;	/* ball */
		if (i & 1)  n = 1;	/* playfield */

		if (i % 16 == 0) 
			printf(" db ");
		printf("%d",n);
		if (i % 16 != 15) 
			printf(","); 
		else 
			printf("\n");
	}
	printf("\n");

}

