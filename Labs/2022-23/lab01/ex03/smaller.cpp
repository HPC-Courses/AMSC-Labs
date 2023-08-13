int smaller_standard(int a, int b) {
  if(a < b)
    return a;
  else
    return b;
}

int smaller_branchless(int a, int b) {
  return a*(a<b) + b*(b<=a);
}

int main() {
  return 0;
}