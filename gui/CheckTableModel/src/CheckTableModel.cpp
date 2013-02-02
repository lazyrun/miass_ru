/*! \file CheckTableModel.cpp
    \brief Табличная модель с возможностью столбцов с чекбоксами
    \author Kiselev Kirill (kiselev@papillon.ru)
    \date 04.10.2012
*/

#include "CheckTableModel.h"

CheckTableModel::CheckTableModel(const QList<ColumnDef> & def, QObject * parent)
: QAbstractTableModel(parent)
{
   def_ = def;
}

void CheckTableModel::setTableData(const TableVariantList & table)
{
   checked_.clear();
   table_ = table;
}

TableVariantList CheckTableModel::tableData() const
{
   return table_;
}

QVariant CheckTableModel::tableData(int row, int col) const
{
   if (isIndexValid(row, col))
   {  
      return table_.at(row).at(col);
   }
   return QVariant();
}

int CheckTableModel::rowCount(const QModelIndex & ) const
{
   return table_.count();
}

int CheckTableModel::columnCount(const QModelIndex & ) const
{
   return def_.count();
}

bool CheckTableModel::isIndexValid(int row, int col) const
{
   if (row < table_.count() && row >= 0)
   {
      if (col < table_.at(row).count() && col >= 0)
         return true;
   }
   return false;
}

QVariant CheckTableModel::data(const QModelIndex & index,
   int role) const
{
   if (!index.isValid())
      return QVariant();
   
   if (!isIndexValid(index.row(), index.column()))
      return QVariant();

   QVariant value = table_.at(index.row()).at(index.column());
   
   if (role == Qt::CheckStateRole && 
      def_.at(index.column()).type() == QVariant::Bool)
   {
      bool b = value.toBool();
      if (b)
         return QVariant(Qt::Checked);

      return QVariant(Qt::Unchecked);
   }

   if (role == Qt::DisplayRole && 
      def_.at(index.column()).type() != QVariant::Bool)
   {
      return value;
   }

   return QVariant();
}

bool CheckTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
   if (!index.isValid())
      return false;

   if (!isIndexValid(index.row(), index.column()))
      return false;

   if (role == Qt::EditRole || role == Qt::CheckStateRole)
   {
      QVariantList lst = table_.at(index.row());
      lst[index.column()] = value;
      table_[index.row()] = lst;
      
      bool b = value.toBool();
      if (b)
         checked_.append(index);
      else
         checked_.removeOne(index);
   }
   return true;
}

void CheckTableModel::addTableLine(const QVariantList & lst)
{
   if (lst.count() != columnCount())
      return;
   table_ << lst;
   emit layoutChanged();
}

void CheckTableModel::deleteTableLine(const QModelIndex & idx)
{
   if (isIndexValid(idx.row(), idx.column()))
   {
      checked_.removeOne(idx);
      table_.removeAt(idx.row());
      emit layoutChanged();
   }
}

void CheckTableModel::deleteTableLines(const QModelIndexList & idxs)
{
   foreach (QModelIndex idx, idxs)
   {
      table_.replace(idx.row(), QVariantList());
   }
   table_.removeAll(QVariantList());
   checked_.clear();
   emit layoutChanged();
}

void CheckTableModel::clearTable()
{
   table_.clear();
   emit layoutChanged();
}

QVariant CheckTableModel::headerData(int section, Qt::Orientation orientation, 
                                     int role) const
{
   if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
   {
      return def_.at(section).name();
   }

   return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags CheckTableModel::flags(const QModelIndex &index) const
{
   if (!index.isValid())
      return 0;
   
   if (def_.at(index.column()).type() == QVariant::Bool)
      return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable;
   
   return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

QModelIndexList CheckTableModel::checkedItems() const
{
   return checked_;
}

