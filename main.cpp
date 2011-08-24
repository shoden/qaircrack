/*
 * Copyright (C) 2011 Juan Roldan Ruiz <juan@shoden.es>
 *  
 * This file is part of QAircrack.
 * 
 * QAircrack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * QAircrack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with QAircrack.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <QtGui/QApplication>
#include "qaircrack.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QAircrack w;
    w.show();
    return a.exec();
}
