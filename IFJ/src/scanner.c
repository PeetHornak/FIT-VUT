#include "header/queue.h"
#include "header/scanner.h"
#include "header/garbagecollector.h"


static tQueue* my_queue;

unsigned length;

void myQueueInit(){
    my_queue = (tQueue*) gb_malloc(sizeof(tQueue));
    if(my_queue == NULL)
        return;

    queueInit(my_queue);
}

void myQueueFree(){
    gb_free(my_queue);
}

void firstChar(char **buffer, char c)
{
    *buffer = (char*)gb_malloc(2 * sizeof(char));
    (*buffer)[0] = c;
    (*buffer)[1] = '\0';
}

void addChar(char **buffer, char c, unsigned length)
{
	*buffer = (char*)gb_realloc(*buffer, (length+1) * sizeof(char));
    (*buffer)[length - 1] = c;
    (*buffer)[length] = '\0';
}

void ungetToken(int type, char *buffer)
{
    queueUp(my_queue, buffer, type);
}


int getNextToken(char **buffer)
{
    FILE* f = stdin;

	if(!queueEmpty(my_queue)){
        tokenType temp = my_queue->temp_arr[my_queue->f_index];
        queueGet(my_queue, buffer);
		return temp;
	}

    *buffer = NULL;

	automataState state = STATE_BEGIN;
    char c;
	//	length is 1 because at index (length - 1) is stored char
	//	and at index length is stored \0
    length = 1;
    c=fgetc(f);
    while(1){
        switch (state) {
            case STATE_BEGIN:
				//	Skip whitespaces, but not newline because we need to return every newline
                while(isspace(c) && c != '\n'){
                    c=fgetc(f);
                }
                    state = STATE_BEGIN;
                switch(c){
                    case EOF :
                        return TYPE_EOF;
                        break;
                    case '+' :
                        return TYPE_PLUS;
                        break;
                    case '-' :
                        return TYPE_MINUS;
                        break;
                    case '*' :
                        return TYPE_MULT;
                        break;
                    case '/' :
                        return TYPE_DIV;
                        break;
                    case '(' :
                        return TYPE_L_BRE;
                        break;
                    case ')' :
                        return TYPE_R_BRE;
                        break;
                    case '"' :
                        state = STATE_QUOT;
                        break;
                    case ',' :
                        return TYPE_COMMA;
                        break;
					case '=' :
						state = STATE_ASSIGN;
						break;
					case '<' :
						state = STATE_LESS;
						break;
					case '>' :
						state = STATE_GREAT;
						break;
					case '!' :
						state = STATE_NEG;
						break;
					case '#' :
						state = STATE_LCOM;
						break;
					case '\n' :
						state = STATE_BCOM;
						break;
					default :
						if(isdigit(c)){
							state = STATE_INT;
                        }else if((c >= 'a' && c <= 'z') || c == '_'){
							state = STATE_ID;
                        }
                        else
							// This will free every allocation and exit program with error code '1'
                            gb_exit_process(1);
                        break;
                }
            break;

            case STATE_QUOT :
                while((c=fgetc(f)) != '"'){
					//	If there is no second quote, end at EOF with error
					if(c == EOF)
						gb_exit_process(1);
					//	'\"' does not end string
					else if(c == '\\'){
						c = fgetc(f);
						//  All posible escape sequentions
						switch (c) {
							//	quote
							case '"' :
								if(length == 1){
									firstChar(buffer, '"');
									length++;
								}else
									addChar(buffer, '"', length++);
								continue;
							//	new line
							case 'n' :
								if(length == 1){
									firstChar(buffer, '\n');
									length++;
								}else
									addChar(buffer, '\n', length++);
								continue;
							//	tab
							case 't' :
								if(length == 1){
									firstChar(buffer, '\t');
									length++;
								}else
									addChar(buffer, '\t', length++);
								continue;
							// space
							case 's' :
								if(length == 1){
									firstChar(buffer, ' ');
									length++;
								}else
									addChar(buffer, ' ', length++);
								continue;
							// backslash
							case '\\' :
								if(length == 1){
									firstChar(buffer, '\\');
									length++;
								}else
									addChar(buffer, '\\', length++);
								continue;
							// hex
							case 'x' :
								//	Check if char == [0-9] || [A-F]
								if(((c = fgetc(f)) >= 48 && c <= 57) || (c >= 65 && c <= 70)){
									char hex_escape[3];		// hexadecimal number
									int char_escape;		// decimal value of char

									hex_escape[0] = c;		//	add first hex number
									//	Check if next number is also hex
									if(((c = fgetc(f)) >= 48 && c <= 57) || (c >= 65 && c <= 70)){
										hex_escape[1] = c;
										hex_escape[2] = '\0';
									}else{
										ungetc(c, f);
										hex_escape[1] = '\0';
									}
									//	Convert hex to decimal
									char_escape = strtol(hex_escape, NULL, 16);
									if(length == 1){
										firstChar(buffer, char_escape);
										length++;
									}else
										addChar(buffer, char_escape, length++);
								}else{
									ungetc(c, f);
									gb_exit_process(1);
								}
								continue;
							//	If char after '\' is something else, skip '\' and save only next char
							default :
								c = fgetc(f);
						}
                    }
                    if(length == 1){
                        firstChar(buffer, c);
                        length++;
                    }else{
                        addChar(buffer,c,length);
                        length ++;
                    }
                }
                if(c == '"' && length == 1)
                    return TYPE_QUOT_EMPTY;
                else
                    return TYPE_QUOT;


			case STATE_ASSIGN :
				if((c = fgetc(f))== '='){
					// ==
					return TYPE_EQUAL;
				}else{
					// =
					ungetc(c, f);
					return TYPE_ASSIGN;
				}


			case STATE_LESS :
				if((c = fgetc(f)) == '='){
					//	<=
					return TYPE_LESS_EQUAL;
				}else{
					//	<
					ungetc(c, f);
					return TYPE_LESS;
				}


			case STATE_GREAT :
				if((c = fgetc(f)) == '='){
					//	>=
					return TYPE_GREAT_EQUAL;
				}else{
					//	>
					ungetc(c, f);
					return TYPE_GREAT;
				}


			case STATE_NEG :
				if((c = fgetc(f)) == '='){
					//	!=
					return TYPE_NEG_EQUAL;
				}else{
					ungetc(c, f);
					gb_exit_process(1);
				}
				break;


			case STATE_INT:
                firstChar(buffer,c);
				//	infinity while, because we do not know how
				//	long we will be reading sequence of number
                while(1){
                    if(!isdigit(c = fgetc(f))){
						//	Except for digits there can be only dot or exponent, nothing else
                        if(c == '.'){
                            length ++;
                            addChar(buffer,c,length);
                            state = STATE_INT_DOT;
                            break;
						}else if(c == 'e' || c == 'E'){
                            length ++;
                            addChar(buffer,c,length);
							state = STATE_INT_E;
							break;
						}else{
                            ungetc(c,f);
                            return TYPE_INT;
                        }
                    }else{
                        length ++;
                        addChar(buffer,c,length);
                	}
				}
                break;


			case STATE_INT_E :
				//	After exponent we can have +/- or number
                if((c = fgetc(f)) == '+' || c == '-'){
                    length ++;
                    addChar(buffer,c,length);
					if(isdigit(c = fgetc(f))){
						length ++;
						addChar(buffer,c,length);
						state = STATE_INT_EXPO;
						break;
					}else{
						ungetc(c, f);
						gb_exit_process(1);
					}
                }else if(isdigit(c)){
                    length ++;
                    addChar(buffer,c,length);
					//	Reading unless there is no number
					while(1){
						if(isdigit(c = fgetc(f))){
							length ++;
							addChar(buffer,c,length);
						}else{
							//	When there is no number, pass it to another state,
							//	where it return type of integer with exponent
							ungetc(c, f);
							state = STATE_INT_EXPO;
							break;
						}
					}
				}else{
					//	When we have after exponent something different as +/- or number
					ungetc(c, f);
					gb_exit_process(1);
				}


			case STATE_INT_EXPO :
				//	Store numbers unless there is no number,
				//	after it will return integer with exponent
				while(1){
					if(isdigit(c = fgetc(f))){
						length++;
						addChar(buffer, c, length);
					}else{
						ungetc(c, f);
						return TYPE_INT_EXPO;
					}
				}
                break;


			case STATE_INT_DOT :
				if(isdigit(c = fgetc(f))){
					length++;
					addChar(buffer, c, length);
					state = STATE_FLOAT;
				}else
					//	After dot can be only number, no exponent or plus/minus
					gb_exit_process(1);
				break;


            case STATE_FLOAT:
				while(1){
                    if(!isdigit(c = fgetc(f))){
						//	If the is not number, there can be only 'e' or 'E' as exponent
                        if(c == 'e' || c == 'E'){
                            length ++;
                            addChar(buffer,c,length);
                            state = STATE_FLOAT_E;
                            break;
                        }else{
							//	In 'c' is not number or 'e', 'E' so it is end of number
                            ungetc(c,f);
                            return TYPE_FLOAT;
                        }
                    }else{
						//	There is number, so we store it in buffer
                        length ++;
                        addChar(buffer,c,length);
                    }
                }
                break;


            case STATE_FLOAT_E:
				//	After exponent we can have only +/- or number
                if((c = fgetc(f)) == '+' || c == '-'){
                    length ++;
                    addChar(buffer,c,length);
                    state = STATE_FLOAT_EXPO_PLUS;
                }else if(isdigit(c)){
                    length ++;
                    addChar(buffer,c,length);
					state = STATE_FLOAT_EXPO;
				}else{
	                ungetc(c,f);
					gb_exit_process(1);
				}
				break;


			case STATE_FLOAT_EXPO_PLUS :
				//	After +/- we have to have at least one number
				if(isdigit(c = fgetc(f))){
					length ++;
					addChar(buffer, c , length);
					state = STATE_FLOAT_EXPO;
				}else
					gb_exit_process(1);
				break;


			case STATE_FLOAT_EXPO :
				while(1){
					if(isdigit(c = fgetc(f))){
						length++;
						addChar(buffer, c, length);
					}else{
						ungetc(c, f);
						return TYPE_FLOAT_EXPO;
					}
				}
                break;


			case STATE_ID :
                firstChar(buffer,c);
				while(1){
					c = fgetc(f);
					//	In ID we can have any alpha or number or '_'
					if(isalnum(c) || c == '_'){
						length++;
						addChar(buffer, c, length);
					//	If there is '?' or '!' at end, it is ID of function
					}else if(c == '?' || c == '!'){
						length++;
						addChar(buffer, c, length);
						return TYPE_FUNC_ID;
					}else{
					//	Check if stored ID is not keyword, nil or predefined function
						ungetc(c, f);
                        if( strcmp(*buffer,"def")   == 0 ||
                            strcmp(*buffer,"do")    == 0 ||
                            strcmp(*buffer,"else")  == 0 ||
                            strcmp(*buffer,"end")   == 0 ||
                            strcmp(*buffer,"if")    == 0 ||
                            strcmp(*buffer,"not")   == 0 ||
                            strcmp(*buffer,"then")  == 0 ||
                            strcmp(*buffer,"while") == 0 )
                            return TYPE_KEYWORD;

                        if(strcmp(*buffer,"nil") == 0)
                            return TYPE_NIL;

                        if( strcmp(*buffer,"print") == 0 ||
                            strcmp(*buffer,"length")== 0 ||
                            strcmp(*buffer,"inputi")== 0 ||
                            strcmp(*buffer,"inputs")== 0 ||
                            strcmp(*buffer,"inputf")== 0 ||
                            strcmp(*buffer,"substr")== 0 ||
                            strcmp(*buffer,"ord")   == 0 ||
                            strcmp(*buffer,"chr")   == 0 )
                            return TYPE_PRE_FUNC;
                        return TYPE_ID;
					}
				}
                break;


			case STATE_LCOM :
				while(1){
					if((c = fgetc(f)) == '\n'){
						state = STATE_BEGIN;
						break;
					}
				}
				break;

			//	In block comment we are checking every char in '=begin'
			case STATE_BCOM :
				if((c = fgetc(f)) == '=')
					state = STATE_BCOM_EQUALS;
				else{
				//	Return EOL, if there is no '=' at beggining of line,
				//	because we get here by reading newline
                    ungetc(c, f);
                    return TYPE_EOL;
				}
				break;


			case STATE_BCOM_EQUALS :
				if((c = fgetc(f)) == 'b')
					state = STATE_BCOM_B;
				else
					gb_exit_process(1);
				break;


			case STATE_BCOM_B :
				if((c = fgetc(f)) == 'e')
					state = STATE_BCOM_E;
				else
					gb_exit_process(1);
				break;


			case STATE_BCOM_E :
				if((c = fgetc(f)) == 'g')
					state = STATE_BCOM_G;
				else
					gb_exit_process(1);
				break;


			case STATE_BCOM_G :
				if((c = fgetc(f)) == 'i')
					state = STATE_BCOM_I;
				else
					gb_exit_process(1);
				break;


			case STATE_BCOM_I :
				if((c = fgetc(f)) == 'n')
					state = STATE_BCOM_N;
				else
					gb_exit_process(1);
				break;


			case STATE_BCOM_N :
				if(isspace(c = fgetc(f)))
					state = STATE_BCOM_COM;
				else
					gb_exit_process(1);
				break;


			case STATE_BCOM_COM :
				//	Read and forget char unless there is newline,
				//	because after newline can be end of block comment
				while(1){
					if((c = fgetc(f)) == 10){
						state = STATE_BCOM_COM_EOL;
						break;
					}
				}
				break;


			case STATE_BCOM_COM_EOL :
				if((c = fgetc(f)) == '=')
					state = STATE_BCOM_COM_EQUALS;
				else
					state = STATE_BCOM_COM;
				break;


			case STATE_BCOM_COM_EQUALS :
				if((c = fgetc(f)) == 'e')
					state = STATE_BCOM_COM_E;
				else
					state = STATE_BCOM_COM;
				break;


			case STATE_BCOM_COM_E :
				if((c = fgetc(f)) == 'n')
					state = STATE_BCOM_COM_N;
				else
					state = STATE_BCOM_COM;
				break;


			case STATE_BCOM_COM_N :
				if((c = fgetc(f)) == 'd')
					state = STATE_BCOM_COM_D;
				else
					state = STATE_BCOM_COM;
				break;


			case STATE_BCOM_COM_D :
				// After '=end' have to be at least one whitespace
				if(isspace(c = fgetc(f)))
					state = STATE_BCOM_COM_2;
				else
					gb_exit_process(1);
				break;


			case STATE_BCOM_COM_2 :
				//	Still on ending line is block of comment
				while(1){
					if((c = fgetc(f)) == 10){
                        ungetc(c ,f);
						state = STATE_BEGIN;
						break;
					}
				}
				break;



	    }   //switch
    }   //cyklus

}
