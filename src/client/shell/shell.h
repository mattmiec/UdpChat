//
// Created by matt on 7/7/19.
//

#ifndef UDPCHAT_SHELL_H
#define UDPCHAT_SHELL_H

#define MAXLEN 1024
#define MAXLEN_STR "1024"
#define MAXCOMMANDLEN 6
#define MAXCOMMANDLEN_STR "6"

/// enum for command type
enum commandtype{reg, dereg, send, quit};

/// get command type and body from user (
/// body buffer size specified by maxlen
/// error returns 1
/// success returns 0
int get_command(enum commandtype *type, char* body, int maxlen);

/// prompt for user input with ">>>"
/// input buffer size specified by maxlen
/// error returns 1
/// success returns 0
void prompt_command(char* input, int maxlen);

/// write output with ">>>" prepended
void write_output(char* output);

#endif //UDPCHAT_SHELL_H
