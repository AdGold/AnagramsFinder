/***********************************************************************
 ********************** Siteswap Anagram Finder ************************
 ************************** Adrian Goldwaser ***************************
 * Usage:															   *
 *   via command line:  ./anagrams <siteswap>						   *
 *   or simply run the program and enter siteswap on prompt			   *
 *   Put a '?' at the front of the siteswap to only output the count   *
 ***********************************************************************/

#define restrict __restrict__

#define MAX_PERIOD 100
#define MAX_HEIGHT 36 //'z' is 35

#include <stdio.h>
#include <string.h>

#define ss2int(c) (('0' <= c && c <= '9')? c-'0' : c-'a'+10)
#define int2ss(i) ((i < 10)? i+'0' : i+'a'-10)

void getAnagrams(int * restrict ss, int * restrict options, int * restrict lands, int minRot, int done, int period);
void printAnagrams(char *ss);

unsigned long long nAnagrams;
bool print;
int modP[MAX_HEIGHT+MAX_PERIOD];

int main (int argc, char* argv[])
{
	if (argc == 2) printAnagrams(argv[1]);
	else
	{
		printf("Psst, you can also pass the siteswap as a command line arg.\n");
		printf("Add '?' before a siteswap to output the number only. Q to exit.\n");
		printf("Enter siteswap: ");
		char ss[MAX_PERIOD];
		while (scanf("%s", ss) && ss[0]!='Q')
		{
			printAnagrams(ss);
			printf("Enter siteswap: ");
		}
	}
}

void printAnagrams(char *ss)
{
	nAnagrams = 0;
	print = (ss[0] != '?');
	ss += !print;

	int period = strlen(ss);
	for (int i = 0; i <= period+MAX_HEIGHT; i++)
		modP[i] = i%period;

	int options[MAX_HEIGHT];
	memset(options, 0, MAX_HEIGHT*sizeof(int));
	for (int i = 0; i < period; i++)
		options[ss2int(ss[i])]++;

	int m = MAX_PERIOD, mp = -1;
	for (int i = 0; i < MAX_HEIGHT; i++)
	{
	    if (options[i] && options[i] <= m) { m = options[i]; mp = i; }
	}
	options[mp]--;

	int siteswap[period+1];
	siteswap[0] = MAX_HEIGHT; //for rotation check of ss[-1]
	siteswap[1] = mp;

	int lands[period];
	memset(lands, 0, sizeof(lands));
	lands[modP[mp]]=1;

	getAnagrams(siteswap+1, options, lands,  -1,1, period);

	if (period == 1 && print)
	{
		printf("%s\n", ss);
		nAnagrams++;
	}
	if (!print)
		printf("Total of %llu anagram%s.\n", nAnagrams, (nAnagrams==1)?"":"s");
}

char sss[MAX_PERIOD];
char *getSS(int* ip, int period)
{
	for (int i = 0; i < period; i++)sss[i] = int2ss(ip[i]);
	sss[period] = '\0';
	return sss;
}

void getAnagrams(int * restrict ss, int * restrict options, int * restrict lands, int minRot, int done, int period)
{
	int f = ss[0];
	for (int i = 0; i < MAX_HEIGHT; i++)
	{
		if ((options[i] < 1) | lands[modP[i + done]] | (i > ss[minRot]) ) continue;
		ss[done] = i;

		int newMinRot = minRot + (minRot > 0);
		if (ss[minRot] > i) newMinRot = -1;
		if ((newMinRot <= 0) & (i == f)) newMinRot = 1;

		if (done+1 == period)
		{
			int good = 1;
			if (newMinRot != -1)
			{
				for (int j = 0; j < period; j++, newMinRot=modP[newMinRot+1])
				{
					if (ss[j] > ss[newMinRot]) { good = 0; break; }
					else if (ss[j] < ss[newMinRot]) break;
				}
			}
			if (good & print) printf("%s\n", getSS(ss,period));
			nAnagrams += good;
		}
		else
		{
			options[i]--;
			lands[modP[i + done]]++;
			getAnagrams(ss, options, lands, newMinRot, done+1, period);
			options[i]++;
			lands[modP[i + done]]--;
		}
	}
}
