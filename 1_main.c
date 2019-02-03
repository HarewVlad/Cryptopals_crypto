#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "utils.h"

const char hex_to_base64[] = 
{
	'A',
	'B',
	'C',
	'D',
	'E',
	'F',
	'G',
	'H',
	'I',
	'J',
	'K',
	'L',
	'M',
	'N',
	'O',
	'P',
	'Q',
	'R',
	'S',
	'T',
	'U',
	'V',
	'W',
	'X',
	'Y',
	'Z',

	'a',
	'b',
	'c',
	'd',
	'e',
	'f',
	'g',
	'h',
	'i',
	'j',
	'k',
	'l',
	'm',
	'n',
	'o',
	'p',
	'q',
	'r',
	's',
	't',
	'u',
	'v',
	'w',
	'x',
	'y',
	'z',

	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'+',
	'/',
};

uint8_t char_to_digit[256] = {
    ['0'] = 0,
    ['1'] = 1,
    ['2'] = 2,
    ['3'] = 3,
    ['4'] = 4,
    ['5'] = 5,
    ['6'] = 6,
    ['7'] = 7,
    ['8'] = 8,
    ['9'] = 9,
    ['a'] = 10, ['A'] = 10,
    ['b'] = 11, ['B'] = 11,
    ['c'] = 12, ['C'] = 12,
    ['d'] = 13, ['D'] = 13,
    ['e'] = 14, ['E'] = 14,
    ['f'] = 15, ['F'] = 15,
};

char digit_to_char[256] = 
{
	[0] = '0',
	[1] = '1',
	[2] = '2',
	[3] = '3',
	[4] = '4',
	[5] = '5',
	[6] = '6',
	[7] = '7',
	[8] = '8',
	[9] = '9',
	[10] = 'a',
	[11] = 'b',
	[12] = 'c',
	[13] = 'd',
	[14] = 'e',
	[15] = 'f',
};

int my_atoi(const char *source, const char *end, int base)
{	
	int result = 0;
	switch (base)
	{
		case 10:
			while (source != end)
			{
				result *= 10;
				result += *source++ - '0';
			}
			break;
		case 2:
			while (source != end)
			{
				result *= 2;
				result += *source++ - '0';
			}
			break;
		case 16:
			while (source != end)
			{
				result *= 16;
				result += char_to_digit[*source++];
			}
			break;
		case 0xCC: // ascii to hex
			while (source != end)
			{
				result *= 256;
				result += *source++ & 0xF0 | *source & 0x0F;
			}
			break;
		default:
			fatal("my_atoi: incorrect base");
	}
	
	return result;
}

char *encode_ascii_to_base64(const char *string)
{
	const char *p_string = string;
	const size_t len = strlen(string);
	char *result_string = malloc(len * 4);
	char *p_result_string = result_string;
	int shift = 2;
	int mask = 0x03;
	int t = 0;
	unsigned int value = 0;
	while (*p_string != '\0')
	{
		if (shift != 8)
		{
			value |= my_atoi(p_string, p_string + 1, 0xCC);
			p_string++;
		}
		else
			mask = 0x0;

		t = value >> shift & 0x3F;
		*result_string++ = hex_to_base64[t];

		value = value & mask;
		value <<= 8;

		mask %= 0x3F;
		mask <<= 2;
		mask |= 0x03;

		shift %= 8;
		shift += 2;
	}
	t = value >> shift & 0x3F;
	*result_string++ = hex_to_base64[t];

	*result_string = '\0';
	return p_result_string;
}

char *encode_hex_to_base64(const char *string)
{
	const char *p_string = string;
	const size_t len = strlen(string);
	char *result_string = malloc(len * 1.5);
	char *p_result_string = result_string;
	int shift = 2;
	int mask = 0x03;
	unsigned int value = 0;
	int t = 0;
	while (*p_string != '\0')
	{
		if (shift != 8)
		{
			value |= my_atoi(p_string, p_string + 2, 16);
			p_string += 2;
		}
		else
			mask = 0x0;

		t = value >> shift & 0x3F;
		*result_string++ = hex_to_base64[t];

		value = value & mask;
		value <<= 8;

		mask %= 0x3F;
		mask <<= 2;
		mask |= 0x03;

		shift %= 8;
		shift += 2;
	}
	t = value >> shift & 0x3F;
	*result_string++ = hex_to_base64[t];
	
	*result_string = '\0';
	return p_result_string;
}

char *fixed_xor(const char *str1, const char *str2)
{
	// assert(strlen(str1) == strlen(str2));
	size_t len = strlen(str1);
	char *result = malloc(len);
	for (int i = 0; i < len; i++)
	{
		result[i] = digit_to_char[(char_to_digit[str1[i]] ^ char_to_digit[str2[i]])];
	}
	result[len] = '\0';
	return result;
}

