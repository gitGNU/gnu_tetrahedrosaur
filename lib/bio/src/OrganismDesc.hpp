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


#ifndef BIO_ORGANISMDESC_HPP
#define BIO_ORGANISMDESC_HPP


#include <ostream>


#include <boost/shared_ptr.hpp>


#include "InitialConditions.hpp"


namespace bio {


class Genome;


struct OrganismDesc
{
   explicit OrganismDesc();
   virtual ~OrganismDesc();

   InitialConditions initialConditions;
   boost::shared_ptr<const bio::Genome> genome;
};


std::ostream & operator<<(std::ostream & os, const OrganismDesc & desc);


}


#endif
