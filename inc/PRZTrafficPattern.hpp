//
// Copyright (C) 1998-2001 by ATC Group, the University of 
//    Cantabria, Spain.
// 
// This file is part of the Sicosys distribution. 
// See LICENSE file for terms of the license. 
//
//
//*************************************************************************
//
// File: ../ATCSimul/inc/PRZTrafficPattern.hpp
//
// Description:  fichero de encabezado de la clase "PRZTrafficPattern"
//
// Notes:  
//
// $Author: vpuente $ 
//
// $RCSfile: PRZTrafficPattern.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZTrafficPattern_HPP__
#define __PRZTrafficPattern_HPP__

//************************************************************************

   #include <stdlib.h>

   #ifndef __PRZRunTimeInformation_HPP__
   #include <PRZRunTimeInformation.hpp>
   #endif

   #ifndef __PRZString_HPP__
   #include <PRZString.hpp>
   #endif

   #ifndef __PRZMessage_HPP__
   #include <PRZMessage.hpp>
   #endif

   #ifndef __PRZSimulation_HPP__
   #include <PRZSimulation.hpp>
   #endif

   #ifndef __PRZArray_HPP__
   #include <PRZArray.hpp>
   #endif

//************************************************************************

   class PRZTag;
   class PRZPosition;
   class PRZNetwork;
   class ifstream;
      
//************************************************************************

   class PRZTrafficPattern
   {      
   public:   
      PRZTrafficPattern(PRZSimulation& simul);
      virtual ~PRZTrafficPattern() { }
      void generateMessages(double q);
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0)) = 0;
      
      virtual double  getInjectFactor(const PRZPosition& pos)
      { return 1.0; }
      
      virtual double getClock()
      {return 0;}
            
      virtual PRZString asString() const 
      { return (isBimodal() ? "Bimodal " : ""); }
      
      void setParameter1(double param)
      { m_Parameter1 = param; }

      void setParameter2(double param)
      { m_Parameter2 = param; }

      void setParameter3(double param)
      { m_Parameter3 = param; }

      double getParameter1() const
      { return m_Parameter1; }

      double getParameter2() const
      { return m_Parameter2; }

      double getParameter3() const
      { return m_Parameter3; }

      Boolean isFromFile() const
      { return m_IsFile; }

      Boolean isBimodal() const
      { return m_Bimodal; }

      virtual Boolean isEmpty() const
      { return false; }
      
      void setProbBimodal(double prob)
      { m_ProbBimodal = prob; }

      void setMessagesBimodal(double nMsg)
      { m_NumMsgBimodal = nMsg; }

      double getProbBimodal() const
      { return m_ProbBimodal; }
      
      double getMessagesBimodal() const
      { return m_NumMsgBimodal; }
      
      void setBimodal()
      { m_Bimodal = true; }

      static PRZTrafficPattern* createTrafficPattern(const PRZTag& tag,
                                                     PRZSimulation& simul);

      static PRZTrafficPattern* createTrafficPattern(const PRZString& fileName,
                                                     PRZSimulation& simul);

      // Run time information
      DEFINE_RTTI(PRZTrafficPattern);

   protected:
      PRZMessage* generateBasicMessage(const PRZPosition& source);
   
      virtual void initialize()
      { }
      
      PRZSimulation& getSimulation() const
      { return m_Simulation; }
            
      void setFromFile(Boolean val=true)
      { m_IsFile = val; }
      
      unsigned getX() const
      { return m_SizeX; }

      unsigned getY() const
      { return m_SizeY; }

      unsigned getZ() const
      { return m_SizeZ; }

      unsigned numberOfNodes() const
      { return m_TotalNodes; }

      
      //Soporte para los traficos virtuales de la midimew
      unsigned MidimewIndex(unsigned posicion)const;
		unsigned InvMidimewIndex(unsigned posicion)const;
      
   private:
      double   m_Parameter1, m_Parameter2, m_Parameter3;
      double   m_ProbBimodal;
      double   m_NumMsgBimodal;  //Bimodal corto-largo y largo-corto
      unsigned m_SizeX;
      unsigned m_SizeY;
      unsigned m_SizeZ;
      unsigned m_TotalNodes;
      Boolean  m_Bimodal;
      Boolean  m_IsFile;
      PRZSimulation& m_Simulation;
   };

