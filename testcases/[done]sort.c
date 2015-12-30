int v[1001];
int swap(int k)
{
	int tmp;
	tmp = v[k];
	v[k] = v[k+1];
	v[k+1] =tmp;
	return 0;
}

int main()
{
	int n,i,j;
	read(n);
	for (i = 0; i < n; ++i) read(v[i]);
	for (i = 0; i < n; ++i) {
		for (j = i-1; j >= 0 && v[j]>v[j+1]; j -= 1) {
			swap(j);
		}
	}
	for (i = 0; i < n; ++i) write(v[i]);
	return 0;
}
