bool substring(char *str, const char *subst)
{
    int i = 0;
    int j = 0;
    while (str[i])
    {
        while (subst[j] && str[i + j] == subst[j])
        {
            j++;
            if (!subst[j])
                return true;
        }
        j = 0;
        i++;
    }
    return false;
}