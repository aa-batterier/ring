#include "ring.h"

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

void skip_line(void)
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

int to_int(char *a)
{
	int sum = 0;
	int numbers[] = {0,1,2,3,4,5,6,7,8,9};
	for (int i = pow(10,strlen(a)-1),j = 0; a[j] != '\0'; i /= 10,j++)
	{
		if (bin_search(numbers,0,9,a[j]-'0'))
		{
			sum += (a[j] - '0')*i;
		}
	}
	return sum;
}

void to_string(char *array,int power,int number)
{
	int place = 0;
	for (; power >= 0; power--,place++)
	{
		int singular = number/pow(10,power);
		number -= singular*pow(10,power);
		array[place] = singular+'0';
	}
	array[place] = '\0';
}

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
