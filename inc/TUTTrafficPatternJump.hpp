//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
#ifndef __TUTTrafficPatternJump_HPP__
#define __TUTTrafficPatternJump_HPP__

#include <PRZConst.hpp>

#include <PRZTrafficPattern.hpp>

class TUTTrafficPatternJump : public PRZTrafficPattern
{
   /* Simple alias for the parent class */
   typedef PRZTrafficPattern Inhereited;
public:
   /* Make the class not abstract */
   TUTTrafficPatternJump(PRZSimulation& s) : Inhereited(s) { }
   /* The function that generates the messages */
   virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
   /* Identification */
   virtual PRZString asString() const 
   { return Inhereited::asString() + "Tutorial Jump"; }
   /*  Define Runtime Information */
   DEFINE_RTTI(TUTTrafficPatternJump);
   /* Set traffic pattern private data */
   void setJump(int x, int y, int z) { dx=x; dy=y; dz=z; }
private:
   /* Data needed by the traffic pattern */
   int dx,dy,dz;
};

#endif
