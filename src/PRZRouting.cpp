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
// File: ../ATCSimul/src/PRZRouting.cpp
//
// Clase      : PRZRouting
//
// Hereda de  : PRZRunnableComponent
//
// $Author: vpuente $ 
//:
// $RCSfile: PRZRouting.cpp,v $
//
// $Date: 2001/10/08 05:43:08 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

static char rcsId[] = "$Header: /home/cvsroot/ATCSimul/src/PRZRouting.cpp,v 1.1.1.1 2001/10/08 05:43:08 vpuente Exp $";


#include <PRZRouting.hpp>
/*
#ifndef __PRZRoutingAdaptativeBurbleFlow_HPP__
#include <PRZRoutingAdaptativeBurbleFlow.hpp>
#endif
*/
#ifndef __PRZRoutingAdaptativeBurbleH1Flow_HPP__
#include <PRZRoutingAdaptativeBurbleH1Flow.hpp>
#endif

#ifndef __PRZRoutingAdaptativeBurbleH1VirtualFlow_HPP__
#include <PRZRoutingAdaptativeBurbleH1VirtualFlow.hpp>
#endif
/*
#ifndef __PRZRoutingAdaptativeBurbleH1DoubleVirtualFlow_HPP__
#include <PRZRoutingAdaptativeBurbleH1DoubleVirtualFlow.hpp>
#endif

#ifndef __PRZRoutinAdapDoubleBurbleH1Flow_HPP__
#include <PRZRoutinAdapDoubleBurbleH1Flow.hpp>
#endif

#ifndef __PRZRouting3DAdaptativeBurbleH1Flow_HPP__
#include <PRZRouting3DAdaptativeBurbleH1Flow.hpp>
#endif

#ifndef __PRZRoutingAdaptativeBurble3CvFlow_HPP__
#include <PRZRoutingAdaptativeBurble3CvFlow.hpp>
#endif
*/
#ifndef __PRZRoutingBurbleFlow_HPP__
#include <PRZRoutingBurbleFlow.hpp>
#endif

#ifndef __PRZRoutingBurbleFlowH1_HPP__
#include <PRZRoutingBurbleFlowH1.hpp>
#endif

#ifndef __PRZRoutingBurbleFlowH1Virtual_HPP__
#include <PRZRoutingBurbleFlowH1Virtual.hpp>
#endif
/*
#ifndef __PRZRoutingBurbleFlowOB_HPP__
#include <PRZRoutingBurbleFlowOB.hpp>
#endif
*/
#ifndef __PRZRoutingCVFlow_HPP__
#include <PRZRoutingCVFlow.hpp>
#endif
/*
#ifndef __PRZRoutingCV2Flow_HPP__
#include <PRZRoutingCV2Flow.hpp>
#endif

#ifndef __PRZRoutingAdaptativeCVFlow_HPP__
#include <PRZRoutingAdaptativeCVFlow.hpp>
#endif
*/
#ifndef __PRZRoutingAdaptativeBurbleFlowMR_HPP__
#include <PRZRoutingAdaptativeBurbleFlowMR.hpp>
#endif
/*
#ifndef __PRZRoutingAdaptativeBurbleFlowMRaleatorio_HPP__
#include <PRZRoutingAdaptativeBurbleFlowMRaleatorio.hpp>
#endif
*/
#ifndef __PRZRoutingAdaptativeBurbleFlowMRVirtual_HPP__
#include <PRZRoutingAdaptativeBurbleFlowMRVirtual.hpp>
#endif
/*
#ifndef __PRZRoutingAdaptativeBurbleFlowMRVirtualXY_HPP__
#include <PRZRoutingAdaptativeBurbleFlowMRVirtualXY.hpp>
#endif
*/
#ifndef __PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits_HPP__
#include <PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits.hpp>
#endif
/*
#ifndef __PRZRouting3DAdaptativeBurbleFlowMR_HPP__
#include <PRZRouting3DAdaptativeBurbleFlowMR.hpp>
#endif

#ifndef __PRZRoutingAdaptativeBurbleFlowMROB_HPP__
#include <PRZRoutingAdaptativeBurbleFlowMROB.hpp>
#endif

#ifndef __PRZRoutingAdaptativeBurbleFlow1R_HPP__
#include <PRZRoutingAdaptativeBurbleFlow1R.hpp>
#endif

#ifndef __PRZRouting3DAdaptativeBurbleFlow1R_HPP__
#include <PRZRouting3DAdaptativeBurbleFlow1R.hpp>
#endif

#ifndef __PRZRouting3DBurbleFlow_HPP__
#include <PRZRouting3DBurbleFlow.hpp>
#endif

#ifndef __PRZRouting3DCVirtualFlow_HPP__
#include <PRZRouting3DCVirtualFlow.hpp>
#endif

#ifndef __PRZRouting3DAdaptativeBurbleFlow_HPP__
#include <PRZRouting3DAdaptativeBurbleFlow.hpp>
#endif

#ifndef __PRZRouting3DAdaptativeCVFlow_HPP__
#include <PRZRouting3DAdaptativeCVFlow.hpp>
#endif
*/
#ifndef __PRZRoutingAdapCV1RequestFlow_HPP__
#include <PRZRoutingAdapCV1RequestFlow.hpp>
#endif

