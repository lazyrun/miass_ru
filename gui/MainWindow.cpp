#include "MainWindow.h"
#include "CheckTableModel.h"
#include "CheckTableItemDelegate.h"

MainWindow::MainWindow() : QMainWindow()
{
   QTranslator * appTranslator = new QTranslator(this);
   appTranslator->load("miass_" + QLocale::system().name(),
      "translations");
   QApplication::installTranslator(appTranslator);
 
   QFont fn = font();
   fn.setPixelSize(14);
   setFont(fn);
   setupUi();
   //Главное окно
   setMainWidget();

   setWindowTitle(tr("Miass.ru Grabber"));
   setMinimumSize(800, 600);
   setWindowState(Qt::WindowMaximized);
}

void MainWindow::setMainWidget()
{
   QLabel * lblAddr = new QLabel(tr("Address:"), this);
   leAddr_ = new QLineEdit(this);
   cmbAddr_ = new QComboBox(this);
   cmbDate_ = new QComboBox(this);
   foreach (QString key, mapAddr_.keys())
   {
      cmbAddr_->addItem(key);
   }
   cmbDate_->addItem(tr("Today"), 0);
   cmbDate_->addItem(tr("+Yesterday"), 1);
   cmbDate_->addItem(tr("3 day"), 2);
   cmbDate_->addItem(tr("All"), 3);

   cmbLada_ = new QComboBox(this);
   cmbLada_->addItem(tr("No Lada"), 0);
   cmbLada_->addItem(tr("Only Lada"), 1);
   cmbLada_->addItem(tr("Off"), 2);

   chkFilter_ = new QCheckBox(tr("&Filter"), this);
   chkFilter_->setChecked(true);

   connect(cmbAddr_, SIGNAL(currentIndexChanged(int)), this, SLOT(resolveAddr(int)));
   QToolButton * btnUpdate = new QToolButton(this);
   btnUpdate->setIconSize(QSize(18, 18));
   btnUpdate->setAutoRaise(true);
   btnUpdate->setIcon(QIcon(":/images/update.png"));
   btnUpdate->setShortcut(QKeySequence("F5"));
   connect(btnUpdate, SIGNAL(clicked()), this, SLOT(loadAddr()));

   tableView_ = new QTableView(this);
   tableView_->setAlternatingRowColors(true);
   tableView_->setSelectionMode(QAbstractItemView::SingleSelection);   
   tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
   tableView_->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
   QPalette pal = tableView_->palette();
   pal.setColor(QPalette::AlternateBase, QColor("gainsboro"));
   tableView_->setPalette(pal);
   tableView_->setStyleSheet("QTableView::item {padding: 3px;}");

   QList<ColumnDef> columnList;
   columnList  << ColumnDef(QVariant::Bool, tr("Select"))
               << ColumnDef(QVariant::String, tr("Aside"))
               << ColumnDef(QVariant::String, tr("Advert"))
               << ColumnDef(QVariant::String, tr("Date"));

   model_ = new CheckTableModel(columnList, this);

   tableView_->setModel(model_);
   tableView_->setItemDelegateForColumn(0, new BoolDelegate(this));
   connect(tableView_, SIGNAL(doubleClicked(const QModelIndex &)),
      this, SLOT(advertClicked(const QModelIndex &)));
   
   BoardItemDelegate * itemDelegate = new BoardItemDelegate(this);
   tableView_->setItemDelegateForColumn(2, itemDelegate);

   //ButtonDelegate * btnHide = new ButtonDelegate(tr("Hide"), this);
   //connect(btnHide, SIGNAL(pressed(const QModelIndex &)), this, 
   //   SLOT(hideItem(const QModelIndex &)));
   //tableView_->setItemDelegateForColumn(1, btnHide);

   ButtonDelegate * btnAside = new ButtonDelegate(tr("Aside"), this);
   connect(btnAside, SIGNAL(pressed(const QModelIndex &)), this, 
      SLOT(asideItem(const QModelIndex &)));
   tableView_->setItemDelegateForColumn(1, btnAside);

   tableView_->verticalHeader()->setMinimumWidth(25);
   tableView_->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
   tableView_->setEditTriggers(QAbstractItemView::NoEditTriggers);
   
   tableView_->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
   tableView_->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
   tableView_->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
   tableView_->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
   tableView_->setColumnWidth(1, 100);

   btnHideSel_ = new QPushButton(tr("Hide"), this);
   btnAsideSel_ = new QPushButton(tr("Aside"), this);

   btnHideSel_->setFocusPolicy(Qt::TabFocus);
   btnAsideSel_->setFocusPolicy(Qt::TabFocus);
   btnHideSel_->setEnabled(false);
   btnAsideSel_->setEnabled(false);

   connect(btnHideSel_,  SIGNAL(clicked()), this, SLOT(hideSelected()));
   connect(btnAsideSel_, SIGNAL(clicked()), this, SLOT(asideSelected()));

   new QShortcut(QKeySequence("Del"), this, SLOT(hideSelected()));
   new QShortcut(QKeySequence("Ins"), this, SLOT(asideSelected()));
   new QShortcut(QKeySequence("Return"), this, SLOT(advertReturned()));

   QTabWidget * tab = new QTabWidget(this);
   tab->setStyleSheet(" QTabWidget::tab-bar {"
      "left: 2px; } QTabBar::tab {font: 14px; min-width: 48ex;}");

   tab->addTab(tableView_, tr("Adverts"));
   
   tableDefView_ = new QTableView(this);
   tableDefView_->setAlternatingRowColors(true);
   tableDefView_->setSelectionMode(QAbstractItemView::SingleSelection);   
   tableDefView_->setSelectionBehavior(QAbstractItemView::SelectRows);
   tableDefView_->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
   tableDefView_->setPalette(pal);
   tableDefView_->setStyleSheet("QTableView::item {padding: 3px;}");

   QList<ColumnDef> columnDefList;
   columnDefList  << ColumnDef(QVariant::Bool, tr("Select"))
               << ColumnDef(QVariant::String, tr("Advert"))
               << ColumnDef(QVariant::String, tr("Date"));

   modelDef_ = new CheckTableModel(columnDefList, this);

   tableDefView_->setModel(modelDef_);
   tableDefView_->setItemDelegateForColumn(0, new BoolDelegate(this));
   tableDefView_->verticalHeader()->setMinimumWidth(25);
   tableDefView_->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
   tableDefView_->setEditTriggers(QAbstractItemView::NoEditTriggers);
   
   tableDefView_->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
   tableDefView_->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
   tableDefView_->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);

   tab->addTab(tableDefView_, tr("Deferred"));

   QHBoxLayout * hbTool = new QHBoxLayout;
   hbTool->addWidget(lblAddr);
   hbTool->addWidget(leAddr_, 1);
   hbTool->addWidget(cmbAddr_);
   hbTool->addWidget(cmbDate_);
   hbTool->addWidget(btnUpdate);

   QHBoxLayout * hbDown = new QHBoxLayout;
   hbDown->addWidget(btnHideSel_);
   hbDown->addWidget(btnAsideSel_);
   hbDown->addStretch(1);

   QHBoxLayout * hbFilter = new QHBoxLayout;
   hbFilter->addWidget(cmbLada_);
   hbFilter->addWidget(chkFilter_);
   
   leFind_ = new QLineEdit(this);
   QPushButton * btnFind = new QPushButton(tr("Find Next"), this);
   connect(btnFind, SIGNAL(clicked()), this, SLOT(findNext()));

   new QShortcut(QKeySequence("Ctrl+F"), this, SLOT(activateFind()));
   new QShortcut(QKeySequence("F3"), this, SLOT(findNext()));
   hbFilter->addStretch(1);
   hbFilter->addWidget(leFind_);
   hbFilter->addWidget(btnFind);

   QVBoxLayout * vbMain = new QVBoxLayout;
   vbMain->addLayout(hbTool);
   vbMain->addLayout(hbFilter);
   vbMain->addWidget(tab);
   vbMain->addLayout(hbDown);

   QWidget * mainWidget = new QWidget(this);
   mainWidget->setLayout(vbMain);
   setCentralWidget(mainWidget);
   
   resolveAddr(0);
}

