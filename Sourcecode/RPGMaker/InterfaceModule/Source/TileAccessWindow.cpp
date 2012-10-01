#include "qsize.h"
#include "qlabel.h"
#include "qpoint.h"
#include "qpixmap.h"
#include "qpainter.h"
#include "qboxlayout.h"
#include "qheaderview.h"
#include "qpushbutton.h"

#include "../ProgramDefaults.h"
#include "../Headers/TileAccessWindow.h"

using namespace interfacemodule;

TileAccessWindow::TileAccessWindow(QWidget *parent, Tileset *tileset)
	: QDialog(parent), _viewmode(ACCESS), _tilesetTable(0), _tileset(tileset) {

		setWindowTitle("Modify tile access");
		createWindow();
		drawTileset();
}

TileAccessWindow::~TileAccessWindow() {}

void TileAccessWindow::createWindow() {
	_tilesetTable = new QTableWidget(this);
	_tilesetTable->clear();
	_tilesetTable->setSelectionBehavior(QAbstractItemView::SelectItems);
	_tilesetTable->setSelectionMode(QAbstractItemView::SingleSelection);
	_tilesetTable->horizontalHeader()->setDefaultSectionSize(g_tilesize);
	_tilesetTable->verticalHeader()->setDefaultSectionSize(g_tilesize);
	_tilesetTable->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	_tilesetTable->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	_tilesetTable->horizontalHeader()->hide();
	_tilesetTable->verticalHeader()->hide();

	QFrame *buttonFrame = new QFrame(this);
	QLabel *label = new QLabel("Click on the tiles to change their access level.", this);
	QPushButton *proceedButton = new QPushButton("Ok", buttonFrame);
	proceedButton->setMaximumWidth(64);
	proceedButton->setDefault(true);
	QPushButton *viewButton = new QPushButton(QIcon(g_iconPath + "eye_icon&24.png"), "" , buttonFrame);
	viewButton->setMaximumSize(g_buttonsize, g_buttonsize);
	viewButton->setToolTip("Toggle access icons");
	QHBoxLayout *buttonLayout = new QHBoxLayout(buttonFrame);
	buttonLayout->setAlignment(Qt::AlignLeft);
	buttonLayout->addWidget(label);
	buttonLayout->addSpacing(20);
	buttonLayout->addWidget(viewButton);
	buttonLayout->addWidget(proceedButton);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setAlignment(Qt::AlignLeft);
	layout->addWidget(buttonFrame);
	layout->addWidget(_tilesetTable);

	connect(proceedButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(viewButton, SIGNAL(clicked()), this, SLOT(changeViewmode()));
	connect(_tilesetTable, SIGNAL(cellClicked(int, int)), this, SLOT(tileClicked(int, int)));
}

void TileAccessWindow::drawTileset() {
	QSize dimensions = _tileset->getDimensions();
	_tilesetTable->clear();
	_tilesetTable->setColumnCount(dimensions.width());
	_tilesetTable->setRowCount(dimensions.height());

	QPixmap passOverIcon(g_iconPath + "rnd_br_up_icon&24.png");
	QPixmap passUnderIcon(g_iconPath + "rnd_br_down_icon&24.png");
	QPixmap noPassIcon(g_iconPath + "round_delete_icon&24.png");

	QVector<QPixmap> tiles = _tileset->getTiles();

	for (int y = 0; y < dimensions.height(); y++) {
		for (int x = 0; x < dimensions.width(); x++) {

			QPixmap compositionTile(g_tilesize, g_tilesize);
			QPainter painter(&compositionTile);
			painter.setCompositionMode(QPainter::CompositionMode_Source);
			painter.fillRect(compositionTile.rect(), Qt::white);
			painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
			painter.drawPixmap(0, 0, tiles.at(dimensions.width()*y + x));

			if (_viewmode == ACCESS) {
				switch (_tileset->getAccess(QPoint(x, y))) {
				case Tileset::OVER:
					painter.drawPixmap((g_tilesize - 24)/2, (g_tilesize - 24)/2, passOverIcon);
					break;
				case Tileset::UNDER:
					painter.drawPixmap((g_tilesize - 24)/2, (g_tilesize - 24)/2, passUnderIcon);
					break;
				case Tileset::NONE:
					painter.drawPixmap((g_tilesize - 24)/2, (g_tilesize - 24)/2, noPassIcon);
					break;
				}
			}

			painter.end();

			QLabel *label = new QLabel();
			label->setPixmap(compositionTile);
			_tilesetTable->setCellWidget(y, x, label);
		}
	}
}

void TileAccessWindow::tileClicked(int row, int column) {
	if (_viewmode == ACCESS) {
		_tilesetTable->removeCellWidget(row, column);
		_tileset->scrollAccess(QPoint(column, row));
		QVector<QPixmap> tiles = _tileset->getTiles();

		QPixmap passOverIcon(g_iconPath + "rnd_br_up_icon&24.png");
		QPixmap passUnderIcon(g_iconPath + "rnd_br_down_icon&24.png");
		QPixmap noPassIcon(g_iconPath + "round_delete_icon&24.png");

		QPixmap compositionTile(g_tilesize, g_tilesize);
		QPainter painter(&compositionTile);
		painter.setCompositionMode(QPainter::CompositionMode_Source);
		painter.fillRect(compositionTile.rect(), Qt::white);
		painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
		painter.drawPixmap(0, 0, tiles.at(_tileset->getDimensions().width() * row + column));

		switch (_tileset->getAccess(QPoint(column, row))) {
		case Tileset::OVER:
			painter.drawPixmap((g_tilesize - 24)/2, (g_tilesize - 24)/2, passOverIcon);
			break;
		case Tileset::UNDER:
			painter.drawPixmap((g_tilesize - 24)/2, (g_tilesize - 24)/2, passUnderIcon);
			break;
		case Tileset::NONE:
			painter.drawPixmap((g_tilesize - 24)/2, (g_tilesize - 24)/2, noPassIcon);
			break;
		}
		painter.end();

		QLabel *label = new QLabel();
		label->setPixmap(compositionTile);
		_tilesetTable->setCellWidget(row, column, label);

	}
}

void TileAccessWindow::changeViewmode() {
	switch (_viewmode) {
	case NORMAL:
		_viewmode = ACCESS;
		break;
	case ACCESS:
		_viewmode = NORMAL;
		break;
	}
	drawTileset();
}