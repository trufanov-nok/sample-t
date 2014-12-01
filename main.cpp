#include <QCoreApplication>
#include <QCommandLineParser>
#include "sample.h"
#include "utils.h"

float _param_probability = 1.0;
uint _param_block_size = 1;
bool _param_verbose = false;

QString in_filename;
QString out_filename;

bool processCommandLineArguments(QCommandLineParser& parser)
{
    if (parser.isSet("r") && parser.isSet("n"))
    {
        print("ERROR: -r and -n parameters are exclusive and can't be used together.");
                return false;
    }


    if (parser.isSet("o"))
        out_filename  = parser.value("o");

    QStringList pos_args = parser.positionalArguments();
    if (pos_args.count() > 0)
      in_filename = pos_args[0];

    uint seed  = 0;
    if(parser.isSet("s"))
    {
        seed  = parser.value("s").toUInt();
    } else
        seed = time(NULL);

    srand(seed);

    if(parser.isSet("p"))
        _param_block_size = parser.value("p").toUInt();

    if (parser.isSet("r"))
        _param_probability = parser.value("r").toFloat();

    _param_verbose = parser.isSet("v");

    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Sample-t");
    QCoreApplication::setApplicationVersion("0.5");

    QCommandLineParser parser;
    initCommandOptionsParser(parser);
    parser.process(app);

    if (!processCommandLineArguments(parser)) return -1;

    uint result = true;

    QTextStream in;

    if( !in_filename.isEmpty() )
    result = openFileSource(in, in_filename);
    else
        {
            print("reading data from stdin...\n");
            in_filename = readStdinToTmpFile();
            result = openFileSource(in, in_filename);
        }

    if (parser.isSet("n"))
        _param_probability = (float) countLines(in) / parser.value("n").toFloat() / _param_block_size;

    QTextStream out;

    if (!out_filename.isEmpty())
    {
        result = openFileDestination(out, out_filename);
    } else {
        result = openStdOutDestination(out);
    }


    sample(in, out);

    closeFileDestination();
    closeFileSource();

    return result;
}
