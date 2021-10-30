/*============================================================================
  message.h
============================================================================*/
#ifndef MESSAGE_H
#define MESSAGE_H
#define NUM_PI_MSGS	50
#define MAX_MESGS       8
extern OSMesg PiMessages[NUM_PI_MSGS];
extern OSMesg dmaMessageBuf;
extern OSMesgQueue PiMessageQ;
extern OSMesgQueue dmaMessageQ;
extern OSIoMesg dmaIOMessageBuf;

#endif /* MESSAGE_H */