void MainWindow::setupUi()
{
   mapAddr_.insert(tr("Auto"), "http://miass.ru/useful/announce.php?section=8");
   mapAddr_.insert(tr("Housing"), "http://miass.ru/useful/announce.php?section=5");
   manager_ = new QNetworkAccessManager(this);
   loop_ = new QEventLoop(this);
   
   progressDlg_ = new QProgressDialog(this);
   progressDlg_->setWindowTitle(tr("Download"));
   progressDlg_->setMinimumWidth(400);
   
   connect(manager_, SIGNAL(finished(QNetworkReply *)), loop_,
      SLOT(quit()));

   QNetworkProxy proxy;
   proxy.setType(QNetworkProxy::HttpProxy);
   proxy.setHostName("proxy.papillon.ru");
   proxy.setPort(3128);
   manager_->setProxy(proxy);
}

void MainWindow::resolveAddr(int)
{
   QString val = cmbAddr_->currentText();
   leAddr_->setText(mapAddr_.value(val));
   leAddr_->selectAll();
   leAddr_->setFocus();
}

void MainWindow::loadAddr()
{
   itemList_.clear();
   model_->clearTable();
   QNetworkReply * reply = manager_->get(QNetworkRequest(QUrl(leAddr_->text())));
   connect(reply, SIGNAL(downloadProgress(qint64, qint64)), 
      this, SLOT(downloadProgress(qint64, qint64)));
   progressDlg_->setLabelText(leAddr_->text());
   loop_->exec();
   reply->disconnect();
   parseReply(reply);
   
   //DEBUG
   //QFile f("board.htm");
   //Q_ASSERT(f.open(QIODevice::ReadOnly));
   //QByteArray ba = f.readAll();
   //parseData(ba);

   tableView_->setFocus();
   btnHideSel_->setEnabled(true);
   btnAsideSel_->setEnabled(true);
}

