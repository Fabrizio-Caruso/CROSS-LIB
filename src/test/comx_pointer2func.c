void _do_nothing(void){}
void main()
{
    void (*bar)(void) = _do_nothing;
    bar();
}
