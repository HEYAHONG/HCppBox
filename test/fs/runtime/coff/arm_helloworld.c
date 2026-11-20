/* Test ARM COFF object used by hcoff tests. */
int arm_foo(int value)
{
    return value + 1;
}
int arm_bar(int value)
{
    return arm_foo(value) * 2;
}
