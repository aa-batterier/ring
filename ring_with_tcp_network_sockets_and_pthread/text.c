/*
 * Information about sourcedevelopment.
 * -------------------------------------
 *  Initial creator: Andreas Johansson.
 *  Date created: 15-01-2018
 *  Last updated by: Andreas Johansson.
 *  Date for update: 22-01-2018
 */

/*
 * File: text.c
 * -------------
 *  In this file is all the source code
 *  for handleing text and strings.
 */

#include "ring.h"

/*
 * Function: read_line
 * Usage: Reads a line from stdin.
 * ------------------------------
 */
int read_line(char *a,int n)
{
	if (fgets(a,n,stdin) == NULL)
	{
		return 0;
	}
	if (!remove_nl(a))
	{
		skip_line();
	}
	return 1;
}

/*
 * Function: remove_nl
 * Usage: Removes the last new line.
 * --------------------------------
 */
int remove_nl(char *a)
{
	int i = strlen(a)-1;
	if (a[i] == '\n')
	{
		a[i] = '\0';
		return 1;
	}
	return 0;
}

/*
 * Function: skip_line
 * Usage: Skips a line.
 * ----------------------
 */
void skip_line(void)
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * Function: to_int
 * Usage: Turns a string into an int.
 * ---------------------------------
 */
int to_int(char *a)
{
	int sum = 0,numbers[] = {0,1,2,3,4,5,6,7,8,9};
	for (int i = pow(10,strlen(a)-1),j = 0; a[j] != '\0'; i /= 10,j++)
	{
		int n = a[j] - '0';
		if (bin_search(numbers,0,9,n))
		{
			sum += n*i;
		}
		else
		{
			return -1; // It's ok to return -1 as error becuase we don't want negative numbers or letters.
		}
	}
	return sum;
}

/*
 * Function: bin_search
 * Usage: Search for an int.
 * ------------------------
 */
int bin_search(int numbers[],int start,int end,int search)
{
	int i1 = start,i2 = end,m = 0;
	while (i1 <= i2)
	{
		m = (i1+i2)/2;
		if (numbers[m] > search)
		{
			i2 = m-1;
		}
		else if (numbers[m] < search)
		{
			i1 = m+1;
		}
		else
		{
			break;
		}
	}
	if (numbers[m] == search)
	{
		return 1;
	}
	return 0;
}
