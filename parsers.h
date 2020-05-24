#ifndef PARSERS_H
#define	PARSERS_H

int checkIfWarning(char* jsonString);
int* parseMessage(char* jsonString, int numValues);;
void writeMsgToXML(int* values, char* datetime);
void writeWarningToXML(char* warningType, int* values, char* datetime);

#endif	/* PARSERS_H */