#include "utility.h"
#include <stdio.h>
#include <string.h>

HatterSpeak *hs_create(char *old, char *hat) {
  HatterSpeak *hs = (HatterSpeak *)malloc(sizeof(HatterSpeak));

  if (!hs) {
    return NULL;
  }

  hs->oldspeak = (char *)calloc(strlen(old) + 1, sizeof(char));

  if (!hs->oldspeak) {
    free(hs);
    return NULL;
  }

  strcpy(hs->oldspeak, old);

  if (!hat) {
    hs->hatterspeak = NULL;
    return hs;
  } else {
    hs->hatterspeak = (char *)calloc(strlen(hat) + 1, sizeof(char));
  }

  if (!hs->hatterspeak) {
    free(hs->oldspeak);
    free(hs);
    return NULL;
  }

  strcpy(hs->hatterspeak, hat);

  return hs;
}

void hs_delete(HatterSpeak *hs) {
  if (!hs) {
    return;
  }

  free(hs->oldspeak);

  if (hs->hatterspeak) {
    free(hs->hatterspeak);
  }

  free(hs);

  return;
}

void hs_print(HatterSpeak *hs) {
  if (!hs) {
    return;
  }
  printf("%s ", hs->oldspeak);
  printf("%s\n", hs->hatterspeak);
  return;
}
