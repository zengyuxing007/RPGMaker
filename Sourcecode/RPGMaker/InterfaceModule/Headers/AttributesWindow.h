#ifndef ATTRIBUTESWINDOW_H
#define ATTRIBUTESWINDOW_H

#include "qlabel.h"
#include "qframe.h"
#include "qboxlayout.h"
#include "qlistwidget.h"
#include "qtablewidget.h"

#include "../Components/Headers/AttributeSystem.h"

using projectcomponents::AttributeSystem;

namespace interfacemodule {

	/**
	*	The attributes window allows to edit the 4 standard attributes as well as create and edit offensive and defensive attributes and attribute links.
	*/
	class AttributesWindow : public QFrame {

		Q_OBJECT

	public:
		AttributesWindow(QWidget *parent, AttributeSystem *attributes);
		virtual ~AttributesWindow();

	private:
		void createWindow();
		void loadAttributes();
		void loadLinks();

		AttributeSystem *_attributes;

		QHBoxLayout *_layout;

		QLabel *_lifesourceLabel;
		QLabel *_energysourceLabel;
		QLabel *_agilityforceLabel;
		QLabel *_evasionforceLabel;
		QListWidget *_offensiveList;
		QListWidget *_defensiveList;
		QTableWidget *_linkTable;

	signals:
		void updateSignal();

	private slots:
		void editLifesource();
		void editEnergysource();
		void editAgilityforce();
		void editEvasionforce();

		void newOffensive();
		void editOffensive();
		void deleteOffensive();
		void moveOffensiveUp();
		void moveOffensiveDown();

		void newDefensive();
		void editDefensive();
		void deleteDefensive();
		void moveDefensiveUp();
		void moveDefensiveDown();

		void addLink();
		void deleteLink();
	};
}
#endif