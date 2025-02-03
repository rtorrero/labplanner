// processor.h
#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QVector>

// El Processor contendrá la lógica para agrupar y calcular los volúmenes y cantidades,
// y para generar el texto en formato Markdown.
// Se definen dos constantes:
// - REPLICATE_VOLUME: volumen (en mL) requerido para cada experimento final (por ejemplo, 10 mL).
// - FACTOR: factor multiplicador para calcular el volumen base a partir del número de experimentos.
class Processor
{
public:
    static const int REPLICATE_VOLUME = 10;
    static const int FACTOR = 10;

    // Genera las instrucciones en formato Markdown a partir de los datos CSV
    // (donde la primera fila es la cabecera) y los índices de columna seleccionados.
    static QString generateMarkdownInstructions(const QList<QStringList>& data, const QVector<int>& selectedCols);
};

#endif // PROCESSOR_H
