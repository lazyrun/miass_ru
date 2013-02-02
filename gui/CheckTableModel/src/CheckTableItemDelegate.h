/*! \file CheckTableItemDelegate.h
    \brief Делегаты для столбцов
    \author Kiselev Kirill (kiselev@papillon.ru)
    \date 04.10.2012
*/

#ifndef CheckTableItemDelegate_H
#define CheckTableItemDelegate_H

/*!
\class BoolDelegate
\brief Класс для булевых ячеек
*/
class BoolDelegate : public QStyledItemDelegate
{
public:
   //! ctor.
   BoolDelegate(QObject *parent = 0);
protected:   
   //! Отрисовка чекбокса
   virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, 
      const QModelIndex & index) const;

   //! Обработка событий чекбокса
   virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, 
      const QStyleOptionViewItem &option,                              
      const QModelIndex &index);
}; 

/*!
\class ButtonDelegate
\brief Кнопка в ячейке
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
   //! Отрисовка кнопки
   virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, 
      const QModelIndex & index) const;

   //! Обработка событий чекбокса
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
\brief Класс ввода IP:port
*/
class IPMaskDelegate : public QStyledItemDelegate
{
public:
   //! ctor.
   IPMaskDelegate(QObject *parent = 0);
protected:
   //! Задает данные редактору из модели
   virtual void setEditorData(QWidget * editor,
                      const QModelIndex & index) const;
   //! Задает данные модели из редактора
   virtual void setModelData(QWidget * editor, QAbstractItemModel * model,
      const QModelIndex & index) const;
   //! Создание редактора
   virtual QWidget * createEditor(QWidget * parent,
                         const QStyleOptionViewItem & ,
                         const QModelIndex & idx) const;
};

/*!
\class IPMaskDelegate
\brief Класс ввода строк
*/
class StringDelegate : public QStyledItemDelegate
{
public:
   //! ctor.
   StringDelegate(QObject *parent = 0);
protected:
   //! Задает данные редактору из модели
   virtual void setEditorData(QWidget * editor,
                      const QModelIndex & index) const;
   //! Задает данные модели из редактора
   virtual void setModelData(QWidget * editor, QAbstractItemModel * model,
      const QModelIndex & index) const;
   //! Создание редактора
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
   //! Отрисовка
   virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, 
      const QModelIndex & index) const;
   //! размер
   virtual QSize sizeHint(const QStyleOptionViewItem & option, 
      const QModelIndex & index ) const;
}; 

#endif
