#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef _SCANNER_H_
#define _SCANNER_H_

typedef enum automataState{
    STATE_BEGIN,
    STATE_INT,
	STATE_INT_DOT,
	STATE_INT_E,
	STATE_INT_EXPO,
    STATE_FLOAT,
    STATE_FLOAT_E,
	STATE_FLOAT_EXPO_PLUS,
	STATE_FLOAT_EXPO,
    STATE_ID,
    STATE_LCOM,
    STATE_BCOM,
	STATE_BCOM_EQUALS,
	STATE_BCOM_B,
	STATE_BCOM_E,
	STATE_BCOM_G,
	STATE_BCOM_I,
	STATE_BCOM_N,
	STATE_BCOM_COM,
	STATE_BCOM_COM_EOL,
	STATE_BCOM_COM_EQUALS,
	STATE_BCOM_COM_E,
	STATE_BCOM_COM_N,
	STATE_BCOM_COM_D,
	STATE_BCOM_COM_2,
	STATE_ASSIGN,
	STATE_LESS,
	STATE_GREAT,
	STATE_NEG,
    STATE_QUOT,
} automataState;

typedef enum tokenType{
    TYPE_PLUS,          //	0  +
    TYPE_MINUS,         //  1	-
    TYPE_MULT,          //  2	*
    TYPE_DIV,           //  3	/
    TYPE_ASSIGN,        //  4	=
    TYPE_EQUAL,         //  5	==
    TYPE_GREAT,         //  6	>
    TYPE_LESS,          //  7	<
    // TYPE_NEG,            !
    TYPE_GREAT_EQUAL,   //  8	>=
    TYPE_LESS_EQUAL,    //  9	<=
    TYPE_NEG_EQUAL,     //  10	!=
    TYPE_L_BRE,         //  11	(
    TYPE_R_BRE,         //  12	)
    TYPE_COMMA,         //  13	,
    TYPE_QUOT,          //  14	"	Return string between quotes in 'buffer'
    TYPE_QUOT_EMPTY,    //  15	""
    TYPE_INT,			//  16	Integer
	TYPE_INT_EXPO,		//	17	Integer with exponential
    TYPE_FLOAT,			//	18	Float (decimal number)
	TYPE_FLOAT_EXPO,	//	19	Float with exponential (decimal number)
	TYPE_ID,			//	20	ID of variable or function
	TYPE_FUNC_ID,		//	21	ID of function ( ? or ! at end )
    TYPE_KEYWORD,       //  22	Keyword
    TYPE_PRE_FUNC,      //  23	Predefined function
    TYPE_EOL,           //  24	End of line
    TYPE_EOF,			// 	25	End of file
	TYPE_ERROR,			//	26	Some Error
    TYPE_NIL            //  27	nil

} tokenType;

void myQueueInit();

void myQueueFree();

/**
 *	Function allocate memory for buffer, where it adds first char from reading text
 *
 *	@brief	Function for initializing buffer
 *
 *	@param	*buffer	Pointer to ...
 *			c		Char to be inserted to buffer
 */
void firstChar(char **buffer, char c);


/**
 *	Function reallocate memory for bigger size and insert other char from reading text
 *
 *	@brief	Insert another char to buffer
 *
 *	@param	*buffer Pointer to ...
 *			c		Char to be inserted to buffer
 *			length	New length of buffer
 */
void addChar(char **buffer, char c, unsigned length);

/**
 *	Function remembers last token and returt it next time again
 *
 *	@brief	Receive token and return it at next calling
 *
 *	@param	type	Type of returned token
 */
void ungetToken(int type, char *buffer);

/**
 *	Function read text and parse it to different types
 *
 *	@param	*buffer	Pointer to ...
 *	@return	Number from enum tokenType.
 */
int getNextToken(char **buffer);

#endif
