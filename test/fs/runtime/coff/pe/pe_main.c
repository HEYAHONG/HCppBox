/* Test EXE used by hcoff PE tests. */
__declspec(dllimport) int hello_add(int a, int b);
__declspec(dllimport) int hello_data;
int main(void)
{
    return hello_add(hello_data, 1) == 1235 ? 0 : 1;
}
