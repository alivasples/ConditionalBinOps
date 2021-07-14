//
// Created by gonzaga on 1/20/16.
//

#ifndef ARBORETUMAPI_H
#define ARBORETUMAPI_H

#include <string>
#include <cstring>
#include <array>

#include <arboretum/stMetricTree.h>
#include <arboretum/stPlainDiskPageManager.h>
#include <arboretum/stDiskPageManager.h>
#include <arboretum/stMemoryPageManager.h>
#include <arboretum/stSlimTree.h>
#include <util/BasicArrayObject.h>
#include <hermes/EuclideanDistance.h>
#include <hermes/DTWDistance.h>
#include <arboretum/stBTree.h>

#include <API/FixedString.h>

using namespace std;

//MULTIDIMENSIONAL INT -------------------------------------------------------------------
typedef BasicArrayObject<int> stIntArrayObject;
typedef EuclideanDistance<stIntArrayObject> stIntArrayEvaluator;
typedef stSlimTree < stIntArrayObject, stIntArrayEvaluator > stIntArrayTree;
typedef stResult < stIntArrayObject > myIntResult;

//MULTIDIMENSIONAL FLOAT -------------------------------------------------------------------
typedef BasicArrayObject<float> stFloatArrayObject;
typedef EuclideanDistance<stFloatArrayObject> stFloatArrayEvaluator;
typedef stSlimTree < stFloatArrayObject, stFloatArrayEvaluator > stFloatArrayTree;
typedef stResult < stFloatArrayObject > myFloatResult;

//MULTIDIMENSIONAL CHARS -------------------------------------------------------------------
typedef BasicArrayObject<char> stCharArrayObject;
typedef DTWDistance<stCharArrayObject> stCharArrayEvaluator;
typedef stSlimTree < stCharArrayObject, stCharArrayEvaluator > stCharArrayTree;
typedef stResult <stCharArrayObject > myCharArrayResult;

//MULTIDIMENSIONAL STRING -------------------------------------------------------------------
typedef BasicArrayObject<string> stStringArrayObject;
typedef EuclideanDistance<stStringArrayObject> stStringArrayEvaluator;
typedef stSlimTree < stStringArrayObject, stStringArrayEvaluator > stStringArrayTree;
typedef stResult <stStringArrayObject > myStringArrayResult;

//METRIC ------------------------------------------------------------------------------------
typedef BasicArrayObject<float> stGenericMetricObject;
typedef EuclideanDistance<stGenericMetricObject> stGenericMetricEvaluator;
typedef stSlimTree <stGenericMetricObject, stGenericMetricEvaluator> stGenericMetricTree;
typedef stResult <stGenericMetricObject> myGenericMetricResult;


// BPLUS Trees
typedef stBPlusTree<int,stIntArrayObject> bpIntTree;
typedef stTOResult<stIntArrayObject, int> bpIntResult;

typedef stBPlusTree<float,stIntArrayObject> bpFloatTree;
typedef stTOResult<stIntArrayObject, float> bpFloatResult;

typedef stBPlusTree<FixedString,stIntArrayObject /*,FixedStringComparator*/> bpStringTree;
typedef stTOResult<stIntArrayObject, FixedString> bpStringResult;



#endif
