#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int count_M(char *a);
int power(int n, int p);
int find_factors(int n, int **res);
void inc(char *a, int min, int depth);
void output(int **pieces, int pieces_c);
void transponate(int **mas, int r, int c);
void swap(int array[], long pos1, long pos2);
void qs_sort(int array[], int *indexes, long start, long end);
int find_patterns(int ****mas, int *factors, int count, int min, int *pow_c);
void read_data(char *path, int *rows, int *cols, int *min, int *max, int ***m);
void slicing(int **pizza, int rows, int cols, int min, int max, int **pieces, int pieces_c);
int search_patterns(int **pizza, int rows, int cols, int ***patterns, int patterns_count, int pow_c, int *factors, int factor_count, int min, int ***pieces);

int main()
{
	int rows, cols, min, max, factor_count, pow_c, pieces_c;
	int **pizza, *factors, **pieces;
	int ***patterns, patterns_c;

	read_data("C:\\input files\\big.in", &rows, &cols, &min, &max, &pizza);
	printf("Data loaded.\n");
	factor_count=find_factors(2 * min, &factors);
	printf("Factors calculated.\n");
	patterns_c=find_patterns(&patterns, factors, factor_count, min, &pow_c);
	printf("Patterns generated.\n");
	pieces_c=search_patterns(pizza, rows, cols, patterns, patterns_c, pow_c, factors, factor_count, min, &pieces);
	printf("Patterns found.\n");
	slicing(pizza, rows, cols, min, max, pieces, pieces_c);
	printf("Slicing complete.\n");
	output(pieces, pieces_c);
	printf("Output file created.\n");


	return 0;
}

void output(int **pieces, int pieces_c)
{
	FILE *out;
	out = fopen("C:\\output files\\big.txt", "wt");

	fprintf(out, "%d\n", pieces_c);
	for (int i = 0; i < pieces_c; ++i)
	{
		fprintf(out, "%d %d %d %d\n", pieces[i][0], pieces[i][1], pieces[i][2], pieces[i][3]);
	}
}

