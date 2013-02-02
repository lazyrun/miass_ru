/*! \file CheckTableItemDelegate.h
    \brief Делегаты для столбцов
    \author Kiselev Kirill (kiselev@papillon.ru)
    \date 04.10.2012
*/

#include "CheckTableItemDelegate.h"
#include "BoardItem.h"

//
// BoolDelegate
//
BoolDelegate::BoolDelegate(QObject *parent)
: QStyledItemDelegate(parent)
{
} 

void BoolDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, 
   const QModelIndex & index) const
{
   QStyleOptionViewItemV4 viewItemOption(option);
   viewItemOption.state &= ~QStyle::State_HasFocus;

   const int textMargin = 
      QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
   QRect newRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
      QSize(option.decorationSize.width() + 5,
      option.decorationSize.height()),
      QRect(option.rect.x() + textMargin, option.rect.y(),
      option.rect.width() - (2 * textMargin), option.rect.height()));        
   
   if ((option.state & QStyle::State_Active) && 
      (option.state & QStyle::State_Selected))
   {
      painter->fillRect(option.rect,
         option.palette.brush(QPalette::Highlight));
   }
   else
   {
      if (option.state & QStyle::State_Selected)
      {
         painter->fillRect(option.rect,
            option.palette.brush(QPalette::Inactive, QPalette::Highlight));
      }
   }
   
   viewItemOption.rect = newRect;

   QStyledItemDelegate::paint(painter, viewItemOption, index);

   if ((option.state & QStyle::State_Active) &&
       (option.state & QStyle::State_HasFocus))
   {
      QStyleOptionFocusRect o;
      o.rect = option.rect;
      o.state |= QStyle::State_KeyboardFocusChange;
      o.state |= QStyle::State_Item;
      QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled)
                           ? QPalette::Normal : QPalette::Disabled;
      o.backgroundColor = option.palette.color(cg, 
         (option.state & QStyle::State_Selected) ? 
                     QPalette::Highlight :
                     QPalette::Window);
      QApplication::style()->drawPrimitive(QStyle::PE_FrameFocusRect, 
         &o, painter);
   }
}

bool BoolDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, 
            const QStyleOptionViewItem & option,
            const QModelIndex & index)
{
   if (!event || !model)
      return false;
   // make sure that the item is checkable         
   Qt::ItemFlags flags = model->flags(index);       
   if (!(flags & Qt::ItemIsUserCheckable) || !(flags & Qt::ItemIsEnabled))       
      return false;         
   // make sure that we have a check state         
   QVariant value = index.data(Qt::CheckStateRole);         
   if (!value.isValid())             
      return false;         
   // make sure that we have the right event type         
   if (event->type() == QEvent::MouseButtonRelease) 
   {
      const int textMargin =
         QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;   
      QRect checkRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,   
                           option.decorationSize,              
                           QRect(option.rect.x() + 
                           (2 * textMargin), option.rect.y(),                    
                           option.rect.width() - (2 * textMargin),        
                           option.rect.height()));             
      if (!checkRect.contains(static_cast<QMouseEvent*>(event)->pos()))          
         return false;    
   }
   else if (event->type() == QEvent::KeyPress) 
   {
      if (static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space &&
          static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select)               
          return false;   
   }
   else 
   {
      return false;  
   }
   Qt::CheckState state = 
      (static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked ?
      Qt::Unchecked : Qt::Checked);
   return model->setData(index, state, Qt::CheckStateRole);     
}

//
// IPMaskDelegate
//
IPMaskDelegate::IPMaskDelegate(QObject * parent)
: QStyledItemDelegate(parent)
{
}

void IPMaskDelegate::setEditorData(QWidget * editor,
                   const QModelIndex & index) const
{
   QString data = index.model()->data(index).toString();
   QLineEdit * edit = qobject_cast<QLineEdit *>(editor);
   if (edit)
   {
      edit->setText(data);
   }
}

