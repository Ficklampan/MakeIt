#ifndef MELIB_STR_H
  #define MELIB_STR_H

#include "metype.h"

#include <wchar.h>

struct string {
  uint32_t allocated;
  uint32_t length;
  char* data;
};

struct wstring {
  uint32_t allocated;
  uint32_t length;
  wchar_t* data;
};

struct string* mestr_new(uint32_t initial_size);
int mestr_init(struct string* str, uint32_t initial_size);
int mestr_apndc(struct string* str, char c);
int mestr_apnd(struct string* str, char* s);
int mestr_erase(struct string* str);
int mestr_clear(struct string* str);
int mestr_free(struct string* str);
char* mestr_extr(struct string* str);
char* mestr_extrd(struct string* str);

struct wstring* mewstr_new(uint32_t initial_size);
int mewstr_init(struct wstring* str, uint32_t initial_size);
int mewstr_apndc(struct wstring* str, wchar_t c);
int mewstr_apnd(struct wstring* str, wchar_t* s);
int mewstr_erase(struct wstring* str);
int mewstr_clear(struct wstring* str);
int mewstr_free(struct wstring* str);
wchar_t* mewstr_extr(struct wstring* str);
wchar_t* mewstr_extrd(struct wstring* str);

char* ctostr(char c);
wchar_t* wctostr(wchar_t c);

char* strjoin(char* str1, char* str2);
char* strjoinc(char* str, char c);
char* strsub(char* str, uint32_t start, uint32_t end);

int strlic(char* str, char c);
int strfic(char* str, char c);

bool strempty(const char* str);
bool strendsc(const char* str, char c);

char* strmem(char* str);

#endif