void slicing(int **pizza, int rows, int cols, int min, int max, int **pieces, int pieces_c)
{
	int max_add = max - 2 * min;
	int check, steps;

	printf("Stepping further into slicing\n");

	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			if (pizza[i][j] >= 0) pizza[i][j] = 1;
	
	//1111111111111111111111111111111111111111111111111111111111111111111111111111111111111

	for (int p = 0; p < pieces_c; ++p)
	{
		max_add = max - (pieces[p][3] - pieces[p][1] + 1)*(pieces[p][2] - pieces[p][0] + 1);
		if (max_add == 0) continue;
		//up
		steps = max_add / (pieces[p][3] - pieces[p][1] + 1);
		for (int i = pieces[p][0] - 1; i > pieces[p][0] - steps - 1; --i)
		{
			check = 1;
			for (int j = pieces[p][1]; j <= pieces[p][3]; ++j)
			{
				if (i < 0 || pizza[i][j] < -8)
				{
					check = 0;
					break;
				}
				else if (pizza[i][j] > 8) pizza[i][j] = 0;
			}
			if (check)
			{
				for (int j = pieces[p][1]; j <= pieces[p][3]; ++j)
					pizza[i][j] = pizza[i][j] == 0 ? 0 : p + 8 + 1;
			}
			else break;
		}
		//right
		steps = max_add / (pieces[p][2] - pieces[p][0] + 1);
		for (int j = pieces[p][3] + 1; j < pieces[p][3] + steps + 1; ++j)
		{
			check = 1;
			for (int i = pieces[p][0]; i <= pieces[p][2]; ++i)
			{
				if (j >= cols || pizza[i][j] < -8)
				{
					check = 0;
					break;
				}
				else if (pizza[i][j] > 8) pizza[i][j] = 0;
			}
			if (check)
			{
				for (int i = pieces[p][0]; i <= pieces[p][2]; ++i)
					pizza[i][j] = pizza[i][j] == 0 ? 0 : p + 8 + 1;
			}
			else break;
		}
		//down
		steps = max_add / (pieces[p][3] - pieces[p][1] + 1);
		for (int i = pieces[p][2] + 1; i < pieces[p][2] + steps + 1; ++i)
		{
			check = 1;
			for (int j = pieces[p][1]; j <= pieces[p][3]; ++j)
			{
				if (i >= rows || pizza[i][j] < -8)
				{
					check = 0;
					break;
				}
				else if (pizza[i][j] > 8) pizza[i][j] = 0;
			}
			if (check)
			{
				for (int j = pieces[p][1]; j <= pieces[p][3]; ++j)
					pizza[i][j] = pizza[i][j] == 0 ? 0 : p + 8 + 1;
			}
			else break;
		}
		//left
		steps = max_add / (pieces[p][2] - pieces[p][0] + 1);
		for (int j = pieces[p][1] - 1; j > pieces[p][1] - steps - 1; --j)
		{
			check = 1;
			for (int i = pieces[p][0]; i <= pieces[p][2]; ++i)
			{
				if (j < 0 || pizza[i][j] < -8)
				{
					check = 0;
					break;
				}
				else if (pizza[i][j] > 8) pizza[i][j] = 0;
			}
			if (check)
			{
				for (int i = pieces[p][0]; i <= pieces[p][2]; ++i)
					pizza[i][j] = pizza[i][j] == 0 ? 0 : p + 8 + 1;
			}
			else break;
		}

		steps = max_add / (pieces[p][2] - pieces[p][0] + 1 + pieces[p][3] - pieces[p][1] + 2);
		//up right
		for (int k = 1; k <= steps; ++k)
		{
			if (pieces[p][0] - k >= 0 && pieces[p][3] + k < cols && pizza[pieces[p][0] - k][pieces[p][3] + k] >= 0 && pizza[pieces[p][0] - k][pieces[p][3]] >= 0 && pizza[pieces[p][0]][pieces[p][3] + k] >= 0)
				pizza[pieces[p][0] - k][pieces[p][3] + k] = pizza[pieces[p][0] - k][pieces[p][3] + k] == 0 ? 0 : p + 8 + 1;
			else break;
		}
		//down right
		for (int k = 1; k <= steps; ++k)
		{
			if (pieces[p][2] + k < rows && pieces[p][3] + k < cols && pizza[pieces[p][2] + k][pieces[p][3] + k] >= 0 && pizza[pieces[p][2] + k][pieces[p][3]] >= 0 && pizza[pieces[p][2]][pieces[p][3] + k] >= 0)
				pizza[pieces[p][2] + k][pieces[p][3] + k] = pizza[pieces[p][2] + k][pieces[p][3] + k] == 0 ? 0 : p + 8 + 1;
			else break;
		}
		//down left
		for (int k = 1; k <= steps; ++k)
		{
			if (pieces[p][2] + k < rows && pieces[p][1] - k >= 0 && pizza[pieces[p][2] + k][pieces[p][1] - k] >= 0 && pizza[pieces[p][2] + k][pieces[p][1]] >= 0 && pizza[pieces[p][2]][pieces[p][1] - k] >= 0)
				pizza[pieces[p][2] + k][pieces[p][1] - k] = pizza[pieces[p][2] + k][pieces[p][1] - k] == 0 ? 0 : p + 8 + 1;
			else break;
		}

		//up left
		for (int k = 1; k <= steps; ++k)
		{
			if (pieces[p][0] - k >= 0 && pieces[p][1] - k >= 0 && pizza[pieces[p][0] - k][pieces[p][1] - k] >= 0 && pizza[pieces[p][0] - k][pieces[p][1]] >= 0 && pizza[pieces[p][0]][pieces[p][1] - k] >= 0)
				pizza[pieces[p][0] - k][pieces[p][1] - k] = pizza[pieces[p][0] - k][pieces[p][1] - k] == 0 ? 0 : p + 8 + 1;
			else break;
		}
	}

	//2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222

	for (int p = 0; p < pieces_c; ++p)
	{
		pieces[p][4] = 0;
		pieces[p][5] = 0;
		max_add = max - (pieces[p][3] - pieces[p][1] + 1)*(pieces[p][2] - pieces[p][0] + 1);
		if (max_add == 0) continue;
		int temp = 0;
		
		//up
		steps = max_add / (pieces[p][3] - pieces[p][1] + 1);
		for (int i = pieces[p][0] - 1; i > pieces[p][0] - steps - 1; --i)
		{
			if (i < 0) break;
			for (int j = pieces[p][1]; j <= pieces[p][3]; ++j)
			{
				if (pizza[i][j] == p + 9 || pizza[i][j] == 0) temp++;
			}
		}
		if (temp > pieces[p][4])
		{
			pieces[p][4] = temp;
			pieces[p][5] = 1;
		}
		//right
		temp = 0;
		steps = max_add / (pieces[p][2] - pieces[p][0] + 1);
		for (int j = pieces[p][3] + 1; j < pieces[p][3] + steps + 1; ++j)
		{
			if (j >= cols) break;
			for (int i = pieces[p][0]; i <= pieces[p][2]; ++i)
			{
				if (pizza[i][j] == p + 9 || pizza[i][j] == 0) temp++;
			}
		}
		if (temp > pieces[p][4])
		{
			pieces[p][4] = temp;
			pieces[p][5] = 2;
		}
		//down
		temp = 0;
		steps = max_add / (pieces[p][3] - pieces[p][1] + 1);
		for (int i = pieces[p][2] + 1; i < pieces[p][2] + steps + 1; ++i)
		{
			if (i >= rows) break;
			for (int j = pieces[p][1]; j <= pieces[p][3]; ++j)
			{
				if (pizza[i][j] == p + 9 || pizza[i][j] == 0) temp++;
			}
		}
		if (temp > pieces[p][4])
		{
			pieces[p][4] = temp;
			pieces[p][5] = 4;
		}
		//left
		temp = 0;
		steps = max_add / (pieces[p][2] - pieces[p][0] + 1);
		for (int j = pieces[p][1] - 1; j > pieces[p][1] - steps - 1; --j)
		{
			if (j < 0) break;
			for (int i = pieces[p][0]; i <= pieces[p][2]; ++i)
			{
				if (pizza[i][j] == p + 9 || pizza[i][j] == 0) temp++;
			}
		}
		if (temp > pieces[p][4])
		{
			pieces[p][4] = temp;
			pieces[p][5] = 8;
		}

		steps = max_add / (pieces[p][2] - pieces[p][0] + 1 + pieces[p][3] - pieces[p][1] + 2);
		//up right
		temp = 0;
		for (int k = 1; k <= steps; ++k)
		{
			if (pieces[p][0] - k >= 0 && pieces[p][3] + k < cols && (pizza[pieces[p][0] - k][pieces[p][3] + k] == p + 9 || pizza[pieces[p][0] - k][pieces[p][3] + k] == 0))
				temp++;
		}

		if (temp > pieces[p][4])
		{
			pieces[p][4] = temp;
			pieces[p][5] = 16;
		}
		//down right
		temp = 0;
		for (int k = 1; k <= steps; ++k)
		{
			if (pieces[p][2] + k < rows && pieces[p][3] + k < cols && (pizza[pieces[p][2] + k][pieces[p][3] + k] == p + 9 || pizza[pieces[p][2] - k][pieces[p][3] + k] == 0))
				temp++;
		}
		if (temp > pieces[p][4])
		{
			pieces[p][4] = temp;
			pieces[p][5] = 32;
		}
		//down left
		temp = 0;
		for (int k = 1; k <= steps; ++k)
		{
			if (pieces[p][2] + k < rows && pieces[p][1] - k >= 0 && (pizza[pieces[p][2] + k][pieces[p][1] - k] == p + 9 || pizza[pieces[p][2] - k][pieces[p][1] + k] == 0))
				temp++;
		}
		if (temp > pieces[p][4])
		{
			pieces[p][4] = temp;
			pieces[p][5] = 64;
		}
		//up left
		temp = 0;
		for (int k = 1; k <= steps; ++k)
		{
			if (pieces[p][0] - k >= 0 && pieces[p][1] - k >= 0 && (pizza[pieces[p][0] - k][pieces[p][1] - k] == p + 9 || pizza[pieces[p][0] - k][pieces[p][1] + k] == 0))
				temp++;
		}
		if (temp > pieces[p][4])
		{
			pieces[p][4] = temp;
			pieces[p][5] = 128;
		}
	}

	//333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333

	int t = 0, zzz;
	int *index, *pieces1;

	index = (int*)malloc(sizeof(int)*pieces_c);
	for (int p = 0; p < pieces_c; ++p)
		index[p] = p;

	pieces1 = (int*)malloc(sizeof(int)*pieces_c);
	for (int p = 0; p < pieces_c; ++p)
		pieces1[p] = pieces[p][4];

	
	qs_sort(pieces1, index, 0, pieces_c-1);
	

	for (int i=pieces_c-1; i >=0; --i)
	{
		int p = index[i];
		if (pieces[p][5] == 0 || pieces[p][4] <= 0) continue;
		else 
		{
			t = 1;
			zzz = 1;
			if (pieces[p][5] == 1 || pieces[p][5] == 16 || pieces[p][5] == 128)
			{
				for (int j = pieces[p][1]; j <= pieces[p][3]; ++j)
					if (pizza[pieces[p][0]-1][j] < -8)
					{
						zzz = 0;
						break;
					}
				if (zzz) {
					pieces[p][0]--;
					for (int j = pieces[p][1]; j <= pieces[p][3]; ++j)
						pizza[pieces[p][0]][j] = -p - 9;
				}
			}
			if (pieces[p][5] == 2 || pieces[p][5] == 16 || pieces[p][5] == 32)
			{
				for (int i = pieces[p][0]; i <= pieces[p][2]; ++i)
					if (pizza[i][pieces[p][3]+1] < -8)
					{
						zzz = 0;
						break;
					}
				if (zzz) {
					pieces[p][3]++;
					for (int i = pieces[p][0]; i <= pieces[p][2]; ++i)
						pizza[i][pieces[p][3]] = -p - 9;
				}
			}
			if (pieces[p][5] == 4 || pieces[p][5] == 32 || pieces[p][5] == 64)
			{
				for (int j = pieces[p][1]; j <= pieces[p][3]; ++j)
					if (pizza[pieces[p][2]+1][j] < -8)
					{
						zzz = 0;
						break;
					}
				if (zzz) {
					pieces[p][2]++;
					for (int j = pieces[p][1]; j <= pieces[p][3]; ++j)
						pizza[pieces[p][2]][j] = -p - 9;
				}
			}
			if (pieces[p][5] == 8 || pieces[p][5] == 64 || pieces[p][5] == 128)
			{
				for (int i = pieces[p][0]; i <= pieces[p][2]; ++i)
					if (pizza[i][pieces[p][1]-1] < -8)
					{
						zzz = 0;
						break;
					}
				if (zzz) {
					pieces[p][1]--;
					for (int i = pieces[p][0]; i <= pieces[p][2]; ++i)
						pizza[i][pieces[p][1]] = -p - 9;
				}
			}
		}
	}

	
	if (t == 0) return;
	else slicing(pizza, rows, cols, min, max, pieces, pieces_c);
}