void MainWindow::downloadProgress(qint64 bytesSent, qint64 bytesTotal)
{
   qreal qPercent = 100. * bytesSent / bytesTotal;
   int percent = qRound(qPercent);
   progressDlg_->setValue(percent);
}

void MainWindow::errorLoading(QNetworkReply::NetworkError)
{
   QMessageBox::critical(this, tr("Error Loading"), tr("Cannot load page."));
}

void MainWindow::parseReply(QNetworkReply * reply)
{
   switch (reply->error()) 
   {
      case QNetworkReply::NoError:
      {
         QByteArray data = reply->readAll();
         parseData(data);
         break;
      }
      default:
         errorLoading(reply->error());
   }
   reply->close();
   reply->deleteLater();
}

void MainWindow::parseData(const QByteArray & ba)
{
   QTextCodec * codec = QTextCodec::codecForLocale();
   int from = 1e3;
   QStringList allowDates;
   QString curr = QDate::currentDate().toString("dd.MM");
   allowDates << curr;
   bool isAll = false;
   int dval = cmbDate_->itemData(cmbDate_->currentIndex()).toInt();
   if (dval == 1)
   {
      QString yest = QDate::fromJulianDay(QDate::currentDate().toJulianDay() - 1)
         .toString("dd.MM");
      allowDates << yest;
   }
   else if (dval == 2)
   {
      for (int i = 1; i < 3; i++)
      {
         QString date = QDate::fromJulianDay(QDate::currentDate().toJulianDay() - i)
            .toString("dd.MM");
         allowDates << date;
      }
   }
   else if (dval == 3)
   {
      isAll = true;
   }
   
   //загрузить слова фильтры
   QStringList filters;
   if (chkFilter_->isChecked())
   {
      QFile ff("filter.txt");
      if (ff.open(QIODevice::ReadOnly | QIODevice::Text))
      {
         QTextStream stream(&ff);
         while (!stream.atEnd())
         {
            filters << stream.readLine();
         }
         ff.close();
      }
   }

   //загрузить фильтры ЛАДА
   QStringList ladaList;
   bool ladaAllow = false;
   if (cmbLada_->itemData(cmbLada_->currentIndex()) == 1)
      ladaAllow = true;

   if (cmbLada_->itemData(cmbLada_->currentIndex()) != 2)
   {
      QFile nl("nolada.txt");
      if (nl.open(QIODevice::ReadOnly | QIODevice::Text))
      {
         QTextStream stream(&nl);
         while (!stream.atEnd())
         {
            ladaList << stream.readLine();
         }
         nl.close();
      }
   }

   //int debug = 0;
   while ((from = ba.indexOf("<li>", from)) != -1)
   {
      int end = ba.indexOf("</li>", from);
      QString li = codec->toUnicode(ba.mid(from, end - from));
      from = end;
      if (li.contains("ff0000"))
         continue;
      int begin = li.indexOf("/");
      end = li.indexOf(" class", begin);
      bool filtered = false;
      foreach (QString filter, filters)
      {
         if (li.contains(filter, Qt::CaseInsensitive))
         {
            filtered = true;
            break;
         }
      }
      if (!filtered)
      {
         if (!ladaAllow)//иномарки
         {
            foreach (QString filter, ladaList)
            {
               if (li.contains(filter, Qt::CaseInsensitive))
               {
                  //запись будет отфильтрована
                  filtered = true;
                  break;
               }
            }
         }
         else//наши
         {
            filtered = true;
            foreach (QString filter, ladaList)
            {
               if (li.contains(filter, Qt::CaseInsensitive))
               {
                  //запись не будет отфильтрована
                  filtered = false;
                  break;
               }
            }
         }
      }

      if (filtered)
         continue;

      BoardItem item;
      if (begin != -1 && end != -1)
      {
         item.addr = "http://miass.ru" + li.mid(begin, end - begin);
         begin = li.indexOf("font", end);
         begin = li.indexOf(">", begin) + 1;
         end = li.indexOf("</font>", begin);
         item.caption = li.mid(begin, end - begin);
         item.caption.replace(QRegExp("\\&\\#.{3,4}\\;"), "");
         item.caption.replace(QRegExp("\\*{2,}"), "");
         item.caption.replace(QRegExp("\\_{2,}"), "");
         item.caption.replace(QRegExp("\\&{2,}"), "");
         item.caption.replace(QRegExp("\\!{2,}"), "");
         item.caption.replace(QRegExp("\\\\{2,}"), "");
         item.caption.replace(QRegExp("\\/{2,}"), "");
         item.caption.replace(QRegExp("\\\"{2,}"), "");
         item.caption.replace(QRegExp("\\@{2,}"), "");
         item.caption.replace(QRegExp("\\={2,}"), "");
         item.caption.replace(QRegExp("\\+{2,}"), "");
         item.caption.replace(QRegExp("\\#{2,}"), "");
         item.caption.replace(QRegExp("\\№{2,}"), "");
         item.caption.replace(QRegExp("\\={2,}"), "");
         item.caption.replace(QRegExp("\\-{2,}"), "");
         item.caption.replace(QRegExp("\\${2,}"), "");

         item.caption = item.caption.trimmed();

         begin = li.indexOf("[", end) + 1;
         end = li.indexOf("]", begin) ;
         item.date = li.mid(begin, end - begin);
         
         if (isAll)
         {
            itemList_ << item;
         }
         else
         {
            if (allowDates.contains(item.date))
               itemList_ << item;
            else
               break;
         }
      }
      //debug++;
      //if (debug > 20)
      //   break;
   }
   loadAdverts();
}