QWidget * IPMaskDelegate::createEditor(QWidget * parent,
                                        const QStyleOptionViewItem & ,
                                        const QModelIndex &) const
{
   QLineEdit * edit = new QLineEdit(parent);
   edit->setInputMask("000.000.000.000:00000;_");
   return edit;
}

void IPMaskDelegate::setModelData(QWidget * editor,
                                          QAbstractItemModel * model,
                                          const QModelIndex & index) const
{
   QLineEdit * edit = qobject_cast<QLineEdit *>(editor);
   if (edit)
   {
      model->setData(index, edit->text());
   }
}

//
// StringDelegate
//
StringDelegate::StringDelegate(QObject * parent)
: QStyledItemDelegate(parent)
{
}

void StringDelegate::setEditorData(QWidget * editor,
                   const QModelIndex & index) const
{
   QString data = index.model()->data(index).toString();
   QLineEdit * edit = qobject_cast<QLineEdit *>(editor);
   if (edit)
   {
      edit->setText(data);
   }
}

QWidget * StringDelegate::createEditor(QWidget * parent,
                                        const QStyleOptionViewItem & ,
                                        const QModelIndex &) const
{
   QLineEdit * edit = new QLineEdit(parent);
   return edit;
}

void StringDelegate::setModelData(QWidget * editor,
                                          QAbstractItemModel * model,
                                          const QModelIndex & index) const
{
   QLineEdit * edit = qobject_cast<QLineEdit *>(editor);
   if (edit)
   {
      model->setData(index, edit->text());
   }
}

//
// ButtonDelegate
//
ButtonDelegate::ButtonDelegate(const QString & text, QObject *parent)
: QStyledItemDelegate(parent)
{
   text_ = text;
}

void ButtonDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, 
   const QModelIndex & index) const
{
   QStyleOptionViewItemV4 viewItemOption(option);
   viewItemOption.state &= ~QStyle::State_HasFocus;

   const int textMargin = 
      QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
   QRect newRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
      QSize(option.decorationSize.width() + 5,
      option.decorationSize.height()),
      QRect(option.rect.x() + textMargin, option.rect.y(),
      option.rect.width() - (2 * textMargin), option.rect.height()));        
   
   if ((option.state & QStyle::State_Active) && 
      (option.state & QStyle::State_Selected))
   {
      painter->fillRect(option.rect,
         option.palette.brush(QPalette::Highlight));
   }
   else
   {
      if (option.state & QStyle::State_Selected)
      {
         painter->fillRect(option.rect,
            option.palette.brush(QPalette::Inactive, QPalette::Highlight));
      }
   }
   
   viewItemOption.rect = newRect;

   QStyledItemDelegate::paint(painter, viewItemOption, index);
   
   QStyleOptionButton buttonOpt;
   QPushButton btn;
   buttonOpt.initFrom(&btn);
   if (pressedMap_[index] == true)
      buttonOpt.state |= QStyle::State_Sunken;
   buttonOpt.text = text_;
   buttonOpt.rect = option.rect;
   buttonOpt.rect.setY(buttonOpt.rect.y() + buttonOpt.rect.height() / 2 - 12);
   buttonOpt.rect.setHeight(24);

   QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOpt, painter);

   if ((option.state & QStyle::State_Active) &&
       (option.state & QStyle::State_HasFocus))
   {
      QStyleOptionFocusRect o;
      o.rect = option.rect;
      o.state |= QStyle::State_KeyboardFocusChange;
      o.state |= QStyle::State_Item;
      QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled)
                           ? QPalette::Normal : QPalette::Disabled;
      o.backgroundColor = option.palette.color(cg, 
         (option.state & QStyle::State_Selected) ? 
                     QPalette::Highlight :
                     QPalette::Window);
      QApplication::style()->drawPrimitive(QStyle::PE_FrameFocusRect, 
         &o, painter);
   }

}

