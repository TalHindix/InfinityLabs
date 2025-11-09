/*****************************************************************************
* Exercise:    Cpp 2 c exercise
* Date:        5/11/2025
* Developer:   Tal Hindi
* Reviewer:    Maria
* Status:      Waiting for 2nd review
*****************************************************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#define MAX_FUNC(t1, t2) ((t1) > (t2) ? (t1) : (t2))

/* Forward Declartion */
typedef struct PublicTransport PublicTransport_t;
typedef struct Taxi Taxi_t;
typedef struct Minibus MiniBus_t;
typedef struct Armyminibus ArmyMiniBus_t;
typedef struct SpecialTaxi SpecialTaxi_t;
typedef struct PT_VTable PublicTransport_Vtable;
typedef struct MB_VTable Minibus_Vtable;

static int s_count = 0;

/* Vtable Structs */
struct PT_VTable
{
    void (*Dtor)(PublicTransport_t*);
    void (*Display)(PublicTransport_t*);
};
struct MB_VTable
{
    PublicTransport_Vtable base;
    void (*Wash)(MiniBus_t*, int);
};

struct PublicTransport
{
    const PublicTransport_Vtable* vptr;
    int m_license_plate;
};
struct Taxi
{
    PublicTransport_t base;
};
struct Minibus
{
    PublicTransport_t base;
    int m_numSeats;
};
struct Armyminibus
{
    MiniBus_t base;
};
struct SpecialTaxi
{
    Taxi_t base;
};

void PublicTransport_Ctor(PublicTransport_t* this);
void PublicTransport_CCtor(PublicTransport_t* this, PublicTransport_t* other);
void PublicTransport_Dtor(PublicTransport_t* this);
void PublicTransport_Display(PublicTransport_t* this);

void Taxi_Ctor(Taxi_t* this);
void Taxi_CCtor(Taxi_t* this, Taxi_t* other);
void Taxi_Dtor(Taxi_t* this);
void Taxi_Display(Taxi_t* this);

void Minibus_Ctor(MiniBus_t* this);
void Minibus_Dtor(MiniBus_t* this);
void Minibus_Display(MiniBus_t* this);
void Minibus_Wash(MiniBus_t* this, int min);

void Armyminibus_Ctor(ArmyMiniBus_t* this);
void Armyminibus_Dtor(ArmyMiniBus_t* this);

void SpecialTaxi_Ctor(SpecialTaxi_t* this);
void SpecialTaxi_Dtor(SpecialTaxi_t* this);
void SpecialTaxi_Display(SpecialTaxi_t* this);

void PrintInfo(void);
void PublicTransport_PrintInfo(PublicTransport_t* pt);
void Minibus_PrintInfo(MiniBus_t* mb);
PublicTransport_t PublicTransport_PrintInfo2(int i);
void Taxi_DisplayByVal(Taxi_t t);

static int GetID(PublicTransport_t* this);
static void PublicTransport_PrintCount();

const PublicTransport_Vtable public_transport_vtable =
{
    PublicTransport_Dtor,
    PublicTransport_Display
};

const PublicTransport_Vtable taxi_vtable =
{
    (void (*)(PublicTransport_t*))Taxi_Dtor,
    (void (*)(PublicTransport_t*))Taxi_Display
};

const Minibus_Vtable mini_bus_vtable =
{
    {
        (void (*)(PublicTransport_t*))Minibus_Dtor,
        (void (*)(PublicTransport_t*))Minibus_Display
    },
    Minibus_Wash
};

const PublicTransport_Vtable special_taxi_vtable =
{
    (void (*)(PublicTransport_t*))SpecialTaxi_Dtor,
    (void (*)(PublicTransport_t*))SpecialTaxi_Display
};

const Minibus_Vtable army_minibus_vtable =
{
    {
        (void (*)(PublicTransport_t*))Armyminibus_Dtor,
        (void (*)(PublicTransport_t*))Minibus_Display
    },
    Minibus_Wash
};

