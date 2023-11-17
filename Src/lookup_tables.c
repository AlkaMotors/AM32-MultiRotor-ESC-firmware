#pragma once

/*
 * lookup_tables.c
 *
 *  Created on: Nov. 17, 2023
 *      Author: Kenneth
 */

#include "functions.h"
#include "targets.h"
#include "stdbool.h"
#include "lookup_tables.h"


void ScaleSearchBinary(scale_t * scale, scale_functions_t * functions);

void Scale(scale_t * scale, scale_functions_t * functions) {
    scale_result_t * result = scale->Result;

    functions->SetValue(result->ScaledValue, scale->SourceValue); //ensure value is maintained from here

    result->IndexHigh = scale->Length - 1;
    result->IndexLow = 0;

    void * scaleHigh = functions->GetArrayValue(scale->Data, result->IndexHigh);
    void * scaleLow = functions->GetArrayValue(scale->Data, result->IndexLow);

    bool boundHigh = functions->Compare(result->ScaledValue, scaleHigh) >= 0;
    bool boundLow = functions->Compare(result->ScaledValue, scaleLow) <= 0;

    
    if (boundHigh || boundLow) {
        if (boundHigh) {
            result->IndexLow = result->IndexHigh;
        } else {
            result->IndexHigh = result->IndexLow;
        }
        functions->SetValue(result->ScaledValue, scaleLow); //bring within scale limits
    } else {
        ScaleSearchBinary(scale, functions);
    }
}


/*
    Scale lookup using binary search pattern.
    Uses recursion to minimise FLASH at the expense of stack use. 
    
    NOTE: If you could gaurantee the scale data was linear, this could be simplified a long way as follows
    Low index = Data length (items in array) * input value / highest value in the array: LI = (DL * IV) / HV 

*/
void Scale_rec(scale_result_t * result, void * data, void * toFind, uint8_t min, uint8_t max, scale_functions_t * functions);

void ScaleSearchBinary(scale_t * scale, scale_functions_t * functions)
{
    scale_result_t * result = scale->Result;
    Scale_rec(result, scale->Data, result->ScaledValue, result->IndexLow, result->IndexHigh, functions);
}


void Scale_rec(scale_result_t * result, void * data, void * toFind, uint8_t min, uint8_t max, scale_functions_t * functions)
{
    uint8_t index = (min + max) >> 1; //find the half way point between min and max, efectively using a binary search

    void * value = functions->GetArrayValue(data, index);
    int cmp = functions->Compare(value, toFind);

    if (cmp == 0)  {
        min = index;
        max = index;
    } else if (cmp > 0)  {
        max = index--; // set high pointer to current place in table, decrement current to check one below
        value = functions->GetArrayValue(data, index);
        if (functions->Compare(value, toFind) < 0)
        {
            min = index;
        }
    } else if (cmp < 0) {
        min = index++; //set min point to current, then increment to test if max found too.        
        value = functions->GetArrayValue(data, index);
        if (functions->Compare(value, toFind) > 0)
        {
            max = index;
        }
    }
    // Indicates either values are found, or we are at the end of the search (min or max)
    if (max - min < 2) {
        result->IndexLow = min;
        result->IndexHigh = max;
    } else {
        Scale_rec(result, data, toFind, min, max, functions);
    }
}


void MapLookup(map_t * map, void * result, scale_functions_t * functions) {
    scale_result_t * scaleResult = map->ScaledValue;
    uint8_t low = scaleResult->IndexLow;
    uint8_t high = scaleResult->IndexHigh;
    void * scaleHigh = functions->GetArrayValue(scaleResult->Scale->Data, high);
    void * scaleLow = functions->GetArrayValue(scaleResult->Scale->Data, low);
    
    void * scaleValue = scaleResult->ScaledValue;

    //No need to do interpolation if we have a direct map
    if (high == low) {
        functions->SetValue(result, functions->GetArrayValue(map->Data, low));
    } else {
        functions->MapValue(result, scaleResult->ScaledValue, scaleLow, scaleHigh, functions->GetArrayValue(map->Data, low), functions->GetArrayValue(map->Data, high));
    }
}

void __scale_functions_t_map_int(void * result, void * source, void * sourceLow, void * sourceHigh, void * mapLow, void * mapHigh) {    
        int32_t scaleFactor = (*(int16_t*)mapHigh - *(int16_t*)mapLow) * (*(int16_t*)source - *(int16_t*)sourceLow);
        int32_t scaleRange = *(int16_t*)sourceHigh - *(int16_t*)sourceLow;
        int offset = (scaleFactor / scaleRange);
        *(int16_t*)result = offset + *(int16_t*)mapLow;
}
void * __scale_functions_t_get_array_value_int(void * data, uint8_t index) {
    int16_t * intData = (int16_t*)data; //need to cast the array first, so that you get the right data size.
    return &(intData[index]);
}

int __scale_functions_t_compare_int(const void * a, const void * b ) {
    return *(int16_t*)a - *(int16_t*)b;
}
void __scale_functions_t_set_value_int(const void * a, const void * b ) {
    *(uint16_t*)a = *(uint16_t*)b;
}

void SetScaleFunctions_int(scale_functions_t * functions) {
    functions->Compare = __scale_functions_t_compare_int;
    functions->GetArrayValue = __scale_functions_t_get_array_value_int;
    functions->MapValue = __scale_functions_t_map_int;
    functions->SetValue = __scale_functions_t_set_value_int;
}
