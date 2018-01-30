/*
 * Information about sourcedevelopment.
 * -------------------------------------
 *  Initial creator: Andreas Johansson.
 *  Date created: 30-01-2018
 *  Last updated by: Andreas Johansson.
 *  Date for update: 30-01-2018
 */

/*
 * File: text.c
 * -------------
 *  In this file is the source code
 *  for all the functions which handels all
 *  the text in this program.
 */

#include "ring.h"

/*
 * Function: read_line
 * Usage: Reads a line from stdin.
 * --------------------------------
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
 * Usage: Removes new line at the end.
 * ------------------------------------
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
 * ---------------------
 */
void skip_line(void)
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

/*
 * Function: to_int
 * Usage: Transform a string into an int.
 * ---------------------------------------
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
			return -1; // It's ok to return -1 as error because we don't want negative numbers or letters.
		}
	}
	return sum;
}

/*
 * Function: to_string
 * Usage: Transform an int into a string.
 * ---------------------------------------
 */
void to_string(char *a,int power,int n)
{
	int j = 0;
	for (int i = pow(10,power); power >= 0; i /= 10,j++,power--)
	{
		int singular = n/i;
		a[j] = singular + '0';
		n -= singular*i;
	}
	a[j] = '\0';
}

/*
 * Function: bin_search
 * Usage: Search after a specific integer.
 * ----------------------------------------
 */
int bin_search(int a[],int start,int end,int x)
{
	int mid = 0;
	while (start <= end)
	{
		mid = (start+end)/2;
		if (a[mid] < x)
		{
			start = mid+1;
		}
		else if (a[mid] > x)
		{
			end = mid-1;
		}
		else
		{
			break;
		}
	}
	if (a[mid] == x)
	{
		return 1;
	}
	return 0;
}