//************************************************************************

   class PRZTrafficPatternEmpty : public PRZTrafficPattern
   {
      typedef PRZTrafficPattern Inhereited;
   public:
      PRZTrafficPatternEmpty(PRZSimulation& s) : PRZTrafficPattern(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      virtual Boolean isEmpty() const
      { return true; }
      virtual PRZString asString() const 
      { return Inhereited::asString() + "Empty"; }

      DEFINE_RTTI(PRZTrafficPatternEmpty);
   
   };

//************************************************************************

   class PRZTrafficPatternRandom : public PRZTrafficPattern
   {
      typedef PRZTrafficPattern Inhereited;
   public:
      PRZTrafficPatternRandom(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      virtual PRZString asString() const 
      { return Inhereited::asString() + "Random"; }
      DEFINE_RTTI(PRZTrafficPatternRandom);
   
   };


      //************************************************************************

   class PRZTrafficPatternRandomCP : public PRZTrafficPattern
   {
      typedef PRZTrafficPattern Inhereited;
   public:
      PRZTrafficPatternRandomCP(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      virtual PRZString asString() const 
      { return Inhereited::asString() + "Random Crossbar Performance"; }
      DEFINE_RTTI(PRZTrafficPatternRandomCP);
   
   };

//************************************************************************

   class PRZTrafficPatternRandom3D : public PRZTrafficPatternRandom
   {
      typedef PRZTrafficPatternRandom Inhereited;
   public:
      PRZTrafficPatternRandom3D(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      DEFINE_RTTI(PRZTrafficPatternRandom3D);   
   };
//************************************************************************

   class PRZTrafficPatternRandomCP3D : public PRZTrafficPatternRandomCP
   {
      typedef PRZTrafficPatternRandomCP Inhereited;
   public:
      PRZTrafficPatternRandomCP3D(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      DEFINE_RTTI(PRZTrafficPatternRandomCP3D);   
   };
//************************************************************************

   class PRZTrafficPatternPermutation : public PRZTrafficPattern
   {
      typedef PRZTrafficPattern Inhereited;
   public:
      PRZTrafficPatternPermutation(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      virtual PRZString asString() const 
      { return Inhereited::asString() + "Transpose matrix"; }
      DEFINE_RTTI(PRZTrafficPatternPermutation);
   
   };
//************************************************************************

   class PRZTrafficPatternPermutationVirtual : public PRZTrafficPattern
   {
      typedef PRZTrafficPattern Inhereited;
   public:
      PRZTrafficPatternPermutationVirtual(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      virtual PRZString asString() const 
      { return Inhereited::asString() + "Transpose matrix virtual mapping (only MDW networks!)"; }
      DEFINE_RTTI(PRZTrafficPatternPermutationVirtual);
   
   };
//************************************************************************

   class PRZTrafficPatternPermutation3D : public PRZTrafficPatternPermutation
   {
      typedef PRZTrafficPatternPermutation Inhereited;
   public:
      PRZTrafficPatternPermutation3D(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      DEFINE_RTTI(PRZTrafficPatternPermutation3D);
   };

//************************************************************************

   class PRZTrafficPatternPerfectShuffle : public PRZTrafficPattern
   {
      typedef PRZTrafficPattern Inhereited;
   public:
      PRZTrafficPatternPerfectShuffle(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      virtual PRZString asString() const
      { return Inhereited::asString() + "Perfect Shuffle"; }
      DEFINE_RTTI(PRZTrafficPatternPerfectShuffle);
   
   };

//************************************************************************

   class PRZTrafficPatternPerfectShuffleVirtual : public PRZTrafficPattern
   {
      typedef PRZTrafficPattern Inhereited;
   public:
      PRZTrafficPatternPerfectShuffleVirtual(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      virtual PRZString asString() const
      { return Inhereited::asString() + "Perfect Shuffle virtual mapping (only MDW networks!)"; }
      DEFINE_RTTI(PRZTrafficPatternPerfectShuffleVirtual);
   
   };

//************************************************************************

   class PRZTrafficPatternPerfectShuffle3D : public PRZTrafficPatternPerfectShuffle
   {
      typedef PRZTrafficPatternPerfectShuffle Inhereited;
   public:
      PRZTrafficPatternPerfectShuffle3D(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      DEFINE_RTTI(PRZTrafficPatternPerfectShuffle3D);
   
   };

//************************************************************************

   class PRZTrafficPatternBitReversal : public PRZTrafficPattern
   {
      typedef PRZTrafficPattern Inhereited;
   public:
      PRZTrafficPatternBitReversal(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      virtual PRZString asString() const
      { return Inhereited::asString() + "Bit reversal"; }
      DEFINE_RTTI(PRZTrafficPatternBitReversal);
   
   };
//************************************************************************

   class PRZTrafficPatternBitReversalVirtual : public PRZTrafficPattern
   {
      typedef PRZTrafficPattern Inhereited;
   public:
      PRZTrafficPatternBitReversalVirtual(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      virtual PRZString asString() const
      { return Inhereited::asString() + "Bit reversal virtual mapping (only MDW networks!)"; }
      DEFINE_RTTI(PRZTrafficPatternBitReversalVirtual);
   
   };
//************************************************************************

   class PRZTrafficPatternBitReversal3D : public PRZTrafficPatternBitReversal
   {
      typedef PRZTrafficPatternBitReversal Inhereited;
   public:
      PRZTrafficPatternBitReversal3D(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      DEFINE_RTTI(PRZTrafficPatternBitReversal3D);   
   };

//************************************************************************

   class PRZTrafficPatternLocal : public PRZTrafficPattern
   {
      typedef PRZTrafficPattern Inhereited;   
   public:
      PRZTrafficPatternLocal(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      virtual PRZString asString() const 
      { return Inhereited::asString() + "Local"; }
      DEFINE_RTTI(PRZTrafficPatternLocal);
   
   };

//************************************************************************

   class PRZTrafficPatternLocal3D : public PRZTrafficPatternLocal
   {
      typedef PRZTrafficPatternLocal Inhereited;   
   public:
      PRZTrafficPatternLocal3D(PRZSimulation& s) : Inhereited(s) { }
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      DEFINE_RTTI(PRZTrafficPatternLocal);
   
   };

//************************************************************************

   class PRZTrafficPatternHotSpot : public PRZTrafficPattern
   {
      typedef PRZTrafficPattern Inhereited;
      typedef PRZArray<Boolean> THotSpotList;
   public:
      PRZTrafficPatternHotSpot(PRZSimulation& s) : PRZTrafficPattern(s), 
                                                   m_HotSpotList(0)
      { }
      virtual ~PRZTrafficPatternHotSpot() 
      { delete m_HotSpotList; }
      
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      virtual double  getInjectFactor(const PRZPosition& pos);

      virtual PRZString asString() const;
      DEFINE_RTTI(PRZTrafficPatternHotSpot);
   
   protected:
      virtual void initialize();

   private:
      THotSpotList* m_HotSpotList;
      
   };

//************************************************************************

   class PRZTrafficPatternFlash : public PRZTrafficPattern
   {
      typedef PRZTrafficPattern Inhereited;
      typedef PRZArray<int> TFlashStateList;

   public:
      PRZTrafficPatternFlash(PRZSimulation& s) : PRZTrafficPattern(s),
                                                 m_StateList(0)
      { }
      virtual ~PRZTrafficPatternFlash() 
      { delete m_StateList; }

      virtual Boolean   injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      virtual double    getInjectFactor(const PRZPosition& pos);
      virtual PRZString asString() const;
      DEFINE_RTTI(PRZTrafficPatternFlash);

   private:
      TFlashStateList* m_StateList;
   };

//************************************************************************

   class PRZTrafficPatternFile : public PRZTrafficPattern
   {
      typedef PRZTrafficPattern Inhereited;   
   public:
      PRZTrafficPatternFile(const PRZString& fileName, PRZSimulation& s);
      virtual Boolean injectMessage(const PRZPosition& src=PRZPosition(0,0,0));
      virtual PRZString asString() const 
      { return PRZString("File=") + m_FileName; }
      DEFINE_RTTI(PRZTrafficPatternFile);
   private:
      Boolean readNextMessage();
      PRZMessage m_Message;
      PRZString  m_FileName;
      ifstream*  m_File;
   };

      
//************************************************************************


#endif


// fin del fichero
