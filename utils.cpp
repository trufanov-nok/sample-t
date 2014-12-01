#include "utils.h"

// internal data
QFile source_file;
QFile destination_file;
bool is_temporary_file;

void initCommandOptionsParser(QCommandLineParser &parser)
{
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.addVersionOption();
    parser.addHelpOption();
    parser.setApplicationDescription(".");
    parser.addOption(QCommandLineOption(QStringList() << "r" << "rate","Sample rate. Equal to probability that sample stats on current line. Value shall be in [0,1]", "VALUE"));
    parser.addOption(QCommandLineOption(QStringList() << "o" << "output", "Write output to OUTPUT-FILE instead of standard output.", "OUTPUT-FILE"));
    parser.addOption(QCommandLineOption(QStringList() << "s" << "seed", "Initialize rand function with given seed. Overwise system time is used.", "VALUE"));
    parser.addOption(QCommandLineOption(QStringList() << "p" << "packet", "Size of sample in lines. Equal to 1 by default.", "SIZE", "1"));
    parser.addOption(QCommandLineOption(QStringList() << "n" << "examples", "Aprox. number of examples to output. The rate will be automatically calculaed as total file lines divided by this value and sample size. Can't be specified together with -r parameter.", "COUNT"));
    parser.addOption(QCommandLineOption(QStringList() << "v" << "verbose", "Enable verbose output."));

}

uint openFileSource(QTextStream& ts, const QString filename)
{
    source_file.close();
    source_file.setFileName(filename);
    int result = -1;
    if (source_file.open(QIODevice::ReadOnly))
    {
        ts.setDevice(&source_file);
        ts.seek(0);
    }

    return result;
}

QString readStdinToTmpFile()
{
   QString fname;
   QTemporaryFile f;
   f.setAutoRemove(false);
    if (f.open())
    {
        fname = f.fileName();
        QTextStream f_stream(&f);
        QFile f2;
        if (f2.open(stdin, QIODevice::ReadOnly))
        {
            QTextStream f2_stream(&f2);
            while(!f2_stream.atEnd())
            {
                f_stream << f2_stream.readLine() + '\n';
            }
            f2.close();

        } else
        {
            f.close();
            qCritical() << "ERROR: Can't open stdin";
            return QString();
        }

        f.close();
    } else {
        qCritical() << "ERROR: Can't create a temp file " + fname;
        return QString();
    }

   is_temporary_file = true;
   return fname;
}

uint countLines(QTextStream& ts)
{
    ts.seek(0);
    uint count = 0;
    while (!ts.atEnd())
    {
        ts.readLine();
        count++;
    }
    ts.seek(0);
    return count;
}

void closeFileSource()
{
    if (!is_temporary_file)
            source_file.close();
    else
            source_file.remove();
}

uint openFileDestination(QTextStream& ts, const QString filename)
{
    destination_file.close();
    destination_file.setFileName(filename);
    int result = -1;
    if (destination_file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        ts.setDevice(&destination_file);
        ts.seek(0);
    }

    return result;
}


uint openStdOutDestination(QTextStream& ts)
{
   if (!destination_file.open(stdout, QIODevice::WriteOnly))
       return -1;

   ts.setDevice(&destination_file);
   return 0;
}

void closeFileDestination()
{
    destination_file.close();
}
