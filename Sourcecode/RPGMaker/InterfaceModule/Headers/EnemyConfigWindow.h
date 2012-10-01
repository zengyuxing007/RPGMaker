#ifndef ENEMYCONFIGWINDOW_H
#define ENEMYCONFIGWINDOW_H

#include "qpair.h"
#include "qlabel.h"
#include "qdialog.h"
#include "qspinbox.h"
#include "qlineedit.h"
#include "qcombobox.h"
#include "qboxlayout.h"

#include "../Components/Headers/Project.h"

using projectcomponents::Project;

namespace interfacemodule {

	/**
	*	The enemy configuration screen allows to create or edit an enemy.
	*	It gives access to an enemy's class, level, experience value, money drop and appearance.
	*/
	class EnemyConfigWindow : public QDialog {

		Q_OBJECT

	public:
		EnemyConfigWindow(QWidget *parent, Project *project);
		virtual ~EnemyConfigWindow();

		QString getName() const;
		int getLevel() const;
		QString getClass() const;
		int getExperience() const;
		QPair<int, int> getCurrency() const;
		QString getEnvironmentImage() const;
		QString getBattleImage() const;

		void setName(QString name);
		void setLevel(int level);
		void setClass(QString name);
		void setExperience(int experience);
		void setCurrency(QPair<int, int> currency);
		void setEnvironmentImage(QString image);
		void setBattleImage(QString image);

	private:
		void loadWindow();

		void createLeftPane();
		void createRightPane();

		Project *_project;
		QString _oldName;

		QHBoxLayout *_layout;
		QLineEdit *_nameField;
		QSpinBox *_levelSelection;
		QComboBox *_classSelection;
		QSpinBox *_experienceSelection;
		QSpinBox *_minCurrencySelection;
		QSpinBox *_maxCurrencySelection;
		QComboBox *_environmnentSelection;
		QComboBox *_battleSelection;
		QLabel *_environmentLabel;
		QLabel *_battleLabel;

	private slots:
		void checkInput();
		void environmentChanged(const QString &name);
		void battleChanged(const QString &name);
	};
}

#endif