#ifndef SAMPLE_H
#define SAMPLE_H

#include <QFile>
#include <QTextStream>
#include <QString>
#include <math.h>


// shuffle settings
extern float _param_probability;
extern uint _param_block_size;


void sample(QTextStream& in_stream, QTextStream& out_stream);

#endif // SAMPLE_H