/*Begin irregular networks includes*/
#ifndef __PRZRoutingFlowUpdown_HPP__
#include <PRZRoutingFlowUpdown.hpp>
#endif


#ifndef __PRZRoutingFlowUpdownAdap_HPP__
#include <PRZRoutingFlowUpdownAdap.hpp>
#endif

#ifndef __PRZRoutingFlowHamilton_HPP__
#include <PRZRoutingFlowHamilton.hpp>
#endif

#ifndef __PRZRoutingFlowHamiltonAdap_HPP__
#include <PRZRoutingFlowHamiltonAdap.hpp>
#endif
/*end irregular networks includes*/

/*
#ifndef __PRZRoutingAdaptativeCVSequentialH1Flow_HPP__
#include <PRZRoutingAdaptativeCVSequentialH1Flow.hpp>
#endif

#ifndef __PRZRoutingAdapCV1RequestFlow3d_HPP__
#include <PRZRoutingAdapCV1RequestFlow3d.hpp>
#endif
*/
#ifndef __PRZRouter_HPP__
#include <PRZRouter.hpp>
#endif

#ifndef __PRZConst_H__
#include <PRZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(PRZRouting,PRZRunnableComponent);

//*************************************************************************
//:
//  f: inline PRZRoutingControl getRoutingControl() const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: PRZRouting(const PRZComponentId& id, PRZRoutingControl control);
//
//  d:
//:
//*************************************************************************

PRZRouting :: PRZRouting( const PRZComponentId& id,
                          PRZRoutingControl control )
            : PRZRunnableComponent(id),
              m_RoutingControl(control),
              m_missRouting(0)
{

}


//*************************************************************************
//:
//  f: virtual ~PRZRouting();
//
//  d:
//:
//*************************************************************************

PRZRouting :: ~PRZRouting()
{

}


//*************************************************************************
//:
//  f: virtual void buildFlowControl();
//
//  d:
//:
//*************************************************************************

