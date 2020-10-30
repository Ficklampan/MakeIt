#ifndef MI_TOKEN_HPP
  #define MI_TOKEN_HPP

#ifndef TOKEN_IS_CONSTANT
  #define TOKEN_IS_CONSTANT(t) (t == mi::Token::STRING || t == mi::Token::CHARACTER || \
      t == mi::Token::INTEGER || t == mi::Token::LONG || t == mi::Token::FLOAT || \
      t == mi::Token::DOUBLE || t == mi::Token::BOOLEAN)
#endif

namespace mi {

  enum Punctuator {
    L_SQUARE,
    R_SQUARE,

    L_BRACE,
    R_BRACE,

    L_PAREN,
    R_PAREN,

    EQUAL,
    PLUS, MINUS,
    PLUS_EQUAL, MINUS_EQUAL,

    COMMA,
    COLON
  };

  struct Token {

    enum Type {
      UNKNOWN,
      IDENTIFIER,
      STRING,
      INTEGER,
      PUNCTUATOR,
      RESET
    } type;

    union Value {
      void* v;
      int* i;
    } value;

    Token(Type type, void* value);

  };

}

#endif
