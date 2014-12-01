#include "sample.h"

void sample(QTextStream &in_stream, QTextStream &out_stream)
{
    int treshhold = (_param_probability >= 1.0)? RAND_MAX: fabs(_param_probability) * RAND_MAX;
    while (!in_stream.atEnd())
    {
        if (rand() <= treshhold)
        {
            uint counter = 0;
            while ((counter++ < _param_block_size) &&  !in_stream.atEnd())
                out_stream << in_stream.readLine() << '\n';
        }
        else
            in_stream.readLine();
    }

}
