int x = 1,y,z=2;
int a = 10;
int arr[2] = {1,2};
int f(int x)
{
  if (x == 0)
     return 1;
  else
     return x * f(x - 1);
}
int main()
{
  int res = f(5);
  return res;
}
