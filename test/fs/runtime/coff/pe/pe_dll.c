/* Test DLL used by hcoff PE tests. */
__declspec(dllexport) int hello_data = 1234;
__declspec(dllexport) const char *hello_name = "hcoff-pe";
__declspec(dllexport) int hello_add(int a, int b)
{
    return a + b;
}
__declspec(dllexport) int hello_mul(int a, int b)
{
    return a * b;
}
