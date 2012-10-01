#ifndef PROJECTTEMPLATEFUNCTIONS_H
#define PROJECTTEMPLATEFUNCTIONS_H

#include "qpair.h"
#include "qvector.h"
#include "qstring.h"
#include "ProjectException.h"

/**
*	This file contains a series of template functions that are widely used across this project.
*	All functions are related to getting or removing items from vectors.
*/

template <class T>
void switchPlaces(QVector<T*> &list, int index1, int index2) {
	if ((index1 < 0) || (index1 >= list.size()))
		throw ProjectException("Index 1 is out of bounds");

	if ((index2 < 0) || (index2 >= list.size()))
		throw ProjectException("Index 2 is out of bounds");

	if (index1 != index2) {
		T* temp1 = list.at(index1);
		T* temp2 = list.at(index2);
		list.replace(index1, temp2);
		list.replace(index2, temp1);
	}
}

template <class T, class U>
void switchPairs(QVector<QPair<T*, U>> &list, int index1, int index2) {
	if ((index1 < 0) || (index1 >= list.size()))
		throw ProjectException("Index 1 is out of bounds");

	if ((index2 < 0) || (index2 >= list.size()))
		throw ProjectException("Index 2 is out of bounds");

	if (index1 != index2) {
		QPair<T*, U> temp1 = list.at(index1);
		QPair<T*, U> temp2 = list.at(index2);
		list.replace(index1, temp2);
		list.replace(index2, temp1);
	}
}

template <class T>
T* getComponent(const QVector<T*> &list, const QString &name) {
	if (name == QString())
		throw ProjectException("Cannot search with an empty name.");

	for (int i = 0; i < list.size(); i++) {
		T *it = list.at(i);
		if (it->getName() == name) {
			return it;
		}
	}
	return 0;
}

template <class T, class U>
QPair<T*, U> getPair(const QVector<QPair<T*, U>> &list, const QString &name) {
	if (name == QString())
		throw ProjectException("Cannot search with an empty name.");

	for (int i = 0; i < list.size(); i++) {
		QPair<T*, U> it = list.at(i);
		if (it.first->getName() == name) {
			return it;
		}
	}
	return QPair<T*, U>(0, 0);
}

template <class T>
T* removeComponent(QVector<T*> &list, const QString &name) {
	if (name == QString())
		throw ProjectException("Cannot search with an empty name.");

	for (int i = 0; i < list.size(); i++) {
		T *it = list.at(i);
		if (it->getName() == name) {
			list.remove(i);
			return it;
		}
	}
	return 0;
}

template <class T, class U>
QPair<T*, U> removePair(QVector<QPair<T*, U>> &list, const QString &name) {
	if (name == QString())
		throw ProjectException("Cannot search with an empty name.");

	for (int i = 0; i < list.size(); i++) {
		QPair<T*, U> it = list.at(i);
		if (it.first->getName() == name) {
			list.remove(i);
			return it;
		}
	}
	return QPair<T*, U>(0, 0);
}

template <class T>
void addComponentAndLog(QVector<T*> &list, T* component, const QString &type, const QString &parent) {
	if (!component)
		throw ProjectException("Cannot add a null " + type + ".");

	if (!!getComponent<T>(list, component->getName()))
		throw ProjectException("Could not add " + type + " " + component->getName() + " because it is already a member of " + parent);

	list.append(component);
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Added " + type + " " + component->getName() + " to " + parent);
}

template <class T, class U>
void addPairAndLog(QVector<QPair<T*, U>> &list, QPair<T*, U> pair, const QString &type, const QString &parent) {
	if (!pair.first)
		throw ProjectException("Cannot add a null " + type + ".");

	if (!!getPair<T, U>(list, pair.first->getName()).first)
		throw ProjectException("Could not add " + type + " " + pair.first->getName() + " because it is already a member of " + parent);

	list.append(pair);
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Added " + type + " " + pair.first->getName() + " to " + parent);
}

template <class T>
T* removeComponentAndLog(QVector<T*> &list, const QString &name, const QString &type, const QString &parent) {
	if (name == QString())
		throw ProjectException("Cannot search for " + type + " with an empty name.");

	if (list.empty())
		throw ProjectException("Could not remove " + type + " " + name + " because the list is empty");

	if (!getComponent<T>(list, name))
		throw ProjectException("Could not remove " + type + " " + name + " because it does not exist in the list");

	T* component = removeComponent<T>(list, name);
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Removed " + type + " " + name + " from " + parent);
	return component;
}

template <class T, class U>
QPair<T*, U> removePairAndLog(QVector<QPair<T*, U>> &list, const QString &name, const QString &type, const QString parent) {
	if (name == QString())
		throw ProjectException("Cannot search for " + type + " with an empty name.");

	if (list.empty())
		throw ProjectException("Could not remove " + type + " " + name + " because the list is empty");

	if (!getPair<T, U>(list, name).first)
		throw ProjectException("Could not remove " + type + " " + name + " because it does not exist in the list");

	QPair<T*, U> pair = removePair<T, U>(list, name);
	Logger *logger = Logger::instance();
	logger->setStatus(true);
	logger->addMessage("Removed " + type + " " + name + " from " + parent);
	return pair;
}

#endif