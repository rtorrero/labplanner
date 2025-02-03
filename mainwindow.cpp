// mainwindow.cpp
#include "mainwindow.h"
#include "csvparser.h"
#include "processor.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTableView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTextStream>
#include <QFile>
#include <QStandardItem>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    model(new QStandardItemModel(this))
{
    // Crear widget central y asignar layout vertical
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Agregar QTableView para visualizar los datos CSV
    QTableView *tableView = new QTableView(centralWidget);
    tableView->setObjectName("tableView");
    tableView->setModel(model);
    // Hacer que las columnas se ajusten automáticamente al ancho de la ventana
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(tableView);

    // Agregar QListWidget para mostrar los encabezados del CSV (se llenará dinámicamente)
    listWidgetColumns = new QListWidget(centralWidget);
    listWidgetColumns->setObjectName("listWidgetColumns");
    layout->addWidget(listWidgetColumns);

    // Agregar botón "Cargar CSV"
    QPushButton *loadButton = new QPushButton("Cargar CSV", centralWidget);
    loadButton->setObjectName("pushButtonLoad");
    layout->addWidget(loadButton);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::onLoadCsvClicked);

    // Agregar botón "Exportar Instrucciones"
    QPushButton *exportButton = new QPushButton("Exportar Instrucciones", centralWidget);
    exportButton->setObjectName("pushButtonExport");
    layout->addWidget(exportButton);
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::onExportCsvClicked);

    setCentralWidget(centralWidget);
    setWindowTitle("Software de Preparación de Muestras");
}

MainWindow::~MainWindow()
{
}

void MainWindow::onLoadCsvClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir Archivo CSV"), "", tr("CSV Files (*.csv)"));
    if (fileName.isEmpty())
        return;

    QList<QStringList> data = CsvParser::parse(fileName);
    if (data.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo leer el archivo o está vacío."));
        return;
    }
    // Almacenar los datos para posteriores cálculos
    csvData = data;
    loadData(data);
}

void MainWindow::loadData(const QList<QStringList> &data)
{
    // Limpiar el modelo y la lista de columnas
    model->clear();
    listWidgetColumns->clear();

    if (data.isEmpty())
        return;

    // La primera fila se asume como encabezado
    QStringList headers = data.first();
    model->setHorizontalHeaderLabels(headers);

    // Llenar el QListWidget con cada encabezado y configurarlo como checkable (por defecto, marcado)
    for (const QString &header : headers) {
        QListWidgetItem *item = new QListWidgetItem(header, listWidgetColumns);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
        listWidgetColumns->addItem(item);
    }

    // Agregar el resto de las filas al modelo
    for (int i = 1; i < data.size(); ++i) {
        const QStringList &row = data.at(i);
        QList<QStandardItem*> items;
        for (const QString &cell : row) {
            items.append(new QStandardItem(cell));
        }
        model->appendRow(items);
    }
}

void MainWindow::onExportCsvClicked()
{
    // Recoger los índices de las columnas seleccionadas (en el mismo orden que aparecen en el QListWidget)
    QVector<int> selectedColumnIndices;
    for (int i = 0; i < listWidgetColumns->count(); ++i) {
        QListWidgetItem *item = listWidgetColumns->item(i);
        if (item->checkState() == Qt::Checked)
            selectedColumnIndices.append(i);
    }
    if (selectedColumnIndices.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No se ha seleccionado ninguna columna para exportar."));
        return;
    }

    // Generar las instrucciones en formato Markdown usando la función del Processor
    QString markdown = Processor::generateMarkdownInstructions(csvData, selectedColumnIndices);

    // Guardar el resultado en un archivo Markdown (.md)
    QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar Instrucciones Markdown"), "", tr("Markdown Files (*.md)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo guardar el archivo."));
        return;
    }

    QTextStream out(&file);
    out << markdown;
    file.close();
    QMessageBox::information(this, tr("Exportar"), tr("El archivo se ha exportado correctamente."));
}
