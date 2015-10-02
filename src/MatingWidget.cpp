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


#include <QtCore/QtGlobal>


static const char * _clear = QT_TRANSLATE_NOOP("MatingWidget", "Clear");
static const char * _swap = QT_TRANSLATE_NOOP("MatingWidget", "Swap");


#include <QtGui/QPainter>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStyleOptionFrame>


#include "GuiOrganismDesc.hpp"
#include "MatingWidget.hpp"


#include "bio/Config.hpp"
#include "bio/Genome.hpp"
#include "bio/mating.hpp"


namespace {


bio::OrganismDesc * _createGuiOrganismDesc()
{
   return new GuiOrganismDesc;
}


} // anonymous namespace;


/***************************************************************************
 *   MatingWidget class implementation                                     *
 ***************************************************************************/


MatingWidget::MatingWidget(QWidget * parent)
   : QWidget(parent),
   m_clearButton(0),
   m_swapButton(0)
{
   m_clearButton = new QPushButton(
      QIcon(style()->standardPixmap(QStyle::SP_TrashIcon)),
      QString(),
      this
   );
   m_clearButton->setFlat(true);
   m_clearButton->setFocusPolicy(Qt::NoFocus);
   m_clearButton->setToolTip(tr(_clear));
   m_clearButton->setGeometry(0, 102, 26, 26);

   m_swapButton = new QPushButton(QIcon(":/swap.png"), QString(), this);
   m_swapButton->setFlat(true);
   m_swapButton->setFocusPolicy(Qt::NoFocus);
   m_swapButton->setToolTip(tr(_swap));
   m_swapButton->setGeometry(102, 0, 26, 26);

   connect(m_clearButton, SIGNAL(clicked()), SLOT(clear()));
   connect(m_swapButton, SIGNAL(clicked()), SLOT(swap()));
}


MatingWidget::~MatingWidget()
{
}


QSize MatingWidget::sizeHint() const
{
   return QSize(128, 128);
}


void MatingWidget::setLeftParent(
   boost::shared_ptr<const GuiOrganismDesc> leftParent
)
{
   m_leftParent = leftParent;
   m_leftPortrait = leftParent->portrait;
   update();
}


void MatingWidget::setRightParent(
   boost::shared_ptr<const GuiOrganismDesc> rightParent
)
{
   m_rightParent = rightParent;
   m_rightPortrait = rightParent->portrait;
   update();
}


std::vector<boost::shared_ptr<GuiOrganismDesc> > MatingWidget::mate(
   size_t count
) const
{
   std::vector<boost::shared_ptr<const bio::OrganismDesc> > parents;
   parents.reserve(2);
   if (m_leftParent)
   {
      parents.push_back(m_leftParent);
   }
   if (m_rightParent)
   {
      parents.push_back(m_rightParent);
   }

   std::vector<boost::shared_ptr<GuiOrganismDesc> > result;
   if (parents.empty())
   {
      result.reserve(count);
      for (size_t i = 0; i < count; ++i)
      {
         boost::shared_ptr<GuiOrganismDesc> desc(new GuiOrganismDesc());
         desc->initialConditions.cellLimit = 100;
         desc->initialConditions.applyMutations();
         desc->genome.reset(new bio::Genome(
            boost::shared_ptr<const bio::Config>(new bio::Config())
         ));
         result.push_back(desc);
      }
   }
   else
   {
      const auto offsprings = std::move(bio::mate(
         parents,
         _createGuiOrganismDesc, // TODO: use lambda;
         count,
         boost::shared_ptr<const bio::Config>(new bio::Config())
      ));
      result.reserve(offsprings.size());
      for (boost::shared_ptr<bio::OrganismDesc> offspring : offsprings)
      {
         if (auto gui = boost::dynamic_pointer_cast<GuiOrganismDesc>(offspring))
         {
            result.push_back(gui);
         }
      }
   }
   return result;
}


void MatingWidget::clear()
{
   m_leftParent.reset();
   m_leftPortrait = QPixmap();
   m_rightParent.reset();
   m_rightPortrait = QPixmap();
   update();
}


void MatingWidget::swap()
{
   m_leftParent.swap(m_rightParent);
   std::swap(m_leftPortrait, m_rightPortrait);
   update();
}


void MatingWidget::paintEvent(QPaintEvent * event)
{
   QPainter painter(this);

   QStyleOptionFrame opt;
   opt.initFrom(this);
   opt.frameShape = QFrame::StyledPanel;
   opt.lineWidth = 2;
   opt.midLineWidth = 0;
   opt.state |= QStyle::State_Sunken;

   int fw = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, &opt, this);
   int dfw = 2 * fw;

   opt.rect = QRect(32 - dfw, 32 - dfw, 96 + dfw, 96 + dfw);
   style()->drawControl(QStyle::CE_ShapedFrame, &opt, &painter, this);

   QRect rect(32 - fw, 32 - fw, 96, 96);
   if (m_rightParent)
   {
      painter.drawPixmap(rect, m_rightPortrait);
   }
   else
   {
      painter.fillRect(rect, QColor(0xbf, 0xbf, 0xbf));
   }

   rect.moveTo(fw, fw);
   opt.rect.moveTo(0, 0);
   style()->drawControl(QStyle::CE_ShapedFrame, &opt, &painter, this);

   if (m_leftParent)
   {
      painter.drawPixmap(rect, m_leftPortrait);
   }
   else
   {
      painter.fillRect(rect, QColor(0xea, 0xea, 0xea));
   }
}
