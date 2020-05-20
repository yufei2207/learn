/*
* Copyright (c) 2012
* All rights reserved.
* 
* Name��     utility.c
* Abstract�� utility functions
* 
* Version�� 1.0
* Author��  yfwang
* Date��    2012/03/19
*
* history   :
*	     when            who    	          what:
*      -------------   --------------    -------------------
*      Mar 19, 2012     yfwang                create
*      Mar 23, 2012     xlsong                add
*
*/
#include "utility.h"
#include "../protocol/pt_define.h"
void DecodePackage(TPackage *pPkg, char *pData, int nLen, char search)
{
	char *ptr = pData;
	pPkg->nItems = 0;

	while (ptr = strchr(pData, search))
	{
		*ptr = '\0';
		pPkg->pItem[pPkg->nItems++] = pData;

		pData = ptr + 1;			
		if ((pPkg->nItems + 1) == MAX_DATA_ITEM)
		{   
			break;
		}
	}

	pPkg->pItem[pPkg->nItems++] = pData;	
}

void DecodePackage_Ex(TPackage *pPkg, char *pData, int nLen, char *search)
{
	int i;
	char *ptr = pData;
	pPkg->nItems = 0;
	for (i = 0; i < nLen; i++, ptr++)
	{
		if (strchr(search, *ptr))
		{
			*ptr = '\0';
			pPkg->pItem[pPkg->nItems++] = pData;

			pData = ptr + 1;			
			if ((pPkg->nItems + 1) == MAX_DATA_ITEM)
			{
				break;
			}
		}
	}

	pPkg->pItem[pPkg->nItems++] = pData;	
}
//-----------------------------------------------------------------------------
// ���ַ���ת����16���ƣ����� "1A"=> 0x1A 
// ���룺Ҫת�����ַ���
// �����ת��������ݣ�8λ�޷�������
//-----------------------------------------------------------------------------
uchar atoh8(char *ptr) 
{
    uchar rst=0;
    uchar tmp=0;
	uchar i = 0;
    for(; i<2; i++)
	{
        tmp = *ptr++;
        if ((tmp <= '9') && (tmp >= '0')) 
		{
            tmp = tmp -'0';
            rst = (rst << 4) + tmp;
        }
        else if ((tmp <= 'F') && (tmp >= 'A')) 
		{
            tmp = tmp -'A' + 10;
            rst = (rst << 4) + tmp;
        }
        else if ((tmp <= 'f') && (tmp >= 'a')) 
		{
            tmp = tmp -'a' + 10;
            rst = (rst<<4) + tmp;
        }
        else 
			break;
    }
    return rst;
}
const unsigned int pow10[10] = {
                                1,
                                10,
                                100,
                                1000,
                                10000,
                                100000,
                                1000000,
                                10000000,
                                100000000,
                                1000000000
                                };

const double DigitsRound[10] = {0.5,
                                0.05,
                                0.005,
                                0.0005,
                                0.00005,
                                0.000005,
                                0.0000005,
                                0.00000005,
                                0.000000005,
                                0.00000000005
                                };

const char ConstNumber[11] = "0123456789";
/*------------------------------------------------------------------------------
*  ��������ת�����ַ���  ftoa
*  ���룺*str    ת������Ҫ������ַ���
*        value  ��Ҫת�����з��ŵĸ�����  
*        ndigit - ��С����λ�������9��������1��
* ����� *str - ת������Ҫ������ַ���
*        return ���ַ���str�ĳ���
* ע�⣺�ú����ľ����ԣ���������ݵ����ֵ�� 2^31��С�����9λ������1λ
*       ��������̫�󣬽�ת������                
*/
int ftoa(double value, uchar *pdest, int nfloat, int ndigit) 
{
    uchar  len=0,tmp=0;
    unsigned int t1 = 0,t2 = 0,n = 0;
    uchar *pdt = pdest;

    if (ndigit>9)  ndigit = 9;//nfloat���9��С��
    if (ndigit==0) ndigit = 1;//nfloat����1��С��
    
    if (nfloat>9)  nfloat = 9;//nfloat���9��С��
    if (nfloat==0) nfloat = 1;//nfloat����1��С��
    
    if (value<0) //�����ж�
    {
       value = -value;
       *pdt++ = '-';
       len = 1;
    }
    value = value + DigitsRound[nfloat];//��������
    t1 = value;//��������ֹ��λ
    t2 = (value - t1)*pow10[nfloat];//С��
    //�޷���
    if(ndigit == -1)//
    {
        for(n = 10;n != 0;n--)
        {
            if(t1 >= pow10[n-1])
            {
                tmp = t1/pow10[n-1]%10;
                *pdt++ = ConstNumber[tmp];
                len++;
            }
        }
    }
    else//����Ҫ������ݳ��ȼ���
    {
        for(n = ndigit;n != 0;n--)
        {
            tmp = t1/pow10[n-1]%10;
            *pdt++ = ConstNumber[tmp];
            len++;

        }        
    }
    *pdt++ = '.';
    len++;

    //С������
    for (n=nfloat;n>0;n--) 
    {
        tmp = t2/pow10[n-1]%10;
        *pdt++ = ConstNumber[tmp];
    }
    *pdt = 0;
    len += nfloat;
    return len;
}


