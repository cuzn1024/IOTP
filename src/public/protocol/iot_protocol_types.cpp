/**
 * Autogenerated by Thrift Compiler (0.10.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "iot_protocol_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>



int _keIOTSignalTypeValues[] = {
  eIOTSignalType::E_Signal_NoThing,
  eIOTSignalType::E_Signal_InitDB
};
const char* _keIOTSignalTypeNames[] = {
  "E_Signal_NoThing",
  "E_Signal_InitDB"
};
const std::map<int, const char*> _eIOTSignalType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(2, _keIOTSignalTypeValues, _keIOTSignalTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));

int _keIOTNodeTypeValues[] = {
  eIOTNodeType::E_Type_Default,
  eIOTNodeType::E_Type_M_AP,
  eIOTNodeType::E_Type_R_AP,
  eIOTNodeType::E_Type_C_AP,
  eIOTNodeType::E_Type_B_AP,
  eIOTNodeType::E_Type_L_AP,
  eIOTNodeType::E_Type_T_AP,
  eIOTNodeType::E_Type_Muti_AP,
  eIOTNodeType::E_Type_N_ED,
  eIOTNodeType::E_Type_LP_ED,
  eIOTNodeType::E_Type_M_ED,
  eIOTNodeType::E_Type_MLP_ED
};
const char* _keIOTNodeTypeNames[] = {
  "E_Type_Default",
  "E_Type_M_AP",
  "E_Type_R_AP",
  "E_Type_C_AP",
  "E_Type_B_AP",
  "E_Type_L_AP",
  "E_Type_T_AP",
  "E_Type_Muti_AP",
  "E_Type_N_ED",
  "E_Type_LP_ED",
  "E_Type_M_ED",
  "E_Type_MLP_ED"
};
const std::map<int, const char*> _eIOTNodeType_VALUES_TO_NAMES(::apache::thrift::TEnumIterator(12, _keIOTNodeTypeValues, _keIOTNodeTypeNames), ::apache::thrift::TEnumIterator(-1, NULL, NULL));