void swap(int array[], long pos1, long pos2)
{
	long tmp;
	tmp=array[pos1];
	array[pos1]=array[pos2];
	array[pos2]=tmp;
}

void qs_sort(int array[], int *indexes, long start, long end)
{
	long head=start, tail=end-1, tmp;
	long diff=end-start;
	long pe_index;
	long pe,mid;
	long m=(start+end)/2;

	if (diff<1) return;
	if (diff==1)
		if (array[start]>array[end]) {
			swap(array, start, end);
			swap(indexes, start, end);
			return;
		}

		if (array[start]<=array[m]) {
			if (array[m]<=array[end]) pe_index=m;
			else if (array[end]<=array[start]) pe_index=start;
			else pe_index=end;
		}
		else {
			if (array[start]<=array[end]) pe_index=start;
			else if(array[end]<=array[m]) pe_index=m;
			else pe_index=end;
		}
		pe=array[pe_index]; 
		swap(array, pe_index, end);
		swap(indexes,pe_index,end);
		while (1) {
				while(array[head]<pe)
					++head;
				while(array[tail]>pe && tail>start)
					--tail;
				if (head>=tail)
					break;
				swap(array,head++, tail--);
				swap(indexes, head - 1, tail + 1);
		}
		swap(array,head,end);
		swap(indexes, head, end);
		mid=head;
		qs_sort(array, indexes, start, mid-1); 
		qs_sort(array, indexes, mid+1, end); 
}

