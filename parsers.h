#ifndef PARSERS_H
#define	PARSERS_H

int* parseMessage(char* jsonString, int start, int numValues);;
void writeMsgToXML(int* values);
void writeWarningToXML(int* values);

#endif	/* PARSERS_H */