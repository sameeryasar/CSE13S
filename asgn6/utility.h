#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HatterSpeak {
  char *oldspeak;
  char *hatterspeak;
} HatterSpeak;

HatterSpeak *hs_create(char *old, char *hat);

void hs_delete(HatterSpeak *hs);

void hs_print(HatterSpeak *hs);

#endif
