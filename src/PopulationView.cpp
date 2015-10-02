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


static const char * _context = "PopulationView";
static const char * _openOrganismInNewTab = QT_TRANSLATE_NOOP("PopulationView", "Open organism in new tab");
static const char * _openGenomeInNewTab = QT_TRANSLATE_NOOP("PopulationView", "Open genome in new tab");


#include <QtGui/QContextMenuEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMenu>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QStyleOptionProgressBar>


#include "Application.hpp"
#include "custom_enums.hpp"
#include "GuiOrganismDesc.hpp"
#include "PopulationModel.hpp"
#include "PopulationView.hpp"


#include "mesh/Figure.hpp"
#include "bio/Genome.hpp"


namespace {


class _Delegate : public QStyledItemDelegate
{
   public:
      explicit _Delegate(QObject * parent = 0);
      virtual ~_Delegate();

      virtual void paint(
         QPainter * painter,
         const QStyleOptionViewItem & option,
         const QModelIndex & index
      ) const;

   protected:
      virtual bool editorEvent(
         QEvent * event,
         QAbstractItemModel * model,
         const QStyleOptionViewItem & option,
         const QModelIndex & index
      );
};


_Delegate::_Delegate(QObject * parent)
   : QStyledItemDelegate(parent)
{
}


_Delegate::~_Delegate()
{
}


void _Delegate::paint(
   QPainter * painter,
   const QStyleOptionViewItem & option,
   const QModelIndex & index
) const
{
   QStyledItemDelegate::paint(painter, option, index);
   if (index.isValid())
   {
      QStyle * style = QApplication::style();

      const QVariant progressVariant = index.data(Custom::ProgressRole);
      if (progressVariant.isValid())
      {
         const int progress = progressVariant.toInt();
         if (progress > 0 && progress < 100)
         {
            QStyleOptionViewItemV4 optCopy(option);
            initStyleOption(&optCopy, index);
            const QRect iconRect = style->subElementRect(
               QStyle::SE_ItemViewItemDecoration,
               &optCopy,
               0
            );
            QStyleOptionProgressBar optBar;
            optBar.rect = QRect(iconRect.bottomLeft() - QPoint(0, 16), QSize(128, 16));
            optBar.minimum = 0;
            optBar.maximum = 100;
            optBar.progress = progress;
            optBar.text = QString::number(progress) + "%";
            optBar.textVisible = true;
            style->drawControl(QStyle::CE_ProgressBar, &optBar, painter);
         }
      }

      if (index.data(Custom::CloseIndicatorRole).toBool())
      { // Paint close button;
         /*QStyleOptionViewItemV4 optCopy(option);
         optCopy.features |= QStyleOptionViewItemV2::HasCheckIndicator;
         optCopy.state = optCopy.state & (~QStyle::State_HasFocus);
         optCopy.state |= (ratingVariant.toBool() ?
            QStyle::State_On :
            QStyle::State_Off
         );
         const QRect checkRect = style->subElementRect(
            QStyle::SE_ItemViewItemCheckIndicator,
            &optCopy,
            0
         );
         optCopy.rect = QRect(
            optCopy.rect.topLeft() + QPoint(10, 10),
            checkRect.size()
         );
         style->drawPrimitive(
            QStyle::PE_IndicatorViewItemCheck,
            &optCopy,
            painter,
            0
         );*/


         int w = style->pixelMetric(QStyle::PM_TabCloseIndicatorWidth, 0, 0);
         int h = style->pixelMetric(QStyle::PM_TabCloseIndicatorHeight, 0, 0);

         QStyleOption opt;
         opt.rect = QRect(
            option.rect.topRight() + QPoint(-4 - w, 4),
            QSize(w, h)
         );
         opt.state |= QStyle::State_AutoRaise;
         /*if (isEnabled() && underMouse() && !isChecked() && !isDown())
            opt.state |= QStyle::State_Raised;
         if (isChecked())
            opt.state |= QStyle::State_On;
         if (isDown())
            opt.state |= QStyle::State_Sunken;*/

         if (option.state & QStyle::State_Selected) {
            opt.state |= QStyle::State_Selected;
         }
         style->drawPrimitive(QStyle::PE_IndicatorTabClose, &opt, painter, 0);
      }
   }
}


bool _Delegate::editorEvent(
   QEvent * event,
   QAbstractItemModel * model,
   const QStyleOptionViewItem & option,
   const QModelIndex & index
)
{
   if (index.isValid() && index.data(Custom::CloseIndicatorRole).toBool())
   {
      if ((option.state & QStyle::State_Enabled) &&
         (model->flags(index) & Qt::ItemIsEnabled)
      )
      {
         switch (event->type())
         {
            case QEvent::MouseButtonRelease:
            case QEvent::MouseButtonDblClick:
            case QEvent::MouseButtonPress:
            {
               QStyle * style = QApplication::style();

               int w = style->pixelMetric(QStyle::PM_TabCloseIndicatorWidth, 0, 0);
               int h = style->pixelMetric(QStyle::PM_TabCloseIndicatorHeight, 0, 0);

               QRect checkRect = QRect(
                  option.rect.topRight() + QPoint(-4 - w, 4),
                  QSize(w, h)
               );

               QMouseEvent * mouseEvent = static_cast<QMouseEvent *>(event);
               if (mouseEvent->button() != Qt::LeftButton || !checkRect.contains(mouseEvent->pos())) {
                  return false;
               }
               if ((event->type() == QEvent::MouseButtonPress) || (event->type() == QEvent::MouseButtonDblClick)) {
                  return true;
               }
               break;
            }
            case QEvent::KeyPress:
            {
               QKeyEvent * keyEvent = static_cast<QKeyEvent *>(event);
               if (keyEvent->key() != Qt::Key_Space && keyEvent->key() != Qt::Key_Select) {
                  return false;
               }
               break;
            }
            default:
               return false;
         }
         if (PopulationModel *popModel = qobject_cast<PopulationModel *>(model))
         {
            if (popModel->orientation() == Qt::Horizontal)
            {
               return model->removeColumn(index.column(), index.parent());
            }
            else
            {
               return model->removeRow(index.row(), index.parent());
            }
         }
         return false;
      }
   }
   return QStyledItemDelegate::editorEvent(event, model, option, index);
}


} // anonymous namespace;