int search_patterns(int **pizza, int rows, int cols, int ***patterns, int patterns_count, int pow_c, int *factors, int factor_count, int min, int ***pieces)
{
	int check, counter = -1, a, b;

	(*pieces) = (int**)malloc(cols*rows / (min * 2) * sizeof(int*));
	for (int i = 0; i < cols*rows / (min * 2); ++i)
		(*pieces)[i] = (int*)malloc(6 * sizeof(int));

	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			for (int k = 0; k < patterns_count; ++k)
			{
				check = 1;
				a = k % 2 == 0 ? factors[k / (2 * pow_c)] : (min * 2) / factors[k / (2 * pow_c)];
				b = k % 2 == 0 ? (min * 2) / factors[k / (2 * pow_c)] : factors[k / (2 * pow_c)];
				for (int x = 0; x < a; ++x)
				{
					for (int y = 0; y < b; ++y)
					{
						if (i + x >= rows || j + y >= cols || patterns[k][x][y] != pizza[i + x][j + y])
						{
							check = 0; break;
						}
					}
					if (!check) break;
				}

				if (check)
				{
					for (int x = 0; x < a; ++x)
						for (int y = 0; y < b; ++y)
							pizza[i + x][j + y] = counter-8;
					(*pieces)[-counter - 1][0] = i;
					(*pieces)[-counter - 1][1] = j;
					(*pieces)[-counter - 1][2] = i+a-1;
					(*pieces)[-counter - 1][3] = j+b-1;
					counter--;
					j += b - 1;
				}
			}
		}
	}
	return -(++counter);
}

