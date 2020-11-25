#include "../include/me/mestr.h"

#include <stdlib.h>

#include <string.h>

static void mestr_realloc(struct string* str, uint32_t size)
{
  str->allocated+=size;
  str->data = realloc(str->data, str->allocated * sizeof(char));
}

static void mewstr_realloc(struct wstring* str, uint32_t size)
{
  str->allocated+=size;
  str->data = realloc(str->data, str->allocated * sizeof(wchar_t));
}

/* string */

struct string* mestr_new(uint32_t initial_size)
{
  if (initial_size == 0)
    return NULL;

  struct string* str = calloc(sizeof(struct string), 1);

  if (mestr_init(str, initial_size) != 1)
    return NULL;

  return str;
}

int mestr_init(struct string* str, uint32_t initial_size)
{
  if (initial_size == 0)
    return 0;

  str->allocated = initial_size;
  str->length = 0;
  str->data = malloc(initial_size * sizeof(char));

  str->data[0] = '\0';
  return 1;
}

int mestr_apndc(struct string* str, char c)
{
  if (c == '\0')
    return 0;

  if (str->length + 1 >= str->allocated)
    mestr_realloc(str, 8);

  str->data[str->length] = c;
  str->length++;

  str->data[str->length] = '\0';
  return 1;
}

int mestr_apnd(struct string* str, char* s)
{
  if (s == NULL)
    return 0;

  uint32_t len = strlen(s);

  if (len == 0)
    return 0;

  if (str->length + len >= str->allocated)
    mestr_realloc(str, len + 8);

  for (uint32_t i = 0; i < len; i++)
    str->data[str->length + i] = s[i];

  str->length+=len;

  str->data[str->length] = '\0';
  return 1;
}

int mestr_erase(struct string* str)
{
  if (str->length == 0)
    return 0;

  str->length--;
  str->data[str->length] = '\0';
  return 1;
}

int mestr_clear(struct string* str)
{
  str->data[0] = '\0';
  str->length = 0;
  return 1;
}

int mestr_free(struct string* str)
{
  str->allocated = 0;
  str->length = 0;

  free(str->data);
  return 1;
}

char* mestr_extr(struct string* str)
{
  char* s = calloc(sizeof(char), str->length + 1);
  for (uint32_t i = 0; i < str->length; i++)
    s[i] = str->data[i];
  return s;
}

char* mestr_extrd(struct string* str)
{
  char* s = mestr_extr(str);
  free(str->data);
  return s;
}



/* wide string */

struct wstring* mewstr_new(uint32_t initial_size)
{
  if (initial_size == 0)
    return NULL;

  struct wstring* str = calloc(sizeof(struct wstring), 1);

  if (mewstr_init(str, initial_size) != 1)
    return NULL;

  return str;
}

int mewstr_init(struct wstring* str, uint32_t initial_size)
{
  if (initial_size == 0)
    return 0;

  str->allocated = initial_size;
  str->length = 0;
  str->data = malloc(initial_size * sizeof(wchar_t));
  str->data[0] = '\0';
  return 1;
}

int mewstr_apndc(struct wstring* str, wchar_t c)
{
  if (c == '\0')
    return 0;

  if (str->length >= str->allocated)
    mewstr_realloc(str, 8);

  str->data[str->length] = c;
  str->length++;

  str->data[str->length] = '\0';
  return 1;
}

int mewstr_apnd(struct wstring* str, wchar_t* s)
{
  if (s == NULL)
    return 0;

  uint32_t len = wcslen(s);

  if (len == 0)
    return 0;

  if (str->length + len >= str->allocated)
    mewstr_realloc(str, len + 8);

  for (uint32_t i = 0; i < len; i++)
    str->data[str->length + i] = s[i];

  str->length+=len;

  str->data[str->length] = '\0';
  return 1;
}

int mewstr_erase(struct wstring* str)
{
  if (str->length == 0)
    return 0;

  str->length--;
  str->data[str->length] = '\0';
  return 1;
}

int mewstr_clear(struct wstring* str)
{
  str->length = 0;
  str->data[0] = '\0';
  return 1;
}

int mewstr_free(struct wstring* str)
{
  str->allocated = 0;
  str->length = 0;

  free(str->data);
  return 1;
}

wchar_t* mewstr_extr(struct wstring* str)
{
  wchar_t* s = calloc(sizeof(wchar_t), str->length + 1);
  for (uint32_t i = 0; i < str->length; i++)
    s[i] = str->data[i];
  return s;
}

wchar_t* mewstr_extrd(struct wstring* str)
{
  wchar_t* s = mewstr_extr(str);
  free(str->data);
  return s;
}

/* utils */

char* ctostr(char c)
{
  char* str = calloc(sizeof(char), 2);
  str[0] = c;
  return str;
}

wchar_t* wctostr(wchar_t c)
{
  wchar_t* str = calloc(sizeof(wchar_t), 2);
  str[0] = c;
  return str;
}

char* strjoin(char* str1, char* str2)
{
  uint32_t len1 = strlen(str1);
  uint32_t len2 = strlen(str2);

  char* str = calloc(sizeof(char), len1 + len2 + 1);

  for (uint32_t i = 0; i < len1; i++)
    str[i] = str1[i];

  for (uint32_t i = 0; i < len2; i++)
    str[len1 + i] = str2[i];

  return str;
}

char* strjoinc(char* str, char c)
{
  uint32_t len = strlen(str);

  char* s = calloc(sizeof(char), len + 2);

  for (uint32_t i = 0; i < len; i++)
    s[i] = str[i];

  s[len] = c;
  return s;
}

char* strsub(char* str, uint32_t start, uint32_t end)
{
  uint32_t len = strlen(str);

  if (end > len)
    return NULL;

  char* s = calloc(sizeof(char), end + 1);

  for (uint32_t i = 0; i < end; i++)
    s[i] = str[i];
  return s;
}

int strlic(char* str, char c)
{
  uint32_t len = strlen(str);

  for (uint32_t i = len - 1; i--; )
  {
    if (str[i] == c)
      return i;
  }
  return -1;
}

int strfic(char* str, char c)
{
  uint32_t len = strlen(str);

  for (uint32_t i = 0; i < len; i++)
  {
    if (str[i] == c)
      return i;
  }
  return -1;
}

bool strempty(const char* str)
{
  if (str == NULL)
    return true;

  uint32_t len = strlen(str);
  return len == 0;
}

bool strendsc(const char* str, char c)
{
  uint32_t len = strlen(str);
  return str[len - 1] == c;
}

char* strmem(char* str)
{
  uint32_t len = strlen(str);

  char* s = calloc(sizeof(char), len);
  
  for (uint32_t i = 0; i < len; i++)
    s[i] = str[i];
  return s;
}