int main(void)
{
    MiniBus_t m = {0};
    PublicTransport_t ret_val = {0};
    PublicTransport_t* array[3] = {NULL};
    PublicTransport_t arr2[3] = {0};
    MiniBus_t m2 = {0};
    MiniBus_t arr3[4] = {0};
    Taxi_t* arr4 = NULL;
    Taxi_t temp_copy = {0};
    MiniBus_t temp_mb = {0};
    SpecialTaxi_t st = {0};
    ArmyMiniBus_t* army_minibus = NULL;
    size_t i = 0;

    Minibus_Ctor(&m);
    Minibus_PrintInfo(&m);

    ret_val = PublicTransport_PrintInfo2(3);
    ret_val.vptr->Display(&ret_val);
    PublicTransport_Dtor(&ret_val);

    array[0] = (PublicTransport_t*)malloc(sizeof(MiniBus_t));
    array[1] = (PublicTransport_t*)malloc(sizeof(Taxi_t));
    array[2] = (PublicTransport_t*)malloc(sizeof(MiniBus_t));

    Minibus_Ctor((MiniBus_t*)array[0]);
    Taxi_Ctor((Taxi_t*)array[1]);
    Minibus_Ctor((MiniBus_t*)array[2]);

    for (i = 0; i < 3; ++i)
    {
        array[i]->vptr->Display(array[i]);
    }

    for (i = 0; i < 3; ++i)
    {
        array[i]->vptr->Dtor(array[i]);
        free(array[i]);
    }

    Minibus_Ctor(&temp_mb);
    PublicTransport_CCtor(&arr2[0], &temp_mb.base);

    Taxi_Ctor(&temp_copy);
    PublicTransport_CCtor(&arr2[1], &temp_copy.base);

    PublicTransport_Ctor(&arr2[2]);

    Taxi_Dtor(&temp_copy);
    Minibus_Dtor(&temp_mb);

    for (i = 0; i < 3; ++i)
    {
        arr2[i].vptr->Display(&arr2[i]);
    }

    arr2[0].vptr->Display(&arr2[0]);

    PrintInfo();

    Minibus_Ctor(&m2);
    PrintInfo();

    for (i = 0; i < 4; ++i)
    {
        Minibus_Ctor(&arr3[i]);
    }

    arr4 = (Taxi_t*)malloc(4 * sizeof(Taxi_t));

    for (i = 0; i < 4; ++i)
    {
        Taxi_Ctor(&arr4[i]);
    }

    for (i = 4; i > 0; --i)
    {
        Taxi_Dtor(&arr4[i - 1]);
    }
    free(arr4);

    printf("2\n");
    printf("2\n");

    SpecialTaxi_Ctor(&st);
    Taxi_CCtor(&temp_copy, &st.base);
    Taxi_DisplayByVal(temp_copy);
    Taxi_Dtor(&temp_copy);

    army_minibus = (ArmyMiniBus_t*)malloc(sizeof(ArmyMiniBus_t));
    Armyminibus_Ctor(army_minibus);
    army_minibus->base.base.vptr->Display(&army_minibus->base.base);
    ((Minibus_Vtable*)army_minibus->base.base.vptr)->Wash(&army_minibus->base,5);
    army_minibus->base.base.vptr->Dtor(&army_minibus->base.base);
    free(army_minibus);

    SpecialTaxi_Dtor(&st);

    for (i = 4; i > 0; --i)
    {
        Minibus_Dtor(&arr3[i - 1]);
    }

    Minibus_Dtor(&m2);

    for (i = 3; i > 0; --i)
    {
        PublicTransport_Dtor(&arr2[i - 1]);
    }

    Minibus_Dtor(&m);

    return 0;
}

/* ------------------- HELPER FUNCTIONS ----------------------*/
static int GetID(PublicTransport_t* this)
{
    return this->m_license_plate;
}

static void PublicTransport_PrintCount()
{
    printf("s_count: %d\n",s_count);
}
/* ------------------------------------------------------------*/

/* ------------------- PublicTransport Implementations -------------------*/
void PublicTransport_Ctor(PublicTransport_t* this)
{
    this->vptr = &public_transport_vtable;
    this->m_license_plate = ++s_count;
    printf("PublicTransport::Ctor()%d\n", this->m_license_plate);
}

void PublicTransport_CCtor(PublicTransport_t* this, PublicTransport_t* other)
{
    (void)other;
    this->vptr = &public_transport_vtable;
    this->m_license_plate = ++s_count;
    printf("PublicTransport::CCtor() %d\n", this->m_license_plate);
}

void PublicTransport_Dtor(PublicTransport_t* this)
{
    --s_count;
    printf("PublicTransport::Dtor()%d\n", this->m_license_plate);
}

