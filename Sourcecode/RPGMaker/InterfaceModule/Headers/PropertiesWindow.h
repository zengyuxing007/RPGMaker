#ifndef PROPERTIESWINDOW_H
#define PROPERTIESWINDOW_H

#include "qtabwidget.h"
#include "qboxlayout.h"

#include "../Components/Headers/Properties.h"

using projectcomponents::Properties;

namespace interfacemodule {

	/**
	*	This window connects the windows of the elements, attributes, abilities, classes, weapons, armor and accessories.
	*/
	class PropertiesWindow : public QTabWidget {
	public:
		PropertiesWindow(QWidget *parent, Properties *properties);
		virtual ~PropertiesWindow();

	private:
		Properties *_properties;
	};

}

#endif