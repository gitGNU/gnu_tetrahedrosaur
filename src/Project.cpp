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


#include <cstdint>


#include <QtCore/QFile>


#include "custom_enums.hpp"
#include "GuiOrganismDesc.hpp"
#include "Project.hpp"


#include "bio/Config.hpp"
#include "bio/Genome.hpp"
#include "bio/InstructionSet.hpp"
#include "bio/OrganismDesc.hpp"


namespace {


#define _INITIALCONDITIONS_MAGIC_NUMBER 0x54494e49
#define _CHROMOSOME_MAGIC_NUMBER 0x454d4843
#define _ORGANISM_DESC_MAGIC_NUMBER 0x4e47524f
#define _PROJECT_MAGIC_NUMBER 0x374b4c45
#define _PROJECT_VERSION 4


#pragma pack(push)
#pragma pack(1)


struct _InitialConditionsHeader
{
   explicit _InitialConditionsHeader()
      : magicNumber(0), cellLimit(0), x(0), y(0)
   {}
   explicit _InitialConditionsHeader(uint16_t cellLimit, int16_t x, int16_t y)
      : magicNumber(_INITIALCONDITIONS_MAGIC_NUMBER),
      cellLimit(cellLimit),
      x(x),
      y(y)
   {}
   uint32_t magicNumber;
   uint16_t cellLimit;
   int16_t x;
   int16_t y;
};


struct _ChromosomeHeader
{
   explicit _ChromosomeHeader() : magicNumber(0), count(0) {}
   explicit _ChromosomeHeader(uint32_t count)
      : magicNumber(_CHROMOSOME_MAGIC_NUMBER),
      count(count)
   {}
   uint32_t magicNumber;
   uint32_t count;
};


struct _OrganismDescHeader
{
   explicit _OrganismDescHeader() : magicNumber(0), chromosomeCount(0) {}
   explicit _OrganismDescHeader(uint32_t chromosomeCount)
      : magicNumber(_ORGANISM_DESC_MAGIC_NUMBER),
      chromosomeCount(chromosomeCount)
   {}
   uint32_t magicNumber;
   uint32_t chromosomeCount;
};


struct _ProjectHeader
{
   explicit _ProjectHeader()
      : magicNumber(0),
      version(0),
      nameSize(0),
      populationSize(0)
   {}
   explicit _ProjectHeader(uint32_t populationSize)
      : magicNumber(_PROJECT_MAGIC_NUMBER),
      version(_PROJECT_VERSION),
      nameSize(0),
      populationSize(populationSize)
   {}
   uint32_t magicNumber;
   uint16_t version;
   uint16_t nameSize;
   uint32_t populationSize;
};


#pragma pack(pop)


bool _readInitialConditions(QIODevice & file, bio::InitialConditions & ic)
{
   // Read header;
   _InitialConditionsHeader hdr;
   qint64 size = file.read(reinterpret_cast<char *>(&hdr), sizeof(hdr));
   if (size != sizeof(hdr) ||
      hdr.magicNumber != _INITIALCONDITIONS_MAGIC_NUMBER
   )
   {
      return false;
   }

   ic.cellLimit = hdr.cellLimit;
   ic.x = hdr.x;
   ic.y = hdr.y;

   return true;
}


bool _writeInitialConditions(
   QIODevice & file,
   const bio::InitialConditions & ic
)
{
   // Write header;
   _InitialConditionsHeader header(ic.cellLimit, ic.x, ic.y);
   if (file.write(reinterpret_cast<const char *>(&header), sizeof(header)) !=
      sizeof(header)
   )
   {
      return false;
   }

   return true;
}


bool _readChromosome(QIODevice & file, std::vector<bio::Chromosome> & crs)
{
   // Read header;
   _ChromosomeHeader hdr;
   qint64 size = file.read(reinterpret_cast<char *>(&hdr), sizeof(hdr));
   if (size != sizeof(hdr) || hdr.magicNumber != _CHROMOSOME_MAGIC_NUMBER)
   {
      return false;
   }

   // Read instructions;
   std::vector<bio::Instruction> code(hdr.count, bio::Instruction());
   size = file.read(
      reinterpret_cast<char *>(code.data()),
      hdr.count * sizeof(bio::Instruction)
   );
   if (size != static_cast<qint64>(hdr.count * sizeof(bio::Instruction)))
   {
      return false;
   }

   crs.emplace_back(code);
   return true;
}


bool _writeChromosome(QIODevice & file, const bio::Chromosome & cr)
{
   const std::vector<bio::Instruction> & code = cr.code();

   // Write header;
   _ChromosomeHeader header(code.size());
   if (file.write(reinterpret_cast<const char *>(&header), sizeof(header)) !=
      sizeof(header)
   )
   {
      return false;
   }
   Q_ASSERT(header.count == code.size());

   // Write data;
   const char * data = reinterpret_cast<const char *>(code.data());
   const qint64 dataSize = code.size() * sizeof(bio::Instruction);
   if (file.write(data, dataSize) != dataSize)
   {
      return false;
   }

   return true;
}


boost::shared_ptr<GuiOrganismDesc> _readOrganismDesc(QIODevice & file)
{
   // Read header;
   _OrganismDescHeader hdr;
   qint64 size = file.read(reinterpret_cast<char *>(&hdr), sizeof(hdr));
   if (size != sizeof(hdr) || hdr.magicNumber != _ORGANISM_DESC_MAGIC_NUMBER)
   {
      return boost::shared_ptr<GuiOrganismDesc>();
   }

   boost::shared_ptr<GuiOrganismDesc> desc(new GuiOrganismDesc);

   // Read initial conditions;
   if (!_readInitialConditions(file, desc->initialConditions))
   {
      return boost::shared_ptr<GuiOrganismDesc>();
   }

   // Read chromosomes;
   std::vector<bio::Chromosome> chromosomes;
   chromosomes.reserve(hdr.chromosomeCount);
   for (uint32_t i = 0; i < hdr.chromosomeCount; ++i)
   {
      if (!_readChromosome(file, chromosomes))
      {
         return boost::shared_ptr<GuiOrganismDesc>();
      }
   }
   Q_ASSERT(chromosomes.size() == hdr.chromosomeCount);
   desc->genome.reset(new bio::Genome(
      boost::shared_ptr<const bio::Config>(new bio::Config()),
      chromosomes
   ));

   return desc;
}


bool _writeOrganismDesc(QIODevice & file, const bio::OrganismDesc & desc)
{
   const std::vector<bio::Chromosome> & crs = desc.genome->chromosomes();

   // Write header;
   _OrganismDescHeader header(crs.size());
   if (file.write(reinterpret_cast<const char *>(&header), sizeof(header)) !=
      sizeof(header)
   )
   {
      return false;
   }
   Q_ASSERT(header.chromosomeCount == crs.size());

   // Write initial conditions;
   if (!_writeInitialConditions(file, desc.initialConditions))
   {
      return false;
   }

   // Write chromosomes;
   for (const bio::Chromosome & cr : crs)
   {
      if (!_writeChromosome(file, cr))
      {
         return false;
      }
   }

   return true;
}


bool _writeProject(QIODevice & file, const Project & project)
{
   const std::vector<boost::shared_ptr<GuiOrganismDesc> > & population =
      project.population();
   QByteArray name = project.projectName().toUtf8();

   // Write header;
   _ProjectHeader header(population.size());
   header.nameSize = name.size();
   if (file.write(reinterpret_cast<const char *>(&header), sizeof(header)) !=
      sizeof(header)
   )
   {
      return false;
   }
   Q_ASSERT(header.populationSize == population.size());

   // Write project name;
   if (file.write(name.data(), header.nameSize) != header.nameSize)
   {
      return false;
   }

   // Write population;
   for (boost::shared_ptr<GuiOrganismDesc> desc : population)
   {
      if (!_writeOrganismDesc(file, *desc))
      {
         return false;
      }
   }

   return true;
}


} // anonymous namespace;