void PublicTransport_Display(PublicTransport_t* this)
{
    printf("PublicTransport::display(): %d\n", this->m_license_plate);
}


/* ------------------- Taxi Implementations -------------------*/
void Taxi_Ctor(Taxi_t* this)
{
    PublicTransport_Ctor(&this->base);
    this->base.vptr = &taxi_vtable;
    printf("Taxi::Ctor()\n");
}

void Taxi_CCtor(Taxi_t* this, Taxi_t* other)
{
    PublicTransport_CCtor(&this->base,&other->base);
    this->base.vptr = &taxi_vtable;
    printf("Taxi::CCtor()\n");
}

void Taxi_Dtor(Taxi_t* this)
{
    this->base.vptr = &public_transport_vtable;
    printf("Taxi::Dtor()\n");
    PublicTransport_Dtor(&this->base);
}

void Taxi_Display(Taxi_t* this)
{
    printf("Taxi::display() ID:%d\n",GetID(&this->base));
}

/* ------------------- MiniBus Implementations ---------------------*/
void Minibus_Ctor(MiniBus_t* this)
{
    PublicTransport_Ctor(&this->base);
    this->base.vptr = (PublicTransport_Vtable*)&mini_bus_vtable;
    this->m_numSeats = 20;
    printf("Minibus::Ctor()\n");
}

void Minibus_Dtor(MiniBus_t* this)
{
    this->base.vptr = &public_transport_vtable;
    printf("Minibus::Dtor()\n");
    PublicTransport_Dtor(&this->base);
}

void Minibus_Display(MiniBus_t* this)
{
    printf("Minibus::display() ID:%d num seats:%d\n",GetID(&this->base),this->m_numSeats);
}

void Minibus_Wash(MiniBus_t* this, int min)
{
    printf("Minibus::wash(%d) ID:%d\n",min,GetID(&this->base));
}

/* -------------------------------------- ArmyMiniBus Implementation --------------------------------------*/
void Armyminibus_Ctor(ArmyMiniBus_t* this)
{
    Minibus_Ctor(&this->base);
    this->base.base.vptr = (PublicTransport_Vtable*)&army_minibus_vtable;
    printf("ArmyMinibus::Ctor()\n");
}

void Armyminibus_Dtor(ArmyMiniBus_t* this)
{
    this->base.base.vptr = (PublicTransport_Vtable*)&mini_bus_vtable;
    printf("ArmyMinibus::Dtor()\n");
    Minibus_Dtor(&this->base);
}


/* ---------------------------- Special Taxi Implementation --------------------------------------------------------*/
void SpecialTaxi_Ctor(SpecialTaxi_t* this)
{
    Taxi_Ctor(&this->base);
    this->base.base.vptr = &special_taxi_vtable;
    printf("SpecialTaxi::Ctor()\n");
}

void SpecialTaxi_Dtor(SpecialTaxi_t* this)
{
    this->base.base.vptr = &taxi_vtable;
    printf("SpecialTaxi::Dtor()\n");
    Taxi_Dtor(&this->base);
}

void SpecialTaxi_Display(SpecialTaxi_t* this)
{
    printf("SpecialTaxi::display() ID:%d \n",GetID(&this->base.base));
}

/* --------------------------------------------------------------------------------------------------------*/
void PrintInfo(void)
{
    PublicTransport_PrintCount();
}

void PublicTransport_PrintInfo(PublicTransport_t* pt)
{
    pt->vptr->Display(pt);
}

void Minibus_PrintInfo(MiniBus_t* mb)
{
    Minibus_Vtable* mb_ptr = (Minibus_Vtable*)mb->base.vptr;
    mb_ptr->Wash(mb,3);
}

PublicTransport_t PublicTransport_PrintInfo2(int i)
{
    MiniBus_t ret = {0};
    PublicTransport_t ret_base = {0};

    (void)i;
    Minibus_Ctor(&ret);
    printf("print_info(int i)\n");
    ret.base.vptr->Display(&ret.base);
    PublicTransport_CCtor(&ret_base, &ret.base);
    Minibus_Dtor(&ret);

    return ret_base;
}

void Taxi_DisplayByVal(Taxi_t t)
{
    t.base.vptr->Display(&t.base);
}


