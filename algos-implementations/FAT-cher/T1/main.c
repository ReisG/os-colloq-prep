unsigned length(unsigned first, unsigned *fat)
{
    unsigned count = 1;
    while (first != 0xFFFFFFFF)
    {
        ++count;
        first = fat[first];
    }
    return count;
}