/*****************************************************************************
* Exercise:    Cpp 2 c exercise
* Date:        5/11/2025
* Developer:   Tal Hindi
* Reviewer:    Maria
* Status:      Waiting for review
*****************************************************************************/

#include <stdio.h> /* printf */

#include "cpp2c.h" /* Ctor */

static int s_count = 0;

struct VTable
{
    void(*Dtor)(void*);
    void(*Wash)(void*, int);
    void(*Display)(void*);
};

const vtable_t g_vtable_pt =
{
    V_PublicTransport_Dtor,
    NULL,
    V_PublicTransport_Display
};

const vtable_t g_vtable_mb =
{
    V_Minibus_Dtor,
    V_Minibus_Wash,
    V_Minibus_Display
};

const vtable_t g_vtable_amb =
{
    V_Armyminibus_Dtor,
    V_Minibus_Wash,
    V_Minibus_Display
};

const vtable_t g_vtable_t =
{
    V_Taxi_Dtor,
    NULL,
    V_Taxi_Display
};

const vtable_t g_vtable_st =
{
    V_SpecialTaxi_Dtor,
    NULL,
    V_SpecialTaxi_Display
};

static int GetID(public_transport_t* this)
{
    return this->m_license_plate;
}

static void PublicTransport_PrintCount()
{
    printf("s_count: %d\n",s_count);
}

void PublicTransport_Ctor(public_transport_t* this, const vtable_t* vptr)
{
    this->vptr = vptr;
    this->m_license_plate = ++s_count;
    printf("PublicTransport::Ctor()%d\n", this->m_license_plate);
}

void PublicTransport_CCtor(public_transport_t* this, public_transport_t* other,const vtable_t* vptr)
{
    (void)other;
    this->vptr = vptr;
    this->m_license_plate = ++s_count;
    printf("PublicTransport::CCtor() %d\n", this->m_license_plate);
}

void V_PublicTransport_Dtor(void* this)
{
    public_transport_t* pt = (public_transport_t*)this;
    --s_count;

    printf("PublicTransport::Dtor()%d\n", pt->m_license_plate);
}

void V_PublicTransport_Display(void* this)
{
    public_transport_t* pt = (public_transport_t*)this;
    printf("PublicTransport::display(): %d\n", pt->m_license_plate);
}

void Taxi_Ctor(taxi_t* this,const vtable_t* vptr)
{
    PublicTransport_Ctor(&this->base,vptr);
    printf("Taxi::Ctor()\n");
}

void Taxi_CCtor(taxi_t* this, taxi_t* other,const vtable_t* vptr)
{
    PublicTransport_CCtor(&this->base,&other->base, vptr);
    printf("Taxi::CCtor()\n");
}

void V_Taxi_Dtor(void* this)
{
    taxi_t* t = (taxi_t*)this;
    printf("Taxi::Dtor()\n");
    V_PublicTransport_Dtor(&t->base);
}

void V_Taxi_Display(void* this)
{
    taxi_t* t = (taxi_t*)this;
    printf("Taxi::display() ID:%d\n",GetID(&t->base));
}

void Minibus_Ctor(minibus_t* this, const vtable_t* vptr)
{
    PublicTransport_Ctor(&this->base, vptr);
    this->m_numSeats = NUMSEAT;
    printf("Minibus::Ctor()\n");
}

void V_Minibus_Dtor(void* this)
{
    minibus_t* mb = (minibus_t*)this;
    printf("Minibus::Dtor()\n");
    V_PublicTransport_Dtor(&mb->base);
}

void V_Minibus_Display(void* this)
{
    minibus_t* mb = (minibus_t*)this;
    printf("Minibus::display() ID:%d num seats:%d\n",GetID(&mb->base),mb->m_numSeats);
}

void V_Minibus_Wash(void* this, int min)
{
    minibus_t* mb = (minibus_t*)this;
    printf("Minibus::wash(%d) ID:%d\n",min,GetID(&mb->base));
}

void Armyminibus_Ctor(army_minibus_t* this)
{
    Minibus_Ctor(&this->base, &g_vtable_amb);
    printf("ArmyMinibus::Ctor()\n");
}

void V_Armyminibus_Dtor(void* this)
{
    army_minibus_t* amb = (army_minibus_t*)this;
    printf("ArmyMinibus::Dtor()\n");
    V_Minibus_Dtor(&amb->base);
}

void SpecialTaxi_Ctor(special_taxi_t* this)
{
    Taxi_Ctor(&this->base,&g_vtable_st);
    printf("SpecialTaxi::Ctor()\n");
}

void V_SpecialTaxi_Dtor(void* this)
{
    special_taxi_t* st = (special_taxi_t*)this;
    printf("SpecialTaxi::Dtor()\n");
    V_Taxi_Dtor(&st->base);
}

void V_SpecialTaxi_Display(void* this)
{
    special_taxi_t* st = (special_taxi_t*)this;
    printf("SpecialTaxi::display() ID:%d \n",GetID(&st->base.base));
}

void PrintInfo(void)
{
    PublicTransport_PrintCount();
}

void PublicTransport_PrintInfo(public_transport_t* pt)
{
    pt->vptr->Display(pt);
}

void Minibus_PrintInfo(minibus_t* mb)
{
    mb->base.vptr->Wash(mb,3);
}

public_transport_t PublicTransport_PrintInfo2(int i)
{
    minibus_t ret = {0};
    public_transport_t ret_base = {0};
    (void)i;

    Minibus_Ctor(&ret, &g_vtable_mb);
    printf("print_info(int i)\n");
    ret.base.vptr->Display(&ret);

    PublicTransport_CCtor(&ret_base, &ret.base, &g_vtable_pt);
    V_Minibus_Dtor(&ret);

    return ret_base;
}

void Taxi_DisplayByVal(taxi_t t)
{
    t.base.vptr->Display(&t);
}


