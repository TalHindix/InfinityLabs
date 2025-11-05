/*****************************************************************************
* Exercise:    Cpp 2 c exercise
* Date:        5/11/2025
* Developer:   Tal Hindi
* Reviewer:    Maria
* Status:      Waiting for review
*****************************************************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#define NUMSEAT (20)
#define MAX_FUNC(t1, t2) ((t1) > (t2) ? (t1) : (t2))

typedef struct PublicTransport public_transport_t;
typedef struct Taxi taxi_t;
typedef struct Minibus minibus_t;
typedef struct Armyminibus army_minibus_t;
typedef struct SpecialTaxi special_taxi_t;
typedef struct VTable vtable_t;


struct VTable
{
    void(*Dtor)(void*);
    void(*Wash)(void*, int);
    void(*Display)(void*);
};

struct PublicTransport
{
    const vtable_t* vptr;
    int m_license_plate;
};

struct Taxi
{
    public_transport_t base;
};

struct Minibus
{
    public_transport_t base;
    int m_numSeats;
};

struct Armyminibus
{
    minibus_t base;
};

struct SpecialTaxi
{
    taxi_t base;
};

static int s_count = 0;


static void PublicTransport_Ctor(public_transport_t* this, const vtable_t* vptr);
static void PublicTransport_CCtor(public_transport_t* this, public_transport_t* other,const vtable_t* vptr);
static void PublicTransport_Dtor(void* this);
static void PublicTransport_Display(void* this);

static void Taxi_Ctor(taxi_t* this, const vtable_t* vptr);
static void Taxi_CCtor(taxi_t* this, taxi_t* other, const vtable_t* vptr);
static void Taxi_Dtor(void* this);
static void Taxi_Display(void* this);

static void Minibus_Ctor(minibus_t* this,const vtable_t* vptr);
static void Minibus_Dtor(void* this);
static void Minibus_Display(void* this);
static void Minibus_Wash(void* this, int min);

static void Armyminibus_Ctor(army_minibus_t* this);
static void Armyminibus_Dtor(void* this);

static void SpecialTaxi_Ctor(special_taxi_t* this);
static void SpecialTaxi_Dtor(void* this);
static void SpecialTaxi_Display(void* this);

static void PrintInfo(void);
static void PublicTransport_PrintInfo(public_transport_t* pt);
static void Minibus_PrintInfo(minibus_t* mb);
static public_transport_t PublicTransport_PrintInfo2(int i);
static void Taxi_DisplayByVal(taxi_t* t);

static const vtable_t g_vtable_pt =
{
    PublicTransport_Dtor,
    NULL,
    PublicTransport_Display
};

static const vtable_t g_vtable_mb =
{
    Minibus_Dtor,
    Minibus_Wash,
    Minibus_Display
};

static const vtable_t g_vtable_amb =
{
    Armyminibus_Dtor,
    Minibus_Wash,
    Minibus_Display
};

static const vtable_t g_vtable_t =
{
    Taxi_Dtor,
    NULL,
    Taxi_Display
};

static const vtable_t g_vtable_st =
{
    SpecialTaxi_Dtor,
    NULL,
    SpecialTaxi_Display
};



int main(void)
{
    minibus_t m = {0};
    public_transport_t ret_val = {0};
    public_transport_t* array[3] = {NULL};
    public_transport_t arr2[3] = {0};
    minibus_t m2 = {0};
    minibus_t arr3[4] = {0};
    taxi_t* arr4 = NULL;
    taxi_t temp_copy = {0};
    minibus_t temp_mb = {0};
    special_taxi_t st = {0};
    army_minibus_t* army_minibus = NULL;
    size_t i = 0;

    Minibus_Ctor(&m, &g_vtable_mb);
    Minibus_PrintInfo(&m);

    ret_val = PublicTransport_PrintInfo2(3);
    ret_val.vptr->Display(&ret_val);
    PublicTransport_Dtor(&ret_val);

    array[0] = (public_transport_t*)malloc(sizeof(minibus_t));
    array[1] = (public_transport_t*)malloc(sizeof(taxi_t));
    array[2] = (public_transport_t*)malloc(sizeof(minibus_t));

    Minibus_Ctor((minibus_t*)array[0], &g_vtable_mb);
    Taxi_Ctor((taxi_t*)array[1],&g_vtable_t);
    Minibus_Ctor((minibus_t*)array[2], &g_vtable_mb);

    for (i = 0; i < 3; ++i)
    {
        array[i]->vptr->Display(array[i]);
    }

    for (i = 0; i < 3; ++i)
    {
        array[i]->vptr->Dtor(array[i]);
        free(array[i]);
    }



    Minibus_Ctor(&temp_mb, &g_vtable_mb);
    PublicTransport_CCtor(&arr2[0], &temp_mb.base, &g_vtable_pt);

    Taxi_Ctor(&temp_copy,&g_vtable_t);
    PublicTransport_CCtor(&arr2[1], &temp_copy.base, &g_vtable_pt);

    PublicTransport_Ctor(&arr2[2], &g_vtable_pt);

    Taxi_Dtor(&temp_copy);
    Minibus_Dtor(&temp_mb);

    for (i = 0; i < 3; ++i)
    {
        arr2[i].vptr->Display(&arr2[i]);
    }

    arr2[0].vptr->Display(&arr2[0]);

    PrintInfo();

    Minibus_Ctor(&m2, &g_vtable_mb);
    PrintInfo();

    for (i = 0; i < 4; ++i)
    {
        Minibus_Ctor(&arr3[i], &g_vtable_mb);
    }

    arr4 = (taxi_t*)malloc(4 * sizeof(taxi_t));

    for (i = 0; i < 4; ++i)
    {
        Taxi_Ctor(&arr4[i],&g_vtable_t);
    }

    for (i = 4; i > 0; --i)
    {
        Taxi_Dtor(&arr4[i - 1]);
    }
    free(arr4);

    printf("2\n");
    printf("2\n");

    SpecialTaxi_Ctor(&st);
    Taxi_CCtor(&temp_copy, &st.base,&g_vtable_t);
    Taxi_DisplayByVal(&temp_copy);
    Taxi_Dtor(&temp_copy);

    army_minibus = (army_minibus_t*)malloc(sizeof(army_minibus_t));
    Armyminibus_Ctor(army_minibus);
    army_minibus->base.base.vptr->Display(army_minibus);
    army_minibus->base.base.vptr->Wash(army_minibus, 5);
    army_minibus->base.base.vptr->Dtor(army_minibus);
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

static void PublicTransport_Ctor(public_transport_t* this, const vtable_t* vptr)
{
    this->vptr = vptr;
    this->m_license_plate = ++s_count;
    printf("PublicTransport::Ctor()%d\n", this->m_license_plate);
}

static void PublicTransport_CCtor(public_transport_t* this, public_transport_t* other,const vtable_t* vptr)
{
    this->vptr = vptr;
    this->m_license_plate = ++s_count;

    (void)other;
    printf("PublicTransport::CCtor()%d\n", this->m_license_plate);
}

static void PublicTransport_Dtor(void* this)
{
    public_transport_t* pt = (public_transport_t*)this;
    --s_count;

    printf("PublicTransport::Dtor()%d\n", pt->m_license_plate);
}

static void PublicTransport_Display(void* this)
{
    public_transport_t* pt = (public_transport_t*)this;
    printf("PublicTransport::Display() : %d\n", pt->m_license_plate);
}

static void Taxi_Ctor(taxi_t* this,const vtable_t* vptr)
{
    PublicTransport_Ctor(&this->base,vptr);
    printf("Taxi::Ctor()\n");
}
static void Taxi_CCtor(taxi_t* this, taxi_t* other,const vtable_t* vptr)
{
    PublicTransport_CCtor(&this->base,&other->base, vptr);
    printf("Taxi::CCtor()\n");
}
static void Taxi_Dtor(void* this)
{
    taxi_t* t = (taxi_t*)this;
    printf("Taxi::Dtor()\n");
    PublicTransport_Dtor(&t->base);
}
static void Taxi_Display(void* this)
{
    taxi_t* t = (taxi_t*)this;
    printf("Taxi::display() ID:%d\n",t->base.m_license_plate);
}

static void Minibus_Ctor(minibus_t* this, const vtable_t* vptr)
{
    (void)vptr;
    PublicTransport_Ctor(&this->base, vptr);
    this->m_numSeats = NUMSEAT;
    printf("Minibus::Ctor()\n");
}

static void Minibus_Dtor(void* this)
{
    minibus_t* mb = (minibus_t*)this;
    printf("Minibus::Dtor()\n");
    PublicTransport_Dtor(&mb->base);
}

static void Minibus_Display(void* this)
{
    minibus_t* mb = (minibus_t*)this;
    printf("Minibus::display() ID:%d num seats:%d\n",mb->base.m_license_plate,mb->m_numSeats);
}

static void Minibus_Wash(void* this, int min)
{
    minibus_t* mb = (minibus_t*)this;
    printf("Minibus::wash(%d) ID:%d \n",min,mb->base.m_license_plate);
}

static void Armyminibus_Ctor(army_minibus_t* this)
{
    Minibus_Ctor(&this->base, &g_vtable_amb);
    printf("Armyminibus::Ctor()\n");
}

static void Armyminibus_Dtor(void* this)
{
    army_minibus_t* amb = (army_minibus_t*)this;
    printf("Armyminibus::Dtor()\n");
    Minibus_Dtor(&amb->base);
}

static void SpecialTaxi_Ctor(special_taxi_t* this)
{
    Taxi_Ctor(&this->base,&g_vtable_st);
    printf("SpecialTaxi::Ctor()\n");
}
static void SpecialTaxi_Dtor(void* this)
{
    special_taxi_t* st = (special_taxi_t*)this;
    printf("SpecialTaxi::Dtor()\n");
    Taxi_Dtor(&st->base);
}
static void SpecialTaxi_Display(void* this)
{
    special_taxi_t* st = (special_taxi_t*)this;
    printf("SpecialTaxi::display() ID:%d \n",st->base.base.m_license_plate);
}

static void PrintInfo(void)
{
    printf("s_count: %d\n",s_count);
}

static void PublicTransport_PrintInfo(public_transport_t* pt)
{
    pt->vptr->Display(pt);
}

static void Minibus_PrintInfo(minibus_t* mb)
{
    mb->base.vptr->Wash(mb,3);
}

static public_transport_t PublicTransport_PrintInfo2(int i)
{
    minibus_t ret = {0};
    public_transport_t ret_base = {0};

    Minibus_Ctor(&ret, &g_vtable_mb);
    printf("print_info(int i)\n");
    ret.base.vptr->Display(&ret);

    PublicTransport_CCtor(&ret_base, &ret.base, &g_vtable_pt);
    Minibus_Dtor(&ret);

    (void)i;

    return ret_base;
}

static void Taxi_DisplayByVal(taxi_t* t)
{
    t->base.vptr->Display(t);
}
