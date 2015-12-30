int leaf(int g, int h, int i, int j)
{
	int f;
	f = (g+h) - (i+j);
	return f;
}

int main()
{
	write(leaf(3,44,555,6666));
	return 0;
}
