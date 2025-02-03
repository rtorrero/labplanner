#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "experimento.h"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnCargarDatos_clicked();
    void on_btnGenerarPreparaciones_clicked();

private:
    Ui::MainWindow *ui;
    // Aquí guardamos los experimentos cargados
    std::vector<Experimento> m_experimentos;
};

#endif // MAINWINDOW_H
