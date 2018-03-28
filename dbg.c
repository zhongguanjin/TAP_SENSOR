
#include "dbg.h"




#define  DBG_CONSOLEBUF_SIZE  64

#define MAX_STRING_SIZE         500



signed int vformat(char *, size_t, const char *, va_list ap);

signed int PutChar(char *pStr, char c);
signed int PutString(char *pStr, signed int width, const char *pSource);
signed int PutSignedInt(    char *pStr,    char fill,    signed int width,    signed int value);
signed int PutUnsignedInt(   char *pStr,    char fill,    signed int width,    unsigned int value);
signed int PutHexa(
								    char *pStr,
								    char fill,
								    signed int width,
								    unsigned char maj,
								    unsigned int value);


//------------------------------------------------------------------------------
//         Local Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Writes a character inside the given string. Returns 1.
// \param pStr  Storage string.
// \param c  Character to write.
//------------------------------------------------------------------------------
signed int PutChar(char *pStr, char c)
{
    *pStr = c;
    return 1;
}

//------------------------------------------------------------------------------
// Writes a string inside the given string.
// Returns the size of the written
// string.
// \param pStr  Storage string.
// \param pSource  Source string.
//------------------------------------------------------------------------------
signed int PutString(char *pStr, signed int width, const char *pSource)
{
    signed int num = 0;

    while ((*pSource != 0)) {

        *pStr++ = *pSource++;
        num++;
		if(width>0)
		{
			width--;
			if(width==0) break;
		}
    }

    return num;
}

//------------------------------------------------------------------------------
// Writes a signed int inside the given string, using the provided fill & width
// parameters.
// Returns the size of the written integer.
// \param pStr  Storage string.
// \param fill  Fill character.
// \param width  Minimum integer width.
// \param value  Signed integer value.
//------------------------------------------------------------------------------
signed int PutSignedInt(char *pStr,char fill,signed int width,signed int value)
{
    signed int num = 0;
    unsigned int absolute;
    // Compute absolute value
    if (value < 0) {

        absolute = -value;
    }
    else {

        absolute = value;
    }
#if 1
	int i ;
	int f = 0;
	int s = 0;
	unsigned int temp;
	uint16 div[5]={10000,10000,1000,100,10};

	for(i=0;i<4;i++) // unsigned int = 16bit
	{
		if(i == 0)
		{
			temp = (absolute/div[i+1]);
		}
		else
		{
			temp = ((absolute%div[i])/div[i+1]);
		}

		if(temp > 0)
		{
			f = 1;

			if(!s)
			{
				s = 1;
				if (value < 0) {
				 	num += PutChar(pStr, '-');
	            	pStr++;
				 }
			}
			num += PutChar(pStr, (char)temp + '0');
			pStr ++;
		}
		else
		{
			if(f)
			{
				PutChar(pStr, '0');
				pStr++;
				num++;
			}
			else
			{
				if(width > 0) {

					PutChar(pStr, fill);
					pStr++;
					num++;
					width--;
				}
			}
		}
	}
#else
    // Take current digit into account when calculating width
    width--;

    // Recursively write upper digits
    if ((absolute / 10) > 0) {

        if (value < 0) {

            num = PutSignedInt(pStr, fill, width, -(absolute / 10));
        }
        else {

            num = PutSignedInt(pStr, fill, width, absolute / 10);
        }
        pStr += num;
    }
    else {

        // Reserve space for sign
        if (value < 0) {

            width--;
        }

        // Write filler characters
        while (width > 0) {

            PutChar(pStr, fill);
            pStr++;
            num++;
            width--;
        }

        // Write sign
        if (value < 0) {

            num += PutChar(pStr, '-');
            pStr++;
        }
    }

#endif

    // Write lower digit
    num += PutChar(pStr, (char)(absolute % 10) + '0');

    return num;
}
//------------------------------------------------------------------------------
// Writes an unsigned int inside the given string, using the provided fill &
// width parameters.
// Returns the size in characters of the written integer.
// \param pStr  Storage string.
// \param fill  Fill character.
// \param width  Minimum integer width.
// \param value  Integer value.
//------------------------------------------------------------------------------
signed int PutUnsignedInt(char *pStr,char fill,signed int width,unsigned int value)
{
    signed int num = 0;

#if 1
	int i ;
	int f = 0;
	unsigned int temp;
	unsigned int div[5]={10000,10000,1000,100,10};

	for(i=0;i<4;i++) // unsigned int = 16bit
	{
		if(i == 0)
		{
			temp = (value/div[i+1]);
		}
		else
		{
			temp = ((value%div[i])/div[i+1]);
		}
		if(temp > 0)
		{
			f = 1;
			num += PutChar(pStr, temp + '0');
			pStr ++;
		}
		else
		{
			if(f)
			{
				PutChar(pStr, '0');
				pStr++;
				num++;
			}
			else
			{
				if(width > 0) {

					PutChar(pStr, fill);
					pStr++;
					num++;
					width--;
				}
			}
		}
	}
#else
    // Take current digit into account when calculating width
    width--;

    // Recursively write upper digits
    if ((value / 10) > 0) {

        num = PutUnsignedInt(pStr, fill, width, value / 10);
        pStr += num;
    }
    // Write filler characters
    else {

        while (width > 0) {

            PutChar(pStr, fill);
            pStr++;
            num++;
            width--;
        }
    }
#endif
    // Write lower digit
    num += PutChar(pStr, (value % 10) + '0');


    return num;
}
//------------------------------------------------------------------------------
// Writes an hexadecimal value into a string, using the given fill, width &
// capital parameters.
// Returns the number of char written.
// \param pStr  Storage string.
// \param fill  Fill character.
// \param width  Minimum integer width.
// \param maj  Indicates if the letters must be printed in lower- or upper-case.
// \param value  Hexadecimal value.
//------------------------------------------------------------------------------
signed int PutHexa(
    char *pStr,
    char fill,
    signed int width,
    unsigned char maj,
    unsigned int value)
{
    signed int num = 0;
#if 1
	int i ;
	int shift = 16;
	unsigned int temp;
	char c;

	for(i=0;i<4;i++)
	{
		shift -= 4;
		temp = (value>>shift);
		c = (char)(temp & 0xF);

		if (c != 0){

			if (c < 10) {

				PutChar(pStr, c + '0');
			}
			else
			{
				c -= 10 ;

				if (maj) {

					PutChar(pStr, c + 'A');
				}
				else {

					PutChar(pStr, c + 'a');
				}
			}
			pStr++;
			num++;
		}
		else
		{
			if(width>0)
			{
				width--;
			}
			PutChar(pStr, fill);
			pStr++;
			num++;
		}
	}
#else
    // Decrement width
    width--;

    // Recursively output upper digits
    if ((value >> 4) > 0) {

        num += PutHexa(pStr, fill, width, maj, value >> 4);
        pStr += num;
    }
    // Write filler chars
    else {

        while (width > 0) {

            PutChar(pStr, fill);
            pStr++;
            num++;
            width--;
        }
    }

    // Write current digit
    if ((value & 0xF) < 10) {

        PutChar(pStr, (value & 0xF) + '0');
    }
    else if (maj) {

        PutChar(pStr, (value & 0xF) - 10 + 'A');
    }
    else {

        PutChar(pStr, (value & 0xF) - 10 + 'a');
    }
    num++;
#endif

    return num;
}

