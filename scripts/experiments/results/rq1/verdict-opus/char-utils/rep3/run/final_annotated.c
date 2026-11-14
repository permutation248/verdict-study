int CHARUTIL_BINARY_COUNT(void);
int CHARUTIL_BINARY_COUNT_GROKKABLE(void);
int CHARUTIL_OCTAL_COUNT(void);
int CHARUTIL_OCTAL_COUNT_GROKKABLE(void);
int CHARUTIL_DIGIT_COUNT(void);
int CHARUTIL_DIGIT_COUNT_GROKKABLE(void);
int CHARUTIL_HEX_COUNT(void);
int CHARUTIL_HEX_COUNT_GROKKABLE(void);
int CHARUTIL_ALPHABET_COUNT(void);
int CHARUTIL_ALPHABET_COUNT_GROKKABLE(void);
int CHARUTIL_ALPHANUMERIC_COUNT(void);
int CHARUTIL_ALPHANUMERIC_COUNT_GROKKABLE(void);
_Bool IS_BINARY(char ch);
_Bool IS_OCTAL(char ch);
_Bool IS_DIGIT(char ch);
_Bool IS_LOWER(char ch);
_Bool IS_UPPER(char ch);
_Bool IS_ALPHA(char ch);
_Bool IS_ALNUM(char ch);
_Bool IS_HEX_DIGIT(char ch);
_Bool IS_PRINTABLE(char ch);
_Bool IS_ASCII(char ch);
_Bool IS_EXTENDED_ASCII(char ch);
_Bool IS_SPACE(char ch);
_Bool IS_PUNCT(char ch);
_Bool IS_BRACKET(char ch);
_Bool IS_SYMBOL(char ch);
_Bool IS_BINARY_GROKKABLE(char ch);
_Bool IS_OCTAL_GROKKABLE(char ch);
_Bool IS_DIGIT_GROKKABLE(char ch);
_Bool IS_LOWER_GROKKABLE(char ch);
_Bool IS_UPPER_GROKKABLE(char ch);
_Bool IS_ALPHA_GROKKABLE(char ch);
_Bool IS_ALNUM_GROKKABLE(char ch);
_Bool IS_HEX_DIGIT_GROKKABLE(char ch);
_Bool IS_PRINTABLE_GROKKABLE(char ch);
int ASCII_TO_BINARY(char ch, int DEFAULT);
char BINARY_TO_ASCII(int num, char DEFAULT);
int FAST_ASCII_TO_BINARY(char ch);
char FAST_BINARY_TO_ASCII(int num);
int ASCII_TO_OCTAL(char ch, int DEFAULT);
char OCTAL_TO_ASCII(int num, char DEFAULT);
int FAST_ASCII_TO_OCTAL(char ch);
char FAST_OCTAL_TO_ASCII(int num);
int ASCII_TO_DIGIT(char ch);
char DIGIT_TO_ASCII(int num, char DEFAULT);
char FAST_DIGIT_TO_ASCII(int num);
int FAST_ASCII_TO_DIGIT(char ch);
int HEX_TO_INT(char ch, int DEFAULT);
int FAST_UPPERCASE_HEX_TO_INT(char ch);
int FAST_LOWERCASE_HEX_TO_INT(char ch);
char NIBBLE_TO_UPPERCASE_HEX(int n, char DEFAULT);
char NIBBLE_TO_LOWERCASE_HEX(int n, char DEFAULT);
char FAST_NIBBLE_TO_UPPERCASE_HEX(int n);
char FAST_NIBBLE_TO_LOWERCASE_HEX(int n);