/***************************************************************************
 *   Project class implementation                                          *
 ***************************************************************************/


Project::~Project()
{
}


boost::shared_ptr<Project> Project::create(const QString & projectName)
{
   boost::shared_ptr<Project> project;
   if (!projectName.isEmpty())
   {
      project.reset(new Project(projectName));
   }
   return project;
}


boost::shared_ptr<Project> Project::open(const QString & fileName)
{
   boost::shared_ptr<Project> project;
   if (!fileName.isEmpty())
   {
      QFile file(fileName);
      if (file.open(QIODevice::ReadOnly))
      {
         _ProjectHeader hdr;
         qint64 size = file.read(reinterpret_cast<char *>(&hdr), sizeof(hdr));
         if (size == sizeof(hdr) && hdr.magicNumber == _PROJECT_MAGIC_NUMBER &&
            hdr.version == _PROJECT_VERSION)
         {
            QByteArray projectName = file.read(hdr.nameSize);
            if (projectName.size() == hdr.nameSize)
            {
               project.reset(new Project(QString::fromUtf8(projectName)));
               project->m_isSaved = true;
               project->m_fileName = fileName;
               project->m_population.reserve(hdr.populationSize);
               for (uint32_t i = 0; i < hdr.populationSize; ++i)
               {
                  auto desc = _readOrganismDesc(file);
                  if (!desc)
                  {
                     project.reset();
                     break;
                  }
                  project->m_population.push_back(desc);
               }
               Q_ASSERT(!project || project->m_population.size() ==
                  hdr.populationSize);
            }
         }
         file.close();
      }
   }
   return project;
}


