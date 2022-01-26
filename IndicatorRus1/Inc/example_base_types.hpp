
#ifndef __example_base_types__
#define __example_base_types__

#include "rus1_mainTitle.hpp"

// строка и её длина
struct t_strAndStrCnt
{
  const unsigned char* str;   // указатель на строку
  const unsigned char strCnt; // длина строки
};

#endif //#ifndef __example_base_types__