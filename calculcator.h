#ifndef CALCULCATOR_H
#define CALCULCATOR_H

#include <QMainWindow>

namespace Ui {
class calculcator;
}

class calculcator : public QMainWindow
{
    Q_OBJECT

public:
    explicit calculcator(QWidget *parent = nullptr);
    ~calculcator();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::calculcator *ui;
};

#endif // CALCULCATOR_H