bool Project::save()
{
   if (!m_fileName.isEmpty())
   {
      return saveAs(m_fileName);
   }
   return false;
}


bool Project::saveAs(const QString & fileName)
{
   if (!fileName.isEmpty())
   {
      QFile file(fileName);
      if (file.open(QIODevice::WriteOnly))
      {
         if (!_writeProject(file, *this))
         {
            file.close();
            return false;
         }

         file.close();
         m_isSaved = true;
         m_fileName = fileName;
         emit projectChanged();
         return true;
      }
   }
   return false;
}


const std::vector<boost::shared_ptr<GuiOrganismDesc> > & Project::population(
) const
{
   return m_population;
}


void Project::append(
   const std::vector<boost::shared_ptr<GuiOrganismDesc> > & descs
)
{
   const size_t oldSize = m_population.size();
   const size_t count = descs.size();
   if (count)
   {
      beginInsertRows(QModelIndex(), oldSize, oldSize + (count - 1));
      m_population.reserve(oldSize + count);
      m_population.insert(m_population.end(), descs.begin(), descs.end());
      endInsertRows();
      m_isSaved = false;
      emit projectChanged();
   }
}


void Project::setDescFigure(
   size_t descIndex,
   boost::shared_ptr<mesh::Figure> figure,
   QPixmap portrait
)
{
   if (descIndex < m_population.size())
   {
      m_population[descIndex]->figure = figure;
      m_population[descIndex]->portrait = portrait;
   }
}


boost::shared_ptr<const GuiOrganismDesc> Project::organismDesc(
   const QModelIndex & index
) const
{
   if (index.isValid())
   {
      const size_t row = index.row();
      if (row < m_population.size())
      {
         return m_population[row];
      }
   }
   return boost::shared_ptr<const GuiOrganismDesc>();
}


int Project::columnCount(const QModelIndex & parent) const
{
   return (parent.isValid() ? 0 : 1);
}


QVariant Project::data(const QModelIndex & index, int role) const
{
   if (index.isValid())
   {
      if (role == Qt::DecorationRole)
      {
         const size_t row = index.row();
         if (row < m_population.size())
         {
            return m_population[row]->portrait;
         }
      }
      else if (role == Custom::CloseIndicatorRole)
      {
         const size_t row = index.row();
         return (row < m_population.size());
      }
   }
   return QVariant();
}


bool Project::removeRows(
   int row,
   int count,
   const QModelIndex & parent
)
{
   if (count && !parent.isValid() && row >= 0 &&
      (row + count) <= static_cast<int>(m_population.size()))
   {
      beginRemoveRows(parent, row, row + count - 1);
      m_population.erase(
         m_population.begin() + row,
         m_population.begin() + row + count
      );
      endRemoveRows();
      m_isSaved = false;
      emit projectChanged();
      return true;
   }
   return false;
}


int Project::rowCount(const QModelIndex & parent) const
{
   return (parent.isValid() ? 0 : m_population.size());
}


Project::Project(const QString & projectName)
   : PopulationModel(Qt::Vertical),
   m_isSaved(false),
   m_projectName(projectName)
{
}
