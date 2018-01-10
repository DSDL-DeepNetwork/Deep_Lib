#ifndef PARSER_H
#define PARSER_H
#include "sg_dsdl.h"
#include "network.h"

void save_weights(network net, char *filename);
void load_weights(network net, char *filename);

void save_weights_upto(network net, char *filename, int cutoff);
void load_weights_upto(network net, char *filename, int cutoff);

//void save_weights_double(network net, char *filename);

#endif