void MainWindow::loadAdverts()
{
   qreal total = itemList_.count();
   //qreal total = 5.;
   progressDlg_->setValue(0);
   int i = 0;
   foreach (BoardItem item, itemList_)
   {
      qreal qPercent = 100. * i / total;
      int percent = qRound(qPercent);
      progressDlg_->setValue(percent);
      progressDlg_->setLabelText(item.addr);

      //DEBUG <
      //if (i > -1)
      //   break;
      //DEBUG >
      urlMap_.insert(QUrl(item.addr), i++);
      QNetworkReply * reply = manager_->get(QNetworkRequest(QUrl(item.addr)));
      loop_->exec();
      parseReplyAdvert(reply);
   }
   addItems();
   progressDlg_->hide();
   
   tableView_->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
   tableView_->setCurrentIndex(model_->index(0, 0));
}

void MainWindow::parseReplyAdvert(QNetworkReply * reply)
{
   int idx = urlMap_.value(reply->url());
   if (idx == -1)
      return;
   switch (reply->error()) 
   {
      case QNetworkReply::NoError:
      {
         QByteArray data = reply->readAll();
         parseAdvert(data, idx);
         break;
      }
      default:
         errorLoading(reply->error());
   }
   reply->close();
   reply->deleteLater();
}

void MainWindow::parseAdvert(const QByteArray & ba, int idx)
{
   QTextCodec * codec = QTextCodec::codecForLocale();
   int from = 1e3;

   //вытащить картинку
   QString img_addr = "announce.img.php";
   int img_from = ba.indexOf(img_addr, from);
   if (img_from != -1)
   {
      int img_end = ba.indexOf(" ", img_from);
      QString addr = codec->toUnicode(ba.mid(img_from, img_end - img_from));
      img_addr = "http://miass.ru/useful/" + addr;
      if (!addr.isEmpty())
      {
         QNetworkReply * reply = manager_->get(QNetworkRequest(QUrl(img_addr)));
         loop_->exec();
         parseImage(reply, idx);
      }
   }

   int begin = ba.indexOf("<code>", from);
   int end = ba.indexOf("</td></tr>", begin);
   if (begin != - 1 && end != -1)
   {
      QString ad = codec->toUnicode(ba.mid(begin, end - begin));
      ad.replace("<br><br>", "");
      ad.replace("<br>", "\n");
      ad.replace("<br />", "");
      ad.replace("<b>", "");
      ad.replace("</b>", "");
      ad.replace("<code>", "");
      
      itemList_[idx].text = ad;
   }
}

