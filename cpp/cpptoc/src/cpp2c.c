/*****************************************************************************
* Exercise:    Cpp 2 c exercise
* Date:        9/11/2025
* Developer:   Tal Hindi
* Reviewer:    Maria
* Status:      Waiting for 3rd review
*****************************************************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#define MAX_FUNC(t1, t2) ((t1) > (t2) ? (t1) : (t2))

/* ------------------- structs typedef ------------------- */
typedef struct PublicTransport PublicTransport_t;
typedef struct Taxi Taxi_t;
typedef struct Minibus MiniBus_t;
typedef struct Armyminibus ArmyMiniBus_t;
typedef struct SpecialTaxi SpecialTaxi_t;

/* ------------------- vtables typedef ------------------- */
typedef struct PT_VTable PublicTransport_Vtable;
typedef struct MB_VTable Minibus_Vtable;

/* ------------------- functions pointer typedef ------------------- */
typedef void (*PT_FuncPtr)(PublicTransport_t*);
typedef void (*MB_WashFuncPtr)(MiniBus_t*, int);

static int s_count = 0;

/* ------------------- Vtables + structs ------------------- */
struct PT_VTable
{
    PT_FuncPtr Dtor;
    PT_FuncPtr Display;
};

struct MB_VTable
{
    PublicTransport_Vtable base;
    MB_WashFuncPtr Wash;
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

/* ------------------- prototypes -------------------*/
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
void Taxi_DisplayByVal(Taxi_t* t);

static int GetID(PublicTransport_t* this);
static void PublicTransport_PrintCount();

/* ------------------- Vtable defintion and initialization ------------------- */
const PublicTransport_Vtable public_transport_vtable =
{
    PublicTransport_Dtor,
    PublicTransport_Display
};

const PublicTransport_Vtable taxi_vtable =
{
    (PT_FuncPtr)Taxi_Dtor,
    (PT_FuncPtr)Taxi_Display
};

const Minibus_Vtable mini_bus_vtable =
{
    {
        (PT_FuncPtr)Minibus_Dtor,
        (PT_FuncPtr)Minibus_Display
    },
    Minibus_Wash
};

const PublicTransport_Vtable special_taxi_vtable =
{
    (PT_FuncPtr)SpecialTaxi_Dtor,
    (PT_FuncPtr)SpecialTaxi_Display
};

const Minibus_Vtable army_minibus_vtable =
{
    {
        (PT_FuncPtr)Armyminibus_Dtor,
        (PT_FuncPtr)Minibus_Display
    },
    Minibus_Wash
};

/* ------------------- helper functions ----------------------*/
static int GetID(PublicTransport_t* this)
{
    return this->m_license_plate;
}

static void PublicTransport_PrintCount()
{
    printf("s_count: %d\n",s_count);
}

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
    PublicTransport_Ctor((PublicTransport_t*)this);
    ((PublicTransport_t*)this)->vptr = &taxi_vtable;
    printf("Taxi::Ctor()\n");
}

void Taxi_CCtor(Taxi_t* this, Taxi_t* other)
{
    PublicTransport_CCtor((PublicTransport_t*)this, (PublicTransport_t*)other);
    ((PublicTransport_t*)this)->vptr = &taxi_vtable;
    printf("Taxi::CCtor()\n");
}

void Taxi_Dtor(Taxi_t* this)
{
    printf("Taxi::Dtor()\n");
    ((PublicTransport_t*)this)->vptr = &public_transport_vtable;
    PublicTransport_Dtor((PublicTransport_t*)this);
}

void Taxi_Display(Taxi_t* this)
{
    printf("Taxi::display() ID:%d\n", GetID((PublicTransport_t*)this));
}

/* ------------------- MiniBus Implementations ---------------------*/
void Minibus_Ctor(MiniBus_t* this)
{
    PublicTransport_Ctor((PublicTransport_t*)this);
    ((PublicTransport_t*)this)->vptr = (PublicTransport_Vtable*)&mini_bus_vtable;
    this->m_numSeats = 20;
    printf("Minibus::Ctor()\n");
}

void Minibus_Dtor(MiniBus_t* this)
{
    printf("Minibus::Dtor()\n");
    ((PublicTransport_t*)this)->vptr = &public_transport_vtable;
    PublicTransport_Dtor((PublicTransport_t*)this);
}

void Minibus_Display(MiniBus_t* this)
{
    printf("Minibus::display() ID:%d num seats:%d\n",GetID((PublicTransport_t*)this), this->m_numSeats);
}

void Minibus_Wash(MiniBus_t* this, int min)
{
    printf("Minibus::wash(%d) ID:%d\n", min, GetID((PublicTransport_t*)this));
}

/* -------------------------------------- ArmyMiniBus Implementation --------------------------------------*/
void Armyminibus_Ctor(ArmyMiniBus_t* this)
{
    Minibus_Ctor(&this->base);
    ((PublicTransport_t*)this)->vptr = (PublicTransport_Vtable*)&army_minibus_vtable;
    printf("ArmyMinibus::Ctor()\n");
}

void Armyminibus_Dtor(ArmyMiniBus_t* this)
{
    printf("ArmyMinibus::Dtor()\n");
    ((PublicTransport_t*)this)->vptr = (PublicTransport_Vtable*)&mini_bus_vtable;
    Minibus_Dtor(&this->base);
}

/* ---------------------------- Special Taxi Implementation --------------------------------------------------------*/
void SpecialTaxi_Ctor(SpecialTaxi_t* this)
{
    Taxi_Ctor(&this->base);
    ((PublicTransport_t*)this)->vptr = &special_taxi_vtable;
    printf("SpecialTaxi::Ctor()\n");
}

void SpecialTaxi_Dtor(SpecialTaxi_t* this)
{
    printf("SpecialTaxi::Dtor()\n");
    ((PublicTransport_t*)this)->vptr = &taxi_vtable;
    Taxi_Dtor(&this->base);
}

void SpecialTaxi_Display(SpecialTaxi_t* this)
{
    printf("SpecialTaxi::display() ID:%d \n",
           GetID((PublicTransport_t*)this));
}

/* ------------------- Non-member functions ------------------- */
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
    const Minibus_Vtable* mb_ptr = (const Minibus_Vtable*)((const PublicTransport_t*)mb)->vptr;
    mb_ptr->Wash(mb,3);
}

PublicTransport_t PublicTransport_PrintInfo2(int i)
{
    MiniBus_t m = {0};
    PublicTransport_t ret = {0};

    (void)i;

    Minibus_Ctor(&m);
    printf("print_info(int i)\n");

    ((PublicTransport_t*)&m)->vptr->Display((PublicTransport_t*)&m);

    PublicTransport_CCtor(&ret, (PublicTransport_t*)&m);
    Minibus_Dtor(&m);

    return ret;
}

void Taxi_DisplayByVal(Taxi_t* t)
{
    ((PublicTransport_t*)t)->vptr->Display((PublicTransport_t*)t);
}

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
    Taxi_DisplayByVal(&temp_copy);
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


