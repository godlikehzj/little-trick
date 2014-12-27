static int compare_p(const void *a, const void *b, void *p)
{
    return strcmp((char*)p+*(int *)a, (char*)p+*(int *)b);
}
void CalcVerify(char *pcUrl, char*pcKey, char*pcMd5)
{
    int iUrlLen = strlen(pcUrl);
    printf("input url is %s\r\n", pcUrl);
    char *begin = NULL;
    int i = 0;
    int count = 0;
    int iparaLen = 0;

    for (; i<iUrlLen; i++)
    {
        if (*(pcUrl+i) == '?')
        {
            begin = pcUrl+i;
            iparaLen = iUrlLen - i;
        }
        if (*(pcUrl+i) == '&')
        {
            count++;
        }
    }

    if ((begin == NULL) || (0 == count))
        return;

    char temp[iparaLen];
    int array[count];

    strcpy(temp, begin);
    array[0] = 1;
    count = 0;

    for(i = 1; i < iparaLen; i++)
    {
        if (*(temp+i) == '&')
        {
            count ++;
            *(temp+i)='\0';
        }
        if (*(temp+i -1) == '\0')
        {
            array[count] = i;
        }
    }
    qsort_r(array, count, sizeof(int), compare_p, temp);

    iparaLen += strlen(pcKey);
    char outputMd5[iparaLen];
    int templen = 0;
    for(i = 0; i < count ; i++)
    {
        templen += snprintf(outputMd5+templen, iparaLen -templen, "%s", temp+array[i]);
    }
    templen += snprintf(outputMd5+templen, iparaLen -templen, "%s", pcKey);
    printf("input md5 string is %s\r\n", outputMd5);
    MD5(outputMd5, strlen(outputMd5), pcMd5);

    return;
}
