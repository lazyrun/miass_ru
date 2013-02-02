/*! \file CheckTableItemDelegate.h
    \brief �������� ��� ��������
    \author Kiselev Kirill (kiselev@papillon.ru)
    \date 04.10.2012
*/

#ifndef CheckTableItemDelegate_H
#define CheckTableItemDelegate_H

/*!
\class BoolDelegate
\brief ����� ��� ������� �����
*/
class BoolDelegate : public QStyledItemDelegate
{
public:
   //! ctor.
   BoolDelegate(QObject *parent = 0);
protected:   
   //! ��������� ��������
   virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, 
      const QModelIndex & index) const;

   //! ��������� ������� ��������
   virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, 
      const QStyleOptionViewItem &option,                              
      const QModelIndex &index);
}; 

/*!
\class ButtonDelegate
\brief ������ � ������
*/
class ButtonDelegate : public QStyledItemDelegate
{
Q_OBJECT
public:
   //! ctor.
   ButtonDelegate(const QString & text, QObject *parent = 0);
signals:
   void pressed(const QModelIndex & index);
protected:   
   //! ��������� ������
   virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, 
      const QModelIndex & index) const;

   //! ��������� ������� ��������
   virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, 
      const QStyleOptionViewItem &option,                              
      const QModelIndex &index);

   //!
   QMap<QModelIndex, bool> pressedMap_;
   //!
   QString text_;
}; 

/*!
\class IPMaskDelegate
\brief ����� ����� IP:port
*/
class IPMaskDelegate : public QStyledItemDelegate
{
public:
   //! ctor.
   IPMaskDelegate(QObject *parent = 0);
protected:
   //! ������ ������ ��������� �� ������
   virtual void setEditorData(QWidget * editor,
                      const QModelIndex & index) const;
   //! ������ ������ ������ �� ���������
   virtual void setModelData(QWidget * editor, QAbstractItemModel * model,
      const QModelIndex & index) const;
   //! �������� ���������
   virtual QWidget * createEditor(QWidget * parent,
                         const QStyleOptionViewItem & ,
                         const QModelIndex & idx) const;
};

/*!
\class IPMaskDelegate
\brief ����� ����� �����
*/
class StringDelegate : public QStyledItemDelegate
{
public:
   //! ctor.
   StringDelegate(QObject *parent = 0);
protected:
   //! ������ ������ ��������� �� ������
   virtual void setEditorData(QWidget * editor,
                      const QModelIndex & index) const;
   //! ������ ������ ������ �� ���������
   virtual void setModelData(QWidget * editor, QAbstractItemModel * model,
      const QModelIndex & index) const;
   //! �������� ���������
   virtual QWidget * createEditor(QWidget * parent,
                         const QStyleOptionViewItem & ,
                         const QModelIndex & idx) const;
};

/*!
\class BoardItemDelegate
*/
class BoardItemDelegate : public QStyledItemDelegate
{
public:
   //! ctor.
   BoardItemDelegate(QObject *parent = 0);
protected:   
   //! ���������
   virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, 
      const QModelIndex & index) const;
   //! ������
   virtual QSize sizeHint(const QStyleOptionViewItem & option, 
      const QModelIndex & index ) const;
}; 

#endif
