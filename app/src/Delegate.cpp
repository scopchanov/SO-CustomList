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

#include "Delegate.h"

Delegate::Delegate(QObject *parent, bool checkable, bool expandable) :
	QStyledItemDelegate(parent),
	m_checkable(checkable),
	m_expandable(expandable)
{

}

bool Delegate::isExpandable() const
{
	return m_expandable;
}

bool Delegate::isCheckable() const
{
	return m_checkable;
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QString id = index.data(DT_Id).toString(), timestamp = index.data(DT_Timestamp).toString(), title = index.data(DT_Title).toString(),
			subtitle = index.data(DT_Subtitle).toString(), colID, colTimestamp, colTitle, colDetails, fontFamily = "MS Shell Dlg 2";
	QFont fntHeader(fontFamily, 9), fntTitle(fontFamily, 11, QFont::Bold), fntSubtitle(fontFamily, 11);
	bool pressed = index.data(DT_Pressed).toBool(), checked = index.data(Qt::CheckStateRole) == Qt::Checked, expanded = index.data(DT_Expanded).toBool(),
			hasSubtitle = !subtitle.isEmpty(), hovered = option.state & QStyle::State_MouseOver;
	int x = index.data(DT_Offset).toInt(), xContent = x + 9, xTimestamp, y = option.rect.y(), yIcon = y + 24, w = option.rect.width(),
			h = option.rect.height() - 1, d = m_expandable ? 48 : 0, wID, wTimestamp, wTitle, wContent = w - d - xContent, spaceForTimestamp;

	painter->save();
	painter->setClipping(true);
	painter->setClipRect(option.rect);

	if (hovered)
	{
		colID = "#D3D6D8";
		colTimestamp = "#C5C9CB";
		colTitle = "#FFFFFF";
		colDetails = "#D3D6D8";

		painter->fillRect(option.rect.adjusted(x, 0, 0, -1), pressed ? "#1ABBBF" : "#15999B");
	}
	else
	{
		int rowBottom = y + h;

		colID = "#616B71";
		colTimestamp = "#A8AEB1";
		colTitle = "#303030";
		colDetails = "#50585D";

		painter->setPen(QPen("#E2E4E5"));
		painter->drawLine(xContent, rowBottom, w, rowBottom);
	}

	painter->setFont(fntHeader);
	painter->setPen(QPen(colID));
	painter->drawText(xContent, y + 22, id);

	wID = painter->fontMetrics().width(id);
	wTimestamp = painter->fontMetrics().width(timestamp);
	xTimestamp = wID + 9;
	spaceForTimestamp = wContent - xTimestamp;

	painter->setPen(QPen(colTimestamp));
	painter->setOpacity((spaceForTimestamp - 9.0)/wTimestamp);
	painter->drawText(QRect(xContent + xTimestamp, y + 10, spaceForTimestamp, 22), Qt::TextSingleLine, timestamp);

	painter->setFont(fntTitle);
	wTitle = painter->fontMetrics().width(title);
	painter->setPen(QPen(colTitle));
	painter->setOpacity((wContent - 9.0)/wTitle);
	painter->drawText(QRect(xContent, y + 35, wContent, 32), Qt::TextSingleLine, title);

	if (hasSubtitle)
	{
		int xSubtitle = wTitle + 12, spaceForSubtitle = wContent - xSubtitle, wSubtitle;

		painter->setFont(fntSubtitle);
		wSubtitle = painter->fontMetrics().width(subtitle);
		painter->setOpacity((spaceForSubtitle - 9.0)/wSubtitle);
		painter->drawText(QRect(xContent + xSubtitle, y + 35, spaceForSubtitle, 22), Qt::TextSingleLine, subtitle);
	}

	if (m_expandable)
	{
		if (expanded)
		{
			QString details = index.data(DT_Details).toString();
			QRect rectDetails = painter->boundingRect(QRect(), Qt::AlignLeft, details);

			painter->setFont(fntSubtitle);
			painter->setPen(QPen(colDetails));
			painter->setOpacity((wContent - 9.0)/rectDetails.width());
			painter->drawText(QRect(xContent, y + 64, wContent, rectDetails.height()), Qt::AlignLeft, details);
			painter->setOpacity(1);
			painter->drawPixmap(w - 31, yIcon, QPixmap(":/pix/images/icons/16/up.png"));
		}
		else
		{
			painter->setOpacity(1);
			painter->drawPixmap(w - 31, yIcon, QPixmap(":/pix/images/icons/16/down.png"));
		}
	}

	painter->setOpacity(1);

	if (checked)
	{
		painter->fillRect(0, y, x, 63, "#C5C9CB");
		if (expanded) { painter->fillRect(0, y + 63, x, h - 63, "#F0F1F2"); }
		painter->drawPixmap(x - 25, yIcon, QPixmap(":/pix/images/icons/16/check.png"));
	}

	painter->restore();
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &index) const
{
	return index.data(Qt::SizeHintRole).toSize();
}