void PRZRouting :: buildFlowControl()
{
   PRZString err;
   err.sprintf("PRZRouting :: %d is unimplemented in this version: please use previous versions\n", getRoutingControl() );
   if( getRoutingControl() == PRZRouting::DOR_BURBLE )
   {
      setFlowControl( new PRZRoutingBurbleFlow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::DOR_BURBLE_H1 )
   {
      setFlowControl( new PRZRoutingBurbleFlowH1(*this) );
   }
   else if( getRoutingControl() == PRZRouting::DOR_BURBLE_H1_REQ_REPLY )
   {
      setFlowControl( new PRZRoutingBurbleFlowH1Virtual(*this) );
   }
   else if( getRoutingControl() == PRZRouting::DOR_BURBLE_OB )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingBurbleFlowOB(*this) );
   }
   else if( getRoutingControl() == PRZRouting::DOR_CV )
   {
      setFlowControl( new PRZRoutingCVFlow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::DOR_CV2 )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingCV2Flow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingAdaptativeBurbleFlow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_H1 )
   {
      setFlowControl( new PRZRoutingAdaptativeBurbleH1Flow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_H1_REQUEST_REPLY )
   {
      setFlowControl( new PRZRoutingAdaptativeBurbleH1VirtualFlow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_H1_REQUEST_REPLY_FULL )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingAdaptativeBurbleH1DoubleVirtualFlow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_H1_DN )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutinAdapDoubleBurbleH1Flow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_3D_H1 )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRouting3DAdaptativeBurbleH1Flow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_MR )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingAdaptativeBurbleFlowMR(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_MR_RAND )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingAdaptativeBurbleFlowMRaleatorio(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_MR_RAND_REQ_REPLY )
   {
      setFlowControl( new PRZRoutingAdaptativeBurbleFlowMRVirtual(*this) );
   }   
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_MR_XY_REQ_REPLY )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingAdaptativeBurbleFlowMRVirtualXY(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_MR_MAXCREDITS_REQ_REPLY )
   {
      setFlowControl( new PRZRoutingAdaptativeBurbleFlowMRVirtualMaxCredits(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_MR_OB )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingAdaptativeBurbleFlowMROB(*this) );
   }
    else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_MR_3D )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRouting3DAdaptativeBurbleFlowMR(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_1R )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingAdaptativeBurbleFlow1R(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_1R_3D )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRouting3DAdaptativeBurbleFlow1R(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_3CV )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingAdaptativeBurble3CvFlow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::CV_ADAPT )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingAdaptativeCVFlow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_3D )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRouting3DBurbleFlow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::DOR_CV_3D )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRouting3DCVirtualFlow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::BURBLE_ADAPT_3D )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRouting3DAdaptativeBurbleFlow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::CV3D_ADAPT )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRouting3DAdaptativeCVFlow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::CV_ADAPT_1REQUEST )
   {
      setFlowControl( new PRZRoutingAdapCV1RequestFlow(*this) );
   }
   else if( getRoutingControl() == PRZRouting::UPDOWN )
   { 
        setFlowControl( new PRZRoutingFlowUpdown(*this) );
   }
   else if( getRoutingControl() == PRZRouting::UPDOWNADAP )
   { 
        setFlowControl( new PRZRoutingFlowUpdownAdap(*this) );
   }
   else if( getRoutingControl() == PRZRouting::HAMILTON )
   { 
        setFlowControl( new PRZRoutingFlowHamilton(*this) );
   }
   else if( getRoutingControl() == PRZRouting::HAMILTONADAP )
   { 
        setFlowControl( new PRZRoutingFlowHamiltonAdap(*this) );
   }
   else if( getRoutingControl() == PRZRouting::CV3D_ADAPT_1REQUEST )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingAdapCV1RequestFlow3d(*this) );
   }
   else if( getRoutingControl() == PRZRouting::CV_OAC )
   {
      EXIT_PROGRAM(err);
//      setFlowControl( new PRZRoutingAdaptativeCVSequentialH1Flow(*this) );
   }
   if( getFlowControl() )
   {
      getFlowControl()->initialize();
   }
   else
   {
      PRZString err;
      err.sprintf(ERR_PRZRTING_001, getRoutingControl() );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual PRZString asString() const;
//
//  d:
//:
//*************************************************************************

PRZString PRZRouting :: asString() const
{
   PRZString string = getName();
   if( getRouter() )
      string += ((PRZRouter*)getRouter())->getPosition().asString();
   return string;
}


//*************************************************************************
//:
//  f: static PRZRouting* newFrom(const PRZTag* tag, PRZComponent* owner);
//
//  d:
//:
//*************************************************************************

PRZRouting* PRZRouting :: newFrom(const PRZTag* tag, PRZComponent* owner)
{
   PRZComponentId idRouting(*tag);
   PRZString routingControl, headerDelay, dataDelay, missRouting; 
   
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_CONTROL, routingControl)) )
   {
      routingControl = ((PRZRouter*)owner)->valueForRoutingControl();
   }
   
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_HEADER_DELAY, headerDelay)) )
   {
      PRZString err;
      err.sprintf( ERR_PRZRTING_002, (char*)PRZ_TAG_HEADER_DELAY );
      EXIT_PROGRAM(err);
   }
   
   if( ! (tag->getAttributeValueWithName(PRZ_TAG_DATA_DELAY, dataDelay)) )
   {
      PRZString err;
      err.sprintf( ERR_PRZRTING_002, (char*)PRZ_TAG_DATA_DELAY );
      EXIT_PROGRAM(err);
   }

   tag->getAttributeValueWithName(PRZ_TAG_MISS_ROUTING, missRouting);
   
   
   PRZRoutingControl control = PRZRouting::NIL;
   if( routingControl == PRZ_TAG_DORBURBLE )
   {
      control = PRZRouting::DOR_BURBLE;
   }
   else if( routingControl == PRZ_TAG_DORBURBLE_H1 )
   {
      control = PRZRouting::DOR_BURBLE_H1;
   }
   else if( routingControl == PRZ_TAG_DORBURBLE_H1_VIRTUAL )
   {
      control = PRZRouting::DOR_BURBLE_H1_REQ_REPLY;
   }
   else if( routingControl == PRZ_TAG_DORBURBLE_OB )
   {
      control = PRZRouting::DOR_BURBLE_OB;
   }
   else if( routingControl == PRZ_TAG_DORCV )
   {
      control = PRZRouting::DOR_CV;
   }
   else if( routingControl == PRZ_TAG_DORCV2 )
   {
      control = PRZRouting::DOR_CV2;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP )
   {
      control = PRZRouting::BURBLE_ADAPT;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_H1 )
   {
      control = PRZRouting::BURBLE_ADAPT_H1;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_H1_REQUEST_REPLY )
   {
      control = PRZRouting::BURBLE_ADAPT_H1_REQUEST_REPLY;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_H1_REQUEST_REPLY_FULL )
   {
      control = PRZRouting::BURBLE_ADAPT_H1_REQUEST_REPLY_FULL;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_H1_DN )
   {
      control = PRZRouting::BURBLE_ADAPT_H1_DN;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_MR )
   {
      control = PRZRouting::BURBLE_ADAPT_MR;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_MR_RAND )
   {
      control = PRZRouting::BURBLE_ADAPT_MR_RAND;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_MR_RAND_REQ_REPLY )
   {
      control = PRZRouting::BURBLE_ADAPT_MR_RAND_REQ_REPLY;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_MR_XY_REQ_REPLY )
   {
      control = PRZRouting::BURBLE_ADAPT_MR_XY_REQ_REPLY;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_MAXCREDITS_RAND_REQ_REPLY )
   {
      control = PRZRouting::BURBLE_ADAPT_MR_MAXCREDITS_REQ_REPLY;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_MR_OB )
   {
      control = PRZRouting::BURBLE_ADAPT_MR_OB;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_MR_3D )
   {
      control = PRZRouting::BURBLE_ADAPT_MR_3D;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_1R )
   {
      control = PRZRouting::BURBLE_ADAPT_1R;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_1R_3D )
   {
      control = PRZRouting::BURBLE_ADAPT_1R_3D;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_3CV )
   {
      control = PRZRouting::BURBLE_ADAPT_3CV;
   }
   else if( routingControl == PRZ_TAG_CV_ADAPT )
   {
      control = PRZRouting::CV_ADAPT;
   }
   else if( routingControl == PRZ_TAG_BURBLE_3D )
   {
      control = PRZRouting::BURBLE_3D;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_3D_H1 )
   {
      control = PRZRouting::BURBLE_ADAPT_3D_H1;
   }
   else if( routingControl == PRZ_TAG_DORCV_3D )
   {
      control = PRZRouting::DOR_CV_3D;
   }
   else if( routingControl == PRZ_TAG_BURBLE_ADAP_3D )
   {
      control = PRZRouting::BURBLE_ADAPT_3D;
   }
   else if( routingControl == PRZ_TAG_CV3D_ADAPT )
   {
      control = PRZRouting::CV3D_ADAPT;
   }
   else if( routingControl == PRZ_TAG_CV_ADAPT_1REQUEST )
   {
      control = PRZRouting::CV_ADAPT_1REQUEST;
   }
   else  if( routingControl == PRZ_TAG_CV3D_ADAPT_1REQUEST )
   {
      control = PRZRouting::CV3D_ADAPT_1REQUEST;
   }
   else  if( routingControl == PRZ_TAG_CV_OAC )
   {
      control = PRZRouting::CV_OAC;
   }
   //Irregular Network case
   else  if( routingControl == PRZ_TAG_UPDOWN )
   {
      control = PRZRouting::UPDOWN;
   }
   else  if( routingControl == PRZ_TAG_UPDOWNADAP )
   {
      control = PRZRouting::UPDOWNADAP;
   }
   else  if( routingControl == PRZ_TAG_HAMILTON )
   {
      control = PRZRouting::HAMILTON;
   }
   else  if( routingControl == PRZ_TAG_HAMILTONADAP )
   {
      control = PRZRouting::HAMILTONADAP;
   }
   else
   {
      PRZString err;
      err.sprintf(ERR_PRZRTING_003, (char*)routingControl );
      EXIT_PROGRAM(err);
   }
   
   PRZRouting* routing = new PRZRouting(idRouting,control);
   
   routing->setHeaderDelay(headerDelay.asInteger());
   routing->setDataDelay(dataDelay.asInteger());
   
   if( missRouting == "" )
   {
      routing->setMissRouting(0);
   }
   else
   {
      int missesalowed = missRouting.asInteger();
      routing->setMissRouting(missesalowed);
   }
   return routing;
}
 
//*************************************************************************

// fin del fichero