double letters_score[256] = 
{
	['a'] = 8.167,
	['b'] = 1.492,
	['c'] = 2.782,
	['d'] = 4.253,
	['e'] = 12.702,
	['f'] = 2.228,
	['g'] = 2.015,
	['h'] = 6.094,
	['i'] = 6.966,
	['j'] = 0.153,
	['k'] = 0.772,
	['i'] = 4.025,
	['m'] = 2.406,
	['n'] = 6.749,
	['o'] = 7.507,
	['p'] = 1.929,
	['q'] = 0.095,
	['r'] = 5.987,
	['s'] = 6.327,
	['t'] = 9.056,
	['u'] = 2.758,
	['v'] = 0.978,
	['w'] = 2.360,
	['x'] = 0.150,
	['y'] = 1.974,
	['z'] = 0.074,

	['A'] = 8.167,
	['B'] = 1.492,
	['C'] = 2.782,
	['D'] = 4.253,
	['E'] = 12.702,
	['F'] = 2.228,
	['G'] = 2.015,
	['H'] = 6.094,
	['I'] = 6.966,
	['J'] = 0.153,
	['K'] = 0.772,
	['I'] = 4.025,
	['M'] = 2.406,
	['N'] = 6.749,
	['O'] = 7.507,
	['P'] = 1.929,
	['Q'] = 0.095,
	['R'] = 5.987,
	['S'] = 6.327,
	['T'] = 9.056,
	['Y'] = 2.758,
	['V'] = 0.978,
	['W'] = 2.360,
	['X'] = 0.150,
	['Y'] = 1.974,
	['Z'] = 0.074,
	[' '] = 12.500,
};

const char alphabet[] = 
{
	'A',
	'B',
	'C',
	'D',
	'E',
	'F',
	'G',
	'H',
	'I',
	'J',
	'K',
	'L',
	'M',
	'N',
	'O',
	'P',
	'Q',
	'R',
	'S',
	'T',
	'U',
	'V',
	'W',
	'X',
	'Y',
	'Z',

	'a',
	'b',
	'c',
	'd',
	'e',
	'f',
	'g',
	'h',
	'i',
	'j',
	'k',
	'l',
	'm',
	'n',
	'o',
	'p',
	'q',
	'r',
	's',
	't',
	'u',
	'v',
	'w',
	'x',
	'y',
	'z',

	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'+',
	'/',
};

const char *decode_hex_encoded_string(const char *str)
{
	size_t len = strlen(str);
	assert(len % 2 == 0);
	char *result = malloc(len / 2);
	char *p_result = result;
	unsigned char ch;
	for (int i = 0; i < len - 1; i += 2)
	{
		ch = char_to_digit[str[i]] << 4 | char_to_digit[str[i + 1]];
		*result++ = ch;
	}
	*result = '\0';
	return p_result;
}

const char *break_single_byte_xor(const char *str, size_t len) // string hex encoded
{
	char *temp = malloc(len);
	char ch = 0;
	int max = 0;
	const char *final_result;
	for (int i = 0; i < sizeof(alphabet); i++)
	{
		for (int j = 0; j < len - 1; j += 2)
		{
			temp[j] = digit_to_char[alphabet[i] >> 4];
			temp[j + 1] = digit_to_char[alphabet[i] & 0xF];
		}
		temp[len] = '\0';

		const char *result = fixed_xor(str, temp);
		const char *decoded_result = decode_hex_encoded_string(result);
		size_t len_decoded = len / 2; // 2 hex -> 1 character
		int count = 0;
		for (int j = 0; j < len_decoded; j++)
		{
			count += letters_score[decoded_result[j]];
		}

		if (max < count)
		{
			max = count;
			ch = alphabet[i];
			final_result = decoded_result;
		}
	}
	free(temp);

	return final_result;
}


void read_string_from_file(char *dst, FILE *f)
{
	char ch;
	while ((ch = fgetc(f)) != '\n' && ch != EOF)
	{
		*dst++ = ch;
	}
	*dst = '\0';
}

const char *break_single_byte_xor_from_file(FILE *f) // string hex encoded
{
	assert(f != 0);
	char str[256];
	char ch = 0;
	int max = 0;
	const char *final_result;
	while (read_string_from_file(str, f), *str != '\0')
	{
		size_t len = strlen(str);
		const char *result = break_single_byte_xor(str, len);
		int count = 0;
		for (int j = 0; j < len; j++)
		{
			count += letters_score[result[j]];
		}

		if (max < count)
		{
			max = count;
			final_result = result;
		}
	}
	printf("Result - %s\n", final_result);
}


int main(void)
{
	// Ch_1
	const char *test_1 = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";
	const char *result_1 = encode_ascii_to_base64(test_1);
	const char *answer_1 = "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0"
							"aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1"
							"c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0"
							"aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdl"
							"LCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4";
	assert(strcmp(result_1, answer_1) == 0);

	const char *test_2 = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	const char *result_2 = encode_hex_to_base64(test_2);
	const char *answer_2 = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
	assert(strcmp(result_2, answer_2) == 0);

	// Ch_2
	const char *test_3_1 = "1c0111001f010100061a024b53535009181c";
	const char *test_3_2 = "686974207468652062756c6c277320657965";
	const char *result_3 = fixed_xor(test_3_1, test_3_2);
	const char *answer_3 = "746865206b696420646f6e277420706c6179";
	assert(strcmp(result_3, answer_3) == 0);

	// Ch_3
	const char *test_4 = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	const char *result_4 = break_single_byte_xor(test_4, strlen(test_4));

	const char *test_5 = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	const char *result_5 = decode_hex_encoded_string(test_5);
	const char *answer_5 = "77316?x+x413=x9x(7-6<x7>x:9;76";
	assert(strcmp(result_5, answer_5) == 0);
	
	// Ch_4
	FILE *f = fopen("4.txt", "r");
	assert(f != 0);

	break_single_byte_xor_from_file(f);
}
