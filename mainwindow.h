// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QListWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadCsvClicked();
    void onExportCsvClicked();

private:
    QStandardItemModel *model;
    QListWidget *listWidgetColumns; // Se llenará dinámicamente con los encabezados del CSV
    QList<QStringList> csvData;     // Almacena los datos completos del CSV (primera fila = encabezados)
    void loadData(const QList<QStringList> &data);
};

#endif // MAINWINDOW_H
