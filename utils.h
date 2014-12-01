#ifndef UTILS_H
#define UTILS_H

#include <QCommandLineParser>
#include <QTextStream>
#include <QFile>
#include <QTemporaryFile>
#include <QDebug>

extern bool _param_verbose;

void initCommandOptionsParser(QCommandLineParser &parser);


uint openFileSource(QTextStream& ts, const QString filename);
uint openFileDestination(QTextStream& ts, const QString filename);
QString readStdinToTmpFile();
uint openStdOutDestination(QTextStream& ts);
uint countLines(QTextStream& ts);

void closeFileDestination();
void closeFileSource();
void closeInputRangeSource();


inline void print(const char* s)
{
    if (_param_verbose)
    {
        fprintf(stdout, "%s",  s);
        fflush(stdout);
    }

}

inline void print(const QString s)
{
    print( s.toStdString().c_str() );
}


#endif // UTILS_H
