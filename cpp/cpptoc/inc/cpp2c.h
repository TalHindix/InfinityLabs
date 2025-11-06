/*****************************************************************************
* Exercise:    Cpp 2 c exercise
* Date:        5/11/2025
* Developer:   Tal Hindi
* Reviewer:    Maria
* Status:      Waiting for review
*****************************************************************************/

#ifndef __ILRD_CPP2C__
#define __ILRD_CPP2C__

#include <stdlib.h> /* malloc */

#define NUMSEAT (20)
#define MAX_FUNC(t1, t2) ((t1) > (t2) ? (t1) : (t2))

typedef struct VTable vtable_t;

typedef struct PublicTransport
{
    const vtable_t* vptr;
    int m_license_plate;
} public_transport_t;

typedef struct Taxi { public_transport_t base; } taxi_t;
typedef struct Minibus { public_transport_t base; int m_numSeats; } minibus_t;
typedef struct Armyminibus { minibus_t base; } army_minibus_t;
typedef struct SpecialTaxi { taxi_t base; } special_taxi_t;

extern const vtable_t g_vtable_pt;
extern const vtable_t g_vtable_mb;
extern const vtable_t g_vtable_amb;
extern const vtable_t g_vtable_t;
extern const vtable_t g_vtable_st;

void PublicTransport_Ctor(public_transport_t* this, const vtable_t* vptr);
void PublicTransport_CCtor(public_transport_t* this, public_transport_t* other,const vtable_t* vptr);
void V_PublicTransport_Dtor(void* this);
void V_PublicTransport_Display(void* this);

void Taxi_Ctor(taxi_t* this, const vtable_t* vptr);
void Taxi_CCtor(taxi_t* this, taxi_t* other, const vtable_t* vptr);
void V_Taxi_Dtor(void* this);
void V_Taxi_Display(void* this);

void Minibus_Ctor(minibus_t* this,const vtable_t* vptr);
void V_Minibus_Dtor(void* this);
void V_Minibus_Display(void* this);
void V_Minibus_Wash(void* this, int min);

void Armyminibus_Ctor(army_minibus_t* this);
void V_Armyminibus_Dtor(void* this);

void SpecialTaxi_Ctor(special_taxi_t* this);
void V_SpecialTaxi_Dtor(void* this);
void V_SpecialTaxi_Display(void* this);

void PrintInfo(void);
void PublicTransport_PrintInfo(public_transport_t* pt);
void Minibus_PrintInfo(minibus_t* mb);
public_transport_t PublicTransport_PrintInfo2(int i);
void Taxi_DisplayByVal(taxi_t t);

#endif /*__ILRD_CPP2C__*/
