#ifndef PARSERS_H
#define	PARSERS_H

int* parseMessage(char* jsonString, int start, int numValues);;
void writeMsgToXML(int* values, char* datetime);
void writeWarningToXML(int* values, char* datetime);

#endif	/* PARSERS_H */