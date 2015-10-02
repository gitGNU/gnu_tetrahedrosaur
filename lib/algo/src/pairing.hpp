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


#ifndef ALGO_PAIRING_HPP
#define ALGO_PAIRING_HPP


#include <algorithm>
#include <ostream>
#include <set>
#include <vector>


#include <boost/optional.hpp>


#include "pairing_pair.hpp"
#include "roommates.hpp"


namespace algo {


namespace pairing {


/***************************************************************************
 *   MetricMatrix class declaration and implementation                     *
 ***************************************************************************/


enum METRIC_SENSE {
   MS_MORE_METRIC_MORE_ALIKE = 0,
   MS_LESS_METRIC_MORE_ALIKE
};


template <typename Metric>
bool doesMetricPassThreshold(
   const Metric & metric,
   const boost::optional<Metric> & threshold,
   METRIC_SENSE metricSense)
{
   if (threshold)
   {
      const bool max = (metricSense == MS_MORE_METRIC_MORE_ALIKE);
      return ((max && metric >= *threshold) || (!max && metric <= *threshold));
   }
   return true;
}


template <typename Metric>
class MetricMatrix
{
   public:
      template <typename T, typename SymmetricCompare>
      explicit MetricMatrix(
         const std::vector<T> & data,
         SymmetricCompare compare,
         METRIC_SENSE metricSense
      );
      MetricMatrix(const MetricMatrix & other);
      virtual ~MetricMatrix();

      void remove(size_t rowAndColumn);
      void reduce(const boost::optional<Metric> & threshold);

      inline size_t size() const {return m_size;}
      Metric at(size_t row, size_t column) const;

      size_t originalIndex(size_t rowOrColumn) const;
      size_t tailSize() const;
      const size_t * tail() const;

      roommates::Matrix<size_t> roommatesMatrix() const;
      std::pair<size_t, size_t> globalExtremum() const;

   private:
      template <typename T>
      static void moveToEnd(size_t index, T * array, size_t size);

      class Comparator
      {
         public:
            explicit Comparator(const MetricMatrix & matrix, size_t row);
            bool operator()(size_t lhs, size_t rhs) const;

         private:
            const MetricMatrix & m_matrix;
            size_t m_row;
      };