void MainWindow::addItems()
{
   foreach (BoardItem item, itemList_)
   {
      addItem(item);
   }
}

void MainWindow::addItem(const BoardItem & item)
{
   QVariant item_variant = QVariant::fromValue(item);

   model_->addTableLine(QVariantList() << Qt::Unchecked << "" 
            << item_variant 
            << item.date);
}

void MainWindow::hideItem(const QModelIndex & index)
{
   model_->deleteTableLine(index);
   if (index.row() == model_->rowCount())
   {
      QModelIndex idx = index.sibling(index.row() - 1, index.column());
      tableView_->setCurrentIndex(idx);
   }
   else
   {
      tableView_->setCurrentIndex(index);
   }
}

void MainWindow::hideSelected()
{
   QModelIndex index = tableView_->currentIndex();
   if (!index.isValid())
      return;
   QModelIndexList idxList = model_->checkedItems();
   if (idxList.isEmpty())
      idxList << index;

   model_->deleteTableLines(idxList);
   
   foreach (QModelIndex idx, idxList)
   {
      itemList_.replace(idx.row(), BoardItem());
   }
   itemList_.removeAll(BoardItem());

   if (!idxList.isEmpty())
      tableView_->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

   if (index.row() >= model_->rowCount())
   {
      QModelIndex idx = index.sibling(index.row() - (model_->rowCount() - index.row()) - 1,
         index.column());
      tableView_->setCurrentIndex(idx);
   }
   else
   {
      tableView_->setCurrentIndex(index);
   }
   tableView_->setFocus();
}

void MainWindow::asideSelected()
{
   QModelIndex index = tableView_->currentIndex();
   QModelIndexList idxList = model_->checkedItems();
   if (idxList.isEmpty())
      idxList << index;
   
   foreach (QModelIndex idx, idxList)
   {
      const BoardItem & item = itemList_.at(idx.row());
      if (!deferred_.contains(item))
      {
         modelDef_->addTableLine(QVariantList() << Qt::Unchecked
            << item.caption + "\n" + item.text 
            << item.date);
         deferred_ << item;
      }
   }
   if (!idxList.isEmpty())
      tableDefView_->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

   hideSelected();
}

void MainWindow::asideItem(const QModelIndex & idx)
{
   const BoardItem & item = itemList_.at(idx.row());
   if (!deferred_.contains(item))
   {
      modelDef_->addTableLine(QVariantList() << Qt::Unchecked
         << item.caption + "\n" + item.text 
         << item.date);
      deferred_ << item;
   }
   tableDefView_->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
   hideSelected();
}

void MainWindow::advertClicked(const QModelIndex & idx)
{
   const BoardItem & item = itemList_.at(idx.row());
   QDesktopServices::openUrl(QUrl::fromUserInput(item.addr));
}

void MainWindow::parseImage(QNetworkReply * reply, int idx)
{
   switch (reply->error()) 
   {
      case QNetworkReply::NoError:
      {
         QByteArray data = reply->readAll();
         QImage image = QImage::fromData(data);
         if (!image.isNull())
            itemList_[idx].image = image;

         break;
      }
   }
   reply->close();
   reply->deleteLater();
}

void MainWindow::advertReturned()
{
   if (leFind_->hasFocus())
   {
      findNext();
      return;
   }
   QModelIndex idx = tableView_->currentIndex();
   if (!idx.isValid())
      return;
   const BoardItem & item = itemList_.at(idx.row());
   QDesktopServices::openUrl(QUrl::fromUserInput(item.addr));
}

void MainWindow::activateFind()
{
   leFind_->setFocus();
   leFind_->selectAll();
}

void MainWindow::findNext()
{
   QString what = leFind_->text();
   static int lastFrom = tableView_->currentIndex().row();
   if (lastFrom < 0 || lastFrom >= itemList_.count() - 1)
      lastFrom = 0;

   for (int i = lastFrom; i < itemList_.count(); i++)
   {
      BoardItem item = itemList_.at(i);
      if (item.caption.contains(what, Qt::CaseInsensitive) ||
         item.text.contains(what, Qt::CaseInsensitive))
      {
         tableView_->setCurrentIndex(model_->index(i, 0));
         tableView_->setFocus();
         lastFrom = i + 1;
         return;
      }
   }
   //если не нашлось начать поиск с начала
   if (lastFrom != 0)
   {
      lastFrom = 0;
      findNext();
   }
}

