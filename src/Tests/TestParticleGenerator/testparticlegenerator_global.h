/* Copyright (C) Jelle Spijker - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * and only allowed with the written consent of the author (Jelle Spijker)
 * This software is proprietary and confidential
 * Written by Jelle Spijker <spijker.jelle@gmail.com>, 2015
 */



#ifndef TESTPARTICLEGENERATOR_GLOBAL_H
#define TESTPARTICLEGENERATOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TESTPARTICLEGENERATOR_LIBRARY)
#  define TESTPARTICLEGENERATORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TESTPARTICLEGENERATORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TESTPARTICLEGENERATOR_GLOBAL_H
