// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME srcdImyDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "myGroups.hh"
#include "myPlaHit.hh"
#include "myTrack.hh"
#include "myDCHit.hh"
#include "myDCHitPara.hh"
#include "myDCevt.hh"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_myDCHit(void *p = nullptr);
   static void *newArray_myDCHit(Long_t size, void *p);
   static void delete_myDCHit(void *p);
   static void deleteArray_myDCHit(void *p);
   static void destruct_myDCHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::myDCHit*)
   {
      ::myDCHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::myDCHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("myDCHit", ::myDCHit::Class_Version(), "myDCHit.hh", 4,
                  typeid(::myDCHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::myDCHit::Dictionary, isa_proxy, 4,
                  sizeof(::myDCHit) );
      instance.SetNew(&new_myDCHit);
      instance.SetNewArray(&newArray_myDCHit);
      instance.SetDelete(&delete_myDCHit);
      instance.SetDeleteArray(&deleteArray_myDCHit);
      instance.SetDestructor(&destruct_myDCHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::myDCHit*)
   {
      return GenerateInitInstanceLocal(static_cast<::myDCHit*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::myDCHit*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_myGroups(void *p = nullptr);
   static void *newArray_myGroups(Long_t size, void *p);
   static void delete_myGroups(void *p);
   static void deleteArray_myGroups(void *p);
   static void destruct_myGroups(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::myGroups*)
   {
      ::myGroups *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::myGroups >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("myGroups", ::myGroups::Class_Version(), "myGroups.hh", 7,
                  typeid(::myGroups), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::myGroups::Dictionary, isa_proxy, 4,
                  sizeof(::myGroups) );
      instance.SetNew(&new_myGroups);
      instance.SetNewArray(&newArray_myGroups);
      instance.SetDelete(&delete_myGroups);
      instance.SetDeleteArray(&deleteArray_myGroups);
      instance.SetDestructor(&destruct_myGroups);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::myGroups*)
   {
      return GenerateInitInstanceLocal(static_cast<::myGroups*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::myGroups*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_myPlaHit(void *p = nullptr);
   static void *newArray_myPlaHit(Long_t size, void *p);
   static void delete_myPlaHit(void *p);
   static void deleteArray_myPlaHit(void *p);
   static void destruct_myPlaHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::myPlaHit*)
   {
      ::myPlaHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::myPlaHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("myPlaHit", ::myPlaHit::Class_Version(), "myPlaHit.hh", 4,
                  typeid(::myPlaHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::myPlaHit::Dictionary, isa_proxy, 4,
                  sizeof(::myPlaHit) );
      instance.SetNew(&new_myPlaHit);
      instance.SetNewArray(&newArray_myPlaHit);
      instance.SetDelete(&delete_myPlaHit);
      instance.SetDeleteArray(&deleteArray_myPlaHit);
      instance.SetDestructor(&destruct_myPlaHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::myPlaHit*)
   {
      return GenerateInitInstanceLocal(static_cast<::myPlaHit*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::myPlaHit*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_myTrack(void *p = nullptr);
   static void *newArray_myTrack(Long_t size, void *p);
   static void delete_myTrack(void *p);
   static void deleteArray_myTrack(void *p);
   static void destruct_myTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::myTrack*)
   {
      ::myTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::myTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("myTrack", ::myTrack::Class_Version(), "myTrack.hh", 7,
                  typeid(::myTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::myTrack::Dictionary, isa_proxy, 4,
                  sizeof(::myTrack) );
      instance.SetNew(&new_myTrack);
      instance.SetNewArray(&newArray_myTrack);
      instance.SetDelete(&delete_myTrack);
      instance.SetDeleteArray(&deleteArray_myTrack);
      instance.SetDestructor(&destruct_myTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::myTrack*)
   {
      return GenerateInitInstanceLocal(static_cast<::myTrack*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::myTrack*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_myDCHitPara(void *p = nullptr);
   static void *newArray_myDCHitPara(Long_t size, void *p);
   static void delete_myDCHitPara(void *p);
   static void deleteArray_myDCHitPara(void *p);
   static void destruct_myDCHitPara(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::myDCHitPara*)
   {
      ::myDCHitPara *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::myDCHitPara >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("myDCHitPara", ::myDCHitPara::Class_Version(), "myDCHitPara.hh", 6,
                  typeid(::myDCHitPara), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::myDCHitPara::Dictionary, isa_proxy, 4,
                  sizeof(::myDCHitPara) );
      instance.SetNew(&new_myDCHitPara);
      instance.SetNewArray(&newArray_myDCHitPara);
      instance.SetDelete(&delete_myDCHitPara);
      instance.SetDeleteArray(&deleteArray_myDCHitPara);
      instance.SetDestructor(&destruct_myDCHitPara);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::myDCHitPara*)
   {
      return GenerateInitInstanceLocal(static_cast<::myDCHitPara*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::myDCHitPara*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr myDCHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *myDCHit::Class_Name()
{
   return "myDCHit";
}

//______________________________________________________________________________
const char *myDCHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::myDCHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int myDCHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::myDCHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *myDCHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::myDCHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *myDCHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::myDCHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr myGroups::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *myGroups::Class_Name()
{
   return "myGroups";
}

//______________________________________________________________________________
const char *myGroups::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::myGroups*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int myGroups::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::myGroups*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *myGroups::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::myGroups*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *myGroups::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::myGroups*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr myPlaHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *myPlaHit::Class_Name()
{
   return "myPlaHit";
}

//______________________________________________________________________________
const char *myPlaHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::myPlaHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int myPlaHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::myPlaHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *myPlaHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::myPlaHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *myPlaHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::myPlaHit*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr myTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *myTrack::Class_Name()
{
   return "myTrack";
}

//______________________________________________________________________________
const char *myTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::myTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int myTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::myTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *myTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::myTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *myTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::myTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr myDCHitPara::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *myDCHitPara::Class_Name()
{
   return "myDCHitPara";
}

//______________________________________________________________________________
const char *myDCHitPara::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::myDCHitPara*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int myDCHitPara::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::myDCHitPara*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *myDCHitPara::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::myDCHitPara*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *myDCHitPara::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::myDCHitPara*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void myDCHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class myDCHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(myDCHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(myDCHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_myDCHit(void *p) {
      return  p ? new(p) ::myDCHit : new ::myDCHit;
   }
   static void *newArray_myDCHit(Long_t nElements, void *p) {
      return p ? new(p) ::myDCHit[nElements] : new ::myDCHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_myDCHit(void *p) {
      delete (static_cast<::myDCHit*>(p));
   }
   static void deleteArray_myDCHit(void *p) {
      delete [] (static_cast<::myDCHit*>(p));
   }
   static void destruct_myDCHit(void *p) {
      typedef ::myDCHit current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::myDCHit

//______________________________________________________________________________
void myGroups::Streamer(TBuffer &R__b)
{
   // Stream an object of class myGroups.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(myGroups::Class(),this);
   } else {
      R__b.WriteClassBuffer(myGroups::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_myGroups(void *p) {
      return  p ? new(p) ::myGroups : new ::myGroups;
   }
   static void *newArray_myGroups(Long_t nElements, void *p) {
      return p ? new(p) ::myGroups[nElements] : new ::myGroups[nElements];
   }
   // Wrapper around operator delete
   static void delete_myGroups(void *p) {
      delete (static_cast<::myGroups*>(p));
   }
   static void deleteArray_myGroups(void *p) {
      delete [] (static_cast<::myGroups*>(p));
   }
   static void destruct_myGroups(void *p) {
      typedef ::myGroups current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::myGroups

//______________________________________________________________________________
void myPlaHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class myPlaHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(myPlaHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(myPlaHit::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_myPlaHit(void *p) {
      return  p ? new(p) ::myPlaHit : new ::myPlaHit;
   }
   static void *newArray_myPlaHit(Long_t nElements, void *p) {
      return p ? new(p) ::myPlaHit[nElements] : new ::myPlaHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_myPlaHit(void *p) {
      delete (static_cast<::myPlaHit*>(p));
   }
   static void deleteArray_myPlaHit(void *p) {
      delete [] (static_cast<::myPlaHit*>(p));
   }
   static void destruct_myPlaHit(void *p) {
      typedef ::myPlaHit current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::myPlaHit

//______________________________________________________________________________
void myTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class myTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(myTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(myTrack::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_myTrack(void *p) {
      return  p ? new(p) ::myTrack : new ::myTrack;
   }
   static void *newArray_myTrack(Long_t nElements, void *p) {
      return p ? new(p) ::myTrack[nElements] : new ::myTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_myTrack(void *p) {
      delete (static_cast<::myTrack*>(p));
   }
   static void deleteArray_myTrack(void *p) {
      delete [] (static_cast<::myTrack*>(p));
   }
   static void destruct_myTrack(void *p) {
      typedef ::myTrack current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::myTrack

//______________________________________________________________________________
void myDCHitPara::Streamer(TBuffer &R__b)
{
   // Stream an object of class myDCHitPara.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(myDCHitPara::Class(),this);
   } else {
      R__b.WriteClassBuffer(myDCHitPara::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_myDCHitPara(void *p) {
      return  p ? new(p) ::myDCHitPara : new ::myDCHitPara;
   }
   static void *newArray_myDCHitPara(Long_t nElements, void *p) {
      return p ? new(p) ::myDCHitPara[nElements] : new ::myDCHitPara[nElements];
   }
   // Wrapper around operator delete
   static void delete_myDCHitPara(void *p) {
      delete (static_cast<::myDCHitPara*>(p));
   }
   static void deleteArray_myDCHitPara(void *p) {
      delete [] (static_cast<::myDCHitPara*>(p));
   }
   static void destruct_myDCHitPara(void *p) {
      typedef ::myDCHitPara current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::myDCHitPara

namespace {
  void TriggerDictionaryInitialization_myDict_Impl() {
    static const char* headers[] = {
"myGroups.hh",
"myPlaHit.hh",
"myTrack.hh",
"myDCHit.hh",
"myDCHitPara.hh",
"myDCevt.hh",
nullptr
    };
    static const char* includePaths[] = {
"/home/daq/exp/anaroot_v4.6.1-install/include",
"/usr/local/root/root_v6.32.08-install/include/",
"/home/daq/exp/2605_NEOLITHs/anaroot/macros/sisir/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "myDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$myDCHit.hh")))  __attribute__((annotate("$clingAutoload$myGroups.hh")))  myDCHit;
class __attribute__((annotate("$clingAutoload$myGroups.hh")))  myGroups;
class __attribute__((annotate("$clingAutoload$myPlaHit.hh")))  myPlaHit;
class __attribute__((annotate("$clingAutoload$myTrack.hh")))  myTrack;
class __attribute__((annotate("$clingAutoload$myDCHitPara.hh")))  myDCHitPara;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "myDict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "myGroups.hh"
#include "myPlaHit.hh"
#include "myTrack.hh"
#include "myDCHit.hh"
#include "myDCHitPara.hh"
#include "myDCevt.hh"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"myDCHit", payloadCode, "@",
"myDCHitPara", payloadCode, "@",
"myGroups", payloadCode, "@",
"myPlaHit", payloadCode, "@",
"myTrack", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("myDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_myDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_myDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_myDict() {
  TriggerDictionaryInitialization_myDict_Impl();
}
