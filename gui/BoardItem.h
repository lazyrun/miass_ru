/*! \file BoardItem.h
    \brief 
    \author Kiselev Kirill
    \date 13.10.2012
*/

#ifndef BoardItem_H
#define BoardItem_H

/*!
\class BoardItem
\brief 
*/
struct BoardItem
{
   QString addr;
   QString caption;
   QString text;
   QString date;
   QString color;
   QImage image;
   bool operator==(const BoardItem & rhs)
   {
      if (addr == rhs.addr)
         return true;
      return false;
   }
};
Q_DECLARE_METATYPE(BoardItem)

#endif
