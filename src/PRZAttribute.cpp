//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
//*************************************************************************
//:
// File: ../ATCSimul/src/PRZAttribute.cpp
//
// Clase      : PRZAttribute
//
// Hereda de  : None
//
// Notes:  Utilizada como clase auxiliar de la clase PRZTag, sirve
//              para encapsular dos PRZString, uno representa el nombre
//              del campo, y el otro el valor asociado.
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZAttribute.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Id: PRZAttribute.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";

#include <PRZAttribute.hpp>

#include <iostream.h>

//*************************************************************************

IMPLEMENT_RTTI_BASE(PRZAttribute);

//*************************************************************************
//:
//  f: inline const PRZString& name() const;
//  d: Retorna el nombre del campo;
//
//  f: inline const PRZString& value() const;
//  d: Retorna el valor del campo;
//:
//*************************************************************************

//*************************************************************************
//:
//  f: PRZAttribute(const PRZString& name, const PRZString& value);
//
//  d: Constructor. Se le pasa el nombre del campo y su valor;
//:
//*************************************************************************

PRZAttribute :: PRZAttribute(const PRZString& name, const PRZString& value)
              : m_Name(name),
                m_Value(value)
{

}


//*************************************************************************
//:
//  f: PRZAttribute(const PRZAttribute& attr);
//
//  d: Constructor copia;
//:
//*************************************************************************

PRZAttribute :: PRZAttribute(const PRZAttribute& attr)
              : m_Name(attr.name()),
                m_Value(attr.value())
{

}


//*************************************************************************
//:
//  f: ~PRZAttribute();
//
//  d: Destructor;
//:
//*************************************************************************

PRZAttribute :: ~PRZAttribute()
{

}


//*************************************************************************
//:
//  f: friend ostream& operator<<(ostream& os, const PRZAttribute& attr);
//
//  d: Funcion amiga para volcar sobre un "ostream" el contenido del
//     PRZAttribute;
//:
//*************************************************************************

ostream& operator<<(ostream& os, const PRZAttribute& attr)
{
   os << "< " << attr.name() << " = \"" << attr.value() << "\" >";
   return os;
}


//*************************************************************************


// fin del fichero
