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

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	QStandardItemModel *model;
	ListView *view;
	QJsonObject entry
	{
		{"Title", "Michael Scopchanov"},
		{"Subtitle", "http://www.simplex.bg/en/products/waagescanlite/"},
		{"Details", {{
			{"Qualification", "PhD in Automation Engineering"},
			{"Occupation", "Software Developer"},
			{"Location", "Europe, Germany"}}
		}}
	};
	QString title = entry["Title"].toString(), subtitle = entry["Subtitle"].toString(), details;
	QJsonObject json = entry["Details"].toObject();
	QStringList keys = json.keys();
	int N = 100024;

	foreach (QString key, keys) { details.append(key + ": " + json[key].toString() + (key == keys.last() ? "" : "\n")); }

	view = new ListView(this, true, keys.count());

	model = static_cast<QStandardItemModel *>(view->model());
	model->blockSignals(true);

	for (int n = 0; n < N; n++)
	{
		QStandardItem *item = new QStandardItem(QString::number(n));

		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

		item->setData(QString("%1").arg(n + 1, 8, 10, QChar('0')), Delegate::DT_Id);
		item->setData(QDateTime::currentDateTime().toString("dd.MM.yy HH:mm"), Delegate::DT_Timestamp);
		item->setData(title, Delegate::DT_Title);
		item->setData(subtitle, Delegate::DT_Subtitle);
		item->setData(details, Delegate::DT_Details);

		item->setData(false, Delegate::DT_Expanded);
		item->setSizeHint(QSize(0, 64));
		model->appendRow(item);
	}

	model->blockSignals(false);

	resize(640, 480);
	setMinimumWidth(250);
	setCentralWidget(view);
}
