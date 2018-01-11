
#include "dbg.h"
/*
char *itoa_my(int value,char *string,int radix)
{
    char zm[37]="0123456789abcdefghijklmnopqrstuvwxyz";
    char aa[100]={0};
    int sum=value;
    char *cp=string;
    int i=0;
    if(radix<2||radix>36)   //增加了对错误的检测
    {
        return string;
    }
    if(value<0)
    {
        return string;
    }
    while(sum>0)
    {
        aa[i++]=zm[sum%radix];
        sum/=radix;
    }

    for(int j=i-1;j>=0;j--)
    {
        *cp++=aa[j];
    }
    *cp='\0';
    return string;
}
*/
/*
* 函数名： itoa
* 描述 ：将整形数据转换成字符串-第 18 页-
* 输入 ： -radix =10 表示 10 进制，其他结果为 0
* -value 要转换的整形数
* -buf 转换后的字符串
* -radix = 10
* 输出 ：无
* 返回 ：无
*/
static char *itoa(int value, char *string, int radix)
 {
    int i, d;
    int flag = 0;
    char *ptr = string;
    /* This implementation only works for decimal numbers. */
    if (radix != 10){
        *ptr = 0;
        return string;
    }
    if (!value){
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }
    /* if this is a negative value insert the minus sign. */
    if (value < 0){
        *ptr++ = '-';
        /* Make the value positive. */
        value *= -1;
    }
    for (i = 10000; i > 0; i /= 10){
        d = value / i;
        if (d || flag){
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }
    /* Null terminate the string. */
    *ptr = 0;
    return string;
 } /* NCL_Itoa */

/*
    * 函数名： dbg
    * 描述 ：格式化输出，类似于 C 库中的 printf，但这里没有用到 C 库
    * 输入 ： -USARTx 串口通道，这里只用到了串口 1，即 USART1
    * -Data 要发送到串口的内容的指针
    * -... 其他参数
    * 输出 ：无
    * 返回 ：无
*/
 void my_dbg(uint8 *Data,...)
 {

 #if 0
    const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap, Data);
    while ( *Data != 0) // 判断是否到达字符串结束符
    {
        if ( *Data == 0x5c ) //'\'
        {
            switch ( *++Data )
            {
                case 'r': //回车符
                uart_send_byte(0x0d);
                Data ++;
                break;
                case 'n': //换行符
                uart_send_byte(0x0a);
                Data ++;
                break;
                default:
                Data ++;
                break;
            }
        }
    else if ( *Data == '%')
    { //
        switch ( *++Data )
        {
            case 's': //字符串
                s = va_arg(ap, const char *);
                for ( ; *s; s++)
                {
                    uart_send_byte(*s);
                }
                Data++;
                break;
            case 'd': //十进制
                d = va_arg(ap, int);
                itoa(d, buf, 10);
                for (s = buf; *s; s++)
                {
                    uart_send_byte(*s);
                }
                Data++;
                break;
            default:
                Data++;
                break;
        }
    } /* end of else if */
    else
        uart_send_byte(*Data++);
    }
#endif
 }


