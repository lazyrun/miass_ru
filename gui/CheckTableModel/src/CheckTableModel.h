/*! \file CheckTableModel.h
    \brief Табличная модель с возможностью столбцов с чекбоксами
    \author Kiselev Kirill (kiselev@papillon.ru)
    \date 04.10.2012
*/

#ifndef CHECKTABLEMODEL_H
#define CHECKTABLEMODEL_H

//! Тип табличных данных
typedef QList<QVariantList> TableVariantList;

/*!
\class ColumnDef
\brief Описание типа колонки таблицы
*/
class ColumnDef
{
public:
   //! ctor.
   ColumnDef(const QVariant::Type & type, const QString & name) 
   {type_ = type; name_ = name;}
   //! type
   QVariant::Type type() const { return type_; }
   //! name 
   QString name() const { return name_; }
protected:
   //! type
   QVariant::Type type_;
   //! name
   QString name_;
};

/*!
\class CheckTableModel
\brief Модель таблицы
*/
class CheckTableModel : public QAbstractTableModel
{
public:
   //! ctor.
   /*!
   \param[in] def Список описаний колонок
   \param[in] parent
   */
   CheckTableModel(const QList<ColumnDef> & def, QObject * parent = 0);
   
   //! Установить данные в таблицу
   void setTableData(const TableVariantList & table);
   //! Вернуть табличные данные
   TableVariantList tableData() const;
   //! Вернуть табличные данные
   QVariant tableData(int row, int col) const;
   //! Очистить всю таблицу
   void clearTable();
   //! Добавить строку
   void addTableLine(const QVariantList & lst);
   //! Удалить строку
   void deleteTableLine(const QModelIndex & idx);
   //! удалить набор
   void deleteTableLines(const QModelIndexList & idxs);
   //! Количество строк
   virtual int rowCount(const QModelIndex & = QModelIndex()) const;
   //!
   QModelIndexList checkedItems() const;
protected:
   //! Флаги
   virtual Qt::ItemFlags flags(const QModelIndex &index) const;
   //! Заголовки
   virtual QVariant headerData(int section, Qt::Orientation orientation, 
      int role = Qt::DisplayRole) const;
   //! Количество колонок
   virtual int columnCount(const QModelIndex & = QModelIndex()) const;
   //! Данные модели
   virtual QVariant data(const QModelIndex & index,
      int role = Qt::DisplayRole) const;
   //! Установка данных
   virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
   
   //! Проверка валидности индекса
   bool isIndexValid(int row, int col) const;
   //! Описание колонок
   QList<ColumnDef> def_;
   //! Табличные данные
   TableVariantList table_;
   //!
   QModelIndexList checked_;
};

#endif