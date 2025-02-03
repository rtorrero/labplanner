#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Hacer que las columnas se expandan
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Configuramos la tabla con 6 columnas
    ui->tableWidget->setColumnCount(6);
    QStringList headers = {"ID Exp", "Variante", "Réplica", "BarleyMalt", "DAP", "Time"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// SLOT: Botón "Cargar Datos"
void MainWindow::on_btnCargarDatos_clicked()
{
    // Dialog para seleccionar el fichero CSV
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Seleccionar archivo CSV",
                                                    "",
                                                    "CSV Files (*.csv);;All Files (*)");

    if (fileName.isEmpty()) {
        return; // usuario canceló
    }

    std::vector<Experimento> temp;
    bool ok = cargarExperimentosDeCSV(fileName.toStdString(), temp);

    if (!ok) {
        QMessageBox::warning(this, "Error", "No se pudo cargar el archivo CSV.");
        return;
    }

    // Guardamos en nuestra variable miembro
    m_experimentos = temp;

    QMessageBox::information(this,
                             "Datos Cargados",
                             QString("Se han cargado %1 experimentos.").arg(m_experimentos.size()));
}

// SLOT: Botón "Generar Preparaciones"
void MainWindow::on_btnGenerarPreparaciones_clicked()
{
    if (m_experimentos.empty()) {
        QMessageBox::warning(this, "Atención", "No hay experimentos cargados aún.");
        return;
    }

    // Generar las preparaciones
    auto preparaciones = generarPreparaciones(m_experimentos);

    // Llenar la tabla
    ui->tableWidget->setRowCount(static_cast<int>(preparaciones.size()));

    for (int row = 0; row < (int) preparaciones.size(); ++row) {
        const auto &p = preparaciones[row];

        // Columna 0: ID
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(p.idExperimento)));

        // Columna 1: Variante
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(p.variante)));

        // Columna 2: Réplica
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(p.replica)));

        // Columna 3: BarleyMalt
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(p.barleyMaltFinal)));

        // Columna 4: DAP
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(p.DAPFinal)));

        // Columna 5: Time
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(p.time)));
    }

    // Ajustamos el tamaño de las columnas para que se vean bien
    ui->tableWidget->resizeColumnsToContents();
}