/*@
assigns \nothing;
ensures \result == 2;
*/
int CHARUTIL_BINARY_COUNT(void)
{
  int __retres;
  __retres = 2;
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 2;
*/
int CHARUTIL_BINARY_COUNT_GROKKABLE(void)
{
  int __retres;
  __retres = (int)(sizeof("01") - (unsigned long)1);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 8;
*/
int CHARUTIL_OCTAL_COUNT(void)
{
  int __retres;
  __retres = 8;
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 8;
*/
int CHARUTIL_OCTAL_COUNT_GROKKABLE(void)
{
  int __retres;
  __retres = (int)(sizeof("01234567") - (unsigned long)1);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 10;
*/
int CHARUTIL_DIGIT_COUNT(void)
{
  int __retres;
  __retres = 10;
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 10;
*/
int CHARUTIL_DIGIT_COUNT_GROKKABLE(void)
{
  int __retres;
  __retres = (int)(sizeof("0123456789") - (unsigned long)1);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 16;
*/
int CHARUTIL_HEX_COUNT(void)
{
  int __retres;
  __retres = 16;
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 16;
*/
int CHARUTIL_HEX_COUNT_GROKKABLE(void)
{
  int __retres;
  __retres = (int)(sizeof("0123456789ABCDEF") - (unsigned long)1);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 26;
*/
int CHARUTIL_ALPHABET_COUNT(void)
{
  int __retres;
  __retres = 26;
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 26;
*/
int CHARUTIL_ALPHABET_COUNT_GROKKABLE(void)
{
  int __retres;
  __retres = (int)(sizeof("abcdefghijklmnopqrstuvwxyz") - (unsigned long)1);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 36;
*/
int CHARUTIL_ALPHANUMERIC_COUNT(void)
{
  int __retres;
  __retres = 36;
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 36;
*/
int CHARUTIL_ALPHANUMERIC_COUNT_GROKKABLE(void)
{
  int __retres;
  __retres = (int)(sizeof("0123456789abcdefghijklmnopqrstuvwxyz") - (unsigned long)1);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 1 <==> (ch == '0' || ch == '1');
ensures \result == 0 <==> (ch != '0' && ch != '1');
*/
_Bool IS_BINARY(char ch)
{
  _Bool __retres;
  int tmp;
  tmp = CHARUTIL_BINARY_COUNT();
  ;
  /*@ assert rte_34: signed_overflow: -2147483648 ≤ (int)ch - (int)'0'; */
  __retres = (_Bool)((unsigned int)((int)ch - '0') < (unsigned int)tmp);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 1 <==> ('0' <= ch <= '7');
ensures \result == 0 || \result == 1;
*/
_Bool IS_OCTAL(char ch)
{
  _Bool __retres;
  int tmp;
  tmp = CHARUTIL_OCTAL_COUNT();
  ;
  /*@ assert rte_38: signed_overflow: -2147483648 ≤ (int)ch - (int)'0'; */
  __retres = (_Bool)((unsigned int)((int)ch - '0') < (unsigned int)tmp);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == ((_Bool)((unsigned int)((int)ch - '0') < 10U));
ensures \result <==> (ch >= '0' && ch <= '9');
*/
_Bool IS_DIGIT(char ch)
{
  _Bool __retres;
  int tmp;
  tmp = CHARUTIL_DIGIT_COUNT();
  ;
  /*@ assert rte_35: signed_overflow: -2147483648 ≤ (int)ch - (int)'0'; */
  __retres = (_Bool)((unsigned int)((int)ch - '0') < (unsigned int)tmp);
  return __retres;
}

/*@
assigns \nothing;
ensures \result <==> ('a' <= ch <= 'z');
*/
_Bool IS_LOWER(char ch)
{
  _Bool __retres;
  int tmp;
  tmp = CHARUTIL_ALPHABET_COUNT();
  ;
  /*@ assert rte_40: signed_overflow: -2147483648 ≤ (int)ch - (int)'a'; */
  __retres = (_Bool)((unsigned int)((int)ch - 'a') < (unsigned int)tmp);
  return __retres;
}

/*@
assigns \nothing;
ensures \result <==> ('A' <= ch <= 'Z');
*/
_Bool IS_UPPER(char ch)
{
  _Bool __retres;
  int tmp;
  tmp = CHARUTIL_ALPHABET_COUNT();
  ;
  /*@ assert rte_41: signed_overflow: -2147483648 ≤ (int)ch - (int)'A'; */
  __retres = (_Bool)((unsigned int)((int)ch - 'A') < (unsigned int)tmp);
  return __retres;
}

/*@
assigns \nothing;
ensures \result <==> ('a' <= ch <= 'z') || ('A' <= ch <= 'Z');
ensures \result == \true || \result == \false;
*/
_Bool IS_ALPHA(char ch)
{
  _Bool __retres;
  int tmp_1;
  _Bool tmp;
  tmp = IS_LOWER(ch);
  if (tmp) tmp_1 = 1;
  else {
    _Bool tmp_0;
    tmp_0 = IS_UPPER(ch);
    if (tmp_0) tmp_1 = 1; else tmp_1 = 0;
  }
  __retres = (_Bool)(tmp_1 != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == \true || \result == \false;
ensures \result <==> (('0' <= ch <= '9') || ('a' <= ch <= 'z') || ('A' <= ch <= 'Z'));
*/
_Bool IS_ALNUM(char ch)
{
  _Bool __retres;
  int tmp_1;
  _Bool tmp;
  tmp = IS_DIGIT(ch);
  if (tmp) tmp_1 = 1;
  else {
    _Bool tmp_0;
    tmp_0 = IS_ALPHA(ch);
    if (tmp_0) tmp_1 = 1; else tmp_1 = 0;
  }
  __retres = (_Bool)(tmp_1 != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result <==> ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'));
ensures \result == 1 || \result == 0;
*/
_Bool IS_HEX_DIGIT(char ch)
{
  _Bool __retres;
  int tmp_0;
  _Bool tmp;
  tmp = IS_DIGIT(ch);
  if (tmp) tmp_0 = 1;
  else 
    /*@ assert rte_36: signed_overflow: -2147483648 ≤ (int)ch - (int)'A'; */
    if ((unsigned int)((int)ch - 'A') < (unsigned int)6) tmp_0 = 1;
    else 
      /*@ assert rte_37: signed_overflow: -2147483648 ≤ (int)ch - (int)'a'; */
      if ((unsigned int)((int)ch - 'a') < (unsigned int)6) tmp_0 = 1;
      else tmp_0 = 0;
  __retres = (_Bool)(tmp_0 != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 1 <==> (ch >= 32 && ch <= 126);
ensures \result == 0 || \result == 1;
*/
_Bool IS_PRINTABLE(char ch)
{
  _Bool __retres;
  /*@ assert rte_39: signed_overflow: -2147483648 ≤ (int)ch - 32; */
  __retres = (_Bool)((unsigned int)((int)ch - 32) < (unsigned int)95);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == ((_Bool)((unsigned int)ch < (unsigned int)128));
*/
_Bool IS_ASCII(char ch)
{
  _Bool __retres;
  __retres = (_Bool)((unsigned int)ch < (unsigned int)128);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == (_Bool)((unsigned int)ch < (unsigned int)256);
*/
_Bool IS_EXTENDED_ASCII(char ch)
{
  _Bool __retres;
  __retres = (_Bool)((unsigned int)ch < (unsigned int)256);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 1 || \result == 0;
ensures \result == 1 <==> ((int)ch == ' ' || (int)ch == '\t' || (int)ch == '\n' || (int)ch == '\r' || (int)ch == '\f' || (int)ch == '\v');
*/
_Bool IS_SPACE(char ch)
{
  _Bool __retres;
  int tmp;
  if ((int)ch == ' ') tmp = 1;
  else 
    if ((int)ch == '\t') tmp = 1;
    else 
      if ((int)ch == '\n') tmp = 1;
      else 
        if ((int)ch == '\r') tmp = 1;
        else 
          if ((int)ch == '\f') tmp = 1;
          else 
            if ((int)ch == '\v') tmp = 1; else tmp = 0;
  __retres = (_Bool)(tmp != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 0 || \result == 1;
ensures \result == 1 ==> (ch >= 32 && ch <= 126);
*/
_Bool IS_PUNCT(char ch)
{
  _Bool __retres;
  int tmp_2;
  _Bool tmp;
  tmp = IS_PRINTABLE(ch);
  if (tmp) {
    _Bool tmp_0;
    tmp_0 = IS_ALNUM(ch);
    if (tmp_0) tmp_2 = 0;
    else {
      _Bool tmp_1;
      tmp_1 = IS_SPACE(ch);
      if (tmp_1) tmp_2 = 0; else tmp_2 = 1;
    }
  }
  else tmp_2 = 0;
  __retres = (_Bool)(tmp_2 != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 1 || \result == 0;
ensures (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']') ==> \result == 1;
ensures !(ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']') ==> \result == 0;
*/
_Bool IS_BRACKET(char ch)
{
  _Bool __retres;
  int tmp;
  if ((int)ch == '(') tmp = 1;
  else 
    if ((int)ch == ')') tmp = 1;
    else 
      if ((int)ch == '{') tmp = 1;
      else 
        if ((int)ch == '}') tmp = 1;
        else 
          if ((int)ch == '[') tmp = 1;
          else 
            if ((int)ch == ']') tmp = 1; else tmp = 0;
  __retres = (_Bool)(tmp != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 0 || \result == 1;
*/
_Bool IS_SYMBOL(char ch)
{
  _Bool __retres;
  int tmp_1;
  _Bool tmp;
  tmp = IS_PUNCT(ch);
  if (tmp) {
    _Bool tmp_0;
    tmp_0 = IS_BRACKET(ch);
    if (tmp_0) tmp_1 = 0; else tmp_1 = 1;
  }
  else tmp_1 = 0;
  __retres = (_Bool)(tmp_1 != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == ((ch == '0') || (ch == '1'));
*/
_Bool IS_BINARY_GROKKABLE(char ch)
{
  _Bool __retres;
  int tmp;
  if ((int)ch == '0') tmp = 1;
  else 
    if ((int)ch == '1') tmp = 1; else tmp = 0;
  __retres = (_Bool)(tmp != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 1 <==> ((int)ch >= '0' && (int)ch <= '7');
ensures \result == 0 || \result == 1;
*/
_Bool IS_OCTAL_GROKKABLE(char ch)
{
  _Bool __retres;
  int tmp;
  if ((int)ch >= '0') 
    if ((int)ch <= '7') tmp = 1; else tmp = 0;
  else tmp = 0;
  __retres = (_Bool)(tmp != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 1 <==> ('0' <= ch && ch <= '9');
ensures \result == 0 || \result == 1;
*/
_Bool IS_DIGIT_GROKKABLE(char ch)
{
  _Bool __retres;
  int tmp;
  if ('0' <= (int)ch) 
    if ((int)ch <= '9') tmp = 1; else tmp = 0;
  else tmp = 0;
  __retres = (_Bool)(tmp != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 1 <==> ('a' <= ch && ch <= 'z');
ensures \result == 0 || \result == 1;
*/
_Bool IS_LOWER_GROKKABLE(char ch)
{
  _Bool __retres;
  int tmp;
  if ('a' <= (int)ch) 
    if ((int)ch <= 'z') tmp = 1; else tmp = 0;
  else tmp = 0;
  __retres = (_Bool)(tmp != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 1 <==> ('A' <= ch && ch <= 'Z');
ensures \result == 0 || \result == 1;
*/
_Bool IS_UPPER_GROKKABLE(char ch)
{
  _Bool __retres;
  int tmp;
  if ('A' <= (int)ch) 
    if ((int)ch <= 'Z') tmp = 1; else tmp = 0;
  else tmp = 0;
  __retres = (_Bool)(tmp != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 1 <==> (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z'));
ensures \result == 0 || \result == 1;
*/
_Bool IS_ALPHA_GROKKABLE(char ch)
{
  _Bool __retres;
  int tmp_1;
  _Bool tmp;
  tmp = IS_LOWER_GROKKABLE(ch);
  if (tmp) tmp_1 = 1;
  else {
    _Bool tmp_0;
    tmp_0 = IS_UPPER_GROKKABLE(ch);
    if (tmp_0) tmp_1 = 1; else tmp_1 = 0;
  }
  __retres = (_Bool)(tmp_1 != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 1 <==> (('0' <= ch && ch <= '9') || ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z'));
ensures \result == 0 || \result == 1;
*/
_Bool IS_ALNUM_GROKKABLE(char ch)
{
  _Bool __retres;
  int tmp_2;
  _Bool tmp;
  tmp = IS_DIGIT_GROKKABLE(ch);
  if (tmp) tmp_2 = 1;
  else {
    _Bool tmp_0;
    tmp_0 = IS_LOWER_GROKKABLE(ch);
    if (tmp_0) tmp_2 = 1;
    else {
      _Bool tmp_1;
      tmp_1 = IS_UPPER_GROKKABLE(ch);
      if (tmp_1) tmp_2 = 1; else tmp_2 = 0;
    }
  }
  __retres = (_Bool)(tmp_2 != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 1 || \result == 0;
ensures \result == 1 <==> (('0' <= ch && ch <= '9') || ('a' <= ch && ch <= 'f') || ('A' <= ch && ch <= 'F'));
*/
_Bool IS_HEX_DIGIT_GROKKABLE(char ch)
{
  _Bool __retres;
  int tmp;
  if ('0' <= (int)ch) {
    if ((int)ch <= '9') tmp = 1; else goto _LAND_0;
  }
  else {
    _LAND_0: ;
    if ('a' <= (int)ch) {
      if ((int)ch <= 'f') tmp = 1; else goto _LAND;
    }
    else {
      _LAND: ;
      if ('A' <= (int)ch) 
        if ((int)ch <= 'F') tmp = 1; else tmp = 0;
      else tmp = 0;
    }
  }
  __retres = (_Bool)(tmp != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures \result == 1 <==> (' ' <= ch && ch <= '~');
ensures \result == 0 || \result == 1;
*/
_Bool IS_PRINTABLE_GROKKABLE(char ch)
{
  _Bool __retres;
  int tmp;
  if (' ' <= (int)ch) 
    if ((int)ch <= '~') tmp = 1; else tmp = 0;
  else tmp = 0;
  __retres = (_Bool)(tmp != 0);
  return __retres;
}

/*@
assigns \nothing;
ensures (ch == '0') ==> \result == 0;
ensures (ch == '1') ==> \result == 1;
ensures (ch != '0' && ch != '1') ==> \result == DEFAULT;
*/
int ASCII_TO_BINARY(char ch, int DEFAULT)
{
  int tmp;
  if ((int)ch == '0') 
    /*@ assert rte_1: signed_overflow: -2147483648 ≤ (int)ch - (int)'0'; */
    tmp = (int)ch - '0';
  else 
    if ((int)ch == '1') 
      /*@ assert rte_2: signed_overflow: -2147483648 ≤ (int)ch - (int)'0'; */
      tmp = (int)ch - '0';
    else tmp = DEFAULT;
  return tmp;
}

/*@
assigns \nothing;
ensures ((unsigned int)num < 2) ==> (\result == (char)(num + '0'));
ensures ((unsigned int)num >= 2) ==> (\result == DEFAULT);
*/
char BINARY_TO_ASCII(int num, char DEFAULT)
{
  char __retres;
  int tmp;
  if ((unsigned int)num < (unsigned int)2) 
    /*@ assert rte_5: signed_overflow: num + (int)'0' ≤ 2147483647; */
    tmp = (int)((char)(num + '0'));
  else tmp = (int)DEFAULT;
  __retres = (char)tmp;
  return __retres;
}

/*@
assigns \nothing;
ensures \result == (int)ch - (int)'0';
*/
int FAST_ASCII_TO_BINARY(char ch)
{
  int __retres;
  /*@ assert rte_7: signed_overflow: -2147483648 ≤ (int)ch - (int)'0'; */
  __retres = (int)ch - '0';
  return __retres;
}

/*@
requires num >= 0;
requires num <= 2147483647 - (int)'0';
assigns \nothing;
ensures \result == (char)(num + '0');
*/
char FAST_BINARY_TO_ASCII(int num)
{
  char __retres;
  /*@ assert rte_10: signed_overflow: num + (int)'0' ≤ 2147483647; */
  __retres = (char)(num + '0');
  return __retres;
}

/*@
assigns \nothing;
ensures ('0' <= ch <= '7') ==> (\result == (int)ch - (int)'0');
ensures ('0' <= ch <= '7') ==> (0 <= \result <= 7);
ensures !(('0' <= ch) && (ch <= '7')) ==> (\result == DEFAULT);
*/
int ASCII_TO_OCTAL(char ch, int DEFAULT)
{
  int tmp;
  if ('0' <= (int)ch) 
    if ((int)ch <= '7') 
      /*@ assert rte_4: signed_overflow: -2147483648 ≤ (int)ch - (int)'0'; */
      tmp = (int)ch - '0';
    else tmp = DEFAULT;
  else tmp = DEFAULT;
  return tmp;
}

/*@
assigns \nothing;
ensures (0 <= num <= 7) ==> \result == (char)(num + '0');
ensures !(0 <= num <= 7) ==> \result == DEFAULT;
ensures (0 <= num <= 7) ==> ('0' <= \result <= '7');
ensures \result == (char)(num + '0') || \result == DEFAULT;
*/
char OCTAL_TO_ASCII(int num, char DEFAULT)
{
  char __retres;
  int tmp;
  if ((unsigned int)num < (unsigned int)8) 
    /*@ assert rte_48: signed_overflow: num + (int)'0' ≤ 2147483647; */
    tmp = (int)((char)(num + '0'));
  else tmp = (int)DEFAULT;
  __retres = (char)tmp;
  return __retres;
}

/*@
assigns \nothing;
ensures \result == (int)ch - (int)'0';
*/
int FAST_ASCII_TO_OCTAL(char ch)
{
  int __retres;
  /*@ assert rte_9: signed_overflow: -2147483648 ≤ (int)ch - (int)'0'; */
  __retres = (int)ch - '0';
  return __retres;
}

/*@
requires 0 <= num <= 7;
assigns \nothing;
ensures \result == (char)(num + '0');
*/
char FAST_OCTAL_TO_ASCII(int num)
{
  char __retres;
  /*@ assert rte_23: signed_overflow: num + (int)'0' ≤ 2147483647; */
  __retres = (char)(num + '0');
  return __retres;
}

/*@
assigns \nothing;
ensures \result >= -1 && \result <= 9;
ensures (\result >= 0) ==> (\result == (int)ch - (int)'0');
ensures (\result == -1) ==> (ch < '0' || ch > '9');
ensures (ch >= '0' && ch <= '9') ==> (\result == (int)ch - (int)'0');
ensures (ch < '0' || ch > '9') ==> (\result == -1);
*/
int ASCII_TO_DIGIT(char ch)
{
  int tmp;
  if ('0' <= (int)ch) 
    if ((int)ch <= '9') 
      /*@ assert rte_3: signed_overflow: -2147483648 ≤ (int)ch - (int)'0'; */
      tmp = (int)ch - '0';
    else tmp = -1;
  else tmp = -1;
  return tmp;
}

/*@
assigns \nothing;
ensures (0 <= num <= 9) ==> (\result == (char)(num + '0'));
ensures !(0 <= num <= 9) ==> (\result == DEFAULT);
ensures (0 <= num <= 9) ==> ('0' <= \result <= '9');
*/
char DIGIT_TO_ASCII(int num, char DEFAULT)
{
  char __retres;
  int tmp;
  if ((unsigned int)num < (unsigned int)10) 
    /*@ assert rte_6: signed_overflow: num + (int)'0' ≤ 2147483647; */
    tmp = (int)((char)(num + '0'));
  else tmp = (int)DEFAULT;
  __retres = (char)tmp;
  return __retres;
}

/*@
requires 0 <= num <= 9;
assigns \nothing;
ensures \result == (char)(num + '0');
ensures '0' <= \result <= '9';
*/
char FAST_DIGIT_TO_ASCII(int num)
{
  char __retres;
  /*@ assert rte_11: signed_overflow: num + (int)'0' ≤ 2147483647; */
  __retres = (char)(num + '0');
  return __retres;
}

/*@
assigns \nothing;
ensures \result == (int)ch - (int)'0';
*/
int FAST_ASCII_TO_DIGIT(char ch)
{
  int __retres;
  /*@ assert rte_8: signed_overflow: -2147483648 ≤ (int)ch - (int)'0'; */
  __retres = (int)ch - '0';
  return __retres;
}

/*@
assigns \nothing;
ensures ('0' <= ch <= '9') ==> \result == (int)ch - '0';
ensures ('A' <= ch <= 'F') ==> \result == ((int)ch - 'A') + 10;
ensures ('a' <= ch <= 'f') ==> \result == ((int)ch - 'a') + 10;
ensures (!(('0' <= ch <= '9') || ('A' <= ch <= 'F') || ('a' <= ch <= 'f'))) ==> \result == DEFAULT;
ensures (('0' <= ch <= '9') || ('A' <= ch <= 'F') || ('a' <= ch <= 'f')) ==> (0 <= \result <= 15);
*/
int HEX_TO_INT(char ch, int DEFAULT)
{
  int __retres;
  if ('0' <= (int)ch) 
    if ((int)ch <= '9') {
      /*@ assert rte_29: signed_overflow: -2147483648 ≤ (int)ch - (int)'0'; */
      __retres = (int)ch - '0';
      goto return_label;
    }
  if ('A' <= (int)ch) 
    if ((int)ch <= 'F') {
      /*@ assert rte_30: signed_overflow: -2147483648 ≤ (int)ch - (int)'A'; */
      /*@ assert
        rte_31: signed_overflow: (int)((int)ch - (int)'A') + 10 ≤ 2147483647;
      */
      __retres = ((int)ch - 'A') + 10;
      goto return_label;
    }
  if ('a' <= (int)ch) 
    if ((int)ch <= 'f') {
      /*@ assert rte_32: signed_overflow: -2147483648 ≤ (int)ch - (int)'a'; */
      /*@ assert
        rte_33: signed_overflow: (int)((int)ch - (int)'a') + 10 ≤ 2147483647;
      */
      __retres = ((int)ch - 'a') + 10;
      goto return_label;
    }
  __retres = DEFAULT;
  return_label: return __retres;
}

/*@
requires ('0' <= ch && ch <= '9') || ('A' <= ch && ch <= 'F');
assigns \nothing;
ensures 0 <= \result <= 15;
ensures ('0' <= ch && ch <= '9') ==> \result == (int)ch - (int)'0';
ensures ('A' <= ch && ch <= 'F') ==> \result == (int)ch - (int)'A' + 10;
*/
int FAST_UPPERCASE_HEX_TO_INT(char ch)
{
  int tmp;
  if ('0' <= (int)ch) 
    if ((int)ch <= '9') 
      /*@ assert rte_24: signed_overflow: -2147483648 ≤ (int)ch - (int)'0'; */
      tmp = (int)ch - '0';
    else 
      /*@ assert rte_25: signed_overflow: -2147483648 ≤ (int)ch - (int)'A'; */
      /*@ assert
        rte_26: signed_overflow: (int)((int)ch - (int)'A') + 10 ≤ 2147483647;
      */
      tmp = ((int)ch - 'A') + 10;
  else 
    /*@ assert rte_27: signed_overflow: -2147483648 ≤ (int)ch - (int)'A'; */
    /*@ assert
        rte_28: signed_overflow: (int)((int)ch - (int)'A') + 10 ≤ 2147483647;
    */
    tmp = ((int)ch - 'A') + 10;
  return tmp;
}

/*@
requires ('0' <= ch && ch <= '9') || ('a' <= ch && ch <= 'f');
assigns \nothing;
ensures 0 <= \result <= 15;
ensures ('0' <= ch && ch <= '9') ==> \result == (int)ch - (int)'0';
ensures ('a' <= ch && ch <= 'f') ==> \result == ((int)ch - (int)'a') + 10;
*/
int FAST_LOWERCASE_HEX_TO_INT(char ch)
{
  int tmp;
  if ('0' <= (int)ch) 
    if ((int)ch <= '9') 
      /*@ assert rte_12: signed_overflow: -2147483648 ≤ (int)ch - (int)'0'; */
      tmp = (int)ch - '0';
    else 
      /*@ assert rte_13: signed_overflow: -2147483648 ≤ (int)ch - (int)'a'; */
      /*@ assert
        rte_14: signed_overflow: (int)((int)ch - (int)'a') + 10 ≤ 2147483647;
      */
      tmp = ((int)ch - 'a') + 10;
  else 
    /*@ assert rte_15: signed_overflow: -2147483648 ≤ (int)ch - (int)'a'; */
    /*@ assert
        rte_16: signed_overflow: (int)((int)ch - (int)'a') + 10 ≤ 2147483647;
    */
    tmp = ((int)ch - 'a') + 10;
  return tmp;
}

/*@
assigns \nothing;
ensures (0 <= n <= 9) ==> \result == (char)(n + '0');
ensures (10 <= n <= 15) ==> \result == (char)((n - 10) + 'A');
ensures !((unsigned int)n < 16) ==> \result == DEFAULT;
*/
char NIBBLE_TO_UPPERCASE_HEX(int n, char DEFAULT)
{
  char __retres;
  int tmp_0;
  if ((unsigned int)n < (unsigned int)16) {
    int tmp;
    if (n < 10) 
      /*@ assert rte_45: signed_overflow: n + (int)'0' ≤ 2147483647; */
      tmp = n + '0';
    else 
      /*@ assert rte_46: signed_overflow: -2147483648 ≤ n - 10; */
      /*@ assert
        rte_47: signed_overflow: (int)(n - 10) + (int)'A' ≤ 2147483647;
      */
      tmp = (n - 10) + 'A';
    tmp_0 = (int)((char)tmp);
  }
  else tmp_0 = (int)DEFAULT;
  __retres = (char)tmp_0;
  return __retres;
}

/*@
assigns \nothing;
ensures (0 <= n <= 9) ==> \result == (char)(n + '0');
ensures (10 <= n <= 15) ==> \result == (char)((n - 10) + 'a');
ensures !((unsigned int)n < (unsigned int)16) ==> \result == DEFAULT;
*/
char NIBBLE_TO_LOWERCASE_HEX(int n, char DEFAULT)
{
  char __retres;
  int tmp_0;
  if ((unsigned int)n < (unsigned int)16) {
    int tmp;
    if (n < 10) 
      /*@ assert rte_42: signed_overflow: n + (int)'0' ≤ 2147483647; */
      tmp = n + '0';
    else 
      /*@ assert rte_43: signed_overflow: -2147483648 ≤ n - 10; */
      /*@ assert
        rte_44: signed_overflow: (int)(n - 10) + (int)'a' ≤ 2147483647;
      */
      tmp = (n - 10) + 'a';
    tmp_0 = (int)((char)tmp);
  }
  else tmp_0 = (int)DEFAULT;
  __retres = (char)tmp_0;
  return __retres;
}

/*@
requires 0 <= n <= 15;
assigns \nothing;
ensures (n < 10) ==> (\result == '0' + n);
ensures (n >= 10) ==> (\result == 'A' + (n - 10));
ensures ('0' <= \result <= '9') || ('A' <= \result <= 'F');
*/
char FAST_NIBBLE_TO_UPPERCASE_HEX(int n)
{
  char __retres;
  int tmp;
  if (n < 10) 
    /*@ assert rte_20: signed_overflow: n + (int)'0' ≤ 2147483647; */
    tmp = n + '0';
  else 
    /*@ assert rte_21: signed_overflow: -2147483648 ≤ n - 10; */
    /*@ assert rte_22: signed_overflow: (int)(n - 10) + (int)'A' ≤ 2147483647;
    */
    tmp = (n - 10) + 'A';
  __retres = (char)tmp;
  return __retres;
}

/*@
requires 0 <= n <= 15;
assigns \nothing;
ensures (n < 10) ==> (\result == '0' + n);
ensures (n >= 10) ==> (\result == 'a' + (n - 10));
ensures ('0' <= \result <= '9') || ('a' <= \result <= 'f');
*/
char FAST_NIBBLE_TO_LOWERCASE_HEX(int n)
{
  char __retres;
  int tmp;
  if (n < 10) 
    /*@ assert rte_17: signed_overflow: n + (int)'0' ≤ 2147483647; */
    tmp = n + '0';
  else 
    /*@ assert rte_18: signed_overflow: -2147483648 ≤ n - 10; */
    /*@ assert rte_19: signed_overflow: (int)(n - 10) + (int)'a' ≤ 2147483647;
    */
    tmp = (n - 10) + 'a';
  __retres = (char)tmp;
  return __retres;
}