/***************************************************************************
 *   Copyright (C) 2015 Andrey Timashov                                    *
 *                                                                         *
 *   This file is part of Tetrahedrosaur.                                  *
 *                                                                         *
 *   Tetrahedrosaur is free software: you can redistribute it and/or       *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   Tetrahedrosaur is distributed in the hope that it will be useful,     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      *
 *   General Public License for more details.                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Tetrahedrosaur. If not, see <http://www.gnu.org/licenses/> *
 ***************************************************************************/


#ifndef PROJECT_HPP
#define PROJECT_HPP


#include <vector>


#include <boost/shared_ptr.hpp>


#include <QtCore/QString>
#include <QtGui/QPixmap>


#include "PopulationModel.hpp"


struct GuiOrganismDesc;


namespace mesh {
class Figure;
}


/***************************************************************************
 *   Project class declaration                                             *
 ***************************************************************************/


class Project : public PopulationModel
{
      Q_OBJECT
   public:
      virtual ~Project();

      static boost::shared_ptr<Project> create(const QString & projectName);
      static boost::shared_ptr<Project> open(const QString & fileName);
      bool save();
      bool saveAs(const QString & fileName);

      inline bool isSaved() const {return m_isSaved;}
      inline QString projectName() const {return m_projectName;}
      inline QString fileName() const {return m_fileName;}

      const std::vector<boost::shared_ptr<GuiOrganismDesc> > & population(
      ) const;
      void append(
         const std::vector<boost::shared_ptr<GuiOrganismDesc> > & descs
      );

      void setDescFigure(
         size_t descIndex,
         boost::shared_ptr<mesh::Figure> figure,
         QPixmap portrait
      );

      virtual boost::shared_ptr<const GuiOrganismDesc> organismDesc(
         const QModelIndex & index
      ) const;

      virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
      virtual QVariant data(
         const QModelIndex & index,
         int role = Qt::DisplayRole
      ) const;
      virtual bool removeRows(
         int row,
         int count,
         const QModelIndex & parent = QModelIndex()
      );
      virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;

   signals:
      void projectChanged();

   private:
      explicit Project(const QString & projectName);

      bool m_isSaved;
      QString m_projectName;
      QString m_fileName;
      std::vector<boost::shared_ptr<GuiOrganismDesc> > m_population;
};


#endif