      METRIC_SENSE m_metricSense;
      size_t m_originalSize;
      size_t m_size;
      Metric m_diagonal; // All diagonal items are considered to be the same;
      Metric ** m_data; // Original size - 1;
      size_t *m_proxy; // Original size;
};


template <typename Metric>
template <typename T, typename SymmetricCompare>
MetricMatrix<Metric>::MetricMatrix(
   const std::vector<T> & data,
   SymmetricCompare compare,
   METRIC_SENSE metricSense
) : m_metricSense(metricSense),
   m_originalSize(data.size()),
   m_size(data.size()),
   m_data(0),
   m_proxy(0)
{
   if (m_size > 1)
   {
      m_data = new Metric *[m_size - 1];
      for (size_t row = 1; row < m_size; ++row)
      {
         Metric * rowMetrics = new Metric[row];
         for (size_t column = 0; column < row; ++column)
         {
            rowMetrics[column] = compare(data[row], data[column]);
         }
         m_data[row - 1] = rowMetrics;
      }
   }

   if (m_size)
   {
      m_diagonal = compare(data[0], data[0]);
      m_proxy = new size_t[m_size];
      for (size_t i = 0; i < m_size; ++i)
      {
         m_proxy[i] = i;
      }
   }
}


template <typename Metric>
MetricMatrix<Metric>::MetricMatrix(const MetricMatrix<Metric> & other)
   : m_metricSense(other.m_metricSense),
   m_originalSize(other.m_originalSize),
   m_size(other.m_size),
   m_diagonal(other.m_diagonal),
   m_data(0),
   m_proxy(0)
{
   if (m_originalSize > 1)
   {
      m_data = new Metric *[m_originalSize - 1];
      for (size_t row = 1; row < m_originalSize; ++row)
      {
         Metric * otherRowMetrics = other.m_data[row - 1];
         Metric * rowMetrics = new Metric[row];
         for (size_t column = 0; column < row; ++column)
         {
            rowMetrics[column] = otherRowMetrics[column];
         }
         m_data[row - 1] = rowMetrics;
      }
   }

   if (m_originalSize)
   {
      m_proxy = new size_t[m_originalSize];
      for (size_t i = 0; i < m_originalSize; ++i)
      {
         m_proxy[i] = other.m_proxy[i];
      }
   }
}


template <typename Metric>
MetricMatrix<Metric>::~MetricMatrix()
{
   if (m_originalSize > 1)
   {
      for (size_t i = 0, count = m_originalSize - 1; i < count; ++i)
      {
         delete[] m_data[i];
      }
   }
   delete[] m_data;
   delete[] m_proxy;
}


template <typename Metric>
void MetricMatrix<Metric>::remove(size_t rowAndColumn)
{
   if (m_size && (rowAndColumn < m_size))
   {
      moveToEnd(rowAndColumn, m_proxy, m_size);
      --m_size;
   }
}


template <typename Metric>
void MetricMatrix<Metric>::reduce(const boost::optional<Metric> & threshold)
{
   const bool max = (m_metricSense == MS_MORE_METRIC_MORE_ALIKE);
   if (m_size > 1)
   {
      // Find extremum for all rows (except diagonal elements);
      Metric *extremum = new Metric[m_size];
      for (size_t row = 0; row < m_size; ++row)
      {
         bool init = false;
         Metric & ext = extremum[row];
         ext = m_diagonal;
         for (size_t column = 0; column < m_size; ++column)
         {
            if (column != row)
            {
               const Metric m = at(row, column);
               if (!init || (max && ext < m) || (!max && ext > m))
               {
                  init = true;
                  ext = m;
               }
            }
         }
      }

      if (threshold)
      {
         // Remove as much unsuitable elements as possible;
         boost::optional<size_t> next;
         for (size_t i = m_size; i > 0; --i) // Backward loop;
         {
            const int row = i - 1;
            const Metric & ext = extremum[row];
            if (!doesMetricPassThreshold(ext, threshold, m_metricSense))
            {
               if (m_size % 2)
               {
                  moveToEnd(row, extremum, m_size);
                  remove(row);
               }
               else if (next)
               {
                  moveToEnd(*next, extremum, m_size);
                  remove(*next);
                  moveToEnd(row, extremum, m_size);
                  remove(row);
                  next = boost::none;
               }
               else
               {
                  next = row;
               }
            }
         }
      }

      if (m_size % 2)
      {
         // Remove worst element in case of odd matrix size;
         // TODO: May be the worst is an element with the worst metric sum for
         // the row and not the one with the worst extremum;
         size_t extRow = 0;
         for (size_t row = 1; row < m_size; ++row)
         {
            if ((max && extremum[extRow] > extremum[row]) || // minimum;
               (!max && extremum[extRow] < extremum[row])) // maximum;
            {
               extRow = row;
            }
         }
         remove(extRow);
      }

      delete[] extremum;
   }
   else if (m_size == 1)
   {
      remove(0);
   }
   assert(!(m_size % 2));
}


template <typename Metric>
Metric MetricMatrix<Metric>::at(size_t row, size_t column) const
{
   if (row < m_size && column < m_size)
   {
      const size_t i = m_proxy[row];
      const size_t j = m_proxy[column];
      if (i > j)
      {
         return m_data[i - 1][j];
      }
      else if (i < j)
      {
         return m_data[j - 1][i];
      }
   }
   return m_diagonal;
}


template <typename Metric>
size_t MetricMatrix<Metric>::originalIndex(size_t rowOrColumn) const
{
   assert(rowOrColumn < m_size);
   return m_proxy[rowOrColumn];
}


template <typename Metric>
size_t MetricMatrix<Metric>::tailSize() const
{
   return m_originalSize - m_size;
}


template <typename Metric>
const size_t * MetricMatrix<Metric>::tail() const
{
   return ((m_size < m_originalSize) ? (m_proxy + m_size) : 0);
}


template <typename Metric>
roommates::Matrix<size_t> MetricMatrix<Metric>::roommatesMatrix() const
{
   roommates::Matrix<size_t> rm(m_size);
   for (size_t i = 0; i < m_size; ++i)
   {
      size_t *row = rm[i];
      const size_t last = m_size - 1;
      for (size_t j = 0; j < last; ++j)
      {
         row[j] = ((j != i) ? j : last);
      }
      row[last] = i;
      std::sort(row, row + last, Comparator(*this, i));
   }
   return rm;
}


template <typename Metric>
std::pair<size_t, size_t> MetricMatrix<Metric>::globalExtremum() const
{
   const bool max = (m_metricSense == MS_MORE_METRIC_MORE_ALIKE);
   std::pair<size_t, size_t> result(0, 0);
   Metric extremum = m_diagonal;
   bool init = false;
   for (size_t row = 1; row < m_size; ++row)
   {
      for (size_t column = 0; column < row; ++column)
      {
         const Metric m = at(row, column);
         if (!init || (max && extremum < m) || (!max && extremum > m))
         {
            init = true;
            result.first = row;
            result.second = column;
            extremum = m;
         }
      }
   }
   return result;
}


template <typename Metric>
template <typename T>
void MetricMatrix<Metric>::moveToEnd(size_t index, T * array, size_t size)
{
   if (size && (index < size))
   {
      size_t newSize = size - 1;
      size_t tmp = array[index];
      for (size_t i = index; i < newSize; ++i)
      {
         array[i] = array[i + 1];
      }
      array[newSize] = tmp;
   }
}


template <typename Metric>
MetricMatrix<Metric>::Comparator::Comparator(
   const MetricMatrix<Metric> & matrix,
   size_t row
) : m_matrix(matrix),
   m_row(row)
{
}


template <typename Metric>
bool MetricMatrix<Metric>::Comparator::operator()(
   size_t lhs,
   size_t rhs
) const
{
   if (m_matrix.m_metricSense == MS_LESS_METRIC_MORE_ALIKE)
   {
      return m_matrix.at(m_row, lhs) < m_matrix.at(m_row, rhs);
   }
   return m_matrix.at(m_row, lhs) > m_matrix.at(m_row, rhs);
}


template <typename Metric>
std::ostream & operator<<(std::ostream & os, const MetricMatrix<Metric> & mm)
{
   const size_t size = mm.size();
   for (size_t i = 0; i < size; ++i)
   {
      if (i)
      {
         os << std::endl;
      }
      for (size_t j = 0; j < size; ++j)
      {
         if (j)
         {
            os << ", ";
         }
         os << mm.at(i, j);
      }
   }

   const size_t tailSize = mm.tailSize();
   const size_t * tail = mm.tail();
   if (tail && tailSize)
   {
      os << std::endl << "tail = {";
      for (size_t i = 0; i < tailSize; ++i)
      {
         if (i)
         {
            os << ", ";
         }
         os << tail[i];
      }
      os << "}";
   }
   return os;
}


template <typename Metric, typename T, typename SymmetricCompare>
std::vector<Pair<Metric> > resolve(
   const std::vector<T> & data,
   SymmetricCompare compare,
   METRIC_SENSE metricSense,
   const boost::optional<Metric> & threshold = boost::none)
{
   size_t usedItems = 0;

   MetricMatrix<Metric> matrix(data, compare, metricSense);
   matrix.reduce(threshold);
   assert(!(matrix.size() % 2));

   const size_t * tail = matrix.tail();
   const size_t tailSize = matrix.tailSize();

   std::vector<Pair<Metric> > pairs;
   pairs.reserve(data.size());
   roommates::Matrix<size_t> rmMatrix = std::move(matrix.roommatesMatrix());
   std::vector<size_t> rmPartners;
   if (roommates::resolve(rmMatrix, rmPartners))
   {
      std::set<size_t> paired;
      assert(rmPartners.size() == matrix.size());
      for (size_t i = 0, count = rmPartners.size(); i < count; ++i)
      {
         size_t left = matrix.originalIndex(i);
         size_t right = matrix.originalIndex(rmPartners[i]);
         if (paired.insert(left).second && paired.insert(right).second)
         {
            if (left > right)
            {
               std::swap(left, right);
            }
            const Metric metric = matrix.at(i, rmPartners[i]);
            if (doesMetricPassThreshold(metric, threshold, metricSense))
            {
               pairs.push_back(Pair<Metric>(left, right, metric));
               usedItems += 2;
            }
            else
            {
               pairs.push_back(Pair<Metric>(left));
               pairs.push_back(Pair<Metric>(right));
               usedItems += 2;
            }
         }
      }
   }
   else
   {
      // Greedy algorithm;
      while (matrix.size())
      {
         // Find global extremum (except diagonal elements);
         std::pair<size_t, size_t> ext = matrix.globalExtremum();
         assert(ext.first != ext.second);
         if (ext.first > ext.second)
         {
            matrix.remove(ext.first);
            matrix.remove(ext.second);
         }
         else if (ext.first < ext.second)
         {
            matrix.remove(ext.second);
            matrix.remove(ext.first);
         }
         else // Impossible case;
         {
            matrix.remove(ext.first);
         }
         size_t left = matrix.originalIndex(ext.first);
         size_t right = matrix.originalIndex(ext.second);
         if (left > right)
         {
            std::swap(left, right);
         }
         const Metric metric = matrix.at(ext.first, ext.second);
         if (doesMetricPassThreshold(metric, threshold, metricSense))
         {
            pairs.push_back(Pair<Metric>(left, right, metric));
            usedItems += 2;
         }
         else
         {
            pairs.push_back(Pair<Metric>(left));
            pairs.push_back(Pair<Metric>(right));
            usedItems += 2;
         }
      }
   }

   for (size_t i = 0; i < tailSize; ++i)
   {
      pairs.push_back(Pair<Metric>(tail[i]));
      ++usedItems;
   }

   assert(usedItems == data.size());
   pairs.shrink_to_fit();
   std::sort(pairs.begin(), pairs.end());
   return pairs;
}


} // namespace pairing;


} // namespace algo;


#endif