//------------------------------------------------------------------------------
//         Global Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Stores the result of a formatted string into another string. Format
/// arguments are given in a va_list instance.
/// Return the number of characters written.
/// \param pStr    Destination string.
/// \param length  Length of Destination string.
/// \param pFormat Format string.
/// \param ap      Argument list.
//------------------------------------------------------------------------------
signed int vformat(char *pStr, size_t length, const char *pFormat, va_list ap)
{
	char          fill;
	unsigned char width;
	signed int    num = 0;
	signed int    size = 0;
	char index;
	// Clear the string
	if (pStr)
	{
		*pStr = 0;
	}

	// Phase string
	while (*pFormat != 0 && size < length)
	{

		// Normal character
		if (*pFormat != '%')
		{

			*pStr++ = *pFormat++;
			 size++;
		}
		// Escaped '%'
		else if (*(pFormat+1) == '%')
		{

			*pStr++ = '%';
			pFormat += 2;
			size++;
		}
		// Token delimiter
		else
		{
			fill = '0';
			width = 0;
			pFormat++;
			// Parse filler
			if (*pFormat == '0')
			{
				fill = '0';
				pFormat++;
			}
			// Parse width
			while ((*pFormat >= '0') && (*pFormat <= '9'))
			{

				width = (width*10);
				index = *pFormat;
				index -= '0';
				width += index;
				pFormat++;
			}
			// Check if there is enough space
			if (size + width > length)
			{
				width = length - size;
			}
			// Parse type
			switch (*pFormat)
			{
				case 'd':
				case 'i': num = PutSignedInt(pStr, fill, width, va_arg(ap, signed int)); break;
				case 'u': num = PutUnsignedInt(pStr, fill, width, va_arg(ap, unsigned int)); break;
				case 's': num = PutString(pStr,width, va_arg(ap, char *)); break;
				case 'c': num = PutChar(pStr, va_arg(ap, unsigned int)); break;
				case 'x': num = PutHexa(pStr, fill, width, 0, va_arg(ap, unsigned int)); break;
				case 'X': num = PutHexa(pStr, fill, width, 1, va_arg(ap, unsigned int)); break;
				default:
				return 0;
			}
			pFormat++;
			pStr += num;
			size += num;
		}
	}

	// NULL-terminated (final \0 is not counted)
	if (size < length)
	{

		*pStr = 0;
	}
	else
	{

		*(--pStr) = 0;
		size--;
	}
	return size;
}

//------------------------------------------------------------------------------
/// Writes a formatted string inside another string.
/// \param pStr  Storage string.
/// \param pFormat  Format string.
//------------------------------------------------------------------------------
/*
void  my_printf(const char *pFormat, ...)
{
    va_list ap;
    signed int result;
    uint8 i;
    char rt_log_buf[DBG_CONSOLEBUF_SIZE];

    // Forward call to vsprintf
    va_start(ap, pFormat);
    result = vformat(rt_log_buf, MAX_STRING_SIZE,pFormat, ap);

    if (result > DBG_CONSOLEBUF_SIZE - 1)
    result = DBG_CONSOLEBUF_SIZE - 1;
    for(i=0;i<result;i++)
    {
    	uart_send_byte(rt_log_buf[i]);
    }
    va_end(ap);
}*/

 void my_dbg(const char *pFormat, ...)
 {
 #if 0
     va_list ap;
     signed int result;
     char rt_log_buf[DBG_CONSOLEBUF_SIZE];

     // Forward call to vsprintf
     va_start(ap, pFormat);
     result = vformat(rt_log_buf, MAX_STRING_SIZE,pFormat, ap);
     if (result > DBG_CONSOLEBUF_SIZE - 1)
     result = DBG_CONSOLEBUF_SIZE - 1;
     uart_send_data(rt_log_buf, result);
     va_end(ap);
#endif
 }




