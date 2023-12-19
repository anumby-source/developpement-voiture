//#define DEBUG
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// codes télécommande supplémentaire
#define UP1    0x807F6897
#define RIGHT1 0x807F0AF5
#define DOWN1  0x807F58A7
#define LEFT1  0x807F8A75
#define ENTER1 0x807FC837
#define BACK1  0x807F9867
#define PLUS1  0x807F18E7
#define MOINS1 0x807F08F7
#define MOUSE1 0x807F00FF
#define HOME1  0x807F8877
#define ONOFF1 0x807F02FD

// codes télécommande
#define UP    0x1FE1CE3
#define RIGHT 0x1FE9C63
#define DOWN  0x1FE02FD
#define LEFT  0x1FEEC13
#define ENTER 0x1FEC837
#define BACK  0x1FEE41B
#define PLUS  0x1FEE11E
#define MOINS 0x1FE916E
#define MOUSE 0x1FE12ED
#define HOME  0x1FECE31
#define ONOFF 0x1FE817E
