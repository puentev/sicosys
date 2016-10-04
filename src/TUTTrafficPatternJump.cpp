//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
#include <TUTTrafficPatternJump.hpp>

IMPLEMENT_RTTI_DERIVED(TUTTrafficPatternJump,PRZTrafficPattern);

Boolean TUTTrafficPatternJump :: injectMessage(const PRZPosition& source)
{
   /* Create the message with source node */
   PRZMessage* msg = generateBasicMessage(source);
   
   /* Calculate target node */
   unsigned x, y, z;
   x = (source.valueForCoordinate(PRZPosition::X) + getX() + dx ) % getX();
   y = (source.valueForCoordinate(PRZPosition::Y) + getY() + dy ) % getY();
   z = (source.valueForCoordinate(PRZPosition::Z) + getZ() + dz ) % getZ();
   PRZPosition destiny = PRZPosition(x,y,z);
   /* Set target node in message */
   msg->setDestiny(destiny);
   /* Send message */
   getSimulation().getNetwork()->sendMessage(msg);

   return true;
}
