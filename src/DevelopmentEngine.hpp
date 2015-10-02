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


#ifndef DEVELOPMENTENGINE_HPP
#define DEVELOPMENTENGINE_HPP


#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>


#include <QtCore/QObject>
#include <QtGui/QPixmap>


struct GuiOrganismDesc;
class OrganismPixelBuffer;


namespace bio {
class Organism;
}
namespace mesh {
class Figure;
}


/***************************************************************************
 *   DevelopmentEngine class declaration                                   *
 ***************************************************************************/


class DevelopmentEngine : public QObject
{
   Q_OBJECT

   public:
      explicit DevelopmentEngine(QObject * parent = 0);
      virtual ~DevelopmentEngine();

      inline bool isReady() const {return m_isReady;}

      void start(
         const std::vector<boost::shared_ptr<GuiOrganismDesc> > & descs
      );
      void abort();

   protected:
      virtual void timerEvent(QTimerEvent * event);

   signals:
      void descStarted(size_t descIndex);
      void descProgressChanged(size_t descIndex, int progress);
      void descFinished(
         size_t descIndex,
         boost::shared_ptr<mesh::Figure> figure,
         QPixmap portrait
      );
      void allFinished();

   private:
      bool m_isReady;
      std::vector<boost::shared_ptr<GuiOrganismDesc> > m_descs;
      boost::scoped_ptr<OrganismPixelBuffer> m_buffer;
      boost::scoped_ptr<bio::Organism> m_processingOrganism;
      size_t m_processingDesc;
      int m_lastProgress;
      int m_timerId;
};


#endif