/***************************************************************************
 *   PopulationView class implementation                                   *
 ***************************************************************************/


PopulationView::PopulationView(QWidget * parent)
   : QTableView(parent), m_contextMenu(0)
{
   setItemDelegate(new _Delegate(this));
   setSelectionMode(QAbstractItemView::SingleSelection);
   setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
   setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
   setShowGrid(false);
   horizontalHeader()->hide();
   horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
   verticalHeader()->hide();
   verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

   m_contextMenu = new QMenu(this);
   m_contextMenu->addAction(
      QCoreApplication::translate(_context, _openOrganismInNewTab),
      this, SLOT(openOrganism())
   );
   m_contextMenu->addAction(
      QCoreApplication::translate(_context, _openGenomeInNewTab),
      this, SLOT(openGenome())
   );
}


QSize PopulationView::sizeHint() const
{
   QSize size = QTableView::sizeHint();
   if (m_populationModel)
   {
      const QStyle * style = this->style();
      const int contentWidth = 128 +
         style->pixelMetric(QStyle::PM_FocusFrameHMargin, 0, this);
      int width = ((frameWidth()/* + spacing()*/) * 2) + contentWidth +
         style->pixelMetric(QStyle::PM_ScrollBarExtent, 0, verticalScrollBar());
      if (style->styleHint(QStyle::SH_ScrollView_FrameOnlyAroundContents))
      {
         width += (style->pixelMetric(QStyle::PM_DefaultFrameWidth) * 2);
         if (frameStyle() != QFrame::NoFrame)
         {
            width += style->pixelMetric(QStyle::PM_ScrollView_ScrollBarSpacing);
         }
      }

      if (m_populationModel->orientation() == Qt::Horizontal)
      {
         size.setHeight(width);
      }
      else
      {
         size.setWidth(width);
      }
   }
   return size;
}


void PopulationView::setModel(QAbstractItemModel * model)
{
   if (m_populationModel)
   {
      disconnect(
         m_populationModel,
         SIGNAL(orientationChanged()),
         this,
         SLOT(updateOrientation())
      );
   }

   m_populationModel = qobject_cast<PopulationModel *>(model);

   if (m_populationModel)
   {
      connect(
         m_populationModel,
         SIGNAL(orientationChanged()),
         SLOT(updateOrientation())
      );
   }
   QTableView::setModel(model);
   updateOrientation();
}


boost::shared_ptr<const mesh::Figure> PopulationView::selectedFigure() const
{
   if (QItemSelectionModel * selModel = selectionModel())
   {
      const QModelIndexList selection = selModel->selectedIndexes();
      if (!selection.isEmpty() && m_populationModel)
      {
         if (auto organism = m_populationModel->organismDesc(selection.front()))
         {
            return organism->figure;
         }
      }
   }
   return boost::shared_ptr<const mesh::Figure>();
}


bool PopulationView::isSomethingSelected() const
{
   if (QItemSelectionModel * selModel = selectionModel())
   {
      if (!selModel->selectedIndexes().isEmpty())
      {
         return true;
      }
   }
   return false;
}


void PopulationView::clearSelection()
{
   if (QItemSelectionModel * selModel = selectionModel())
   {
      selModel->clearSelection();
      selModel->setCurrentIndex(QModelIndex(), QItemSelectionModel::NoUpdate);
   }
}


void PopulationView::scrollToIndex(const QModelIndex & index)
{
   if (m_populationModel && index.model() == m_populationModel)
   {
      scrollTo(index);
   }
}


void PopulationView::contextMenuEvent(QContextMenuEvent * event)
{
   const QModelIndex index = indexAt(event->pos());
   if (m_populationModel && index.isValid())
   {
      if (m_populationModel->organismDesc(index))
      {
         m_contextMenuOriginIndex = index;
         m_contextMenu->popup(event->globalPos());
      }
   }
}


void PopulationView::selectionChanged(
   const QItemSelection & selected,
   const QItemSelection & deselected
)
{
   QTableView::selectionChanged(selected, deselected);
   emit selectionChanged();
}


void PopulationView::openOrganism()
{
   if (m_populationModel)
   {
      auto desc = m_populationModel->organismDesc(m_contextMenuOriginIndex);
      if (desc)
      {
         Application::instance()->addOrganismTab(desc);
      }
   }
}


void PopulationView::openGenome()
{
   if (m_populationModel)
   {
      auto desc = m_populationModel->organismDesc(m_contextMenuOriginIndex);
      if (desc)
      {
         Application::instance()->addGenomeTab(desc);
      }
   }
}


void PopulationView::updateOrientation()
{
   if (m_populationModel)
   {
      if (m_populationModel->orientation() == Qt::Horizontal)
      {
         setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
         setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
         setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
      }
      else
      {
         setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
         setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
         setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
      }
      updateGeometry();
   }
}
