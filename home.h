#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include "loadshedding.h"
#include <QNetworkReply>
#include <QString>

namespace Ui {
  class Home;
}

class Home : public QMainWindow
{
  Q_OBJECT

public:
  explicit Home(QWidget *parent = 0);
  Loadshedding lh;
  void updateUI();
  void updateData(QByteArray arr);
  void getData(QString groupd);
  ~Home();

private slots:
  //void on_comboBox_currentIndexChanged(int index);

  void on_comboBox_currentIndexChanged(const QString &arg1);
  void on_pushButton_clicked();

public slots:
  void replyFinished (QNetworkReply *reply);

private:
  Ui::Home *ui;
};

#endif // HOME_H
