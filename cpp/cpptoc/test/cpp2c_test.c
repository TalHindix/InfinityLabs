/*****************************************************************************
* Exercise:    Cpp 2 c exercise
* Date:        5/11/2025
* Developer:   Tal Hindi
* Reviewer:    Maria
* Status:      Waiting for review
*****************************************************************************/

#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */

#include "cpp2c.h" /* Ctor */

struct VTable
{
    void(*Dtor)(void*);
    void(*Wash)(void*, int);
    void(*Display)(void*);
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
    V_PublicTransport_Dtor(&ret_val);

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

    V_Taxi_Dtor(&temp_copy);
    V_Minibus_Dtor(&temp_mb);

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
        V_Taxi_Dtor(&arr4[i - 1]);
    }
    free(arr4);

    printf("2\n");
    printf("2\n");

    SpecialTaxi_Ctor(&st);
    Taxi_CCtor(&temp_copy, &st.base,&g_vtable_t);
    Taxi_DisplayByVal(temp_copy);
    V_Taxi_Dtor(&temp_copy);

    army_minibus = (army_minibus_t*)malloc(sizeof(army_minibus_t));
    Armyminibus_Ctor(army_minibus);
    army_minibus->base.base.vptr->Display(army_minibus);
    army_minibus->base.base.vptr->Wash(army_minibus, 5);
    army_minibus->base.base.vptr->Dtor(army_minibus);
    free(army_minibus);

    V_SpecialTaxi_Dtor(&st);

    for (i = 4; i > 0; --i)
    {
        V_Minibus_Dtor(&arr3[i - 1]);
    }

    V_Minibus_Dtor(&m2);

    for (i = 3; i > 0; --i)
    {
        V_PublicTransport_Dtor(&arr2[i - 1]);
    }

    V_Minibus_Dtor(&m);

    return 0;
}

