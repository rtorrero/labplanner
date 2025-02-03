// processor.cpp
#include "processor.h"
#include <QMap>
#include <QString>
#include <QStringList>

// Función recursiva que genera el texto Markdown con encabezados jerárquicos.
// Parámetros:
// - rows: filas a agrupar (sin la cabecera).
// - selectedCols: vector de índices de columna a usar (el primero se usa en este nivel).
// - headers: lista completa de encabezados (primera fila del CSV).
// - labelPrefix: etiqueta jerárquica acumulada (por ejemplo, "1" o "1.2").
// - markdown: cadena donde se acumula el texto generado.
// - level: nivel de agrupación (0 para solución madre, 1 para subdivisión, etc.).
// - parentVolume: volumen disponible del grupo padre (para niveles > 0).
static void generateInstructionsRecursive(const QList<QStringList>& rows,
                                          const QVector<int>& selectedCols,
                                          const QStringList& headers,
                                          const QString& labelPrefix,
                                          QString& markdown,
                                          int level,
                                          int parentVolume)
{
    if (!selectedCols.isEmpty()) {
        int currentCol = selectedCols.first();
        QString ingredient = headers.at(currentCol);
        // Agrupar las filas por el valor de la columna actual.
        QMap<QString, QList<QStringList>> groups;
        for (const QStringList& row : rows) {
            if (currentCol < row.size()) {
                QString key = row.at(currentCol).trimmed();
                groups[key].append(row);
            }
        }

        int numberOfSubgroups = groups.size();
        int groupIndex = 1;

        // Preparamos el vector de columnas para el siguiente nivel.
        QVector<int> nextSelected = selectedCols;
        nextSelected.remove(0);

        // Iterar sobre cada grupo.
        for (auto it = groups.begin(); it != groups.end(); ++it) {
            QString currentLabel = labelPrefix.isEmpty() ? QString::number(groupIndex)
                                                         : labelPrefix + "." + QString::number(groupIndex);
            int groupVolume;
            if (level == 0) {
                // Para la solución madre, el volumen se calcula en función del número de filas.
                int count = it.value().size();
                groupVolume = count * Processor::REPLICATE_VOLUME * Processor::FACTOR;
            } else {
                // Para subdivisiones, se reparte equitativamente el volumen del grupo padre.
                groupVolume = parentVolume / numberOfSubgroups;
            }

            bool ok;
            double concentration = it.key().toDouble(&ok);
            double grams = 0.0;
            if (ok)
                grams = concentration * (groupVolume / 1000.0);

            // Generar el encabezado jerárquico.
            QString headingPrefix = QString(level + 1, '#'); // Nivel 0: "#", Nivel 1: "##", etc.
            QString groupHeader;
            if (level == 0) {
                groupHeader = headingPrefix + " Preparación de Solución Madre con concentración de " +
                              ingredient + " de " + it.key() + " g/L\n\n";
            } else if (level == 1) {
                groupHeader = headingPrefix + " Subdivisión de Solución Madre " + labelPrefix +
                              " con concentración de " + ingredient + " de " + it.key() + " g/L\n\n";
            } else {
                groupHeader = headingPrefix + " Subdivisión de Solución " + labelPrefix +
                              " con concentración de " + ingredient + " de " + it.key() + " g/L\n\n";
            }
            markdown += groupHeader;

            // Generar las instrucciones para este grupo.
            if (level == 0) {
                markdown += currentLabel + ". Preparar " + QString::number(groupVolume) +
                            " mL de solución madre de " + ingredient + " (" + it.key() + " g/L)\n";
                markdown += "   - Disolver " + QString::number(grams, 'f', 1) +
                            " g de " + ingredient + " en " + QString::number(groupVolume) + " mL de agua destilada.\n\n";
            } else {
                markdown += currentLabel + ". Preparar " + QString::number(groupVolume) +
                            " mL de solución con " + ingredient + " (" + it.key() + " g/L)\n";
                markdown += "   - Tomar " + QString::number(groupVolume) + " mL de la solución " + labelPrefix + "\n";
                markdown += "   - Añadir " + QString::number(grams, 'f', 1) + " g de " + ingredient + " y mezclar bien.\n\n";
            }

            // Si hay más columnas para agrupar, continuar recursivamente.
            if (!nextSelected.isEmpty()) {
                generateInstructionsRecursive(it.value(), nextSelected, headers, currentLabel, markdown, level + 1, groupVolume);
            } else {
                // En el último nivel, generar las instrucciones para la preparación de muestras finales.
                QString finalHeader = QString(level + 1, '#') + " Preparación de Muestras Finales\n\n";
                markdown += finalHeader;
                markdown += currentLabel + ".1 Distribuir " + QString::number(Processor::REPLICATE_VOLUME) +
                            " mL en tubos para el experimento " + currentLabel + " (Líquido)\n";
                markdown += "   - Tomar " + QString::number(Processor::REPLICATE_VOLUME) + " mL de la solución " + currentLabel + ".\n";
                markdown += "   - Transferir a un tubo marcado como \"Exp. " + currentLabel + " - SmF\".\n";
                markdown += "   - Repetir 3 veces (triplicado estadístico).\n\n";

                markdown += currentLabel + ".2 Distribuir " + QString::number(Processor::REPLICATE_VOLUME) +
                            " mL en tubos para el experimento " + currentLabel + " (Sólido)\n";
                markdown += "   - Tomar " + QString::number(Processor::REPLICATE_VOLUME) + " mL de la solución " + currentLabel + ".\n";
                markdown += "   - Transferir a un tubo marcado como \"Exp. " + currentLabel + " - SSF\".\n";
                markdown += "   - Repetir 3 veces (triplicado estadístico).\n\n";
            }
            groupIndex++;
        }
    }
    // Fin de la recursión.
}

QString Processor::generateMarkdownInstructions(const QList<QStringList>& data, const QVector<int>& selectedCols)
{
    if (data.size() < 2)
        return "No hay datos para procesar.";

    QString markdown;
    // La primera fila son los encabezados.
    QStringList headers = data.first();
    // El resto de filas se procesan.
    QList<QStringList> rows = data.mid(1);

    // Iniciar la recursión en el nivel 0; en este nivel, el parámetro parentVolume no se usa.
    generateInstructionsRecursive(rows, selectedCols, headers, "", markdown, 0, 0);

    return markdown;
}