int find_patterns(int ****mas, int *factors, int count, int min, int *pow_c)
{
	(*mas) = (int***)malloc(sizeof(int**)*count * 2 * power(2, min * 2));
	for (int i = 0; i < count * 2 * power(2, min * 2); ++i)
	{
		(*mas)[i] = (int**)malloc(sizeof(int*)*min * 2);
		for (int j = 0; j < min * 2; ++j)
		{
			(*mas)[i][j] = (int*)calloc(1, sizeof(int)*min * 2);
		}
	}

	char *bin = (char*)calloc(1, min * 2 * sizeof(char)+1);
	char *end = (char*)calloc(1, min * 2 * sizeof(char)+1);
	int k = 0;
	*pow_c = 0;

	for (int i = 0; i < count; ++i)
	{
		for (int j = 0; j < 2 * min; ++j)
		{
			end[j] = j < min ? 'M' : 'T';
			bin[j] = j < min ? 'T' : 'M';
		}

		int temp = 1;
		
		while (temp)
		{
			if (strcmp(bin, end) == 0) temp = 0;
			for (int r = 0; r < factors[i]; ++r)
			{
				for (int c = 0; c < (min * 2) / factors[i]; ++c)
				{
					(*mas)[k][r][c] = bin[c + r * (min * 2) / factors[i]];
					(*mas)[k+1][r][c] = bin[c + r * (min * 2) / factors[i]];
				}
			}
			transponate((*mas)[k + 1], factors[i], (min * 2) / factors[i]);
			if (temp) inc(bin, min, 0);
			k += 2;
			if (i == 0) (*pow_c)++;
		}
		
	}

	return k;
}

void inc(char *a, int min, int depth)
{
	if (a[strlen(a) - 1] == 'T')
	{
		a[strlen(a) - 1] = 'M';
	}
	else if (a[strlen(a) - 2] == 'T')
	{
		a[strlen(a) - 2] = 'M';
		a[strlen(a) - 1] = 'T';
	}
	else
	{
		a[strlen(a) - 2] = 0;
		inc(a, min, depth+1);
		a[strlen(a)] = 'T';
		a[strlen(a) - 1] = 'T';
	}
	if (count_M(a) != min && depth == 0)
		inc(a, min, depth);
}

int count_M(char *a)
{
	int count = 0;
	for (int i = 0; i < strlen(a); ++i)
		if (a[i] == 'M') ++count;
	return count;
}

void transponate(int **mas, int r, int c)
{
	int temp;
	for (int i = 0; i < r; ++i)
	{
		for (int j = i+1; j < c; ++j)
		{
			temp = mas[j][i];
			mas[j][i] = mas[i][j];
			mas[i][j] = temp;
		}
	}
}

int power(int n, int p)
{
	int res=1;
	if (p == 1) return n;
	else if (p == 0) return 1;
	else for (;p>0; --p)
	{
		res *= n;
	}
	return res;
}

void read_data(char *path, int *rows, int *cols, int *min, int *max, int ***m)
{
	FILE *inp;

	inp=freopen(path, "rt", stdin);
	scanf("%d %d %d %d\n", rows, cols, min, max);

	*m = (int**)malloc(*rows * sizeof(int*));
	for (int i = 0; i < *rows; ++i)
		(*m)[i] = (int*)malloc(*cols*sizeof(int));

	for (int i = 0; i < *rows; ++i)
	{
		for (int j = 0; j < *cols; ++j)
			(*m)[i][j] = getchar();
		getchar();
	}
}

int find_factors(int n, int **res)
{
	int temp = (int)sqrt(n);
	int k = 1;
	*res = (int*)malloc(10 * sizeof(int));
	(*res)[0] = 1;

	for (int i = 2; i <= temp; ++i)
	{
		if (n%i == 0)
			(*res)[k++] = i;
	}

	return k;
}