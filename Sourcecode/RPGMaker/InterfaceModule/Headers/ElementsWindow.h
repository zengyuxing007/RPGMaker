#ifndef ELEMENTSWINDOW_H
#define ELEMENTSWINDOW_H

#include "qlabel.h"
#include "qframe.h"
#include "qboxlayout.h"
#include "qlistwidget.h"
#include "ProgramDefaults.h"
#include "../Components/Headers/Properties.h"

using projectcomponents::Properties;

namespace interfacemodule {

	/**
	*	The elements window allows to create and edit items.
	*/
	class ElementsWindow : public QFrame {

		Q_OBJECT

	public:
		ElementsWindow(QWidget *parent, Properties *properties);
		virtual ~ElementsWindow();

	private:
		void createElementsBox();
		void createVulnerableBox();
		void createResistantBox();
		void loadElements();
		void loadVulnerables(QString name);
		void loadResistants(QString name);

		Properties *_properties;
		QHBoxLayout *_layout;

		QListWidget *_elementsList;
		QListWidget *_vulnerableList;
		QListWidget *_resistantList;
		QLabel *_vulnerableLabel;
		QLabel *_resistantLabel;

	signals:
		void updateSignal();

	private slots:
		void elementSelected(const QString &name);
		void newElement();
		void editElement();
		void deleteElement();
		void moveElementUp();
		void moveElementDown();

		void addVulnerableElement();
		void deleteVulnerableElement();
		void moveVulnerableElementUp();
		void moveVulnerableElementDown();

		void addResistantElement();
		void deleteResistantElement();
		void moveResistantElementUp();
		void moveResistantElementDown();
		
	};

}

#endif