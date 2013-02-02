/*! \file CheckTableModel.h
    \brief ��������� ������ � ������������ �������� � ����������
    \author Kiselev Kirill (kiselev@papillon.ru)
    \date 04.10.2012
*/

#ifndef CHECKTABLEMODEL_H
#define CHECKTABLEMODEL_H

//! ��� ��������� ������
typedef QList<QVariantList> TableVariantList;

/*!
\class ColumnDef
\brief �������� ���� ������� �������
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
\brief ������ �������
*/
class CheckTableModel : public QAbstractTableModel
{
public:
   //! ctor.
   /*!
   \param[in] def ������ �������� �������
   \param[in] parent
   */
   CheckTableModel(const QList<ColumnDef> & def, QObject * parent = 0);
   
   //! ���������� ������ � �������
   void setTableData(const TableVariantList & table);
   //! ������� ��������� ������
   TableVariantList tableData() const;
   //! ������� ��������� ������
   QVariant tableData(int row, int col) const;
   //! �������� ��� �������
   void clearTable();
   //! �������� ������
   void addTableLine(const QVariantList & lst);
   //! ������� ������
   void deleteTableLine(const QModelIndex & idx);
   //! ������� �����
   void deleteTableLines(const QModelIndexList & idxs);
   //! ���������� �����
   virtual int rowCount(const QModelIndex & = QModelIndex()) const;
   //!
   QModelIndexList checkedItems() const;
protected:
   //! �����
   virtual Qt::ItemFlags flags(const QModelIndex &index) const;
   //! ���������
   virtual QVariant headerData(int section, Qt::Orientation orientation, 
      int role = Qt::DisplayRole) const;
   //! ���������� �������
   virtual int columnCount(const QModelIndex & = QModelIndex()) const;
   //! ������ ������
   virtual QVariant data(const QModelIndex & index,
      int role = Qt::DisplayRole) const;
   //! ��������� ������
   virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
   
   //! �������� ���������� �������
   bool isIndexValid(int row, int col) const;
   //! �������� �������
   QList<ColumnDef> def_;
   //! ��������� ������
   TableVariantList table_;
   //!
   QModelIndexList checked_;
};

#endif