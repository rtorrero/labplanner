// csvparser.h
#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QFile>
#include <QTextStream>

class CsvParser {
public:
    // Parsea el archivo CSV usando el delimitador ';'
    static QList<QStringList> parse(const QString &filePath, QChar delimiter = ';') {
        QList<QStringList> data;
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return data;

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList row = line.split(delimiter);
            data.append(row);
        }
        file.close();
        return data;
    }
};

#endif // CSVPARSER_H
