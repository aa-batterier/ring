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
 * Usage: Removes the last new line from a string.
 * ------------------------------------------------
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
	int sum = 0,nL[] = {0,1,2,3,4,5,6,7,8,9};
	for (int i = pow(10,strlen(a)-1),j = 0; a[j] != '\0'; i /= 10,j++)
	{
		int n = a[j] - '0';
		if (!bin_search(nL,0,9,n))
		{
			return -1;
		}
		sum += n*i;
	}
	return sum;
}

/*
 * Function: to_string
 * Usage: Transform an int into a string.
 * ---------------------------------------
 */
void to_string(char *a,int power,int number)
{
	int place = 0;
	for (int i = pow(10,power); i > 0; place++,i /= 10)
	{
		int singular = number/i;
		a[place] = singular + '0';
		number -= singular*i;
	}
	a[place] = '\0';
}

/*
 * Function: bin_search
 * Usage: Search for an int in a list.
 * ------------------------------------
 */
int bin_search(int list[],int start,int end,int x)
{
	int mid;
	while (start <= end)
	{
		mid = (start+end)/2;
		if (list[mid] < x)
		{
			start = mid+1;
		}
		else if (list[mid] > x)
		{
			end = mid-1;
		}
		else
		{
			break;
		}
	}
	if (list[mid] == x)
	{
		return 1;
	}
	return 0;
}
