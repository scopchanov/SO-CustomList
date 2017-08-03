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

#include "ListView.h"

ListView::ListView(QWidget *parent, bool checkable, int detailRows) : QListView(parent)
{
	setModel(new QStandardItemModel(this));
	setItemDelegate(new Delegate(this, checkable, detailRows));
	setFrameStyle(QFrame::NoFrame | QFrame::Plain);
	setAutoScroll(false);
	setDragEnabled(false);
	setEditTriggers(QListView::NoEditTriggers);

	m_heightPanel = detailRows*QFontMetrics(QFont("MS Shell Dlg 2", 11)).lineSpacing() + m_heightHeader + 13;

	connect(this, &ListView::clicked, this, &ListView::on_itemClicked);
	connect(this, &ListView::doubleClicked, [=](const QModelIndex &index){ itemSelected(index.data(Qt::UserRole).toInt()); });
}

void ListView::on_itemClicked(const QModelIndex &index)
{
	Delegate *delegate = static_cast<Delegate *>(itemDelegate());
	QStandardItem *item = static_cast<QStandardItemModel *>(model())->itemFromIndex(index);
	bool expand = delegate->isExpandable() && mapFromGlobal(QCursor::pos()).x() >= visualRect(index).width() - 48,
			checked = index.data(Qt::CheckStateRole).toInt() == Qt::Checked,
			expanded = index.data(Delegate::DT_Expanded).toBool();

	if (m_ready)
	{
		if (expand)
		{
			QVariantAnimation *anim = new QVariantAnimation(this);

			m_ready = false;

			if (!expanded) item->setData(true, Delegate::DT_Expanded);

			anim->setStartValue(expanded ? m_heightPanel : m_heightHeader);
			anim->setEndValue(expanded ? m_heightHeader : m_heightPanel);
			anim->setDuration(150);

			anim->start(QVariantAnimation::DeleteWhenStopped);

			connect(anim, &QVariantAnimation::valueChanged, [=](const QVariant &value){ item->setSizeHint(QSize(0, value.toInt())); });
			connect(anim, &QVariantAnimation::finished, [=](){
				item->setData(!expanded, Delegate::DT_Expanded);
				m_ready = true;
			});
		}
		else if (delegate->isCheckable())
		{
			QVariantAnimation *anim = new QVariantAnimation(this);

			m_ready = false;

			if (!checked) item->setData(Qt::Checked, Qt::CheckStateRole);

			anim->setStartValue(checked ? 32 : 0);
			anim->setEndValue(checked ? 0 : 32);
			anim->setDuration(150);

			anim->start(QVariantAnimation::DeleteWhenStopped);

			connect(anim, &QVariantAnimation::valueChanged, [=](const QVariant &value){ item->setData(value.toInt(), Delegate::DT_Offset); });
			connect(anim, &QVariantAnimation::finished, [=](){
				item->setData(checked ? Qt::Unchecked : Qt::Checked, Qt::CheckStateRole);
				m_ready = true;
			});
		}
	}
}
