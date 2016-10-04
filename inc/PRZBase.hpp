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
// File: ../ATCSimul/inc/PRZBase.hpp
//
// Description:  fichero de encabezado de la clase "PRZBase"
//
// Notes:   La clase PRZBase es el root de la jerarquia de herencia
//               de la mayor parte de las clases en la PRZApplication.
//
// $Author: vpuente $ 
//
// $RCSfile: PRZBase.hpp,v $
//
// $Date: 2001/10/08 05:43:07 $
//
// $Revision: 1.1.1.1 $
//
//*************************************************************************

#ifndef __PRZBase_HPP__
#define __PRZBase_HPP__

//************************************************************************

   #ifndef __PRZRunTimeInformation_HPP__
   #include <PRZRunTimeInformation.hpp>
   #endif

   #ifndef __PRZError_H__
   #include <PRZError.hpp>
   #endif

   #ifndef __PRZString_HPP__
   #include <PRZString.hpp>
   #endif

   #include <iostream.h>

//************************************************************************

   typedef unsigned long uTIME;
   typedef unsigned Boolean;
   typedef unsigned int INDEX;
   typedef enum{ _Unknow_ = 0,
                 _Xplus_,
                 _Xminus_,
                 _Yplus_,
                 _Yminus_,
                 _LocalNode_,
                 _Zplus_,
                 _Zminus_
               } przROUTINGTYPE;

   enum { CHANNEL_X=1, CHANNEL_Y, CHANNEL_Z, CHANNEL_N };
   
//************************************************************************

   #ifdef true
      #undef true
   #endif
   
   #ifdef false
      #undef false
   #endif

   #define true  1
   #define false 0
   
//************************************************************************

   przROUTINGTYPE _string2routingType(const PRZString& string);
   PRZString      _routingType2string(przROUTINGTYPE type);
   
//************************************************************************

   class PRZBase
   {
   public:
      PRZBase();
      PRZBase(const PRZBase& aBase);
      virtual ~PRZBase();

      static void printError(const PRZString& errorText);
      static void exitProgram(int exitCode = 0);

      static void setErrorLocation(const PRZString& loc)
      { sm_ErrorLocation = loc; }

      static void setErrorText(const PRZString& text)
      { sm_ErrorText = text; }

      static PRZString errorText()
      { return sm_ErrorText; }

      static PRZString errorLocation()
      { return sm_ErrorLocation; }


      // Run time information
      DEFINE_RTTI(PRZBase);

   private:
      static PRZString sm_ErrorLocation;
      static PRZString sm_ErrorText;
   };


//************************************************************************
//
//  Se definen una serie de macros para facilitar la traza de los errores:
//
//   THROW_ERROR(texto,rc) : Pone "texto" en la traza y retorna "rc"
//   THROW_BERROR(text)    : Retorno Boolean "false"
//   THROW_PERROR(TEXT)    : Retorno Pointer NULL
//   EXIT_PROGRAM_ERROR(c) : Sale del programa con el codigo de error "c"
//
//************************************************************************

   #include <stdlib.h>   // exit()

   #define THROW_ERROR(text,rc) { PRZBase::setErrorLocation(       \
                                      PRZString(" File=     ") +        \
                                      PRZString(__FILE__)+         \
                                      PRZString("\n Line=     ") +     \
                                      PRZString(__LINE__) );       \
                                  PRZBase::setErrorText(text);     \
                                  return rc; }

   #define THROW_BERROR(text)  THROW_ERROR(text,false)
   #define THROW_PERROR(text)  THROW_ERROR(text,0L)

   #define EXIT_PROGRAM_ERROR(code) { cerr << PRZBase::errorLocation() << endl;  \
                                      cerr << " Text=     " << PRZBase::errorText() << endl;\
                                      exit(code); }

   #define PRZTRAZA(text) cout << " File=     "    <<  \
                                    __FILE__        << \
                                    "\n Line=     " << \
                                    __LINE__        << \
                                    "\n Text=     " << \
                                    text << endl

   #define EXIT_PROGRAM(text) { cerr << " File=     "    << \
                                         __FILE__        << \
                                         "\n Line=     " << \
                                         __LINE__        << \
                                         "\n Text=     " << \
                                         text << endl;      \
                                exit(-1); }

   #define EXIT(text) { cerr << text << endl; \
                        exit(-1); }


   #define MINIMO(x,y) (((x)<(y)) ? (x) : (y) )
   #define MAXIMO(x,y) (((x)>(y)) ? (x) : (y) )


#endif


// fin del fichero
