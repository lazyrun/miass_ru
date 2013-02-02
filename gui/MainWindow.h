#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class CheckTableModel;

#include "BoardItem.h"

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
   MainWindow();
private slots:
   void resolveAddr(int idx);
   void loadAddr();
   void parseReply(QNetworkReply *);
   void parseReplyAdvert(QNetworkReply *);
   void hideItem(const QModelIndex &);
   void asideItem(const QModelIndex &);
   void downloadProgress(qint64, qint64);
   void hideSelected();
   void asideSelected();
   void advertClicked(const QModelIndex &);
   void advertReturned();
   void activateFind();
   void findNext();
private:
   void setMainWidget();
   void setupUi();
   void errorLoading(QNetworkReply::NetworkError);
   void parseData(const QByteArray & ba);
   void addItem(const BoardItem & item);
   void loadAdverts();
   void parseAdvert(const QByteArray & data, int idx);
   void parseImage(QNetworkReply * reply, int idx);
   void addItems();

   QLineEdit * leAddr_;
   QComboBox * cmbAddr_;
   QComboBox * cmbDate_;
   QTableView * tableView_;
   QTableView * tableDefView_;
   CheckTableModel * model_;
   CheckTableModel * modelDef_;
   QMap<QString, QString> mapAddr_;
   //! Manager
   QNetworkAccessManager * manager_;
   QEventLoop * loop_;
   QProgressDialog * progressDlg_;

   QList<BoardItem> itemList_;
   QList<BoardItem> deferred_;
   QMap<QUrl, int> urlMap_;
   QPushButton * btnHideSel_;
   QPushButton * btnAsideSel_;
   QComboBox * cmbLada_;
   QCheckBox * chkFilter_;
   QLineEdit * leFind_;
};


#endif
