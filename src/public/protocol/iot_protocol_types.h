/**
 * Autogenerated by Thrift Compiler (0.10.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef iot_protocol_TYPES_H
#define iot_protocol_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/cxxfunctional.h>




struct eIOTSignalType {
  enum type {
    E_Signal_NoThing = 0,
    E_Signal_InitDB = 1
  };
};

extern const std::map<int, const char*> _eIOTSignalType_VALUES_TO_NAMES;

struct eIOTNodeType {
  enum type {
    E_Type_Default = 0,
    E_Type_M_AP = 10,
    E_Type_R_AP = 11,
    E_Type_C_AP = 12,
    E_Type_B_AP = 13,
    E_Type_L_AP = 14,
    E_Type_T_AP = 15,
    E_Type_Muti_AP = 16,
    E_Type_N_ED = 30,
    E_Type_LP_ED = 31,
    E_Type_M_ED = 32,
    E_Type_MLP_ED = 33
  };
};

extern const std::map<int, const char*> _eIOTNodeType_VALUES_TO_NAMES;



#endif