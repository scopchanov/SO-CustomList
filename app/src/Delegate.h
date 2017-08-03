/****************************************************************************
 *
 *	MIT License
 *
 * 	Copyright (c) 2017 Michael Scopchanov
 *
 * 	Permission is hereby granted, free of charge, to any person obtaining a copy
 * 	of this software and associated documentation files (the "Software"), to deal
 * 	in the Software without restriction, including without limitation the rights
 * 	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * 	copies of the Software, and to permit persons to whom the Software is
 * 	furnished to do so, subject to the following conditions:
 *
 * 	The above copyright notice and this permission notice shall be included in all
 * 	copies or substantial portions of the Software.
 *
 * 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * 	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * 	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * 	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * 	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * 	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * 	SOFTWARE.
 *
 ***************************************************************************/

#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QTextLayout>

class Delegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	enum DataType { DT_Id = Qt::UserRole, DT_Timestamp, DT_Title, DT_Subtitle, DT_Details, DT_Pressed, DT_Expanded, DT_Offset };

	explicit Delegate(QObject *parent, bool checkable, bool expandable);

	bool isCheckable() const;
	bool isExpandable() const;

protected:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &index) const;

private:
	bool m_checkable, m_expandable;
};

#endif // DELEGATE_H
