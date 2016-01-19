int main()
{
  int i = 10;
  int j = 10;
  int sum = 0;
  for (;i >=0; --i)
  {
    j = 10;
    for (;j>=0;--j)
    {
      ++sum;
    }
    if (i > 5)
      write(1);
  }
  return 0;
}