bool ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, 
   const QStyleOptionViewItem &option,                              
   const QModelIndex &index)
{
   if (!event || !model)
      return false;

   QRect btnRect = option.rect;
   btnRect.setY(btnRect.y() + btnRect.height() / 2 - 12);
   btnRect.setHeight(24);
   
   if (!btnRect.contains(static_cast<QMouseEvent*>(event)->pos()))
      return false;

   if (event->type() == QEvent::MouseButtonPress)
   {
      pressedMap_[index] = true;
      return true;
   }
   else if (event->type() == QEvent::MouseButtonRelease)
   {
      pressedMap_[index] = false;
      emit pressed(index);
      return true;
   }

   return false;
}

//
// BoardItemDelegate
//
BoardItemDelegate::BoardItemDelegate(QObject *parent)
: QStyledItemDelegate(parent)
{
}

void BoardItemDelegate::paint(QPainter * painter, 
                              const QStyleOptionViewItem & option, 
                              const QModelIndex & index) const
{
   QStyledItemDelegate::paint(painter, option, index);
   QVariant var = index.model()->data(index);
   if (qVariantCanConvert<BoardItem>(var))
   {
      BoardItem item = qvariant_cast<BoardItem>(var);
      QPen pen;
      if ((option.state & QStyle::State_Active) && 
         (option.state & QStyle::State_Selected))
      {
         pen.setColor(Qt::white);
      }
      
      painter->setPen(pen);
      QFont fn = option.font;
      fn.setBold(true);
      
      QFontMetrics fm(fn);
      QRect rectCapt = fm.boundingRect(option.rect, Qt::AlignLeft | Qt::TextWordWrap, 
         item.caption);
      
      painter->setFont(fn);
      painter->drawText(rectCapt.adjusted(3, 1, 3, 0), 
         Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, 
         item.caption);
      painter->setFont(option.font);
      
      if (!item.image.isNull())
      {
         painter->drawImage(option.rect.x() + 3, option.rect.y() + fm.height(),
            item.image);
         painter->drawText(option.rect.adjusted(item.image.width() + 6, fm.height(),
            0, 0), 
            Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, 
            item.text);
      }
      else
      {
         painter->drawText(option.rect.adjusted(3, fm.height(), 0, 0), 
            Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, 
            item.text);
      }
   }   
}

QSize BoardItemDelegate::sizeHint(const QStyleOptionViewItem & option, 
   const QModelIndex & index) const
{
   QVariant var = index.model()->data(index);
   
   if (qVariantCanConvert<BoardItem>(var))
   {
      BoardItem item = qvariant_cast<BoardItem>(var);
      //вычислить размер
      //QFontMetrics fm(option.font);
      if (item.image.isNull())
      {
         //просто текст
         QFont fn = option.font;
         fn.setBold(true);
         QFontMetrics fmCapt(fn);

         QRect rect = option.rect;
         rect = rect.adjusted(12, 0, 0, 0);
         QRect rectCapt = fmCapt.boundingRect(rect, 
            Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, 
            item.caption);
         
         rect = option.rect;
         fn.setBold(false);
         QFontMetrics fmText(fn);
         rect = rect.adjusted(12, rectCapt.height(), -12, 0);
         QRect rectText = fmText.boundingRect(rect, 
            Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, 
            item.text);

         QSize sz(option.rect.width(), 
            rectCapt.height() + rectText.height() + fmText.height());
         return sz;
      }
      else
      {
         QFontMetrics fm(option.font);
         //текст с картинкой
         QRect imgRect = item.image.rect();
         //вычислить область под текст
         QRect rect = option.rect;
         rect = rect.adjusted(imgRect.width() + 6, 0, -12, 0);
         rect = fm.boundingRect(rect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, 
            item.caption + "\n" + item.text);
         
         QSize sz(option.rect.width(), rect.height() + 12);
         //размер текста меньше чем картинка
         if (sz.height() < imgRect.height() + fm.height())
         {
            //выровнять по картинке
            return QSize(option.rect.width(), imgRect.height() + 2*fm.height());
         }
         else
         {
            //вернуть размер по тексту
            return sz;
         }
      }
   }
   
   return QStyledItemDelegate::sizeHint(option, index);
}

